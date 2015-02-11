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
#include "InputManager.h"
#include "ZInterface.h"

InputManager* INPUTMANAGER = nullptr;

InputManager::InputManager() : ISystem("Input", SystemType::ST_Input)
{
  INPUTMANAGER = this;
  ZInterface::Input = this;

  m_Keyboard = new KeyBoard();
  m_Controller = new InputHandler();
  ControllerCheckTimer = 0.0;
}

ZilchDefineType(InputManager, SpinningZilch)
{
	type->HandleManager = ZilchManagerId(Zilch::PointerManager);
	ZilchBindMethod(IsKeyDown);
	ZilchBindMethodAs(IsKeyPressed, "IsKeyTriggered");
	ZilchBindMethodAs(IsKeyPressed, "IsKeyPressed");
	ZilchBindMethod(IsKeyReleased);
	ZilchBindMethod(IsAnyKeyTriggered);

}

InputManager::~InputManager()
{
	INPUTMANAGER = nullptr;
}

bool InputManager::Initialize()
{
	if(!m_Keyboard)
	{
		return false;
	}

  if(!m_Controller)
	{
		return false;
	}

	// Initialize the input object.
	if(!m_Keyboard->Initialize())
	{
		//MessageBox(m_hwnd, "Could not initialize the input object.", "Error", MB_OK);
		return false;
	}

  if(!m_Controller->Initialize())	
  {
		//MessageBox(m_hwnd, "Could not initialize the Controller object.", "Error", MB_OK);
		return false;
	}

  return true;
}

void InputManager::Update(float dt)
{
  m_Controller->Update(dt);
  m_Keyboard->Update(dt);

  ControllerCheckTimer += dt;

  if (ControllerCheckTimer > 5.0)
  {
      m_Controller->Check_for_Controllers();
      ControllerCheckTimer = 0.0;
  }
}

void InputManager::Shutdown()
{
  if(m_Controller)
  {
    delete m_Controller;
  }

  if(m_Keyboard)
  {
    delete m_Keyboard;
  }

}

bool InputManager::IsKeyDown(int keyNumber)
{
  return m_Keyboard->IsKeyDown(keyNumber);
}

bool InputManager::IsKeyPressed(int KeyNumber)
{
  return m_Keyboard->IsKeyTriggered(KeyNumber);
}

bool InputManager::IsKeyReleased(int KeyNumber)
{
  return m_Keyboard->IsKeyReleased(KeyNumber);
}

bool InputManager::IsAnyKeyTriggered()
{
  return m_Keyboard->IsAnyKeyTriggered();
}



void InputManager::InitializeAnyPlayback(std::string levelname)
{
  m_Controller->InitializeAnyPlayback(levelname);
}
void InputManager::StartAnyRecording(std::string levelname)
{
  m_Controller->StartAnyRecording(levelname);
}

void InputManager::UninitializeAnyPlayback()
{
  m_Controller->UninitializeAnyPlayback();
}
void InputManager::StopAnyRecording()
{
  m_Controller->StopAnyRecording();
}

bool InputManager::IsButtonDown(int Controller, int button)
{
  return m_Controller->ButtonDown(Controller, button);
}

bool InputManager::IsButtonPressed(int Controller, int button)
{
  return m_Controller->ButtonTriggered(Controller, button);
}

bool InputManager::IsButtonReleased(int Controller, int button)
{
  return m_Controller->ButtonReleased (Controller, button);
}

bool InputManager::isStickPressed(int Controller, int Stick)
{
  return m_Controller->StickTriggered(Controller, Stick);
}

bool InputManager::isStickDown(int Controller, int Stick)
{
  return m_Controller->StickDown(Controller, Stick);
}

bool InputManager::isStickReleased(int Controller, int Stick)
{
  return m_Controller->StickReleased(Controller, Stick);
}

bool InputManager::isTriggerPressed(int Controller, int trigger)
{
  return m_Controller->TriggerTriggered(Controller, trigger);
}

bool InputManager::isTriggerDown(int Controller, int trigger)
{
  return m_Controller->TriggerDown(Controller, trigger);
}

bool InputManager::isTriggerReleased(int Controller, int trigger)
{
  return m_Controller->TriggerReleased(Controller, trigger);
}

Vector2D  InputManager::GetStickPosition(int Controller, int Stick)
{
  return m_Controller->GetStickVector(Controller, Stick);
}
