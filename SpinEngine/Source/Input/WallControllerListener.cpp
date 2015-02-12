/****************************************************************************/
/*!
\author Steven Gallwas
\par    email: s.gallwas\@digipen.edu
\par    Course: GAM 200
\brief

Copyright: All content © 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#include "Precompiled.h"
#include "Engine.h"
#include "Physics\rigidbody.h"
#include "DebugMessages.h"
#include "FactoryAccess.h"
#include "SpriteRenderer.h"
#include "WallControllerListener.h"
#include "GameStateManager.h"
#include "MenuController.h"
#include "SoundEmitter.h"
#include "ScrollComponent.h"
#include "DynamicHand.h"
#include "Wall.h"
#include "WallController.h"

/*************************************************************************/
/*!
\brief

*/
/*************************************************************************/
WallControllerListener::WallControllerListener(IEntity *playerObj, int ControllerNum) :
controllerNum(ControllerNum), player(playerObj)
{
  //ControlType = 1;
  float *temp = NULL;

  GlobalSettings->GetFloatValue("___ WALL CONTROLLER SETTINGS ___", temp, false);
  GlobalSettings->GetIntValue("Wall Control Type", ControlType, true);
}

/*************************************************************************/
/*!
\brief

*/
/*************************************************************************/
void WallControllerListener::Initialize()
{
    //PlayerBody = reinterpret_cast<RigidBody *>(player->GetComponent(CT_Body));
    PlayerSprite = reinterpret_cast<SpriteRenderer *>(player->GetComponent(CT_SpriteRenderer));
    PlayerTransform = reinterpret_cast<Transform *>(player->GetComponent(CT_TransformComponent));
    WallControl = reinterpret_cast<WallController *>(player->GetComponent(CT_WALL_CONTROLLER));
}

/*************************************************************************/
/*!
\brief

*/
/*************************************************************************/
void WallControllerListener::OnMessageRecieved(Message * SentMessage)
{
    if (!SentMessage)
    {
        MessageBox(NULL, "PlayerControllerListener got a null message? What?", NULL, NULL);
        return;
    }

    if (WallControl->WallActive == false)
    {
      return;
    }

    InputMessage *GivenInput = reinterpret_cast<InputMessage *>(SentMessage);

    if (GivenInput->Controller_Num != controllerNum)
        return;

    if (gGameStatePaused == true)
    {
        return;
    }

    // left stick aiming, right trigger for shooting
    if ((GivenInput->LeftStickState == STICK_TRIGGERED || GivenInput->LeftStickState == STICK_DOWN))
    {
		  //PlayerBody->getPosition() += (Vector2D(GivenInput->leftStick.first, GivenInput->leftStick.second) * 5);
		  this->player->GetTransform()->GetWorldPosition() += (Vector3D(GivenInput->leftStick.first, GivenInput->leftStick.second, 0) * (8 * GivenInput->time));
    
      if (GivenInput->RightTriggerState == TRIGGER_TRIGGERED  && *ControlType == 0)
      {
        Vector3D Target = (player->GetTransform()->GetWorldPosition() + (Vector3D(GivenInput->leftStick.first, GivenInput->leftStick.second, 0) * 20));

        WallControl->LaunchTentacle(Target);
      }    
    }

    else if ((GivenInput->LeftStickState == STICK_NEUTRAL && GivenInput->RightTriggerState == TRIGGER_TRIGGERED) && *ControlType == 0)
    {
      Vector3D Target = (player->GetTransform()->GetWorldPosition() + Vector3D(20, 0, 0));

      WallControl->LaunchTentacle(Target);
    }

    // Novelty Fire all at once with right stick
    if ((GivenInput->RightStickState == STICK_TRIGGERED || GivenInput->RightStickState == STICK_DOWN) && *ControlType == 1)
    {
      Vector3D Target = (player->GetTransform()->GetWorldPosition() + (Vector3D(GivenInput->rightStick.first, GivenInput->rightStick.second, 0).Normalize() * 20));

      WallControl->LaunchTentacle(Target);
    }

    // Aim with left stick, fire with right trigger
    if ((GivenInput->RightStickState == STICK_DOWN && GivenInput->RightTriggerState == TRIGGER_TRIGGERED) && *ControlType == 2)
    {
      Vector3D Target = (player->GetTransform()->GetWorldPosition() + (Vector3D(GivenInput->rightStick.first, GivenInput->rightStick.second, 0).Normalize() * 20));

      WallControl->LaunchTentacle(Target);
    }
    
    else if ((GivenInput->RightStickState == STICK_NEUTRAL && GivenInput->RightTriggerState == TRIGGER_TRIGGERED) && *ControlType == 2)
    {
      Vector3D Target = (player->GetTransform()->GetWorldPosition() + Vector3D(20,0,0));

      WallControl->LaunchTentacle(Target);
    }

    // fire with flick left stick
	  if (GivenInput->RightStickState == STICK_TRIGGERED && *ControlType == 3)
    {
      Vector3D Target = (player->GetTransform()->GetWorldPosition() + (Vector3D(GivenInput->rightStick.first, GivenInput->rightStick.second, 0).Normalize() * 20));

      WallControl->LaunchTentacle(Target);
    }

    switch (GivenInput->TriggeredKey)
    {
        case BUTTONS_A:
        break;

        case BUTTONS_X:
        break;

        case BUTTONS_B:
        break;

        case BUTTONS_Y:
        break;

        case BUTTONS_RIGHT_SHOULDER:
        break;

        case BUTTONS_LEFT_SHOULDER:
        break;

        case BUTTONS_BACK:
        break;

        case BUTTONS_START:
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
        break;

        case DPAD_LEFT:
        break;

        case DPAD_RIGHT:
        break;

        case DPAD_UP:
        break;

        case DPAD_DOWN:
        break;

        case LEFT_STICK:
        break;

        case RIGHT_STICK:
        break;
    }
}
