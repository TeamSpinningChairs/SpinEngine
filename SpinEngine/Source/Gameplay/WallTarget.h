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
#include"IComponent.h"
#include"IEntity.h"
#include"Wall.h"

#define TARGETDISTANCE 25.0f

class WallTarget : public IComponent
{

public:
	WallTarget(IEntity * Owner);

	bool AddTarget(void);
	
	bool RemoveTarget(void);
	
	bool Initialize(void);
	void Serialize (DynamicElement* props, Serializer::DataNode* data) {}
	void Update(float dt);
	
	void Release(void);

private:
	WallComp *Wall;
	bool Added;

};