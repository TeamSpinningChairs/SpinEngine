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
#include "SysCompListener.h"

SysComponentMessage::SysComponentMessage()
{
  this->MessageType = MESSAGE_COMPONENT;
}

SysComponentListener::SysComponentListener(ISystem* Sys_Owner) : System_Owner(Sys_Owner)
{
}

void SysComponentListener::OnMessageRecieved(Message * SentMessage)
{
	SysComponentMessage * GivenInput = reinterpret_cast<SysComponentMessage *>(SentMessage);

	if (GivenInput)
	{
		if (GivenInput->SystemType = this->System_Owner->GetType())
		{
			if (GivenInput->ActionType == COMP_ADD)
				this->System_Owner->Components.push_back(GivenInput->Component);

			else if (GivenInput->ActionType == COMP_REMOVE)
				this->System_Owner->Components.remove(GivenInput->Component);
		}
	}
}