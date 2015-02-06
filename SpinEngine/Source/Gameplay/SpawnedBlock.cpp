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
#include "SpawnedBlock.h"
#include "ExtendBlock.h"

SpawnedBlock::SpawnedBlock(GameObject Parent, ExtendBlock * block, int SpawnDirection) : IComponent(Component_Type::CT_SPAWN_BLOCK, Parent), CollisionDelegate(Parent)
{
    parentBlock = block;
    this->Direction = SpawnDirection;
    TileDetection = NULL;
}

bool SpawnedBlock::Initialize()
{
    InitializeCollisionCallback();
    TileDetection = reinterpret_cast<TileMapDetection *>(this->Owner->GetComponent(CT_TileMapDetector));
    return true;
}

void SpawnedBlock::Update(float dt)
{
    if (TileDetection->GetCurrentGameObject(Owner))
    {
        parentBlock->DeactivateDirection(Direction);
    }
}

void SpawnedBlock::Release()
{

}

void SpawnedBlock::OnCollision(GameObject otherObject)
{
    if (otherObject->GetComponent(CT_EXTEND_BLOCK) != NULL)
    {
        parentBlock->DeactivateDirection(Direction);
    }
}
