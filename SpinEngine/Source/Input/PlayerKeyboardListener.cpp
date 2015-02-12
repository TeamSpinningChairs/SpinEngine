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
#include "KeyBoard.h"
#include "FactoryAccess.h"
#include "SpriteRenderer.h"
#include "PlayerController.h"
#include "PlayerKeyboardListener.h"
#include "SoundEmitter.h"
#include "GameStateManager.h"
#include "WallListener.h"
#include "GraphicsManager.h"
#include "SoundInstance.h"

PlayerKeyboardListener::PlayerKeyboardListener(IEntity *playerObj, PlayerController *parent) :
  playerController(parent), player(playerObj)
{
  KeyReleasedDuringPause = false;
}

void PlayerKeyboardListener::Initialize()
{
  PlayerBody = reinterpret_cast<RigidBody *>(player->GetComponent(CT_Body));
  PlayerSprite = reinterpret_cast<SpriteRenderer *>(player->GetComponent(CT_SpriteRenderer));
  PlayerTransform = reinterpret_cast<Transform *>(player->GetComponent(CT_TransformComponent));
}

void PlayerKeyboardListener::OnMessageRecieved(Message * SentMessage)
{
  if (!SentMessage)
  {
    MessageBox(NULL, "PlayerKeyboardListener got a null message? What?", NULL, NULL);
    return;
  }
  
  //Okay, this function is registered and hangs out inside the PlayerController.
  KeyMessage * GivenInput = reinterpret_cast<KeyMessage *>(SentMessage);

  if(gGameStatePaused == true)
  {
      if (GivenInput->KeyNumber == VK_LEFT || GivenInput->KeyNumber == VK_RIGHT || GivenInput->KeyNumber == 'A' || GivenInput->KeyNumber == 'D')
    {
      if(GivenInput->KeyStatus == KEY_RELEASED)
      {
        KeyReleasedDuringPause = true;
      }
    }

    return;
  }

  if(KeyReleasedDuringPause == true)
  {
      KeyReleasedDuringPause = false;
      PlayerSprite->ChangeState("idle");
      PlayerBody->velocity.x = 0.0f;
  }


  switch( GivenInput->KeyNumber)
  {

  case 'W':
      if (GivenInput->KeyStatus == KEY_STATE::KEY_TRIGGERED)
      {

          if (this->playerController->getJumpState() == JumpStates::JS_Grounded)
          {
            //((SoundEmitter*)player->GetComponent(CT_SoundEmitter))->SetVolume(1.0f, "player_jump");
            //((SoundEmitter*)player->GetComponent(CT_SoundEmitter))->StopEvent("player_jump");
            ((SoundEmitter*)player->GetComponent(CT_SoundEmitter))->Play("CB/player_jump", false);
              PlayerSprite->GetCurrentSprite()->PauseAt(4);
          }
          
          playerController->PressJump();
      }

      else if (GivenInput->KeyStatus == KEY_STATE::KEY_RELEASED)
      {
          playerController->ReleaseJump();
          PlayerSprite->GetCurrentSprite()->PauseAt(5);
      }
      break;    
  
  case 'A':
      if (this->playerController->getJumpState() == JumpStates::JS_Grounded || this->playerController->getJumpState() == JumpStates::JS_PLATFORM)
      {
          PlayerSprite->GetCurrentSprite()->Set_Paused(false);

          if (this->playerController->WalkTimer > 0.4 && this->playerController->getJumpTimer() < BIT_MORE_THAN_ONE_FRAME)
          {
              //((SoundEmitter*)player->GetComponent(CT_SoundEmitter))->StopEvent("player_footsteps");
              ((SoundEmitter*)player->GetComponent(CT_SoundEmitter))->Play("CB/player_footsteps", false);
              this->playerController->WalkTimer = 0.0f;
          }
      }

      if (GivenInput->KeyStatus == KEY_TRIGGERED || GivenInput->KeyStatus == KEY_DOWN)
      {
          PlayerSprite->ChangeState("running");
          PlayerBody->velocity.x = -(*playerController->playerRunSpeed);

          //Make the player face left
          if (PlayerTransform->GetScale().x > 0)
          {
              PlayerTransform->GetScale().x *= -1.0f;
          }
      }

      if (GivenInput->KeyStatus == KEY_RELEASED)
      {
          PlayerSprite->ChangeState("idle");
          PlayerBody->velocity.x = 0.0f;
      }

      break; 
  
  case 'D':

      // if the player is grounded and the player is pressing right, play move sound effect
      if (this->playerController->getJumpState() == JumpStates::JS_Grounded || this->playerController->getJumpState() == JumpStates::JS_PLATFORM)
      {
          PlayerSprite->GetCurrentSprite()->Set_Paused(false);

          if (this->playerController->WalkTimer > 0.4 && this->playerController->getJumpTimer() < BIT_MORE_THAN_ONE_FRAME)
          {
              //((SoundEmitter*)player->GetComponent(CT_SoundEmitter))->StopEvent("player_footsteps");
              ((SoundEmitter*)player->GetComponent(CT_SoundEmitter))->Play("CB/player_footsteps", false);
              this->playerController->WalkTimer = 0.0f;
          }
      }

      // change sprite and flip sprite if necessary
      if (GivenInput->KeyStatus == KEY_TRIGGERED || GivenInput->KeyStatus == KEY_DOWN)
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
      if (GivenInput->KeyStatus == KEY_RELEASED)
      {
          PlayerSprite->ChangeState("idle");
          PlayerBody->velocity.x = 0.0f;
      }
    break;

    case 'E':
	    if (GivenInput->KeyStatus == KEY_TRIGGERED)
	    {
          GlobalSystem->SendActivate(this->player->GetTransform()->GetPosition());
	    }
	    break;

      
    case '1':
        if (GivenInput->KeyStatus == KEY_TRIGGERED)
        {
            WallMessage WallMSG(Wall_MESSAGE_TYPES::DEACTIVATE);

            GlobalSystem->MyDispatchMessage(&WallMSG);
        }
        break;

    case 'T':
        if (GivenInput->KeyStatus == KEY_TRIGGERED)
        {
          /*
            IEntity *splashscreenObj;
            SpriteRenderer* splashRend;

            splashscreenObj = new IEntity(GlobalFactory->GetGameObjectCount(), player->GetTransform()->GetPosition(), Vector3D());
            splashRend = new SpriteRenderer(splashscreenObj, GlobalFactory->GetDevice(), d3dColors::White);
            splashRend->AddSprite("Assets\\Textures\\Text_To_Screen2.png", 0.0, 127, 128, 1.0002f, "idle", true);
            splashscreenObj->AddGameComponent(CT_SpriteRenderer, splashRend);
            GlobalFactory->GetGraphicsManager()->AddSpriteRend(splashRend);
            GlobalFactory->AddGameObject(splashscreenObj);
            //splashscreenObj->GetTransform()->GetScale().x *= 1024;
            //splashRend->GetCurrentSprite()->PauseAt('A');
            */
            
        }
        break;
      
    case VK_UP:
      if (GivenInput->KeyStatus == KEY_STATE::KEY_TRIGGERED)
      {
          //((SoundEmitter*)player->GetComponent(CT_SoundEmitter))->SetVolume(1.0f, "player_jump");
          //((SoundEmitter*)player->GetComponent(CT_SoundEmitter))->StopEvent("player_jump");
          ((SoundEmitter*)player->GetComponent(CT_SoundEmitter))->Play("CB/player_jump", false);

          if (this->playerController->getJumpState() == JumpStates::JS_Grounded)
          {
              PlayerSprite->GetCurrentSprite()->PauseAt(4);
          }

        playerController->PressJump();
      }
    
      else if (GivenInput->KeyStatus == KEY_STATE::KEY_RELEASED)
      {
          PlayerSprite->GetCurrentSprite()->PauseAt(5);
          playerController->ReleaseJump();
      }
      else if (GivenInput->KeyStatus == KEY_STATE::KEY_DOWN)
      {
        //Not getting triggered, how do Pressed/Released/Down work?
        playerController->HoldJump();
        //Could put jetpack particle stuff here, but it should probably go in PlayerController.
      }

      //((SoundEmitter*)player->GetComponent(CT_SoundEmitter))->SetPause(true, "music_escape");
      break;
    
    case VK_DOWN: 
      //((SoundEmitter*)player->GetComponent(CT_SoundEmitter))->SetPause(false, "music_escape");
      //// For testing
      //if (GivenInput->KeyStatus == KEY_TRIGGERED || GivenInput->KeyStatus == KEY_DOWN)
      //{
      //PlayerSprite->ChangeState("running");
      //PlayerBody->velocity.y = -(*playerController->playerRunSpeed);
      //}
      break;
    
    case VK_LEFT:
        if (this->playerController->getJumpState() == JumpStates::JS_Grounded || this->playerController->getJumpState() == JumpStates::JS_PLATFORM)
      {
          PlayerSprite->GetCurrentSprite()->Set_Paused(false);

        if(this->playerController->WalkTimer > 0.4 && this->playerController->getJumpTimer() < BIT_MORE_THAN_ONE_FRAME)
        {
          //((SoundEmitter*)player->GetComponent(CT_SoundEmitter))->StopEvent("player_footsteps");
          ((SoundEmitter*)player->GetComponent(CT_SoundEmitter))->Play("CB/player_footsteps", false);
          this->playerController->WalkTimer = 0.0f;
        }
      }

      if (GivenInput->KeyStatus == KEY_TRIGGERED || GivenInput->KeyStatus == KEY_DOWN)
      {
        PlayerSprite->ChangeState("running");
        PlayerBody->velocity.x = -(*playerController->playerRunSpeed);
        
        //Make the player face left
        if (PlayerTransform->GetScale().x > 0)
        {
          PlayerTransform->GetScale().x *= -1.0f;
        }
      }

      if (GivenInput->KeyStatus == KEY_RELEASED)
      {
        PlayerSprite->ChangeState("idle");
        PlayerBody->velocity.x = 0.0f;
      }
    
      break;

    case VK_RIGHT:
      
      // if the player is grounded and the player is pressing right, play move sound effect
        if (this->playerController->getJumpState() == JumpStates::JS_Grounded || this->playerController->getJumpState() == JumpStates::JS_PLATFORM)
      {
         PlayerSprite->GetCurrentSprite()->Set_Paused(false);

        if (this->playerController->WalkTimer > 0.4 && this->playerController->getJumpTimer() < BIT_MORE_THAN_ONE_FRAME)
        {
          //((SoundEmitter*)player->GetComponent(CT_SoundEmitter))->StopEvent("player_footsteps");
          ((SoundEmitter*)player->GetComponent(CT_SoundEmitter))->Play("CB/player_footsteps", false);
          this->playerController->WalkTimer = 0.0f;
        }
      }

      // change sprite and flip sprite if necessary
      if (GivenInput->KeyStatus == KEY_TRIGGERED || GivenInput->KeyStatus == KEY_DOWN)
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
      if (GivenInput->KeyStatus == KEY_RELEASED)
      {
        PlayerSprite->ChangeState("idle");
        PlayerBody->velocity.x = 0.0f;
      }
      
      break;

  case VK_SPACE:
    	if (GivenInput->KeyStatus == KEY_TRIGGERED)
	    {
          GlobalSystem->SendActivate(this->player->GetTransform()->GetPosition());
	    }
      //Holding spacebar: SHOOT LASERS
      if (GivenInput->KeyStatus == KEY_DOWN)
      {
        playerController->KBFireLaser(PlayerTransform->GetScale().x > 0);
      }


       break;

  case VK_CONTROL:
      if (GivenInput->KeyStatus == KEY_TRIGGERED)
      {
          GlobalSystem->SendActivate(this->player->GetTransform()->GetPosition());
      }
      break;

  case VK_RETURN:
    if (GivenInput->KeyStatus == KEY_TRIGGERED)
    {
      GlobalSystem->SendActivate(this->player->GetTransform()->GetPosition());
    }
    break;
    //((SoundEmitter*)player->GetComponent(CT_SoundEmitter))->SetVolume(0.1f, "music_escape");
    
    /*
    if (GivenInput->KeyStatus == KEY_STATE::KEY_TRIGGERED)
    {
      //((SoundEmitter*)player->GetComponent(CT_SoundEmitter))->SetVolume(0.1f, "Jump");
      playerController->PressJump();
    }
    
    else if (GivenInput->KeyStatus == KEY_STATE::KEY_RELEASED)
      playerController->ReleaseJump();
	  */
    //break;

    //Test rotation/scaling cases (to double-check anchor points)
    //Finished testing (anchor points for rotation and scaling are at
    //the center of the sprite), so feel free to erase later on.

    //Rotate left/right
  /*case 'U':
    PlayerTransform->GetRotation().z += 1.0f / 12.0f;
    break;
  case 'I':
    PlayerTransform->GetRotation().z -= 1.0f / 12.0f;
    break;
    //scale up/down
  case 'Y':
    PlayerTransform->GetScale().y += 1.0f / 12.0f;
    break;
  case 'H':
    PlayerTransform->GetScale().y -= 1.0f / 12.0f;
    break;*/
  }
}

