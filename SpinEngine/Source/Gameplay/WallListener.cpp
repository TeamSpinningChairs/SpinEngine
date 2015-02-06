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
#include "Precompiled.h"
#include "WallListener.h"

/*************************************************************************/
/*!
\brief
Constructor for the dynamic hand, sets all values to default
*/
/*************************************************************************/
WallMessage::WallMessage(int Type)
{
	this->MessageType = MESSAGE_WALL;
	this->WallMsgType = Type;
  Target = NULL;
  TargetNumber = -1;
  Delay = 0.0f;
}

/*************************************************************************/
/*!
\brief
Constructor for the dynamic hand, sets all values to default
*/
/*************************************************************************/
WallListener::WallListener(WallComp * Wall)
{
  WallOwner = Wall;
}

/*************************************************************************/
/*!
\brief
    handles what to do when a message is recieved
*/
/*************************************************************************/
void WallListener::OnMessageRecieved(Message * SentMessage)
{
  // cast the message as the correct type
  WallMessage * GivenInput = reinterpret_cast<WallMessage *>(SentMessage);

	//if the message is to activate the wall, set the wall to active
	if (GivenInput->WallMsgType == Wall_MESSAGE_TYPES::ACTIVATE)
	{
		WallOwner->WallActive = true;
	}

  //if the message is to deactivate wall, set the wallactive flag to false
	else if (GivenInput->WallMsgType == Wall_MESSAGE_TYPES::DEACTIVATE)
	{
		WallOwner->WallActive = false;
	}

  //add targets to the walls list of targets
  else if(GivenInput->WallMsgType == Wall_MESSAGE_TYPES::ADD_TARGET)
  {
    if(GivenInput->Target != NULL)
    {
      RemoveTarget(GivenInput->Target);
    }
  }

  //remove a target from the list of targets
  else if(GivenInput->WallMsgType == Wall_MESSAGE_TYPES::RESET_TARGET)
  {
    if(GivenInput->TargetNumber != -1)
    {
      WallOwner->Targets[GivenInput->TargetNumber].Targeted = false;
    }
  }

  //launch one of the reservered tentacles at a specific target, so it can be removed immediately.
  else if (GivenInput->WallMsgType == Wall_MESSAGE_TYPES::GRAB_IMMEDIATELY)
  {
      if (GivenInput->Target != NULL)
      {
          WallOwner->GrabImmediately(GivenInput->Target);
      }
  }

  //launch one of the reservered tentacles at a specific target, so it can be removed immediately.
  else if (GivenInput->WallMsgType == Wall_MESSAGE_TYPES::DELAY)
  {
      WallOwner->WallDelay = GivenInput->Delay;
  }
}

/*************************************************************************/
/*!
\brief
    Removes a given target gameobject from the list of targets.
*/
/*************************************************************************/
void WallListener::RemoveTarget(GameObject Target)
{
  for(int i = 0; i < TargetCount; i++)
  {
    if(WallOwner->Targets[i].TargetObject == Target)
    {
      WallOwner->Targets[i].Targeted = false;
      WallOwner->Targets[i].TargetObject = NULL;
    }
  }
}