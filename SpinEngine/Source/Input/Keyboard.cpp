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
#include "KeyBoard.h"

KeyBoard::KeyBoard()
{
}

KeyBoard::~KeyBoard()
{
}

void KeyBoard::UpdateKeyStates(void)
{
  for(int i = 0; i < 256; ++i)
  {
    // set the old key states equal to the new key states
    keyPast[i] = keyPresent[i];

    if(GetAsyncKeyState(i) & 0x8000)
    {
      keyPresent[i] = true;
    }
    
    else
      keyPresent[i] = false;
  }
}

void KeyBoard::SendKeyMessages(float time)
{
  KeyMessage KeyEvent;
  KeyEvent.MessageType = MESSAGE_KEYBOARD;

  for(unsigned int i = 0; i < 256; ++i)
  {
    KeyEvent.KeyNumber = i;
    KeyEvent.deltaTime = time;
    
    if(IsKeyTriggered(i))
    {
      KeyEvent.KeyStatus = KEY_STATE::KEY_TRIGGERED;
      GlobalSystem->MyDispatchMessage(&KeyEvent);
    }
    
    else if(IsKeyDown(i))
    {
      KeyEvent.KeyStatus = KEY_STATE::KEY_DOWN;
      GlobalSystem->MyDispatchMessage(&KeyEvent);
    }
    
    else if(IsKeyReleased(i))
    {
      KeyEvent.KeyStatus = KEY_STATE::KEY_RELEASED;
      GlobalSystem->MyDispatchMessage(&KeyEvent);
    }
  }

}

bool KeyBoard::Initialize()
{
	int i;

	// Initialize all the keys to being released and not pressed.
	for(i=0; i<256; i++)
	{
		keyPast[i] = false;
    keyPresent[i] = false;
	}

	return true;
}

void KeyBoard::KeyDown(unsigned int input)
{
	// If a key is pressed then save that state in the key array.
  keyPresent[input] = true;
	return;
}

void KeyBoard::KeyUp(unsigned int input)
{
	// If a key is released then clear that state in the key array.
  keyPresent[input] = false;
	return;
}

bool KeyBoard::IsKeyDown(unsigned int key)
{
	// Return what state the key is in (pressed/not pressed).
  return keyPresent[key];
}

bool KeyBoard::IsKeyTriggered(unsigned int key)
{
  return(keyPresent[key] && !keyPast[key]);
}

bool KeyBoard::IsKeyReleased(unsigned int key)
{
  return(!keyPresent[key] && keyPast[key]);
}

void KeyBoard::Shutdown()
{
  return;
}

void KeyBoard::Update(float time)
{
  this->UpdateKeyStates();
  this->SendKeyMessages(time);
}

bool KeyBoard::IsAnyKeyTriggered()
{
  for (int i = 0; i < 256; i++)
  {
    if ((keyPresent[i] && !keyPast[i]))
    {
      return true;
    }
  }

  return false;
}
