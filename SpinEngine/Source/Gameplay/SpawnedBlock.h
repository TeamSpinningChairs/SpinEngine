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
#include "TileMapDetection.h"

class ExtendBlock;

class SpawnedBlock : public IComponent, public CollisionDelegate
{

public:

    SpawnedBlock(GameObject, ExtendBlock *, int);

    bool Initialize();
	void Serialize (DynamicElement* props, Serializer::DataNode* data) {}
    void Update(float dt);

    void Release();

private:

    TileMapDetection * TileDetection;

    ExtendBlock * parentBlock;

    int Direction;

    void OnCollision(GameObject) override;
};