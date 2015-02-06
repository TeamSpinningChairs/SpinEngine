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

#include "Core\IComponent.h"
#include "Input\InputHandler.h"
#include "Core\NewMessageSystem.h"
#include "PlayerControllerListener.h"

class ControllerComponent : public IComponent
{
  public:
  ControllerComponent(IEntity * Parent, int Controller);

  bool Initialize();
  void Update(float dt);
  void Release() override;
  
  ~ControllerComponent();
  
 // PlayerControllerListener ThisController;

  private:
    ControllerComponent();
    IEntity *parent;
	  int controllerNum;
};