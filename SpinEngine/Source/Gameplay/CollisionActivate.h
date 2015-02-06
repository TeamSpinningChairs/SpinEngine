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
#pragma once

#include "IComponent.h"
#include "CollisionDelegate.h"
#include "SwitchComponent.h"

class CollisionActivate : public IComponent, public CollisionDelegate
{

public:

    CollisionActivate(GameObject);

    bool Initialize();

	void Serialize (DynamicElement* props, Serializer::DataNode* data) {}

    void Update(float dt);

    void Release();

    bool doorTriggered;

    bool Locked;

private:
    Vector3D StartingPoint;

    SwitchComponent * ObjectSwitch;
    
    void OnCollision(GameObject) override;
};