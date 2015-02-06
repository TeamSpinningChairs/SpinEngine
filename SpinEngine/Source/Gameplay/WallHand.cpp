/****************************************************************************/
/*!
\author Henry Morgan and Steven Gallwas
\par    email: henry.morgan\@digipen.edu & s.gallwas\@digipen.edu
\par    Course: GAM 200
\brief
This file contains the implementation for the base messaging system.

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#include "Precompiled.h"
#include "WallHand.h"
#include "FactoryAccess.h"
#include "SpriteRenderer.h"
#include "GraphicsManager.h" //to manipulate it through GlobalFactory

WallHand::WallHand(GameObject Parent, GameObject Wall, int handNumber, int *targ) :
IComponent(Component_Type::CT_HAND, Parent), myState(HST_CHASING), isEnteringState(true), myHandNumber(handNumber), allTargets(targ),
myTargetsPosition(NULL), returnTimer(0.0f)
{
	moveSpeed = 4.0f;
	this->Wall = Wall;
	this->TileCheck = NULL;
	this->myTarget = NULL;
	this->Active = false;
	this->StartingPoint.x = 0;
	this->StartingPoint.y = 0;
	this->Chasing = false;
	this->Returning = false;
	//this->myTargetNumber = -1;
}

bool WallHand::Initialize()
{  
  this->TileCheck = reinterpret_cast<TileMapDetection *>(this->Owner->GetComponent(CT_TileMapDetector));

  myTransform = reinterpret_cast<Transform *>(Owner->GetComponent(CT_TransformComponent));

  SetStartingPoint(reinterpret_cast<Transform *>(this->Owner->GetComponent(CT_TransformComponent))->GetPosition());

	return true;
}

void WallHand::Update(float dt)
{
	//float TargetDistance = this->CalculateDistance(this->target);
  switch (myState)
  {
  case HST_CHASING:
    ChaseUpdate(dt);
    break;
  case HST_RETURNING:
    ReturnUpdate(dt);
    break;
  }
}

void WallHand::ChaseUpdate(float dt)
{
  if (isEnteringState)
  {
   /* do
    {
      ChooseTarget();
    } while (!TargetIsValid());*/

    SetStartingPoint(Owner->GetTransform()->GetPosition());

    //Choose a random vector to extend along
    myTargetVector = GetRandomTargetVector();

    myTargetXUnitVector = myTargetVector.x;
    myTargetYUnitVector = myTargetVector.y;
    float csquared = (myTargetXUnitVector * myTargetXUnitVector) + (myTargetYUnitVector * myTargetYUnitVector);
    float c = sqrtf(csquared);
    myTargetXUnitVector = myTargetXUnitVector / c;
    myTargetYUnitVector = myTargetYUnitVector / c;
    isEnteringState = false;
    
  }

  //If we're moving the wall backwards ( for demoing)
  //if (moveSpeed < 0)
  //{
  //  myState = HST_RETURNING;
  //  //isEnteringState = true;
  //  return;
  //}

  //Advance along vector
  myTransform->GetPosition().x += moveSpeed * myTargetXUnitVector * dt;
  myTransform->GetPosition().y += moveSpeed * myTargetYUnitVector * dt;

  // check if distance from starting point is greater than max Chase range
  //float Distance = this->CalculateDistance(this->StartingPoint);
  float Distance = myTransform->GetPosition().x - Owner->GetTransform()->GetPosition().x;
  if (Distance >= MAX_DISTANCE)
  {
    myState = HST_RETURNING;
    isEnteringState = true;
    return;
  }
  
  
  if (TileCheck->GetCurrentGameObject(myTarget) != 0)
  {
    currObjIndex = TileCheck->GetCurrentGameObjectIndex();
    TileCheck->RemoveCurrentTileFromMap();
    //THE TILE IS OURS, SIR
    myTargetsPosition = &(myTarget->GetTransform()->GetPosition());
    (*(reinterpret_cast<SpriteRenderer*>(myTarget->GetComponent(CT_SpriteRenderer)))).ToggleAsTile();
    myState = HST_RETURNING;
    isEnteringState = true;
    return;
  }

}

void WallHand::ReturnUpdate(float dt)
{
  if (isEnteringState)
  {
    //Head back
    myTargetXUnitVector = StartingPoint.x - myTransform->GetPosition().x;
    myTargetYUnitVector = StartingPoint.y - myTransform->GetPosition().y;
    float csquared = (myTargetXUnitVector * myTargetXUnitVector) + (myTargetYUnitVector * myTargetYUnitVector);
    float c = sqrtf(csquared);
    myTargetXUnitVector = myTargetXUnitVector / c;
    myTargetYUnitVector = myTargetYUnitVector / c;

    isEnteringState = false;
  }

  //Move towards our start point
  //Advance along vector
  myTransform->GetPosition().x += moveSpeed * myTargetXUnitVector * dt;
  myTransform->GetPosition().y += moveSpeed * myTargetYUnitVector * dt;

  if (myTargetsPosition != NULL)
  {
    *myTargetsPosition = myTransform->GetPosition();
    myTargetsPosition->x += 0.5f;
  }

    //STEVEN: the object index is the index of our object in the game objects array

    
    //We can't delete the object yet. I need to adjust the way the Factory tracks these so that
    //the initial Tile area is its own separate thing. If we remove these guys now, the object
    //count will get off, we'll start replacing existing dynamic game objects, etc....
    //(this reminder was mostly for me)
    //(hi steven)
    //GlobalFactory->RemoveTileObject(currObjIndex);

  // check if distance from the wall if close enough set to inactive set chase target to null 
  //float ReturnDistance = CalculateDistance(Wall);

  if ((myTargetsPosition != NULL) && (myTransform->GetPosition().x < Wall->GetTransform()->GetPosition().x))
  {
    //Destroy our object
    GlobalFactory->RemoveTileObject(myTarget->GetEntityId());

    myTargetsPosition = NULL;

    myState = HST_CHASING;
    isEnteringState = true;
  }
}

Vector2D WallHand::GetRandomTargetVector()
{
  Vector2D rtarget;
  rtarget.x = static_cast<float>((std::rand() % 20)) + 20.f;
  //If we're below the center of the thing and more than a bit away from the center
  
  if (myTransform->GetPosition().y < 4.0f)
    rtarget.y = static_cast<float>(std::rand() % 20);
  
  else if (myTransform->GetPosition().y > 8.0f)
    rtarget.y = -static_cast<float>(std::rand() % 20);
  
  else
    rtarget.y = static_cast<float>(std::rand() % 10) * ((std::rand() % 2) == 0 ? -1.0f : 1.0f);

  return rtarget;
}

void WallHand::AdjustMoveSpeed(float wallMoveSpeed)
{
  moveSpeed = wallMoveSpeed * 8.0f;
}

void WallHand::Release()
{
	// currently nothing
}

void WallHand::SetTarget(GameObject NewTarget)
{
	this->myTarget = NewTarget;
}


void WallHand::SetStartingPoint(Vector3D &startPoint)
{
  //kinda hacky
	StartingPoint.x = startPoint.x;
  StartingPoint.y = startPoint.y + (static_cast<float>((std::rand() % 30)) / 4.0f) * (std::rand() % 2 == 0 ? -1 : 1);

  //StartingPoint.x -= Owner->GetTransform()->GetPosition().x;
  //StartingPoint.y -= Owner->GetTransform()->GetPosition().y;
}

void WallHand::ChooseTarget()
{
  //myTargetsNumber = TileCheck->GetHandTarget();
}
bool WallHand::TargetIsValid()
{

 /* for (int i = 0; i < HANDCOUNT; ++i)
  {
    if (i == myHandNumber)
      continue;

    if (allTargets[i] == myTargetsNumber)
      return false;
  }

  allTargets[myHandNumber] = myTargetsNumber;
  return true;*/
  return true;
}

