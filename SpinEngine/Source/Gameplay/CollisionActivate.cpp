/****************************************************************************/
/*!
\file   NewMessageSystem.cpp
\author Steven Gallwas
\par    email: s.gallwas\@digipen.edu
\par    Course: GAM 200
\date   9/16/14
\brief
This file contains the implementation for the base messaging system.

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#include "Precompiled.h"
#include "CollisionActivate.h"



CollisionActivate::CollisionActivate(GameObject Parent) : IComponent(Component_Type::CT_DOOR, Parent), CollisionDelegate(Parent)
{
    ObjectSwitch = NULL;
}

bool CollisionActivate::Initialize()
{
    ObjectSwitch = reinterpret_cast<SwitchComponent *>(Owner->GetComponent(CT_SWITCH));
    InitializeCollisionCallback();
    if (ObjectSwitch != NULL)
    {
        return true;
    }

    return false;
}

void CollisionActivate::Update(float dt)
{

}

void CollisionActivate::Release()
{

}

void CollisionActivate::OnCollision(GameObject)
{
    ObjectSwitch->ActivateSwitch();
}


