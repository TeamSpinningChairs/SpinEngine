/****************************************************************************/
/*!
    \file   NewMessageSystem.cpp
    \author Steven Gallwas
    \par    email: s.gallwas\@digipen.edu
    \par    Course: GAM 200
    \date   9/16/14
    \brief  
        This file contains the implementation for the base messaging system.

    Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#include "Precompiled.h"
#include "InputHandler.h"
#include "DebugMessages.h"
#include "Core\NewMessageSystem.h"
#include "InputPlayback.h"
#include "InputRecord.h"

InputMessage::InputMessage(void)
{
	time = 0.0f;
	Controller_Num = 0;
	KeyDown = false;
	KeyPressed = false;
	KeyReleased = false;
	Key_Hold_Time = 0;
	leftStick.first = 0.0f;
	leftStick.second = 0.0f;
	MessageType = MESSAGE_INPUT;
	rightStick.first = 0.0f;
	rightStick.second = 0.0f;

  LeftStickState = STICK_NEUTRAL;
  RightStickState = STICK_NEUTRAL;
  LeftTriggerState = TRIGGER_NEUTRAL;
  RightTriggerState = TRIGGER_NEUTRAL;
}

Button_Status::Button_Status(void)
{
  ButtonDown = false;
  TimeHeld = 0;
}

Controller_Status::Controller_Status(void) : Buttons(ButtonCount), 
											 LeftStick(0.0f,0.0f), RightStick(0.0f,0.0f)
{
  RightTrigger = 0.0f;
  LeftTrigger = 0.0f;
}

/*************************************************************************/
/*!
  \brief
    Constructor for input handler
*/
/*************************************************************************/
InputHandler::InputHandler(void) : Active_Controllers(XUSER_MAX_COUNT),
									Controllers(XUSER_MAX_COUNT),
									Current_States(XUSER_MAX_COUNT),
									XInputDefines(ButtonCount),
                  playback(), recording(), SticksPast(XUSER_MAX_COUNT), SticksPresent(XUSER_MAX_COUNT)
{
  this->Controllers[0].ControllerNum = 0;
  this->Controllers[0].ControllerNum = 1;
  this->Controllers[0].ControllerNum = 2;
  this->Controllers[0].ControllerNum = 3;

  Active_Controllers[0] = false;
  Active_Controllers[1] = false;
  Active_Controllers[2] = false;
  Active_Controllers[3] = false;

  this->XInputDefines[BUTTONS_A] = XINPUT_GAMEPAD_A;
  this->XInputDefines[BUTTONS_X] = XINPUT_GAMEPAD_X;
  this->XInputDefines[BUTTONS_B] = XINPUT_GAMEPAD_B;
  this->XInputDefines[BUTTONS_Y] = XINPUT_GAMEPAD_Y;
  this->XInputDefines[BUTTONS_RIGHT_SHOULDER] = XINPUT_GAMEPAD_RIGHT_SHOULDER;
  this->XInputDefines[BUTTONS_LEFT_SHOULDER] = XINPUT_GAMEPAD_LEFT_SHOULDER;
  this->XInputDefines[BUTTONS_BACK] = XINPUT_GAMEPAD_BACK;
  this->XInputDefines[BUTTONS_START] = XINPUT_GAMEPAD_START;
  this->XInputDefines[DPAD_LEFT] = XINPUT_GAMEPAD_DPAD_LEFT;
  this->XInputDefines[DPAD_RIGHT] = XINPUT_GAMEPAD_DPAD_RIGHT;
  this->XInputDefines[DPAD_UP] = XINPUT_GAMEPAD_DPAD_UP;
  this->XInputDefines[DPAD_DOWN] = XINPUT_GAMEPAD_DPAD_DOWN;
  this->XInputDefines[LEFT_STICK] = XINPUT_GAMEPAD_LEFT_THUMB;
  this->XInputDefines[RIGHT_STICK] = XINPUT_GAMEPAD_RIGHT_THUMB;
  
  AutoPlay = false;
  ZeroMemory(&CurrentControls, sizeof(XINPUT_STATE));
  AutoTimer = AutoDelay;
}

/*************************************************************************/
/*!
\brief
Updates the current state of a given button for all controllers.
Called in the update function

\param ButtonType
the type of button to check.

*/
/*************************************************************************/
void InputHandler::Update(float dt)
{
    if (AutoPlay)
    {
        AutoTimer += dt;
    }
    
    // set the past button and stick states to the current
    this->Update_Controller_States();

    // get the new states of each button and sticks and triggers
    this->Update_Active_Controllers();
    
    this->NormalizeSticks();
    

    // based on changes in button states, send messages
    this->SendButtonMessages(dt);

    // basd on chanes to the sticks states, send messages
    SendStickandTriggerMessages(dt);
}


/*************************************************************************/
/*!
  \brief
    set the GlobalSystem pointer to a new messagesystem. should only
    be called once
*/
/*************************************************************************/
void InputHandler::Update_Active_Controllers(void)
{
  // if the game is in autoplay mode generate random input
	if (AutoPlay)
	{
		if (AutoTimer >= AutoDelay)
		{
			// call function to generate random input
			GenerateRandomInput(this->CurrentControls.Gamepad);
			AutoTimer = 0.0f;
		}

		Current_States[0] = CurrentControls;
		return;
	}

  // if the game is in playback mode, read input from a file
	else if (*playback.enabled_)
	{
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		playback.GetNextInput(state.Gamepad);
		Current_States[0] = state;
		return;
	}

   // variable to track if a controller exits
  DWORD ControllerResult;    

  // check the status of four controllers
  for (DWORD i=0; i< XUSER_MAX_COUNT; i++)
  {
    // if the controller is active get its current state
	  if (this->Active_Controllers[i] == true)
	  {
		  XINPUT_STATE state;
		  ZeroMemory(&state, sizeof(XINPUT_STATE));

		  // get the state for the current controller
		  ControllerResult = XInputGetState(i, &state);	

      // store the current button imputs for the designated controller
      if(ControllerResult == ERROR_SUCCESS)
      {
        // store the most recent controller states in the current states vector
        this->Current_States[i] = state;

        // if recording is enabled, write the current controller state to a file
		    if (*recording.enabled_)
		    {
			    recording.Record(state.Gamepad);
		    }
      }
    
    } // end of if check
  
  }   // end of for loop

}     // end of function

/*************************************************************************/
/*!
  \brief
    Checks to see which controllers are currently connected.
*/
/*************************************************************************/
void InputHandler::Check_for_Controllers(void)
{
  // if the game is in autoplay mode, set one controller to active
	if (*playback.enabled_ || AutoPlay)
	{
		this->Active_Controllers[0] = true;
		return;
	}

  // variable to track if a controller exists
  DWORD ControllerResult;    
  
  int ControllersActive = 0;

  // check the status of four controllers
  for (DWORD i=0; i< XUSER_MAX_COUNT; i++)
  {
    XINPUT_STATE state;
    ZeroMemory(&state, sizeof(XINPUT_STATE));

    // get the state for the current controller
    ControllerResult = XInputGetState(i, &state);

    // toggle the boolean flag in the list of controllers 
    //for the current controller if active
    if(ControllerResult == ERROR_SUCCESS)
    {
      ControllersActive++;
      this->Active_Controllers[i] = true;
      //OutputDebugString("\n\nController Connected\n\n");
    }
  }

  // if there is no controller connected, print a statement, we can use this later to detect when a controller is disconnected.
  /*
  if(ControllersActive == 0)
  {
    //SendDebugMessage("No Controller Connected");
  }

  else
  {
    for(int i = 0; i < 4; i++)
    {
      std::cout<< Active_Controllers[i] << std::endl;
    }
  }
  */

}

/*************************************************************************/
/*!
  \brief
    set the GlobalSystem pointer to a new messagesystem. should only
    be called once
*/
/*************************************************************************/
void InputHandler::Update_Controller_States(void)
{
  for(int i = 0; i < XUSER_MAX_COUNT; i++)
  {
    if(this->Active_Controllers[i] != true)
      continue;
    
    else
      this->UpdateSticks_and_Triggers(i);
  }

  for(int i = 0; i < ButtonCount; i++)
  {    
    this->UpdateButton(i);
  }
}

/*************************************************************************/
/*!
  \brief
    Updates the current state of a given button for all controllers.
    Called in the update function
  
  \param ButtonType
    the type of button to check.
  
*/
/*************************************************************************/
void InputHandler::UpdateSticks_and_Triggers(int ControllerNum)
{
    for (int i = 0; i < XUSER_MAX_COUNT; ++i)
    {
        if (this->Active_Controllers[i] != true)
            continue;

        SticksPast[i].LeftStick = SticksPresent[i].LeftStick;
        SticksPast[i].RightStick = SticksPresent[i].RightStick;
        SticksPast[i].Triggers = SticksPresent[i].Triggers;
    }


}

/*************************************************************************/
/*!
  \brief
    Updates the current state of a given button for all controllers.
    Called in the update function
  
  \param ButtonType
    the type of button to check.
  
*/
/*************************************************************************/
void InputHandler::UpdateButton(int ButtonType)
{
  for(int i = 0; i < XUSER_MAX_COUNT; i++)
  {
    if(this->Active_Controllers[i] != true)
        continue;

    if (this->Current_States[i].Gamepad.wButtons & XInputDefines[ButtonType])
    {
        this->Controllers[i].Buttons[ButtonType].ButtonDown = true;
    }

    else
    {
        this->Controllers[i].Buttons[ButtonType].ButtonDown = false;
    }

  }
}

/*************************************************************************/
/*!
  \brief
    Updates the current state of a given button for all controllers.
    Called in the update function
  
  \param ButtonType
    the type of button to check.
  
*/
/*************************************************************************/
void InputHandler::SendButtonMessages(float time)
{
	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		if (this->Active_Controllers[i] != true)
			continue;

    InputMessage ButtonMessage;

    /*
        add the current position for each of the sticks to 
        the message in case the button messages need the information 
    */

    ButtonMessage.rightStick = SticksPresent[i].RightStick;

    ButtonMessage.leftStick = SticksPresent[i].LeftStick;

    // loop through all of the buttons, and send messages if any changes were made
		for (int j = 0; j < ButtonCount; j++)
		{
			if (this->ButtonTriggered(i, j))
				ButtonMessage.KeyPressed = true;
			
			else if (this->ButtonDown(i, j))
				ButtonMessage.KeyDown = true;
			
			else if (this->ButtonReleased(i, j))
				ButtonMessage.KeyReleased = true;
			
			else
				continue;

      ButtonMessage.TriggeredKey = j;

      ButtonMessage.time = time;
			
      ButtonMessage.Controller_Num = i;

      GlobalSystem->MyDispatchMessage(&ButtonMessage);
		}
	
	}
}

/*************************************************************************/
/*!
\brief
    
*/
/*************************************************************************/
void InputHandler::SendStickandTriggerMessages(float time)
{
    for (int i = 0; i < XUSER_MAX_COUNT; i++)
    {
        if (this->Active_Controllers[i] != true)
            continue;

        InputMessage ButtonMessage;
        ButtonMessage.TriggeredKey = -1;
        ButtonMessage.Controller_Num = i;

        //CheckControllerStates(i, ButtonMessage);

        if (this->CheckControllerStates(i, ButtonMessage) != 4)
        {
            ButtonMessage.leftStick = SticksPresent[i].LeftStick;

            ButtonMessage.rightStick = SticksPresent[i].RightStick;

            ButtonMessage.time = time;

            ButtonMessage.Triggers = SticksPresent[i].Triggers;

            ButtonMessage.time = time;

            GlobalSystem->MyDispatchMessage(&ButtonMessage);
        }
    }
}

/*************************************************************************/
/*!
  \brief
    Updates the current state of a given button for all controllers.
    Called in the update function
  
  \param ButtonType
    the type of button to check.
  
*/
/*************************************************************************/
bool InputHandler::ButtonTriggered(int ControllerNum, int ButtonType)
{
  //check the button is currently down
  if(this->Current_States[ControllerNum].Gamepad.wButtons & XInputDefines[ButtonType])
  {  
    //check that the button was not down last frame
    if(this->Controllers[ControllerNum].Buttons[ButtonType].ButtonDown == false)
      return true;
  }
      return false;
}

/*************************************************************************/
/*!
  \brief
    Updates the current state of a given button for all controllers.
    Called in the update function
  
  \param ButtonType
    the type of button to check.
  
*/
/*************************************************************************/
bool InputHandler::ButtonDown(int ControllerNum, int ButtonType)
{
  // if the button is down return true
  if(this->Current_States[ControllerNum].Gamepad.wButtons & XInputDefines[ButtonType])
    return true;
  
  return false;
}

/*************************************************************************/
/*!
  \brief
    Updates the current state of a given button for all controllers.
    Called in the update function
  
  \param ButtonType
    the type of button to check.
  
*/
/*************************************************************************/
bool InputHandler::ButtonReleased(int ControllerNum, int ButtonType)
{
    //check the button is currently down return false if it is
    if (this->Current_States[ControllerNum].Gamepad.wButtons & XInputDefines[ButtonType])
    {
        return false;
    }
    
    //check if the button was down last frame if so return true
    if(this->Controllers[ControllerNum].Buttons[ButtonType].ButtonDown == true)
        return true;

    // default return statement
    return false;
}

/*************************************************************************/
/*!
  \brief
    Updates the current state of a given button for all controllers.
    Called in the update function
  
  \param ButtonType
    the type of button to check.
  
*/
/*************************************************************************/
bool InputHandler::Initialize()
{
    playback.Initialize();
    recording.Initialize();
    
    XInputEnable(true);
    this->Check_for_Controllers();

    return true;
}
	

/*************************************************************************/
/*!
\brief
	doesn't do anything right now
	
*/
/*************************************************************************/
void InputHandler::Shutdown()
{
    return;
}

/*************************************************************************/
/*!
\brief


*/
/*************************************************************************/
void InputHandler::NormalizeSticks(void)
{
  for(int i = 0; i < XUSER_MAX_COUNT; i++)
  {
    // check if the controller is active
    if(this->Active_Controllers[i] != true)
      continue;

      //get the current xy coordinates for the thumbsticks
	    float LeftX = Current_States[i].Gamepad.sThumbLX;
	    float LeftY = Current_States[i].Gamepad.sThumbLY;
  
      float RightX = Current_States[i].Gamepad.sThumbRX;
      float RightY = Current_States[i].Gamepad.sThumbRY;


	    //determine how far the controllers are pushed
	    float magnitudeLeft = sqrt(LeftX*LeftX + LeftY*LeftY);

      float magnitudeRight = sqrt(RightX*RightX + RightY*RightY);

	    //determine the direction the sticks are pushed
	    float normalizedLX = LeftX / magnitudeLeft;
	    float normalizedLY = LeftY / magnitudeLeft;

      float normalizedRX = RightX / magnitudeRight;
      float normalizedRY = RightY / magnitudeRight;

	    float normalizedMagnitudeLeft = 0.0f;
      float normalizedMagnitudeRight = 0.0f;

	    //check if the controller is outside a circular dead zone
	    if (magnitudeLeft > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	    {
		    //clip the magnitude to its expected maximum value
        if (magnitudeLeft > 32767)
        {
            magnitudeLeft = 32767;
        }

		    //adjust magnitude relative to the end of the dead zone
		    magnitudeLeft -= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

		    //normalize the magnitude with respect to its expected range giving a magnitude value of 0.0 to 1.0
		    normalizedMagnitudeLeft = magnitudeLeft / (32767 - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	    }
	
	    else //if the stick position is in the deadzone zero out the magnitude
	    {
		    magnitudeLeft = 0.0f;
		    normalizedMagnitudeLeft = 0.0f;
	    }

      //check if the controller is outside a circular dead zone
      if (magnitudeRight > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
      {
          //clip the magnitude to its expected maximum value
          if (magnitudeRight > 32767)
          {
              magnitudeRight = 32767;
          }

          //adjust magnitude relative to the end of the dead zone
          magnitudeRight -= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;

          //normalize the magnitude with respect to its expected range giving a magnitude value of 0.0 to 1.0
          normalizedMagnitudeRight = magnitudeRight / (32767 - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
      }

      else //if the controller is in the deadzone zero out the magnitude
      {
          magnitudeRight = 0.0f;
          normalizedMagnitudeRight = 0.0f;
      }

      float test = normalizedLX * normalizedMagnitudeLeft;

      
      // store the normalized stick values in the stick present structs. 
      SticksPresent[i].LeftStick.first = normalizedLX * normalizedMagnitudeLeft;
      SticksPresent[i].LeftStick.second = normalizedLY * normalizedMagnitudeLeft;
      
      SticksPresent[i].RightStick.first = normalizedRX * normalizedMagnitudeRight;
      SticksPresent[i].RightStick.second = normalizedRY * normalizedMagnitudeRight;

      if (Current_States[i].Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
      {
          SticksPresent[i].Triggers.first = Current_States[i].Gamepad.bLeftTrigger;
      }
      else
      {
          SticksPresent[i].Triggers.first = 0.0;
      }

      if (Current_States[i].Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
      {
          SticksPresent[i].Triggers.second = Current_States[i].Gamepad.bRightTrigger;
      }
      else
      {
          SticksPresent[i].Triggers.second = 0.0;
      }
  }
}

/*************************************************************************/
/*!
\brief


*/
/*************************************************************************/
void InputHandler::GenerateRandomInput(XINPUT_GAMEPAD &input)
{
	int choice;

	choice = rand() % ButtonCount;

	if (choice == BUTTONS_BACK)
	{
		// skip the close button
		while (choice == BUTTONS_BACK)
		{
			choice = rand() % ButtonCount;
		}
	}

	input.wButtons = XInputDefines[choice];
}

/*************************************************************************/
/*!
\brief


*/
/*************************************************************************/
void InputHandler::UninitializeAnyPlayback()
{
  if (*playback.enabled_)
    playback.UninitializePlayback();
}

/*************************************************************************/
/*!
\brief


*/
/*************************************************************************/
void InputHandler::StopAnyRecording()
{
  if (*recording.enabled_)
    recording.EndRecording();
}

/*************************************************************************/
/*!
\brief


*/
/*************************************************************************/
void InputHandler::InitializeAnyPlayback(std::string levelname)
{
  if (*playback.enabled_)
    playback.InitializePlayback(levelname + "Playback.json");
}

/*************************************************************************/
/*!
\brief


*/
/*************************************************************************/
void InputHandler::StartAnyRecording(std::string levelname)
{
  if (*recording.enabled_)
    recording.InitializeRecording(levelname);
}

/*************************************************************************/
/*!
\brief


*/
/*************************************************************************/
bool InputHandler::StickTriggered(int ControllerNum, int Check)
{
    if (Check == CONTROLLER_DIR::LEFT)
    {
        if (SticksPresent[ControllerNum].LeftStick.first && !SticksPast[ControllerNum].LeftStick.first)
        {
            return true;
        }

        else if (SticksPresent[ControllerNum].LeftStick.second && !SticksPast[ControllerNum].LeftStick.second)
        {
            return true;
        }

        return false;
    }

    else if (Check == CONTROLLER_DIR::RIGHT)
    {
        if (SticksPresent[ControllerNum].RightStick.first && !SticksPast[ControllerNum].RightStick.first)
        {
            return true;
        }

        else if (SticksPresent[ControllerNum].RightStick.second && !SticksPast[ControllerNum].RightStick.second)
        {
            return true;
        }

        return false;
    }

    return false;
}

/*************************************************************************/
/*!
\brief


*/
/*************************************************************************/
bool InputHandler::StickReleased(int ControllerNum, int Check)
{
    if (Check == CONTROLLER_DIR::LEFT)
    {
        if (SticksPresent[ControllerNum].LeftStick == std::pair<float, float>(0.0f, 0.0f) && SticksPast[ControllerNum].LeftStick != std::pair<float, float>(0.0f, 0.0f))
        {
            return true;
        }
        return false;
    }

    if (Check == CONTROLLER_DIR::RIGHT)
    {
        if (SticksPresent[ControllerNum].RightStick == std::pair<float, float>(0.0f, 0.0f) && SticksPast[ControllerNum].RightStick != std::pair<float, float>(0.0f, 0.0f))
        {
            return true;
        }
        return false;
    }

    return false;
}

/*************************************************************************/
/*!
\brief


*/
/*************************************************************************/
bool InputHandler::StickDown(int ControllerNum, int Check)
{
  if (this->Active_Controllers[ControllerNum] != true)
    false;

    if (Check == CONTROLLER_DIR::LEFT)
    {
        if (SticksPresent[ControllerNum].LeftStick.first)
        {
            return true;
        }

        else if (SticksPresent[ControllerNum].LeftStick.second)
        {
            return true;
        }

        return false;
    }

    else if (Check == CONTROLLER_DIR::RIGHT)
    {
        if (SticksPresent[ControllerNum].RightStick.first)
        {
            return true;
        }

        else if (SticksPresent[ControllerNum].RightStick.second)
        {
            return true;
        }

        return false;
    }

    return false;

}

/*************************************************************************/
/*!
\brief


*/
/*************************************************************************/
bool InputHandler::TriggerTriggered(int ControllerNum, int Check)
{
  if (this->Active_Controllers[ControllerNum] != true)
    false;

    if (Check == CONTROLLER_DIR::LEFT)
    {
        if (SticksPresent[ControllerNum].Triggers.first && !SticksPast[ControllerNum].Triggers.first)
        {
            return true;
        }

        return false;
    }

    else if (Check == CONTROLLER_DIR::RIGHT)
    {
        if (SticksPresent[ControllerNum].Triggers.second && !SticksPast[ControllerNum].Triggers.second)
        {
            return true;
        }

        return false;
    }

    return false;
}

/*************************************************************************/
/*!
\brief


*/
/*************************************************************************/
bool InputHandler::TriggerReleased(int ControllerNum, int Check)
{
  if (this->Active_Controllers[ControllerNum] != true)
    false;

    if (Check == CONTROLLER_DIR::LEFT)
    {
        if (!SticksPresent[ControllerNum].Triggers.first && SticksPast[ControllerNum].Triggers.first)
        {
            return true;
        }

        return false;
    }

    else if (Check == CONTROLLER_DIR::RIGHT)
    {
        if (!SticksPresent[ControllerNum].Triggers.second && SticksPast[ControllerNum].Triggers.second)
        {
            return true;
        }

        return false;
    }


    return false;
}

/*************************************************************************/
/*!
\brief


*/
/*************************************************************************/
bool InputHandler::TriggerDown(int ControllerNum, int Check)
{
  if (this->Active_Controllers[ControllerNum] != true)
    false;

    if (Check == CONTROLLER_DIR::LEFT)
    {
        if (SticksPresent[ControllerNum].Triggers.first)
        {
            return true;
        }

        return false;
    }

    else if (Check == CONTROLLER_DIR::RIGHT)
    {
        if (SticksPresent[ControllerNum].Triggers.second)
        {
            return true;
        }

        return false;
    }


    return false;
}


int InputHandler::CheckControllerStates(int i, InputMessage & Message)
{
    int Inactive = 0;

    // check left stick
    if (StickTriggered(i, CONTROLLER_DIR::LEFT))
    {
        Message.LeftStickState = STICK_TRIGGERED;
    }

    else if (StickDown(i, CONTROLLER_DIR::LEFT))
    {
        Message.LeftStickState = STICK_DOWN;
    }

    else if (StickReleased(i, CONTROLLER_DIR::LEFT))
    {
        Message.LeftStickState = STICK_RELEASED;
    }
    else
    {
        ++Inactive;
        Message.LeftStickState = STICK_NEUTRAL;
    }

    // check right stick
    if (StickTriggered(i, CONTROLLER_DIR::RIGHT))
    {
        Message.RightStickState = STICK_TRIGGERED;
    }

    else if (StickDown(i, CONTROLLER_DIR::RIGHT))
    {
        Message.RightStickState = STICK_DOWN;
    }

    else if (StickReleased(i, CONTROLLER_DIR::RIGHT))
    {
        Message.RightStickState = STICK_RELEASED;
    }
    else
    {
        ++Inactive;
        Message.RightStickState = STICK_NEUTRAL;
    }

    // check left trigger
    if (TriggerTriggered(i, CONTROLLER_DIR::LEFT))
    {
        Message.LeftTriggerState = TRIGGER_TRIGGERED;
    }

    else if (TriggerDown(i, CONTROLLER_DIR::LEFT))
    {
        Message.LeftTriggerState = TRIGGER_DOWN;
    }

    else if (TriggerReleased(i, CONTROLLER_DIR::LEFT))
    {
        Message.LeftTriggerState = TRIGGER_RELEASED;
    }
    else
    {
        ++Inactive;
        Message.LeftTriggerState = TRIGGER_NEUTRAL;
    }

    // check right trigger
    if (TriggerTriggered(i, CONTROLLER_DIR::RIGHT))
    {
        Message.RightTriggerState = TRIGGER_TRIGGERED;
    }

    else if (TriggerDown(i, CONTROLLER_DIR::RIGHT))
    {
        Message.RightTriggerState = TRIGGER_DOWN;
    }

    else if (TriggerReleased(i, CONTROLLER_DIR::RIGHT))
    {
        Message.RightTriggerState = TRIGGER_RELEASED;
    }
    else
    {
        ++Inactive;
        Message.RightTriggerState = TRIGGER_NEUTRAL;
    }

    return Inactive;
}

Vector2D InputHandler::GetStickVector(int ControllerNum, int Check)
{
  if (Check == CONTROLLER_DIR::LEFT)
  {
    return Vector2D(SticksPresent[ControllerNum].LeftStick.first, SticksPresent[ControllerNum].LeftStick.second);
  }

  else
  {
    return Vector2D(SticksPresent[ControllerNum].RightStick.first, SticksPresent[ControllerNum].RightStick.second);
  }
}
