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
#include "PlayerTrigger.h"


// the message to be passed

PlayerActivateMessage::PlayerActivateMessage()
{
    MessageType = MESSAGE_PLAYERACTIVATE;

    this->PlayerPosition.x = 0;
    this->PlayerPosition.y = 0;
    this->PlayerPosition.z = 0;
}


PlayerTriggerListener::PlayerTriggerListener(GameObject ParentObj)
{
    this->Parent = ParentObj;
    this->PlayerSwitch = reinterpret_cast<SwitchComponent *>(Parent->GetComponent(CT_SWITCH));
}


void PlayerTriggerListener::OnMessageRecieved(Message * SentMessage)
{
    if (this->PlayerSwitch)
    {
        PlayerActivateMessage * GivenInput = reinterpret_cast<PlayerActivateMessage *>(SentMessage);

        // calculate the distance between the player and the parent object and see if it is in range if it is toggle the switch
        float TempX = Parent->GetTransform()->GetPosition().x - GivenInput->PlayerPosition.x;
        float TempY = Parent->GetTransform()->GetPosition().y -  GivenInput->PlayerPosition.y;

        float distance = sqrt((TempX * TempX) + (TempY * TempY));

        if (distance < 2.5f)
        {
            PlayerSwitch->ToggleActive();
        }

    }
}

PlayerTrigger::PlayerTrigger(GameObject parent) : IComponent(Component_Type::CT_PLAYERTRIGGER, parent)
{ 
    Trigger = new PlayerTriggerListener(Owner);
}

bool PlayerTrigger::Initialize()
{
    // add the listener to the messaging system
    GlobalSystem->RegisterListener(MESSAGE_PLAYERACTIVATE, Trigger);
    
    return true;
}

void PlayerTrigger::Update(float dt)
{

}

void PlayerTrigger::Release()
{
    // remove the listener from the messaging system   
    GlobalSystem->UnregisterListener(MESSAGE_PLAYERACTIVATE, Trigger);

    // delete the allocated data for the trigger listener
    delete Trigger;
}

PlayerTrigger::~PlayerTrigger()
{
    GlobalSystem->UnregisterListener(MESSAGE_PLAYERACTIVATE, Trigger);

}

