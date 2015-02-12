/****************************************************************************/
/*!
\author Steven Gallwas(Primary) and Henry Morgan
\par    email: s.gallwas\@digipen.edu
\par    Course: GAM 200
\brief  
    This file contains the implementation for the base messaging system.
    -Henry (me) tweaked/adapted some things to add this to PlayerController.

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#include "Precompiled.h"
#include "Engine.h"
#include "Physics\rigidbody.h"
#include "DebugMessages.h"
#include "FactoryAccess.h"
#include "SpriteRenderer.h"
#include "PlayerControllerListener.h"
#include "PlayerController.h"
#include "GameStateManager.h"
#include "MenuController.h"
#include "SoundEmitter.h"
#include "WallControllerListener.h"
#include "WallController.h"
#include "HeadlessChickenAI.h"
#include "HeadController.h"

/*************************************************************************/
/*!
  \brief
    Updates the current state of a given button for all controllers.
    Called in the update function
  
  \param ButtonType
    the type of button to check.
  
*/
/*************************************************************************/

PlayerControllerListener::PlayerControllerListener(IEntity *playerObj, PlayerController *parent, int ControllerNum) :
  playerController(parent), controllerNum(ControllerNum), player(playerObj)
{
}

void PlayerControllerListener::Initialize()
{
  PlayerBody = reinterpret_cast<RigidBody *>(player->GetComponent(CT_Body));
  PlayerSprite = reinterpret_cast<SpriteRenderer *>(player->GetComponent(CT_SpriteRenderer));
  PlayerTransform = reinterpret_cast<Transform *>(player->GetComponent(CT_TransformComponent));
}


void PlayerControllerListener::OnMessageRecieved(Message * SentMessage)
{
  if (!SentMessage)
  {
    MessageBox(NULL, "PlayerControllerListener got a null message? What?", NULL, NULL);
    return;
  }
  InputMessage *GivenInput = reinterpret_cast<InputMessage *>(SentMessage);
	
  if (GivenInput->Controller_Num != controllerNum)
		return;

    if(gGameStatePaused == true)
  {
    return;
  }

    if (playerController->Active == false)
    {
      return;
    }

    // move left
    if (GivenInput->leftStick.first < -0.3 && GivenInput->leftStick.first >= -1.0f && (GivenInput->LeftStickState == STICK_TRIGGERED || GivenInput->LeftStickState == STICK_DOWN))
    {      
        if (this->playerController->getJumpState() == JumpStates::JS_Grounded || this->playerController->getJumpState() == JumpStates::JS_PLATFORM)
        {
            PlayerSprite->GetCurrentSprite()->Set_Paused(false);

            if (this->playerController->WalkTimer > 0.4 && this->playerController->getJumpTimer() < BIT_MORE_THAN_ONE_FRAME)
            {
                //((SoundEmitter*)player->GetComponent(CT_SoundEmitter))->StopEvent("player_footsteps");
                //((SoundEmitter*)player->GetComponent(CT_SoundEmitter))->Play("CB/player_footsteps", false);
                this->playerController->WalkTimer = 0.0f;
            }
        }


        PlayerSprite->ChangeState("running");
        PlayerBody->velocity.x = -(*playerController->playerRunSpeed);

        //Make the player face left
        if (PlayerTransform->GetScale().x > 0)
        {
            PlayerTransform->GetScale().x *= -1.0f;
        }
    }

    // move Right
    else if (GivenInput->leftStick.first > 0.3 && GivenInput->leftStick.first <= 1.0f && (GivenInput->LeftStickState == STICK_TRIGGERED || GivenInput->LeftStickState == STICK_DOWN))
    {
        // if the player is grounded and the player is pressing right, play move sound effect
        if (this->playerController->getJumpState() == JumpStates::JS_Grounded || this->playerController->getJumpState() == JumpStates::JS_PLATFORM)
        {
            PlayerSprite->GetCurrentSprite()->Set_Paused(false);

            if (this->playerController->WalkTimer > 0.4 && this->playerController->getJumpTimer() < BIT_MORE_THAN_ONE_FRAME)
            {
                //((SoundEmitter*)player->GetComponent(CT_SoundEmitter))->StopEvent("player_footsteps");
                //((SoundEmitter*)player->GetComponent(CT_SoundEmitter))->Play("CB/player_footsteps", false);
                this->playerController->WalkTimer = 0.0f;
            }
        }


        PlayerSprite->ChangeState("running");
        PlayerBody->velocity.x = *playerController->playerRunSpeed;

        if (PlayerTransform->GetScale().x < 0)
        {
            //Face the player right
            PlayerTransform->GetScale().x *= -1.0f;
        }
    }
    
    // return to idle
    if (GivenInput->LeftStickState == STICK_RELEASED)
    {
        PlayerSprite->ChangeState("idle");
        PlayerBody->velocity.x = 0.0f;
    }
    
    // Throw Head Prototype
    if (GivenInput->RightTriggerState == TRIGGER_TRIGGERED)
    {
      GameObject newObject = GlobalFactory->CreateGameObject("Trigger", "Head_Idle.png", this->PlayerTransform->GetPosition() + Vector3D(2 * PlayerTransform->GetScale().x, 0, 0));
      //newObject->Initialize();
      
      //Box collider and rigidbody
      AABB *box = new AABB(newObject);
      box->SetHalfSize(0.2f, 0.9f);
      newObject->AddGameComponent(CT_BoxCollider, box);
      
      RigidBody *body = new RigidBody(newObject, reinterpret_cast<Primitive*>(box));
      body->set(5.0f);
      body->restitution = 0;
      newObject->AddGameComponent(CT_Body, body);
      
      //Tilemap collider
      TileMapCollision *tcoll = new TileMapCollision(newObject, GlobalFactory->GetTileMapData());
      newObject->AddGameComponent(CT_TileMapCollider, tcoll);
      
      HeadController * head = new HeadController(newObject, 0);
      newObject->AddGameComponent(CT_HEAD_CONTROLLER, reinterpret_cast<Component>(head));

      this->playerController->Active = false;

      box->Initialize();
      body->Initialize();
      tcoll->Initialize();
      head->Initialize();

      body->velocity.Set(10 * PlayerTransform->GetScale().x, 10);
      
    }



  switch(GivenInput->TriggeredKey)
  {
	case BUTTONS_A:
    if (GivenInput->KeyPressed == true)
    {
          //((SoundEmitter*)player->GetComponent(CT_SoundEmitter))->SetVolume(1.0f, "player_jump");
          //((SoundEmitter*)player->GetComponent(CT_SoundEmitter))->StopEvent("player_jump");
          //((SoundEmitter*)player->GetComponent(CT_SoundEmitter))->Play("CB/player_jump", false);

          if (this->playerController->getJumpState() == JumpStates::JS_Grounded)
          {
              PlayerSprite->GetCurrentSprite()->PauseAt(4);
          }

          playerController->PressJump();
    }
    
    else if (GivenInput->KeyReleased == true)
    {
        PlayerSprite->GetCurrentSprite()->PauseAt(5);
        playerController->ReleaseJump();
    }
    else if (GivenInput->KeyDown)
    {
      //Not getting triggered, how do Pressed/Released/Down work?
      playerController->HoldJump();
      //Could put jetpack particle stuff here, but it should probably go in PlayerController.
    }

    break;

      
  case BUTTONS_X:
      if (GivenInput->KeyPressed)
      {
          GlobalSystem->SendActivate(this->player->GetTransform()->GetWorldPosition());
      }

		break;

	case BUTTONS_B:			

		break;
      
	case BUTTONS_Y:
    break;

  case BUTTONS_RIGHT_SHOULDER:
  {
    if (GivenInput->KeyPressed == true)
    {

      if (abs(GivenInput->rightStick.first) > 0.1f || abs(GivenInput->rightStick.second) > 0.1f)
      {
        float theta = atan2f(GivenInput->rightStick.second, GivenInput->rightStick.first); //Mathf.Atan2(hackVector.y, hackVector.x);
        // theta = MathF::RadToDegree(theta);

        playerController->ControllerFireLaser(theta);
      }
      else
      {
        //Right stick not pressed, launch in direction we're currently facing
        playerController->KBFireLaser(PlayerTransform->GetScale().x > 0);
      }
    }
    break;
  }

  case BUTTONS_LEFT_SHOULDER:
      //this->ParentObject->GetTransform()->GetWorldRotation().z += 2 * GivenInput->time;
    break;
    
  case BUTTONS_BACK:
      PostQuitMessage(0);
    break;
    
  case BUTTONS_START:
     if(gGameStateCurr == GS_LEVEL)
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
    //LEFT / -1.0f

    //PlayerBody->velocity.x -= 2.5f * GivenInput->time;

    //if (PlayerBody->velocity.x <= -10.0f)
    //{
    //  PlayerBody->velocity.x = -10.0f;
    //}

      if (this->playerController->getJumpState() == JumpStates::JS_Grounded || this->playerController->getJumpState() == JumpStates::JS_PLATFORM)
      {
          PlayerSprite->GetCurrentSprite()->Set_Paused(false);

        if(this->playerController->WalkTimer > 0.4 && this->playerController->getJumpTimer() < BIT_MORE_THAN_ONE_FRAME)
        {
          //((SoundEmitter*)player->GetComponent(CT_SoundEmitter))->StopEvent("player_footsteps");
          //((SoundEmitter*)player->GetComponent(CT_SoundEmitter))->Play("CB/player_footsteps", false);
          this->playerController->WalkTimer = 0.0f;
        }
      }

      if (GivenInput->KeyPressed == true || GivenInput->KeyDown == true)
      {
        PlayerSprite->ChangeState("running");
        PlayerBody->velocity.x = -(*playerController->playerRunSpeed);
        
        //Make the player face left
        if (PlayerTransform->GetScale().x > 0)
        {
          PlayerTransform->GetScale().x *= -1.0f;
        }
      }

      if (GivenInput->KeyReleased == true)
      {
        PlayerSprite->ChangeState("idle");
        PlayerBody->velocity.x = 0.0f;
      }
    
      break;

    
  case DPAD_RIGHT:

    //PlayerBody->velocity.x += 2.5f * GivenInput->time;

   // if (PlayerBody->velocity.x >= 10.0f)
   // {
   //   PlayerBody->velocity.x = 10.0f;
   // }

          // if the player is grounded and the player is pressing right, play move sound effect
      if (this->playerController->getJumpState() == JumpStates::JS_Grounded || this->playerController->getJumpState() == JumpStates::JS_PLATFORM)
      {
         PlayerSprite->GetCurrentSprite()->Set_Paused(false);

        if (this->playerController->WalkTimer > 0.4 && this->playerController->getJumpTimer() < BIT_MORE_THAN_ONE_FRAME)
        {
          //((SoundEmitter*)player->GetComponent(CT_SoundEmitter))->StopEvent("player_footsteps");
          //((SoundEmitter*)player->GetComponent(CT_SoundEmitter))->Play("CB/player_footsteps", false);
          this->playerController->WalkTimer = 0.0f;
        }
      }

      // change sprite and flip sprite if necessary
      if (GivenInput->KeyPressed == true || GivenInput->KeyDown == true)
      {
        PlayerSprite->ChangeState("running");
        PlayerBody->velocity.x = *playerController->playerRunSpeed;
        
        if (PlayerTransform->GetScale().x < 0)
        {
          //Face the player right
          PlayerTransform->GetScale().x *= -1.0f; 
        }
      }

      // revert changes when the key is released
      if (GivenInput->KeyReleased == true)
      {
        PlayerSprite->ChangeState("idle");
        PlayerBody->velocity.x = 0.0f;
      }
      
      break;

    
  case DPAD_UP:
    //this->ParentObject->GetTransform()->GetWorldPosition().y += 15 * GivenInput->time;
    break;
    
  case DPAD_DOWN:
		//PlayerBody->velocity.y -= 2.5f * GivenInput->time;
    //this->ParentObject->GetTransform()->GetWorldPosition().y -= 15 * GivenInput->time;
    break;
    
  case LEFT_STICK:
		break;
    
  case RIGHT_STICK:
    break;
  }

}

void Throw();