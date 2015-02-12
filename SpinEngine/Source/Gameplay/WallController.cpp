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
#include "WallController.h"
#include "DynamicHand.h"

/*************************************************************************/
/*!
\brief

*/
/*************************************************************************/
WallController::WallController(GameObject Parent, int ControllerNum) : parent(Parent),
                              IComponent(Component_Type::CT_Player_Controller, Parent)
{
  ControllerNumber = ControllerNum;
  WallListener = NULL;
  PrisonPart = NULL;
  movevalue = 0.25f;
  WallActive = false;
  
  BodyPart = NULL;
  PrisonPart = NULL;
}

/*************************************************************************/
/*!
\brief

*/
/*************************************************************************/
WallController::~WallController()
{
  GlobalSystem->UnregisterListener(MESSAGE_INPUT, WallListener);
  delete WallListener;
}

/*************************************************************************/
/*!
\brief

*/
/*************************************************************************/
bool WallController::Initialize()
{
  WallListener = new WallControllerListener(Owner, ControllerNumber);
  GlobalSystem->RegisterListener(MESSAGE_INPUT, WallListener);
  WallListener->Initialize();

  // fill in the array for dynamic hands, that will target the player, or platforms, or in a random direction;
  for (int i = 0; i < DYNAMIC_HANDCOUNT + RESERVED_HANDS; i++)
  {
    GameObject hand = GlobalFactory->CreateGameObject("hand", "Claw-Open.png", this->Owner->GetTransform()->GetPosition());

    SpriteRend Handrend = reinterpret_cast<SpriteRend>(hand->GetComponent(CT_SpriteRenderer));
    //Handrend->GetSpriteColor() = d3dColors::Red;

    Handrend->AddSprite("Claw-Closed.png", 0, 0, 1, 1 / 30.0f, "Closed", false);

    DynamicHand * DHand = new DynamicHand(hand, this->Owner);

    TileMapDetection * TileCheck = new TileMapDetection(hand, GlobalFactory->GetTileMapData(), GlobalFactory->GetGameObjectList());

    hand->AddGameComponent(CT_HAND, DHand);

    Dynamic_Hands[i] = DHand;

    hand->AddGameComponent(CT_TileMapDetector, TileCheck);

    /*
    AABB *box = new AABB(hand);
    box->SetHalfSize(1.0f, 1.0f);
    hand->AddGameComponent(CT_BoxCollider, box);
    box->Initialize();

    RigidBody *body = new RigidBody(hand, reinterpret_cast<Primitive*>(box));
    body->set(5.0f);
    body->restitution = 0.0f;
    body->useGravity = false;
    body->Initialize();
    hand->AddGameComponent(CT_Body, body);
    body->SetActive(false);
    */

    DHand->Initialize();

    hand->GetTransform()->GetScale() *= .85;
  }

  //PrisonPart = GlobalFactory->CreateGameObject("Prison", "SCPrison.png", this->Owner->GetTransform()->GetPosition());
  //PrisonPart->GetTransform()->GetScale() *= 5;

 return true;
}

/*************************************************************************/
/*!
\brief
  Updates all of the tentacles
*/
/*************************************************************************/
void WallController::Update(float dt)
{
  //PrisonPart->GetTransform()->GetPosition() = this->Owner->GetTransform()->GetPosition();
  if (BodyPart != NULL && PrisonPart != NULL)
  {
    BodyPart->GetTransform()->GetPosition() = this->Owner->GetTransform()->GetPosition() - Vector3D(2,0,8);
    PrisonPart->GetTransform()->GetPosition() = this->Owner->GetTransform()->GetPosition() - Vector3D(2, 0, 8);
  }

  //movevalue *= -1;

  for (int i = 0; i < DYNAMIC_HANDCOUNT + RESERVED_HANDS; ++i)
  {
    Dynamic_Hands[i]->Update(dt);
  }
}

/*************************************************************************/
/*!
\brief
  Frees all alocated memory
*/
/*************************************************************************/
void WallController::Release()
{
  // fill in the array for dynamic hands, that will target the player, or platforms, or in a random direction;
  for (int i = 0; i < DYNAMIC_HANDCOUNT + RESERVED_HANDS; i++)
  {
    delete Dynamic_Hands[i];
  }
}

/*************************************************************************/
/*!
\brief
  Launches an inactive tentacle in a given direction

\param Direction
  The direction to launch the tentacle
*/
/*************************************************************************/
void WallController::LaunchTentacle(Vector3D Direction)
{
  if (Direction.x < Owner->GetTransform()->GetPosition().x)
  {
    return;
  }

  for (int i = 0; i < DYNAMIC_HANDCOUNT; i++)
  {
    if (this->Dynamic_Hands[i]->GetHandActive() == false)
    {
      Dynamic_Hands[i]->ActivateHand();
      Dynamic_Hands[i]->TargetNumber = -1;
      Dynamic_Hands[i]->Re_Initialize(Direction);
      return;
    }
  }
}