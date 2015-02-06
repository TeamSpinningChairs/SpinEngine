/****************************************************************************/
/*!
\author Henry Morgan
\par    email: henry.morgan@digipen.edu
\par    Course: GAM 200
\brief

Saves player progress on collision with player.

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#include "Precompiled.h"
#include "Checkpoint.h"


Checkpoint::Checkpoint(GameObject parent, std::string name) :
CollisionDelegate(parent), IComponent(CT_CHECKPOINT, parent),
triggered_(false), myCheckpointName_(name)
{

}

bool Checkpoint::Initialize()
{
  InitializeCollisionCallback();
  return true;
}

void Checkpoint::Update(float dt)
{

}

void Checkpoint::Release()
{

}

void Checkpoint::OnCollision(GameObject otherObject)
{
  if (!triggered_ && otherObject->GetComponent(CT_Player_Controller))
  {
    triggered_ = true;
    gCurrentLevel = myCheckpointName_;
  }
}