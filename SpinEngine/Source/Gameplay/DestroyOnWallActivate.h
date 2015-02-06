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
#include "IComponent.h"

class DestroyMessage : public Message
{
public:
	DestroyMessage();

};

class DestroyListener : public MessageListener
{
public:
  DestroyListener(GameObject owner);

	void OnMessageRecieved(Message * SentMessage);

private:

  GameObject owner; 
};

class DestroyOnActivate : public IComponent
{
public:
  DestroyOnActivate(GameObject);

    bool Initialize();

	void Serialize (DynamicElement* props, Serializer::DataNode* data) {}

    void Update(float dt);

    void Release();

private:

  DestroyListener * DestList;
};