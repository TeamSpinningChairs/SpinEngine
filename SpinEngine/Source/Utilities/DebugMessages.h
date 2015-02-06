/****************************************************************************/
/*!
\file   DebugMessages.h
\author Steven Gallwas
\par    email: s.gallwas\@digipen.edu
\par    Course: GAM 200
\date   9/16/14
\brief
This file contains the definitions for handling debug messages

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.
*/
/****************************************************************************/
#pragma once

#include "Core\NewMessageSystem.h"
#include <iostream>
#include <string>

// function, which creates a debug listener and registers it
void DebugInitialize(void);

void SendDebugMessage(const char * textToSend);


// debug message type
class ConsoleMessage : public Message
{

public:
  std::string TextToWrite;
};

// debug message type
class ConsoleDebug : public MessageListener
{
  public:
       virtual void OnMessageRecieved(Message * SentMessage);
};
