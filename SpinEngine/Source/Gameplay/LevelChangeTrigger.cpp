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
#include "LevelChangeTrigger.h"


LevelChangeTrigger::LevelChangeTrigger(GameObject Parent, std::string level) :
IComponent(Component_Type::CT_LEVELTRIGGER, Parent), CollisionDelegate(Parent),
leveltoLoad(level)
{
}

bool  LevelChangeTrigger::Initialize()
{
    InitializeCollisionCallback();
    return true;
}

void  LevelChangeTrigger::Update(float dt)
{
    
}

void  LevelChangeTrigger::Release()
{
    
}

void  LevelChangeTrigger::OnCollision(GameObject otherObject)
{
    if (otherObject->GetComponent(CT_Player_Controller) != NULL)
    {
      gGameStateCurr = GS_LOAD;
      gCurrentLevel = leveltoLoad;
    }
}

