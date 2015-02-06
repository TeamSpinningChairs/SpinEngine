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

#include"IComponent.h"
#include "NewMessageSystem.h"
#include "mathlib.h"
#include "SwitchComponent.h"

// the message to be passed
class PlayerActivateMessage : public Message
{
    public:

        PlayerActivateMessage();

        Vector3D PlayerPosition;
};

class PlayerTriggerListener : public MessageListener
{

public:

    PlayerTriggerListener(GameObject);

    void OnMessageRecieved(Message * SentMessage);


private:

    SwitchComponent * PlayerSwitch;

    GameObject Parent;
};


class PlayerTrigger : public IComponent
{
public:

    PlayerTrigger(GameObject parent);

    bool Initialize();
	void Serialize (DynamicElement* props, Serializer::DataNode* data) {}
    void Update(float dt);

    void Release();

    ~PlayerTrigger();

private:
    PlayerTriggerListener * Trigger;
};
