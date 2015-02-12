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
#include "DoorType2Comp.h"
#include "FactoryAccess.h"
#include "WallListener.h"
#include "Engine.h"

/*
    when the door is initialized, add a trigger region to the indicated 
    side  the trigger region will have a switch and trigger on collide if 
    it gets triggered, the door will slam down
*/


/*
Door 2 Trigger stuff
*/

Door2Trigger::Door2Trigger(GameObject Parent) : IComponent(Component_Type::CT_DOOR2TRIGGER, Parent), CollisionDelegate(Parent)
{
    doorTriggered = false;
}

bool Door2Trigger::Initialize()
{
    InitializeCollisionCallback();

    return true;
}

void Door2Trigger::Update(float dt)
{

}

void Door2Trigger::Release()
{

}

void Door2Trigger::OnCollision(GameObject otherObject)
{
    if (otherObject->GetComponent(CT_Player_Controller))
    {
        doorTriggered = true;
    }
}

/*
Door 2 Component stuff
*/

DoorType2Comp::DoorType2Comp(GameObject Parent, int CloseDirection) : IComponent(Component_Type::CT_DOOR2, Parent)
{
    this->DoorCloseDirection = CloseDirection;

    Slammed = false;

    movedistace = 0.0;
}

// creates the door trigger
bool DoorType2Comp::Initialize()
{
    GameObject Trigger = GlobalFactory->CreateGameObject("Trigger", "Claw-Open.png", this->Owner->GetTransform()->GetWorldPosition());

    Trigger->GetTransform()->GetWorldScale().x = 2;
    Trigger->GetTransform()->GetWorldScale().y = 5;

    AABB *box = new AABB(Trigger);
    box->SetHalfSize(2, 5);
    Trigger->AddGameComponent(CT_BoxCollider, box);

    RigidBody *body = new RigidBody(Trigger, reinterpret_cast<Primitive*>(box));
    body->SetStatic();
    body->isGhost = true;
    Trigger->AddGameComponent(CT_Body, body);

    DoorTrigger = new Door2Trigger(Trigger);
    Trigger->AddGameComponent(CT_DOOR2TRIGGER, DoorTrigger);

    Trigger->SetInVisible();

    if (DoorCloseDirection == DOORCLOSE_LEFT)
    {
        // offset trigger position to the left
        Trigger->GetTransform()->GetWorldPosition().x -= (Owner->GetTransform()->GetWorldScale().x + 1);
    }
    else
    {
        Trigger->GetTransform()->GetWorldPosition().x += (Owner->GetTransform()->GetWorldScale().x + 1);
    }

    Trigger->GetTransform()->GetWorldPosition().y -= (Owner->GetTransform()->GetWorldScale().y);
    return true;
}

void DoorType2Comp::Update(float dt)
{
    // if the player touches the trigger region, move down to the closed position;
    if (DoorTrigger->doorTriggered == true && movedistace < 5.0f)
    {
      if (this->Slammed == false)
      {
        ENGINE->m_Sound->PlayEvent("environment_door_crash", &instance, false);
        Slammed = true;
      }

        reinterpret_cast<RigidBody *>(this->Owner->GetComponent(CT_Body))->position.y -= 25 * dt;
        this->Owner->GetTransform()->GetWorldPosition().y -= 25 * dt;
        movedistace += (25 * dt);
        WallMessage WallMSG(Wall_MESSAGE_TYPES::DEACTIVATE);

        GlobalSystem->MyDispatchMessage(&WallMSG);
    }


}

void DoorType2Comp::Release()
{

}


