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
#include "SpriteRenderer.h"
#include "SoundInstance.h"

class PlayerController;

class PlayerKeyboardListener : public MessageListener
{
public:
  PlayerKeyboardListener(IEntity *playerObj, PlayerController *parent);
  void OnMessageRecieved(Message *SentMessage);

  void Initialize();

private:
  IEntity *const player;
  RigidBody *PlayerBody;
  SpriteRenderer *PlayerSprite;
  Transform *PlayerTransform;
  
  bool KeyReleasedDuringPause;
  PlayerController *const playerController;
  SoundInstance player_jump;

};