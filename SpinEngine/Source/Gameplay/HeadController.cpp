#include "Precompiled.h"
#include "HeadController.h"
#include "Engine.h"
#include "MenuController.h"

HeadController::HeadController(GameObject Parent, int ControllerNum) : parent(Parent),
IComponent(Component_Type::CT_HEAD_CONTROLLER, Parent)
{
  ControllerNumber = ControllerNum;
  HeadBody = NULL;
  HeadSprite = NULL;
  HeadCollision = NULL;
}

HeadController::~HeadController()
{

}

bool HeadController::Initialize()
{
  HeadBody =  reinterpret_cast<RigidBody *>(parent->GetComponent(CT_Body));
  HeadSprite = reinterpret_cast<SpriteRend>(parent->GetComponent(CT_SpriteRenderer));
  HeadCollision = reinterpret_cast<TileMapCollision *>(parent->GetComponent(CT_TileMapCollider));
  return true;
}

void HeadController::Update(float dt)
{
  //if (ENGINE->m_Input->IsButtonDown(ControllerNumber, BUTTONS::DPAD_RIGHT))
  //{
  //  this->Owner->GetTransform()->GetRotation().z += 1.25 * dt;
  //}

  if (ENGINE->m_Input->IsButtonDown(ControllerNumber, BUTTONS::DPAD_RIGHT))
  {
      if (HeadBody != NULL)
      {
        HeadBody->position += Vector2D(1, 0) * 7.5 * dt;
      
        if (Owner->GetTransform()->GetScale().x < 0)
        {
          Owner->GetTransform()->GetScale().x *= -1.0f;
        }

        HeadSprite->ChangeState("Moving");
      }
  }

  else if (ENGINE->m_Input->IsButtonDown(ControllerNumber, BUTTONS::DPAD_LEFT))
  {
    if (HeadBody != NULL)
    {
      HeadBody->position += Vector2D(-1, 0) * 7.5 * dt;

      if (Owner->GetTransform()->GetScale().x > 0)
      {
        Owner->GetTransform()->GetScale().x *= -1.0f;
      }

      HeadSprite->ChangeState("Moving");
    }
  }

  else if (HeadCollision->BottomIsColliding() == true)
  {
    HeadSprite->ChangeState("idle");
  }

  if (ENGINE->m_Input->IsButtonPressed(ControllerNumber, BUTTONS::BUTTONS_A))
  {
    if (HeadBody != NULL && HeadCollision->BottomIsColliding() == true)
    {
      HeadSprite->ChangeState("Moving");
      HeadSprite->GetCurrentSprite()->PauseAt(7);
      HeadBody->velocity.y = 14;
    }
  }


  if (HeadCollision->BottomIsColliding() == true)
  {
    HeadSprite->GetCurrentSprite()->Set_Paused(false);
  }

  if (ENGINE->m_Input->isStickDown(ControllerNumber, CONTROLLER_DIR::LEFT))
  {
    if (HeadBody != NULL)
    {
      Vector2D stickpos = ENGINE->m_Input->GetStickPosition(ControllerNumber, CONTROLLER_DIR::LEFT);
      HeadSprite->ChangeState("Moving");

      if (HeadCollision->BottomIsColliding() == true)
      {
        HeadSprite->GetCurrentSprite()->Set_Paused(false);
      }

      if (stickpos.x > 0)
      {
        HeadBody->position += Vector2D(1, 0) * 7.5 * dt;
        if (Owner->GetTransform()->GetScale().x < 0)
        {
          Owner->GetTransform()->GetScale().x *= -1.0f;
        }
      }

      else
      {
        HeadBody->position += Vector2D(-1, 0) * 7.5 * dt;
        if (Owner->GetTransform()->GetScale().x > 0)
        {
          Owner->GetTransform()->GetScale().x *= -1.0f;
        }
      }
    }
  }

  else if(ENGINE->m_Input->isStickReleased(ControllerNumber, CONTROLLER_DIR::LEFT) && HeadCollision->BottomIsColliding() == true)
  {
    HeadSprite->ChangeState("idle");
  }


  if (ENGINE->m_Input->IsButtonPressed(this->ControllerNumber, BUTTONS_START))
  {
    if (gGameStateCurr == GS_LEVEL)
    {
      if (gGameStatePaused)
      {
        MenuMessage msg_pause_deactivate(NULL);
        msg_pause_deactivate.Type = DeactivatePause;

        GlobalSystem->MyDispatchMessage(&msg_pause_deactivate);
        gGameStatePaused = false;
        ENGINE->m_Sound->ResumeAll();
      }

      else
      {
        MenuMessage msg1(NULL);
        msg1.Type = UPDATE_POSITION;
        msg1.newPosition = ENGINE->m_Graphics->GetMainCamera()->GetPosition();
        GlobalSystem->MyDispatchMessage(&msg1);

        MenuMessage msg_pause_activate(NULL);
        msg_pause_activate.Type = ActivatePause;

        GlobalSystem->MyDispatchMessage(&msg_pause_activate);
        //ShowPauseMenu();
        gGameStatePaused = true;
        ENGINE->m_Sound->PauseAll();
      }
    }
  }




}

void HeadController::Release()
{

}

