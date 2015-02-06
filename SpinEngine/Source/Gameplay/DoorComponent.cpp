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
#include "DoorComponent.h"
#include "Engine.h"
#include "SoundInstance.h"

DoorComponent::DoorComponent(GameObject Parent) : IComponent(Component_Type::CT_DOOR, Parent), CollisionDelegate(Parent)
{
    StartingPoint = Parent->GetTransform()->GetPosition();
    DoorSwitch = NULL;
    DoorBody = NULL;
    doorTriggered = false;
    Locked = false;
}

bool DoorComponent::Initialize()
{
    DoorSwitch = reinterpret_cast<SwitchComponent *>(Owner->GetComponent(CT_SWITCH));
    DoorBody   = reinterpret_cast<RigidBody *>(Owner->GetComponent(CT_Body));
    InitializeCollisionCallback();

    if (DoorSwitch == NULL || DoorBody == NULL)
    {
        return false;
    }

    return true;
}

void DoorComponent::Update(float dt)
{
    DoorBody->acceleration.x = 0.0f;
    DoorBody->acceleration.y = 0.0f;
    DoorBody->velocity.x = 0.0f;
    DoorBody->velocity.y = 0.0f;

    // if the door is active, move it up the height of the door.
    if (DoorSwitch->GetSwitchStatus() == true)
    {
        float distance = Owner->GetTransform()->GetPosition().y - StartingPoint.y;
        
        if (distance >= DOOR_HEIGHT)
        {
            Owner->GetTransform()->GetPosition().y = StartingPoint.y + DOOR_HEIGHT;
            DoorBody->position.y = StartingPoint.y + DOOR_HEIGHT;
            this->DoorSwitch->ToggleActive();
            return;
        }
        
        Owner->GetTransform()->GetPosition().y += 4.0 * dt; 
        DoorBody->position.y += 4.0 * dt;
    }

    // if the door is not active, move back down to the origin
    else
    {
        float distance = Owner->GetTransform()->GetPosition().y - StartingPoint.y;

        if (distance <= 0)
        {
            Owner->GetTransform()->GetPosition().y = StartingPoint.y;
            DoorBody->position.y = StartingPoint.y;
            doorTriggered = false;

        }

        else
        {
            Owner->GetTransform()->GetPosition().y -= 3.0 * dt;
            DoorBody->position.y -= 3.0 * dt;
        }
    }

}

void DoorComponent::Release()
{

}

void DoorComponent::OnCollision(GameObject)
{
    if (doorTriggered == false && Locked == false)
    {
      SoundInstance environmental_smalldoor;
        this->DoorSwitch->ToggleActive();
        ENGINE->m_Sound->PlayEvent("CB/environment_smalldoor", &environmental_smalldoor, false);
        doorTriggered = true;
        //Locked = true;
    }
}

