/****************************************************************************/
/*!
\file   DebugMessages.cpp
\author Steven Gallwas
\par    email: s.gallwas\@digipen.edu
\par    Course: GAM 200
\date   9/16/14
\brief
This file contains the implementation for handling debug messages

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.
*/
/****************************************************************************/
#include "Precompiled.h"
#include "DebugMessages.h"

void DebugInitialize(void)
{
	return;
}

void ConsoleDebug::OnMessageRecieved(Message * SentMessage)
{
  std::string Output = reinterpret_cast<ConsoleMessage *>(SentMessage)->TextToWrite;

  if(!Output.empty())
  {
	  // send messages straight to the output window.
	  OutputDebugString("\n");
	  OutputDebugString((LPCTSTR)Output.c_str());
	  OutputDebugString("\n\n");
  }
  else
    std::cout << "ERROR WRONG MESSAGE TYPE SENT TO DEBUG" << std::endl;

}

void SendDebugMessage(const char * textToSend)
{
	ConsoleMessage msg;

	msg.MessageType = MESSAGE_DEBUG;

	msg.TextToWrite = textToSend;
  if(GlobalSystem)
  {
	  GlobalSystem->MyDispatchMessage(&msg);
  }
}