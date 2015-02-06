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
#include "Input\KeyboardComponent.h"
#include "Core\NewMessageSystem.h"

KeyboardComponent::KeyboardComponent(IEntity * Parent) : IComponent(Component_Type::CT_Keyboard_Control, Owner)
{
  // set the parent pointer contained in keyboard listener object
 // myPlayerKeyboardListener.parentController = Parent;
}

bool KeyboardComponent::Initialize()
{
  //GlobalSystem->RegisterListener(MessageTypes::MESSAGE_KEYBOARD, &(this->ThisKeyboard));
  return true;
}
  
void KeyboardComponent::Update(float dt)
{
  return;
}
  
KeyboardComponent::~KeyboardComponent()
{
    //GlobalSystem->UnregisterListener(MessageTypes::MESSAGE_KEYBOARD, &(this->ThisKeyboard));
}

void KeyboardComponent::Release()
{

}
