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
#include "ControllerComponent.h"

ControllerComponent::ControllerComponent(IEntity *Parent, int controller) : IComponent(Component_Type::CT_Player_Controller, Owner),
parent(Parent)
{
  //this->ThisController.parentController = Parent;
  this->controllerNum = controller;
  //this->ThisController.ControllerNum = controller;
}

bool ControllerComponent::Initialize()
{ 
  //GlobalSystem->RegisterListener(MessageTypes::MESSAGE_INPUT, &this->ThisController);
  return true;
}
 
void ControllerComponent::Update(float dt)
{
  return;
}

ControllerComponent::~ControllerComponent()
{
  //GlobalSystem->UnregisterListener(MessageTypes::MESSAGE_INPUT, &this->ThisController);
}

void ControllerComponent::Release()
{
}
