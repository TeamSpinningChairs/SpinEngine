/****************************************************************************/
/*!
\file   NewMessageSystem.cpp
\author Steven Gallwas
\par    email: s.gallwas\@digipen.edu
\par    Course: GAM 200
\date   9/16/14
\brief  
    This file contains the implementation for the base messaging system.

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#include "Precompiled.h"
#include "NewMessageSystem.h"
#include "SysCompListener.h"
#include "PlayerTrigger.h"
#include "GameStateManager.h"

MessageSystem * GlobalSystem = new MessageSystem;

/*************************************************************************/
/*!
  \brief
    set the GlobalSystem pointer to a new messagesystem. should only
    be called once
*/
/*************************************************************************/
void MessageSystemInitialize(void)
{
  MessageSystem * GlobalSystem = new MessageSystem;
}


/*************************************************************************/
/*!
  \brief
	This function dispatchs messages to all listeners registerd to 
	accept this type of message.

  \param MessageToSend
	This is the message that is to be passed to all listeners of the 
	appropriate type.

*/
/*************************************************************************/
void MessageSystem::MyDispatchMessage(Message * MessageToSend)
{
    // only send menu, and input messages anything revieving input must 
    if (gGameStatePaused == true && MessageToSend->MessageType != MESSAGE_MENU 
        && MessageToSend->MessageType != MESSAGE_INPUT
        && MessageToSend->MessageType != MESSAGE_KEYBOARD)
    {
        return;
    }

 // itterate over all of the listeners and call functionality
  for(MessageListener * PL: MailingList[MessageToSend->MessageType])
  {
    PL->OnMessageRecieved(MessageToSend);
  }
  
  // code in case c++ 11 is not supported use this instead.
  /*
  std::list<MessageListener*>::iterator it;
  for(it = MailingList[MessageToSend->MessageType].begin(); it != MailingList[MessageToSend->MessageType].end(); ++it)
  {
    (*it)->OnMessageRecieved(MessageToSend);
  }
  */
}

/*************************************************************************/
/*!
\brief
This function dispatchs messages to all listeners registerd to
accept this type of message.

\param MessageToSend
This is the message that is to be passed to all listeners of the
appropriate type.

*/
/*************************************************************************/
void MessageSystem::RegisterListener(MessageTypes MessageType, MessageListener *l)
{
  MailingList[MessageType].push_back(l);
}

/*************************************************************************/
/*!
\brief
This function removes a listener from the list of listeners
should be called when an object is destroyed.

\param MessageType

the type of message the listener was registered to react to.

\param l

the listener to be removed.

*/
/*************************************************************************/
void MessageSystem::UnregisterListener(MessageTypes MessageType, MessageListener *l)
{
      MailingList[MessageType].remove(l);
}

/*************************************************************************/
/*!
\brief
Message System constructor.

*/
/*************************************************************************/
MessageSystem::MessageSystem(void) : MailingList(MessageTypes::MESSAGE_TOTAL)
{

}

/*************************************************************************/
/*!
\brief
This function dispatchs deletes the message system, call during shutdown

*/
/*************************************************************************/
void MessageSystemShutdown(void)
{
	if (GlobalSystem)
	{
		delete GlobalSystem;
	}
}

void MessageSystem::AddComponent(IComponent * Comp, int SystemType)
{
  // create a system component message
  SysComponentMessage CompMsg;

  CompMsg.ActionType = COMP_ADD;
  CompMsg.SystemType = SystemType;
  CompMsg.Component = Comp;

  this->MyDispatchMessage(&CompMsg);
}

void MessageSystem::RemoveComponent(IComponent * Comp, int SystemType)
{
  // create a system component message
  SysComponentMessage CompMsg;

  CompMsg.ActionType = COMP_REMOVE;
  CompMsg.SystemType = SystemType;
  CompMsg.Component = Comp;

  this->MyDispatchMessage(&CompMsg);
}

void MessageSystem::SendActivate(Vector3D Position)
{
    // create a system component message
    PlayerActivateMessage ActivateMsg;

    ActivateMsg.PlayerPosition = Position;
    this->MyDispatchMessage(&ActivateMsg);
}

void MessageSystem::ClearMessageSystem(void)
{
  for (int i = 0; i < MessageTypes::MESSAGE_TOTAL; i++)
  {
    for (auto it = MailingList[i].begin(); it != MailingList[i].end(); it++)
    {
      SPIN_ENGINE_SAFE_DELETE(*it);
    }
    MailingList[i].clear();
  }

  //MailingList.clear();
}