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
#include "WallTarget.h"
#include "DebugMessages.h"
#include "FactoryAccess.h"
#include "NewMessageSystem.h"

WallTarget::WallTarget(IEntity *Parent) : IComponent(Component_Type::CT_WALL_TARGET, Parent), Added(false)
{
}

bool WallTarget::AddTarget(void)
{
	for (int i = 0; i < TargetCount; i++)
	{
		if (this->Wall->Targets[i].TargetObject == NULL)
		{
			this->Wall->Targets[i].TargetObject = this->Owner;
			return true;
		}
	}

	return false;
}

bool WallTarget::RemoveTarget()
{
	for (int i = 0; i < TargetCount; i++)
	{
		if (this->Owner == this->Wall->Targets[i].TargetObject)
		{
			this->Wall->Targets[i].TargetObject = NULL;
		}
	}
	
	return true;
}

bool WallTarget::Initialize(void)
{
  Wall = reinterpret_cast<WallComp*>(GlobalFactory->GetWall());
	return true;
}

void WallTarget::Release(void)
{
	// I'm not sure what to do here
	return;
}

void WallTarget::Update(float dt)
{
	if (this->Added == true)
		return;
	
	//actuall calculate distance 
	float distance = this->Owner->GetTransform()->GetPosition().x - this->Wall->Position.x;

	if(distance)
	{
		if (this->AddTarget())
		{
			this->Added = true;
		}
	}
}