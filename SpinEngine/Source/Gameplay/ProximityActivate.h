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

enum ActivateDirection{AD_UP, AD_Down, AD_Left, AD_Right};

// the message to be passed
class ProximityActivateMessage : public Message
{
    public:

        bool& Check;

        ProximityActivateMessage(bool &);

        int CheckDir;

        Vector3D Position;
};

class ProximityTriggerListener : public MessageListener
{

public:

    ProximityTriggerListener(GameObject);

    void OnMessageRecieved(Message * SentMessage);


private:

    SwitchComponent * OwnerSwitch;

    GameObject Parent;
};


class ProximityTrigger : public IComponent
{
public:

    ProximityTrigger(GameObject parent);

    bool Initialize();
	void Serialize (DynamicElement* props, Serializer::DataNode* data) {}
    void Update(float dt);

    void Release();

private:
    ProximityTriggerListener * Trigger;
};
