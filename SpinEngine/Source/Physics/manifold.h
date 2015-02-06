/****************************************************************************/
/*!
\author Judy Cheng
\par    email: j.cheng\@digipen.edu (Optional)
\par    Course: GAM 200
\date   xx/xx/xx (Optional)
\brief

Contains the header of the manifold class.


Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#pragma once
#include "rigidbody.h"
#include "collision_narrow.h"
#include "primitives.h"

struct Contact
{
  Contact(): Pn(0.0f), Pt(0.0f), Pnb(0.0f), massNormal(0.0f), massTangent(0.0f), bias(0.0f) {}
  Vector2D position;
  Vector2D normal;
  Vector2D r1, r2;
  float seperation;
  float Pn; // accumulated normal impulse
  float Pt; // accumulated tangent impulse
  float Pnb; // accumulated normal impulse for position bias
  float  massNormal, massTangent;
  float bias;

};

struct PhysicsMaterial
{
  float staticFriction;
  float staticFriction2;
  float dynamicFriction;
  float dynamicFriction2;
  float frictionCombine;

  PhysicsMaterial();
};
// Contact Generation or Manifold
struct Manifold
{
  Manifold() {}

  Manifold(RigidBody *a, RigidBody *b) : A(a), B(b) {}

  RigidBody *A;

  RigidBody *B;

  float penetration;

  float restitution;

  // Combined frictions
  float friction;
  
  Vector2D normal;

  Vector2D contact[2];

  float staticFriction;

  float dynamicFriction;

  float e; // epsilon

  Contact contacts[2];

  unsigned contactCount; // The number of contacts found so far

  void Solve(void);
  void PreStep(float dt);
  void AppyImpulse(void);
  void CorrectPosition(void);

};
