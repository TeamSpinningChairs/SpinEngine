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
#include "DestroyOnWall.h"
#include "FactoryAccess.h"
#include "PlayerController.h"

DestroyOnWall::DestroyOnWall(GameObject Parent, GameObject Wall) : IComponent(Component_Type::CT_WALLDESTROY, Parent)
{
	DeathWall = Wall;
}

bool DestroyOnWall::Initialize()
{
	return true;
}

void DestroyOnWall::Update(float dt)
{
	// destroy self when behind the wall
	if (Owner->GetTransform()->GetWorldPosition().x <= DeathWall->GetTransform()->GetWorldPosition().x)
	{
    reinterpret_cast<PlayerController*>(Owner->GetComponent(CT_Player_Controller))->KillPlayer();
     //Henrykillplayerfunction();
		 //GlobalFactory->RemoveGameObject(Owner->GetEntityId());	
	}
}

void DestroyOnWall::Release()
{

}


