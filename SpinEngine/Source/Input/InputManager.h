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
#pragma once

#include "Input\KeyBoard.h"
#include "InputHandler.h"
#include "KeyBoard.h"
#include "Precompiled.h" //ISystem.h

class InputManager : public ISystem
{
	
  private:
    InputHandler * m_Controller;
    KeyBoard *m_Keyboard;

  public:
	  ZilchDeclareDerivedType(InputManager, ISystem);
    InputManager();
    ~InputManager();

    bool Initialize() override;
    void Update(float dt) override;
    void Shutdown() override;
    bool IsAnyKeyTriggered();
    bool IsKeyDown(int keyNumber);
    bool IsKeyPressed(int KeyNumber);
    bool IsKeyReleased(int KeyNumber);
    
    bool IsButtonDown(int Controller, int button);
    bool IsButtonPressed(int Controller, int button);
    bool IsButtonReleased(int Controller, int button);

    bool isStickPressed(int Controller, int Stick);
    bool isStickDown(int Controller, int Stick);
    bool isStickReleased(int Controller, int Stick);
    
    bool isTriggerPressed(int Controller, int Stick);
    bool isTriggerDown(int Controller, int Stick);
    bool isTriggerReleased(int Controller, int Stick);

    Vector2D GetStickPosition(int Controller, int Stick);

    float ControllerCheckTimer;

    //When the GSM loads a level, it calls these
    void InitializeAnyPlayback(std::string levelname);
    void StartAnyRecording(std::string levelname);
    //When the GSM unloads a level, it calls these
    void UninitializeAnyPlayback();
    void StopAnyRecording();
};
