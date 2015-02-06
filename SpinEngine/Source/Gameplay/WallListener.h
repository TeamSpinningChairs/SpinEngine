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
#include "Wall.h"

enum Wall_MESSAGE_TYPES{ACTIVATE, DEACTIVATE, DELAY, ADD_TARGET, REMOVE_TARGET, RESET_TARGET, GRAB_IMMEDIATELY};

class WallMessage : public Message
{
public:
	WallMessage(int type);

	int WallMsgType;

	GameObject Target;

	int TargetNumber;

  float Delay;
};

class WallListener : public MessageListener
{
public:
	WallListener(WallComp * Wall);

	void OnMessageRecieved(Message * SentMessage);

private:
  void RemoveTarget(GameObject);

	 WallComp * WallOwner;
};