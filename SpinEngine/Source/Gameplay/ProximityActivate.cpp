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
#include "ProximityActivate.h"


// the message to be passed

ProximityActivateMessage::ProximityActivateMessage(bool& test) : Check(test)
{
    MessageType =   MESSAGE_PROXIMITYACTIVATE;

    this->CheckDir = AD_UP;

    Position.x = 0;
    Position.y = 0;
    Position.z = 0;
}


ProximityTriggerListener::ProximityTriggerListener(GameObject ParentObj)
{
    this->Parent = ParentObj;
    this->OwnerSwitch = reinterpret_cast<SwitchComponent *>(Parent->GetComponent(CT_SWITCH));
}


void ProximityTriggerListener::OnMessageRecieved(Message * SentMessage)
{
  if (this->OwnerSwitch->GetSwitchStatus() == false)
    {
        
        ProximityActivateMessage * GivenInput = reinterpret_cast<ProximityActivateMessage *>(SentMessage);

        // calculate the distance between the player and the parent object and see if it is in range if it is toggle the switch
        float TempX = Parent->GetTransform()->GetPosition().x - GivenInput->Position.x;
        float TempY = Parent->GetTransform()->GetPosition().y -  GivenInput->Position.y;

        float Totaldistance = sqrt((TempX * TempX) + (TempY * TempY));

        float distance = 3.0f;
        
        if(GivenInput->CheckDir == AD_Left)
        {
          distance = Parent->GetTransform()->GetPosition().x - GivenInput->Position.x;
        }

        else if(GivenInput->CheckDir == AD_Right)
        {
          distance = GivenInput->Position.x - Parent->GetTransform()->GetPosition().x;
        }

        else if(GivenInput->CheckDir == AD_Down)
        {
          distance = Parent->GetTransform()->GetPosition().y - GivenInput->Position.y;
        }

        else if(GivenInput->CheckDir == AD_UP)
        {
          distance = GivenInput->Position.y - Parent->GetTransform()->GetPosition().y;
        }
        
        if (distance < 2.0f && Totaldistance < 2.0f)
        {
          OwnerSwitch->ActivateSwitch();
          GivenInput->Check = true;
        }

    }
}

ProximityTrigger::ProximityTrigger(GameObject parent) : IComponent(Component_Type::CT_PROXIMITYACTIVATE, parent)
{ 
    Trigger = new ProximityTriggerListener(Owner);
}

bool ProximityTrigger::Initialize()
{
    // add the listener to the messaging system
    GlobalSystem->RegisterListener(MESSAGE_PROXIMITYACTIVATE, Trigger);
    
    return true;
}

void ProximityTrigger::Update(float dt)
{
    return;
}

void ProximityTrigger::Release()
{
    // remove the listener from the messaging system   
    GlobalSystem->UnregisterListener(MESSAGE_PROXIMITYACTIVATE, Trigger);

    // delete the allocated data for the trigger listener
    delete Trigger;
}
