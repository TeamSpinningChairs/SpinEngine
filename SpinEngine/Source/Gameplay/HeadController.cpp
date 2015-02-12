#include "Precompiled.h"
#include "HeadController.h"
#include "Engine.h"

HeadController::HeadController(GameObject Parent, int ControllerNum) : parent(Parent),
IComponent(Component_Type::CT_HEAD_CONTROLLER, Parent)
{
  ControllerNumber = ControllerNum;
  HeadBody = NULL;
}

HeadController::~HeadController()
{

}

bool HeadController::Initialize()
{
  HeadBody =  reinterpret_cast<RigidBody *>(parent->GetComponent(CT_Body));
  return true;
}

void HeadController::Update(float dt)
{
  //if (ENGINE->m_Input->IsButtonDown(ControllerNumber, BUTTONS::DPAD_RIGHT))
  //{
  //  this->Owner->GetTransform()->GetWorldRotation().z += 1.25 * dt;
  //}


  if (ENGINE->m_Input->IsButtonDown(ControllerNumber, BUTTONS::DPAD_RIGHT))
  {
      if (HeadBody != NULL)
      {
        HeadBody->position += Vector2D(1, 0) * 10 * dt;
      }
  }

  else if (ENGINE->m_Input->IsButtonDown(ControllerNumber, BUTTONS::DPAD_LEFT))
  {
    if (HeadBody != NULL)
    {
      HeadBody->position += Vector2D(-1, 0) * 10 * dt;
    }
  }

  if (ENGINE->m_Input->IsButtonPressed(ControllerNumber, BUTTONS::BUTTONS_A))
  {
    if (HeadBody != NULL)
    {
      HeadBody->velocity.y = 10;
    }
  }


  if (ENGINE->m_Input->isStickDown(ControllerNumber, CONTROLLER_DIR::LEFT))
  {
    if (HeadBody != NULL)
    {
      Vector2D stickpos = ENGINE->m_Input->GetStickPosition(ControllerNumber, CONTROLLER_DIR::LEFT);

      if (stickpos.x > 0)
      {
        HeadBody->position += Vector2D(1, 0) * 10 * dt;
      }

      else
      {
        HeadBody->position += Vector2D(-1, 0) * 10 * dt;
      }
    }
  }
}

void HeadController::Release()
{

}

