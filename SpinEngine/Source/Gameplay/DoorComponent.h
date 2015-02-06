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
#include "IComponent.h"
#include "SwitchComponent.h"
#include "CollisionDelegate.h"


class ExtendBlock;

#define DOOR_HEIGHT 7.5f

class DoorComponent : public IComponent, public CollisionDelegate
{

public:

DoorComponent(GameObject);

bool Initialize();

void Serialize (DynamicElement* props, Serializer::DataNode* data) {}

void Update(float dt);

void Release();

bool doorTriggered;

bool Locked;

private:
    Vector3D StartingPoint;
    SwitchComponent * DoorSwitch;
    RigidBody * DoorBody;
    void OnCollision(GameObject) override;
};