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
#include "Core\NewMessageSystem.h"
#include "SpriteRenderer.h"

class PlayerController;

class PlayerControllerListener : public MessageListener
{
public:
  PlayerControllerListener(IEntity *playerObj, PlayerController *parent, int controllerNum);
  void OnMessageRecieved(Message * SentMessage);

  void Initialize();

private:
  IEntity *const player;
  RigidBody *PlayerBody;
  SpriteRenderer *PlayerSprite;
  Transform *PlayerTransform;

  PlayerController *const playerController;
  int controllerNum;
};
