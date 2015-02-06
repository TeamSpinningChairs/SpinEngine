/****************************************************************************/
/*!
\author Judy Cheng
\par    email: j.cheng\@digipen.edu (Optional)
\par    Course: GAM 200
\date   xx/xx/xx (Optional)
\brief

Header for the narrow collision detection.


Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#pragma once
#include "primitives.h"
#include "manifold.h"
#include "collision_broad.h"
#include "mathlib.h"

struct Manifold;
class RigidBody;

typedef void (*CollisionTest)(RigidBody *a, RigidBody *b, Manifold *data);
extern CollisionTest CollisionRegistry[Primitive::pCount][Primitive::pCount];

void CircleAndCircle(RigidBody *a, RigidBody *b, Manifold *data);
void AABBAndAABB(RigidBody *a, RigidBody *b, Manifold *data);
void AABBAndCircle(RigidBody *a, RigidBody *b, Manifold *data);
void CircleAndAABB(RigidBody *b, RigidBody *a, Manifold *data);
float Clamp(float, float, float);

