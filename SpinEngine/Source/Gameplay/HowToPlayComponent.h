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

#include "IEntity.h"
#include "IComponent.h"


class HowToPlayComponent : public IComponent
{
public:

HowToPlayComponent(GameObject);

bool Initialize();

void Serialize (DynamicElement* props, Serializer::DataNode* data) {}

void Update(float dt);

void Release();
};
