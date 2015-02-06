/****************************************************************************/
/*!
\author Judy Cheng
\par    email: j.cheng\@digipen.edu (Optional)
\par    Course: GAM 200
\date   xx/xx/xx (Optional)
\brief

Header for the physics manager class


Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#pragma once

#include "Precompiled\Precompiled.h"
#include "collision_narrow.h"
#include "rigidbody.h"
#include "manifold.h"

// Physics system
class PhysicsManager : public ISystem
{
public:
  PhysicsManager();
  ~PhysicsManager();

  std::vector<RigidBody *>bodies; // Holds the bodies
  std::vector<Manifold> contacts; // Holds the contacts
  std::vector<Primitive *>colliders; // Holds the colliders

  // Overloaded methods
  bool Initialize() override;
  void Update(float dt) override;
  void Shutdown() override;

  void IntegrateForces(RigidBody *body, float dt);
  void IntegrateVelocity(RigidBody *body, float dt);
  void Step(float dt);
  void ColliderCheck();
  void RigidBodyCheck();

  //I changed this from a Vector2D so I could load it from Settings
  //in the new, non-breaking-between-levels persistent version.
  float *GRAVITY; 
};

extern PhysicsManager *physics;