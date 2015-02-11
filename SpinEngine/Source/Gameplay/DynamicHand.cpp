/****************************************************************************/
/*!
\author Steven Gallwas
\par    email: s.gallwas\@digipen.edu
\par    Course: GAM 200
\brief
This file contains the implementation for the base messaging system.

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/

#include "Precompiled.h"
#include "DynamicHand.h"
#include "SpriteRenderer.h"
#include "Math\mathlib.h"
#include "WallListener.h"
#include "FactoryAccess.h"
#include "PlayerController.h"
#include "Engine.h"
#include "WallController.h"

/*************************************************************************/
/*!
  \brief
    Constructor for the dynamic hand, sets all values to default
*/
/*************************************************************************/
DynamicHand::DynamicHand(GameObject Parent, GameObject Wall) : IComponent(Component_Type::CT_HAND, Parent), Target(NULL), WallOwner(Wall), CollisionDelegate(Parent)
{
	HandActive = false;
	xPosition = 0;
	WavePeriodScalar = 0;
	StartingPoint.x = 0;
	StartingPoint.y = 0;
	StartingPoint.z = 0;
	LinkTimer = 0.0f;
	TimeActive = 0.0f;
  TargetNumber = 0;

  WallComponent = NULL;
	ChaseRange = BASE_RANGE;
	WaveAmplitude = BASE_AMPLITUDE;
	HandDuration = BASE_DURATION;
	ReturnSpeed = RETURN_SPEED;
	TimeBetweenLinks = HandDuration / 18;
	Target = NULL;
	angle = 0.0f;
	TotalDistance = 0.0f;
	TileDetection = NULL;
	HitObject = false;
	CapturedObj = NULL;
	ActiveLinks = 0;
  currentState = HAND_INACTIVE;
  OwnerPosition = WallOwner->GetTransform()->GetPosition();

  for (int i = 0; i < LINK_COUNT; ++i)
  {
      Links[i] = NULL;
  }
}

/*************************************************************************/
/*!
  \brief
    Initialize function which will set the tile detector pointer
*/
/*************************************************************************/
bool DynamicHand::Initialize()
{
	TileDetection = reinterpret_cast<TileMapDetection *>(this->Owner->GetComponent(CT_TileMapDetector));
  WallComponent = reinterpret_cast<WallComp  *>(WallOwner->GetComponent(CT_WALL));
  HandBody = reinterpret_cast<RigidBody *>(Owner->GetComponent(CT_Body));
  
  if (HandBody)
  {
    InitializeCollisionCallback();
  }

	return true;
}

/*************************************************************************/
/*!
  \brief
    This function should be called when we want to give the hand a new
    target, calculates a new angle, and 
*/
/*************************************************************************/
void DynamicHand::Re_Initialize(GameObject newTarget)
{
	// set the target as the given target
	Target = newTarget;

	// calculate the distance between the target and the hand
	TotalDistance = CalculateDistance(newTarget);

	//calculate the wave period scalar, whic will be used to determine the proper sine wave
	WavePeriodScalar = (2 * PI) / TotalDistance;

	// set the starting point of the hand to be the current position of the wall
	StartingPoint = this->WallOwner->GetTransform()->GetPosition();

	// set the inital x position to be the center of the wall.
	xPosition = this->WallOwner->GetTransform()->GetPosition().x;

	 // create a vector to calculate the angle from the wall to the target
	Vector2D Temp;

	// Vector2D = target - origin
	Temp.x = newTarget->GetTransform()->GetPosition().x - WallOwner->GetTransform()->GetPosition().x;
	Temp.y = newTarget->GetTransform()->GetPosition().y - WallOwner->GetTransform()->GetPosition().y;

	angle = atan(Temp.y / Temp.x);
	 
	if (angle < 0)
	{
		angle += (2 * PI);
	}

	for (int i = 0; i < LINK_COUNT; i++)
	{
		Links[i] = NULL;
	}

  	currentState = HAND_CHASING;

	// reset the timers
	TimeActive = 0.0f;

	LinkTimer = 0.0f;

	CapturedObj = NULL;
	HitObject = false;
	ActiveLinks = 0;
}

/*************************************************************************/
/*!
  \brief
      this version of the function will be used when we want to launch the 
      hand at a random target point somewhere in front of the wall
*/
/*************************************************************************/
void DynamicHand::Re_Initialize(Vector3D TargetPosition)
{
	// set the target as the given target
	Target = NULL;

	// calculate the distance between the target and the hand
	TotalDistance = CalculateDistance(TargetPosition);

	//calculate the wave period scalar, whic will be used to determine the proper sine wave
	WavePeriodScalar = (2 * PI) / TotalDistance;

	// set the starting point of the hand to be the current position of the wall
	StartingPoint = this->WallOwner->GetTransform()->GetPosition();

	// set the inital x position to be the center of the wall.
	xPosition = this->WallOwner->GetTransform()->GetPosition().x;

	// create a vector to calculate the angle from the wall to the target
	Vector2D Temp;

	// Vector2D = target - origin
	Temp.x = TargetPosition.x - WallOwner->GetTransform()->GetPosition().x;
	Temp.y = TargetPosition.y - WallOwner->GetTransform()->GetPosition().y;

	angle = atan(Temp.y / Temp.x);

	if (angle < 0)
	{
		angle += (2 * PI);
	}

	for (int i = 0; i < LINK_COUNT; i++)
	{
		Links[i] = NULL;
	}

	currentState = HAND_CHASING;

	// reset the timers
	TimeActive = 0.0f;

	LinkTimer = 0.0f;

	CapturedObj = NULL;
	HitObject = false;
	ActiveLinks = 0;
}

/*************************************************************************/
/*!
  \brief
      Update function which moves the hand in a sin wave towards the 
      target
*/
/*************************************************************************/
void DynamicHand::Update(float dt)
{
	// if the hand is not active, make it invisible, and translate it with the wall
  if (this->HandActive == false)
	{
		Owner->GetTransform()->GetPosition() = this->WallOwner->GetTransform()->GetPosition();
    Owner->SetInVisible();

    if (HandBody)
    {
      *(HandBody->getPosition()) += Vector2D(WallOwner->GetTransform()->GetPosition().x, WallOwner->GetTransform()->GetPosition().y);
    }
    return;
	}
  
  Owner->SetVisible();

	// update time 
	TimeActive += dt;

	// update the timer
	LinkTimer += dt;

	if (this->currentState == HAND_CHASING)
	{
		DynamicHandMovement();

		// check if the hand has collided with a tile
		if(TileDetection != NULL)
		{
			CheckforTile();
		}

		// check if distance from starting point is greater than max Chase range
		if(Target != NULL)
		{
			float TargetDistance = CalculateDistance(Target);

			// check if the distance between the hand and its target is within the capture range
			if (TargetDistance < DYNAMIC_GRAB)
			{
				// come up with a way to capture the player
        CapturedObj = Target;

				currentState = HAND_RETURNING;
				Owner->GetTransform()->GetRotation().z = angle;
        StraightenLink();
			}
		}

    else if (CapturedObj == NULL)
    {
      CapturedObj = GlobalFactory->CheckTargets(this->Owner->GetTransform()->GetPosition());

      if (CapturedObj)
      {
        currentState = HAND_RETURNING;
        Owner->GetTransform()->GetRotation().z = angle;
        StraightenLink();
      }
    }

		// check if distance from starting point is greater than max Chase range
		float Distance = CalculateDistance(StartingPoint);
		
		if (Distance >= ChaseRange || this->TimeActive > this->HandDuration)
		{
 			currentState = HAND_RETURNING;
			Owner->GetTransform()->GetRotation().z = angle;
      StraightenLink();
		}
	}

	// when the hand is returning, have it travel in a straight line back to the wall.
	if (currentState == HAND_RETURNING)
	{
		Vector2D Direction;

		Direction.x = WallOwner->GetTransform()->GetPosition().x - Owner->GetTransform()->GetPosition().x;
		Direction.y = WallOwner->GetTransform()->GetPosition().y - Owner->GetTransform()->GetPosition().y;
	
		// normalize the direction
		Direction = Direction.Normalize();

    if (CapturedObj && CapturedObj->GetComponent(CT_Player_Controller) != NULL)
    {
      if (WallComponent != NULL)
      {
        Owner->GetTransform()->GetPosition().x += Direction.x * (.85 * WallComponent->GetMoveSpeed() * dt);
        Owner->GetTransform()->GetPosition().y += Direction.y * (.85 * WallComponent->GetMoveSpeed() * dt);
      }
      else
      {
        Owner->GetTransform()->GetPosition().x += Direction.x * (.85 * 5 * dt);
        Owner->GetTransform()->GetPosition().y += Direction.y * (.85 * 5 * dt);

        if (HandBody)
        {
          *(HandBody->getPosition()) += Vector2D(Direction.x * (.85 * 5 * dt), Direction.y * (.85 * 5 * dt));
        }
      }

    }
    else
    {
      if (WallComponent != NULL)
      {
        Owner->GetTransform()->GetPosition().x += Direction.x * (1.65 * WallComponent->GetMoveSpeed() * dt);
        Owner->GetTransform()->GetPosition().y += Direction.y * (1.65 * WallComponent->GetMoveSpeed() * dt);
      }
      else
      {
        Owner->GetTransform()->GetPosition().x += Direction.x * (9 * dt);
        Owner->GetTransform()->GetPosition().y += Direction.y * (9 * dt);

        if (HandBody)
        {
          *(HandBody->getPosition()) += Vector2D(Direction.x * (9 * dt), Direction.y * (9 * dt));
        }
      }
    }
    
    if (CapturedObj)
		{
			CapturedObj->GetTransform()->GetPosition() = Owner->GetTransform()->GetPosition();

      if (CapturedObj->GetComponent(CT_Body) != NULL)
      {
          reinterpret_cast<RigidBody *>(CapturedObj->GetComponent(CT_Body))->position.x = Owner->GetTransform()->GetPosition().x;
          reinterpret_cast<RigidBody *>(CapturedObj->GetComponent(CT_Body))->position.y = Owner->GetTransform()->GetPosition().y;
      }
      
		}

		// move all links towards the wall
		LinkReturn(dt);

		// check if distance from the wall if close enough set to inactive set chase target to null 
		float ReturnDistance = CalculateDistance(WallOwner);

    if (ReturnDistance < RETURN_DISTANCE || (Owner->GetTransform()->GetPosition().x < this->WallOwner->GetTransform()->GetPosition().x))
		{
			HandReturn();
		} // end of wall return check		
	
	} // end of returning function

}	 //end of update function

/*************************************************************************/
/*!
\brief
	Function to calculate the sine wave movement of the hands.
*/
/*************************************************************************/
void DynamicHand::DynamicHandMovement()
{
	// update the xposition
	xPosition = CalculateNextPosition(StartingPoint.x, TotalDistance, TimeActive, HandDuration);

	// calculate the y position on the sin wave
	float yposition = (sin(this->xPosition * this->WavePeriodScalar) * this->WaveAmplitude) + this->StartingPoint.y;

	// create a vector to calculate the correct position of the hand
  Vector3D newPosition = Vector3D();

	// x = xcos(theta) - ysin(theta)
	// y = xsin(theta) + y cos(theta)

	// calculate the slope of the line, so that the claw orients itself correctly
	float Slope = WavePeriodScalar * WaveAmplitude * (cos(xPosition * WavePeriodScalar));

	float RotationAngle = atan(Slope);

	// rotate the hand so it faces the correct directon
	Owner->GetTransform()->GetRotation().z = RotationAngle + angle;

  for (int i = 0; i < LINK_COUNT; i++)
  {
    if (Links[i])
    {
      Vector2D Direction;

      Direction.x = WallOwner->GetTransform()->GetPosition().x - OwnerPosition.x;
      Direction.y = WallOwner->GetTransform()->GetPosition().y - OwnerPosition.y;

      // normalize the direction
      //Direction.Normalize();

      Links[i]->GetTransform()->GetPosition().x += Direction.x;
      Links[i]->GetTransform()->GetPosition().y += Direction.y;

      // check if the link is near the wall
      float distance = CalculateDistance(WallOwner, Links[i]);
    }
  }

	// create the first link of the hand.
	if (ActiveLinks == 0 || LinkTimer > TimeBetweenLinks)
	{
		CreateLink(Owner->GetTransform()->GetPosition() - Vector3D(0,0,1), RotationAngle + angle);
    
		if (ActiveLinks != 0)
		{
			LinkTimer = 0.0f;
		}
	}

  OwnerPosition = WallOwner->GetTransform()->GetPosition();

	xPosition -= WallOwner->GetTransform()->GetPosition().x;
	yposition -= WallOwner->GetTransform()->GetPosition().y;

	newPosition.x = ((xPosition * cos(angle)) - (yposition * sin(angle)));

	newPosition.y = ((xPosition * sin(angle)) + (yposition * cos(angle))); 

	newPosition.z = Owner->GetTransform()->GetPosition().z;

	newPosition.x += WallOwner->GetTransform()->GetPosition().x;
	newPosition.y += WallOwner->GetTransform()->GetPosition().y;

	// move the hand to the correct position
	Owner->GetTransform()->GetPosition() = newPosition;

  if (HandBody != NULL)
  {
    *HandBody->getPosition() = Vector2D(newPosition.x, newPosition.y);
  }
}

/*************************************************************************/
/*!
\brief
	frees any allocated memory created by the hands
*/
/*************************************************************************/
void DynamicHand::Release()
{
	// check if any of the hands are still active
	for (int i = 0; i < LINK_COUNT; i++)
	{
		if (this->Links[i])
		{
			GlobalFactory->RemoveGameObject(Links[i]->GetEntityId());
			Links[i] = NULL;
			--ActiveLinks;
		}
	}
}

/*************************************************************************/
/*!
\brief
	setter to set the target for the hand.
*/
/*************************************************************************/
void DynamicHand::SetTarget(GameObject TargetObj)
{
	Target = TargetObj;
}

/*************************************************************************/
/*!
\brief
	Sets the starting point for the hand
*/
/*************************************************************************/
void DynamicHand::SetStartingPoint(Vector3D &startPos)
{
	StartingPoint.x = startPos.x;
	StartingPoint.y = startPos.y;
	StartingPoint.z = startPos.z;
}

/*************************************************************************/
/*!
\brief
	activates the hand
*/
/*************************************************************************/
void DynamicHand::ActivateHand(void)
{
	HandActive = true;
}

/*************************************************************************/
/*!
\brief
	sets the hand to inactive so that it can be relaunched later
*/
/*************************************************************************/
void DynamicHand::DeActivateHand(void)
{
  HandActive = false;
}

/*************************************************************************/
/*!
\brief
	returns the the hands current active status
*/
/*************************************************************************/
bool DynamicHand::GetHandActive(void)
{
	return HandActive;
}

/*************************************************************************/
/*!
\brief
	overloaded function which calculates the distance between the owner
	and another gameobject, or a target vector.
*/
/*************************************************************************/
float DynamicHand::CalculateDistance(Vector3D TargetPoint)
{
	float X1 = Owner->GetTransform()->GetPosition().x;
	float Y1 = Owner->GetTransform()->GetPosition().y;

	float X2 = TargetPoint.x;
	float Y2 = TargetPoint.y;

	float XDistance = X2 - X1;

	float YDistance = Y2 - Y1;

	float Total = sqrt((XDistance * XDistance) + (YDistance * YDistance));

	return Total;
}

float DynamicHand::CalculateDistance(GameObject TargetObject)
{
	float X1 = Owner->GetTransform()->GetPosition().x;
	float Y1 = Owner->GetTransform()->GetPosition().y;

	float X2 = TargetObject->GetTransform()->GetPosition().x;
	float Y2 = TargetObject->GetTransform()->GetPosition().y;

	float XDistance = X2 - X1;

	float YDistance = Y2 - Y1;

	float Total = sqrt((XDistance * XDistance) + (YDistance * YDistance));

	return Total;
}

float DynamicHand::CalculateDistance(GameObject TargetObject, GameObject StartObj)
{
	float X1 = StartObj->GetTransform()->GetPosition().x;
	float Y1 = StartObj->GetTransform()->GetPosition().y;

	float X2 = TargetObject->GetTransform()->GetPosition().x;
	float Y2 = TargetObject->GetTransform()->GetPosition().y;

	float XDistance = X2 - X1;

	float YDistance = Y2 - Y1;

	float Total = sqrt((XDistance * XDistance) + (YDistance * YDistance));

	return Total;
}


/*************************************************************************/
/*!
\brief
	Function to calculate the hands next position given the total
	time the hand has been traveling, how long we want it to take
	and the starting and ending points.
*/
/*************************************************************************/
float DynamicHand::CalculateNextPosition(float startingVal, float endingVal, float TotalTime, float Duration)
{
	float RemainingTime = Duration - TotalTime;

	float PercentTravelled = (TotalTime * 100) / Duration;

	return startingVal + ((endingVal * PercentTravelled) / 100);
}

/*************************************************************************/
/*!
\brief
		checks to see if the hand has collided with a tile.  if so,
		set capturedobj to the tile, and return to the wall 
*/
/*************************************************************************/
void DynamicHand::CheckforTile(void)
{
	int TileCheck = TileDetection->GetCurrentGameObject(CapturedObj);

	if (TileCheck)
	{
		currentState = HAND_RETURNING;
		Owner->GetTransform()->GetRotation().z = angle;
		HitObject = true;

		//remove the tile from the tile map, and connect it to the hand
		TileDetection->RemoveCurrentTileFromMap();

    (*(reinterpret_cast<SpriteRenderer*>(CapturedObj->GetComponent(CT_SpriteRenderer)))).ToggleAsTile();

		// rotate all links to the correct direction
		for (int i = 0; i < LINK_COUNT; ++i)
		{
			if (Links[i] != NULL)
			{
				Links[i]->GetTransform()->GetRotation().z = angle;
			}
		}
	}
}

/*************************************************************************/
/*!
\brief

*/
/*************************************************************************/
void DynamicHand::CreateLink(Vector3D position, float angle)
{
	if (ActiveLinks < LINK_COUNT)
	{
		//Links[ActiveLinks] = GlobalFactory->CreateGameObject("Link", "Link.png", position);
		Links[ActiveLinks] = GlobalFactory->CreateGameObject("Link", "Segment.png", position);
		Links[ActiveLinks]->GetTransform()->GetRotation().z = angle;
    Links[ActiveLinks]->GetTransform()->GetScale().y *= .5;
    Links[ActiveLinks]->GetTransform()->GetScale().x *= 1.20;
		++ActiveLinks;
	}
}

/*************************************************************************/
/*!
\brief

*/
/*************************************************************************/
void DynamicHand::LinkReturn(float dt)
{
	for (int i = 0; i < LINK_COUNT; i++)
	{
		if (Links[i])
		{
			Vector2D Direction;

			Direction.x = WallOwner->GetTransform()->GetPosition().x - Links[i]->GetTransform()->GetPosition().x;
			Direction.y = WallOwner->GetTransform()->GetPosition().y - Links[i]->GetTransform()->GetPosition().y;

			// normalize the direction
			Direction.Normalize();

      Links[i]->GetTransform()->GetPosition().x += Direction.x * (ReturnSpeed * dt);
			Links[i]->GetTransform()->GetPosition().y += Direction.y * (ReturnSpeed * dt);

			// check if the link is near the wall
			float distance = CalculateDistance(WallOwner, Links[i]);

			if (distance < RETURN_DISTANCE)
			{
				GlobalFactory->RemoveGameObject(Links[i]->GetEntityId());
				Links[i] = NULL;
				--ActiveLinks;
			}
		}
	}
}

/*************************************************************************/
/*!
\brief

*/
/*************************************************************************/
void DynamicHand::HandReturn()
{
	currentState = HAND_INACTIVE;

	Owner->GetTransform()->GetRotation().z = 0.0f;

	Owner->GetTransform()->GetPosition() = WallOwner->GetTransform()->GetPosition();

	for (int i = 0; i < LINK_COUNT; i++)
	{
		if (this->Links[i] != NULL)
		{
			GlobalFactory->RemoveGameObject(Links[i]->GetEntityId());
			Links[i] = NULL;
			--ActiveLinks;
		}
	}

	DeActivateHand();

	// if hand has a child object kill the child object
	if (CapturedObj)
	{  
      GlobalFactory->RemoveTarget(CapturedObj);

      if (CapturedObj->GetComponent(CT_Player_Controller) != NULL)
      {
            //reinterpret_cast<PlayerController *>(CapturedObj->GetComponent(CT_Player_Controller))->KillPlayer();
            reinterpret_cast<PlayerController *>(CapturedObj->GetComponent(CT_Player_Controller))->Active = false;
            
            if (WallComponent != NULL)
            {
              reinterpret_cast<WallController *>(WallOwner->GetComponent(CT_WALL_CONTROLLER))->PrisonPart = WallComponent->WallImage;
              reinterpret_cast<WallController *>(WallOwner->GetComponent(CT_WALL_CONTROLLER))->BodyPart = WallComponent->WallImage2;

              WallComponent->WallActive = false;
            }

            reinterpret_cast<WallController *>(WallOwner->GetComponent(CT_WALL_CONTROLLER))->WallActive = true;
            CapturedObj->SetInVisible();

            //GameObject newObject = GlobalFactory->CreateGameObject("Trigger", "SCBody.png", this->Owner->GetTransform()->GetPosition(), d3dColors::White, true);

            //newObject->GetTransform()->GetScale() *= 5;

            ////add a rigid body
            //AABB *box = new AABB(newObject);
            //box->SetHalfSize(1.0f, 1.0f);
            //newObject->AddGameComponent(CT_BoxCollider, box);

            //WallController * walltest = new WallController(newObject, 0);
            //newObject->AddGameComponent(CT_WALL_CONTROLLER, walltest);
            //walltest->Initialize();

            //GlobalFactory->RemoveGameObject(CapturedObj->GetEntityId());
      }

      else if (CapturedObj->GetComponent(CT_Player_Controller) == NULL && CapturedObj->GetComponent(CT_WALL_TRIGGER) == NULL)
      {
          GlobalFactory->RemoveTileObject(CapturedObj->GetEntityId());
      }

      else
      {
        GlobalFactory->RemoveGameObject(CapturedObj->GetEntityId());
      }

      CapturedObj = NULL;
	}

	// else reset the target to be a valid target
	WallMessage msg(RESET_TARGET);

	msg.TargetNumber = this->TargetNumber;

	if (GlobalSystem)
	{
		GlobalSystem->MyDispatchMessage(&msg);
	}
}

/*************************************************************************/
/*!
\brief

*/
/*************************************************************************/
void DynamicHand::StraightenLink()
{
    // calculate the distance between each link, then translate it to the claw position, then move it back
    for (int i = 0; i < LINK_COUNT; i++)
    {
        if (Links[i] != NULL)
        {
            Links[i]->GetTransform()->GetRotation().z = Owner->GetTransform()->GetRotation().z;

            continue;

            // need to figure out how to line the links up properly
            float LinkDistance = CalculateDistance(Links[i]);

            Links[i]->GetTransform()->GetPosition() = Owner->GetTransform()->GetPosition();
            
            Links[i]->GetTransform()->GetPosition().x -= LinkDistance;
            Links[i]->GetTransform()->GetPosition().y -= LinkDistance;
        }
    }
}


void DynamicHand::OnCollision(GameObject otherObject)
{
  //printf("hit something\n");

    //currentState = HAND_RETURNING;
    //Owner->GetTransform()->GetRotation().z = angle;
    //StraightenLink();
    //
    //this->CapturedObj = otherObject;
}
