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
#include "IComponent.h"
#include "PlayerKeyboardListener.h"

class KeyboardComponent : public IComponent
{
public:
  ~KeyboardComponent();

  KeyboardComponent(IEntity * Parent);

  bool Initialize();
  void Update(float dt);
  void Release();
  
  KeyboardComponent();

  //PlayerKeyboardListener myPlayerKeyboardListener;
};