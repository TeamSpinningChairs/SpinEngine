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
#include "WallTriggerComp.h"
#include "NewMessageSystem.h"
#include "WallListener.h"
#include "FactoryAccess.h"
#include "Engine.h"
#include "DestroyOnWallActivate.h"

WallTriggerComp::WallTriggerComp(GameObject Parent) : IComponent(Component_Type::CT_DOOR, Parent), CollisionDelegate(Parent)
{
    triggered = false;
}

WallTriggerComp::~WallTriggerComp()
{
  ENGINE->m_Sound->StopEvent(&instance);
}


bool WallTriggerComp::Initialize()
{
    InitializeCollisionCallback();
    return true;
}

void WallTriggerComp::Update(float dt)
{
}

void WallTriggerComp::Release()
{

}

void WallTriggerComp::OnCollision(GameObject Player)
{
    if (Player->GetComponent(CT_Player_Controller) != NULL && triggered == false)
    {
        // create a activate wall message and send it to the wall
        WallMessage WallMSG(Wall_MESSAGE_TYPES::ACTIVATE);

        GlobalSystem->MyDispatchMessage(&WallMSG);

        WallMessage WallMSG2(Wall_MESSAGE_TYPES::GRAB_IMMEDIATELY);

        WallMSG2.Target = Owner;

        GlobalSystem->MyDispatchMessage(&WallMSG2);

        WallMessage WallMSG3(Wall_MESSAGE_TYPES::DELAY);

        WallMSG3.Delay = 2.0f;

        GlobalSystem->MyDispatchMessage(&WallMSG3);


        //ENGINE->m_Sound->StopSound("music_explore");
        //ENGINE->m_Sound->PlayEvent("music_escape");
        ENGINE->m_Sound->StopAll();
        ENGINE->m_Sound->PlayEvent("CB/music_escape", &instance, true);

        triggered = true;

        DestroyMessage destmsg;
        GlobalSystem->MyDispatchMessage(&destmsg);

        // @@ Henry Remove the trigger object.
        //GlobalFactory->RemoveGameObject(Owner->GetEntityId());
    }
}
