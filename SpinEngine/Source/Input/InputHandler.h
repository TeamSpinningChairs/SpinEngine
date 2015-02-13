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
#pragma once
#include "Precompiled.h"
#include <Windows.h>
#include <XInput.h>
#include "Core\NewMessageSystem.h"
#include "Utilities\iSystem.h"
#include "Core\IEntity.h"
#include <vector>
#include <utility>
#include "InputPlayback.h"
#include "InputRecord.h"

#define ButtonCount 14
#define AutoDelay 0.25f

//message type for controller input messages
class InputMessage : public Message
{
public:

    InputMessage(void);

    float time;
    int Controller_Num;
    int Key_Hold_Time;
    int TriggeredKey;
    bool KeyPressed;
    bool KeyDown;
    bool KeyReleased;
    std::pair<float, float> leftStick;
    std::pair<float, float> rightStick;
    std::pair<float, float> Triggers;
    int LeftStickState;
    int RightStickState;
    int LeftTriggerState;
    int RightTriggerState;
};


enum CONTROLLER_DIR
{
    LEFT,
    RIGHT
};

enum STICK_STATE
{
    STICK_TRIGGERED,
    STICK_DOWN,
    STICK_NEUTRAL,
    STICK_RELEASED
};

enum TRIGGER_STATE
{
    TRIGGER_TRIGGERED,
    TRIGGER_DOWN,
    TRIGGER_NEUTRAL,
    TRIGGER_RELEASED
};

enum BUTTONS
{
  BUTTONS_A,
  BUTTONS_X,
  BUTTONS_B,
  BUTTONS_Y,
  BUTTONS_RIGHT_SHOULDER,
  BUTTONS_LEFT_SHOULDER,
  BUTTONS_BACK,
  BUTTONS_START,
  DPAD_LEFT,
  DPAD_RIGHT,
  DPAD_UP,
  DPAD_DOWN,
  LEFT_STICK,
  RIGHT_STICK
};

struct Button_Status
{
  Button_Status(void);

  bool ButtonPressed;
  bool ButtonDown;
  int TimeHeld;
};

struct Controller_Status
{
  Controller_Status(void);

  int ControllerNum;
  std::vector<Button_Status> Buttons;
  std::pair<float, float> LeftStick;
  std::pair<float, float> RightStick;
  float RightTrigger;
  float LeftTrigger;
};

struct Sticks_and_Triggers
{
    std::pair<float, float> LeftStick;
    std::pair<float, float> RightStick;
    std::pair<float, float> Triggers;
};


class InputHandler
{
public:
  
  bool Initialize();
	
  void Shutdown();

  // Constructor for the input handler class
  InputHandler(void);

  // Updates the state of all currently connected controllers.
  void Update(float dt);

  // Checks to see which controllers are currently connected
  void Check_for_Controllers(void);

  void VibrateController(int ControllerNum, float time);

  // checks if a button has been pressed this frame
  bool ButtonTriggered(int ControllerNum, int ButtonType);

  // checks if a button is down
  bool ButtonDown(int ControllerNum, int ButtonType);

  // checks to see if a button was released this frame
  bool ButtonReleased(int ControllerNum, int ButtonType);

  bool StickTriggered(int ControllerNum, int Check);
  bool StickReleased(int ControllerNum, int Check);
  bool StickDown(int ControllerNum, int Check);

  bool TriggerTriggered(int ControllerNum, int Check);
  bool TriggerReleased(int ControllerNum, int Check);
  bool TriggerDown(int ControllerNum, int Check);

  bool CheckControllerActive(int ControllerNum);

  Vector2D GetStickVector(int ControllerNum, int Check);

  //When the GSM loads a level, it calls these
  void InitializeAnyPlayback(std::string levelname);
  void StartAnyRecording(std::string levelname);


  //When the GSM unloads a level, it calls these
  void UninitializeAnyPlayback();
  void StopAnyRecording();

  bool AutoPlay;

private:

  // for autoplay/playback
  XINPUT_STATE CurrentControls;

  // timer for autoplay
  float AutoTimer;

  InputPlayback playback;
  InputRecord recording;

  void GenerateRandomInput(XINPUT_GAMEPAD &input);

  void UpdateButton(int ButtonType);

  void UpdateSticks_and_Triggers(int ControllerNum);

  void Update_Active_Controllers(void);

  void Update_Controller_States(void);

  void SendButtonMessages(float dt);

  void SendStickandTriggerMessages(float dt);

  void NormalizeSticks(void);
  
  int CheckControllerStates(int controllerNum, InputMessage & Message);

  // vector for translating between my button enums, and xinput types
  std::vector<int> XInputDefines;

  // vector of active controllers
  std::vector<bool> Active_Controllers;
  
  // vector for the state of controllers during the previous update
  std::vector<Controller_Status> Controllers; 

  // Vector to hold the most recent state of the connected controllers
  std::vector<XINPUT_STATE> Current_States; 


  //make two vectors to keep track of the sticks and trigger states
  std::vector<Sticks_and_Triggers> SticksPast;

  std::vector<Sticks_and_Triggers> SticksPresent;

};


