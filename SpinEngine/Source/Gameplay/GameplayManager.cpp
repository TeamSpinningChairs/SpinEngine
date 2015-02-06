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
#include "GameplayManager.h"
#include "SysCompListener.h"

GameplayManager::GameplayManager(void) : ISystem("Input", SystemType::ST_Input)
{
}

bool GameplayManager::Initialize(void)
{
	this->SystemComponents = new SysComponentListener(this);
	GlobalSystem->RegisterListener(MESSAGE_COMPONENT, this->SystemComponents);
	return true;
}

void GameplayManager::Update(float dt)
{
	for (IComponent * PC : this->Components)
	{
		PC->Update(dt);
	}
}

void GameplayManager::Shutdown()
{
	GlobalSystem->RegisterListener(MESSAGE_COMPONENT, this->SystemComponents);
	delete this->SystemComponents;
}

