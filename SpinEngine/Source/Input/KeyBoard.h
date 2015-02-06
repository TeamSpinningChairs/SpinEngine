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

#include "NewMessageSystem.h"
#include "IEntity.h"

enum KEY_STATE{KEY_DOWN, KEY_RELEASED, KEY_TRIGGERED, KEY_UP};

enum KEY_DEFINES
{
	KEY_W = 87,
	KEY_A = 65,
  KEY_S = 83,
  KEY_D = 68,
  KEY_UPARROW,
  KEY_DOWNARROW,
  KEY_LEFTARROW,
  KEY_RIGHT,
  KEY_SPACE
};

class KeyMessage : public Message
{
public:
  float deltaTime;
  int KeyNumber;
  int KeyStatus;
};

class KeyBoard
{
public:

  bool Initialize();
	void Update(float dt);
	void Shutdown();

	KeyBoard();
	
  ~KeyBoard();
  void UpdateKeyStates(void);
  void SendKeyMessages(float time);

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);
  
  bool IsKeyTriggered(unsigned int key);
	bool IsKeyDown(unsigned int);
  bool IsKeyReleased(unsigned int key);

  bool IsAnyKeyTriggered();

private:
  bool keyPast[256];
  bool keyPresent[256];
};
