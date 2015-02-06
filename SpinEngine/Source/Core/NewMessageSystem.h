/****************************************************************************/
/*!
\file   NewMessageSyste.h
\author Steven Gallwas
\par    email: s.gallwas\@digipen.edu
\par    Course: GAM 200
\date   9/16/14
\brief  
    This file contains the definition of the base messaging system.

*/
/****************************************************************************/
#pragma once

// required for the messaging system to hold the mailing list
#include <vector>
#include <list>
#include "IComponent.h"
#include "ISystem.h"
#include "mathlib.h"

// forward declare for GlobalSystem to know what MessageSystem is
class MessageSystem;

// Declare a pointer so everything can access the messaging system
extern MessageSystem * GlobalSystem;

// enumeration for all possible message types
enum MessageTypes
{
  MESSAGE_INPUT,
  MESSAGE_KEYBOARD,
  MESSAGE_COLLISION,
  MESSAGE_TEXT,
  MESSAGE_DEBUG,
  MESSAGE_COMPONENT,
  MESSAGE_WALL,
  MESSAGE_PLAYERACTIVATE,
  MESSAGE_MENU,
  MESSAGE_PAUSE,
  MESSAGE_PROXIMITYACTIVATE,
  MESSAGE_DESTROY,
  MESSAGE_TOTAL,

};

// Base message class only contains the message type
class Message
{
  public:
    MessageTypes MessageType;
};

// Pure Virtual Message Class inherit from whenever you want different behavior
class MessageListener
{
  public:
       virtual void OnMessageRecieved(Message * SentMessage) = 0;
};


// Initialize Function, should set the GlobalSystem, but currently does nothing because reasons.
void MessageSystemInitialize(void);


// the class definistion for the MessageSystem, There should only be one message system
class MessageSystem
{
  public:

    MessageSystem(void);

    void MyDispatchMessage(Message * Message);

    void RegisterListener(MessageTypes MessageType, MessageListener *l);

    void UnregisterListener(MessageTypes MessageType, MessageListener *l);

    void AddComponent(IComponent * Comp, int SystemType);

    void RemoveComponent(IComponent * Comp, int SystemType);

	  void MessageSystemShutdown(void);

    void SendActivate(Vector3D);

    void ClearMessageSystem(void);

  private:

    std::vector<std::list<MessageListener * >> MailingList;
};

