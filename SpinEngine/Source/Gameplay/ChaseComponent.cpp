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
#include "Precompiled.h"
#include "ChaseComponent.h"
#include "NewMessageSystem.h"
#include "rigidbody.h"
#include "Math\mathlib.h"

ChaseComponent::ChaseComponent(IEntity * Parent) : IComponent(Component_Type::CT_CHASE, Parent)
{
	this->ChaseSpeed = 2.5f;
	this->target = NULL;
}


ChaseComponent::ChaseComponent(IEntity * Parent, float speed) : IComponent(Component_Type::CT_CHASE, Parent), ChaseSpeed(speed)
{
	this->target = NULL;
}

bool ChaseComponent::Initialize()
{
	return true;
}

void ChaseComponent::Update(float dt)
{
	if (this->target == NULL)
	{
		return;
	}

	// get the velocity of the target
	RigidBody *PlayerBody = reinterpret_cast<RigidBody *>(this->Owner->GetComponent(CT_Body));

	// calculate the distance between this object and the target object
	Vector2D Direction;

	Direction.x = this->target->GetTransform()->GetWorldPosition().x - this->Owner->GetTransform()->GetWorldPosition().x;
	Direction.y = this->target->GetTransform()->GetWorldPosition().y - this->Owner->GetTransform()->GetWorldPosition().y;
	
	// normalize the direction
	Direction.Normalize();

	// if the owner of this script does not have a rigid body, translate it instead
	if (PlayerBody == NULL)
	{
		this->Owner->GetTransform()->GetWorldPosition().x += Direction.x * dt;
		this->Owner->GetTransform()->GetWorldPosition().y += Direction.y * dt;

		return;
	}

	PlayerBody->velocity = Direction * this->ChaseSpeed;
}

void ChaseComponent::Release()
{
}

void ChaseComponent::SetTarget(GameObject Target)
{
	this->target = Target;
}

void ChaseComponent::SetChaseSpeed(float Speed)
{
	this->ChaseSpeed = Speed;
}

GameObject ChaseComponent::GetTarget()
{
	return this->target;
}