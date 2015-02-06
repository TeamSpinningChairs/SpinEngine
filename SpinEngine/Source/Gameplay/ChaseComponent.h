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

#include "Precompiled.h"
#include "IComponent.h"
#include "IEntity.h"

class ChaseComponent : public IComponent
{

public:
	ChaseComponent(IEntity * Parent);
	
	ChaseComponent(IEntity * Parent, float speed);
	
	bool Initialize();

	void Serialize (DynamicElement* props, Serializer::DataNode* data) {}

	void Update(float dt);
	
	void Release();

	void SetTarget(GameObject);

	void SetChaseSpeed(float);

	GameObject GetTarget();

private:
	GameObject target;
	float ChaseSpeed;
	int ScrollDirection;
};


