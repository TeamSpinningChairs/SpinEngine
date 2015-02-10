/****************************************************************************/
/*!
\author Henry Morgan
\par    email: henry.morgan@digipen.edu
\par    Course: GAM 200
\brief

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#include "Precompiled.h"
#include "IEntity.h"
#include "CollisionDelegate.h"

ZilchDefineType(CollisionDelegate, SpinningZilch)
{
	type->HandleManager = ZilchManagerId(Zilch::PointerManager);
}

CollisionDelegate::CollisionDelegate(IEntity *parent) : parent_(parent)
{
}

void CollisionDelegate::InitializeCollisionCallback()
{
  reinterpret_cast<RigidBody*>(parent_->GetComponent(CT_Body))->AddTriggerCallback(this);
}
