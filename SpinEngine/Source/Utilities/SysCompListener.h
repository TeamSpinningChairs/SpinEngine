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

#include "ISystem.h"
#include "NewMessageSystem.h"

enum COMPONENT_ACTION {COMP_ADD, COMP_REMOVE};

class SysComponentMessage : public Message
{
public:

  SysComponentMessage(void);
	int ActionType;
	int SystemType;
	IComponent * Component;
};

class SysComponentListener : public MessageListener
{

public:
	SysComponentListener(System name);
	
  void OnMessageRecieved(Message * SentMessage);

	System System_Owner;
};