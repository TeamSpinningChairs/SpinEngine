/****************************************************************************/
/*!
\author Judy Cheng
\par    email: j.cheng\@digipen.edu (Optional)
\par    Course: GAM 200
\date   xx/xx/xx (Optional)
\brief

The physic manager that handles all of the physics events.


Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#include "Precompiled.h"
#include "PhysicsManager.h"
#include "Settings.h"

PhysicsManager *physics = nullptr;

ZilchDefineType(PhysicsManager, SpinningZilch)
{
	type->HandleManager = ZilchManagerId(Zilch::PointerManager);
}


PhysicsManager::PhysicsManager() : ISystem("Physics", SystemType::ST_Physics)
{
  float *temp = NULL;
  GlobalSettings->GetFloatValue("___ PHYSICS SETTINGS ___", temp, false);
  GlobalSettings->GetFloatValue("Gravity", GRAVITY, true);
}
PhysicsManager::~PhysicsManager()
{
}

bool PhysicsManager::Initialize()
{
  return true;
}

void PhysicsManager::Update(float dt)
{
  Step(dt);
}

void PhysicsManager::Shutdown()
{
  colliders.clear();
  bodies.clear();
}

void PhysicsManager::ColliderCheck()
{
  //  COLLIDER LIST
  for (unsigned int i = 0; i < colliders.size(); ++i)
  {
      if (colliders[i] == NULL)
      {
          continue;
      }

    Primitive *a = colliders[i];

    for (unsigned int j = i + 1; j < colliders.size(); ++j)
    {     
      Primitive *b = colliders[j];

      if (a->active == false && b->active == false)
      {
        continue;
      }

      Vector3D posA = a->GetOwner()->GetTransform()->GetPosition();
      Vector3D posB = b->GetOwner()->GetTransform()->GetPosition();

      if (a->Id == Primitive::pCircle && b->Id == Primitive::pCircle)
      {      
        if (CollisionDetectorBroad::CircleAndCircle(reinterpret_cast<Circle *>(a), posA, reinterpret_cast<Circle *>(b), posB))
        {
          printf("Circle");
        }
      }
      else if (a->Id == Primitive::pAABB && b->Id == Primitive::pAABB)
      {       
        if (CollisionDetectorBroad::AABBAndAABB(reinterpret_cast<AABB *>(a), posA, reinterpret_cast<AABB *>(b), posB))
        {
          //printf("AABB");
          //We get to this point twice for one collision, right? If so, only need this one call.
          reinterpret_cast<RigidBody*>((a->GetOwner())->GetComponent(CT_Body))->Trigger(b->GetOwner());
          reinterpret_cast<RigidBody*>((b->GetOwner())->GetComponent(CT_Body))->Trigger(a->GetOwner());

        }
      }
      else if (a->Id == Primitive::pAABB && b->Id == Primitive::pCircle)
      {
        if (CollisionDetectorBroad::AABBAndCircle(reinterpret_cast<AABB *>(a), posA, reinterpret_cast<Circle *>(b), posB))
        {
          printf("AABB to Circle");
        }
      }
      else if (a->Id == Primitive::pCircle && b->Id == Primitive::pAABB)
      { 
        if (CollisionDetectorBroad::CircleAndAABB(reinterpret_cast<Circle *>(a), posA, reinterpret_cast<AABB *>(b), posB))
        {
          printf("Circle to AABB");
        }
      }
    }
  }
}

void PhysicsManager::RigidBodyCheck()
{
  // Go through the bodies
  for (unsigned int i = 0; i < bodies.size(); ++i)
  {
      if (bodies[i] == NULL)
      {
          continue;
      }

    RigidBody *a = bodies[i];

    // Loop through the next body
    for (unsigned int j = i + 1; j < bodies.size(); ++j)
    {
      RigidBody *b = bodies[j];

      // Check if these object are static
      if (a->isStatic == true && b->isStatic == true)
      {
        continue;
      }

      // Set manifold data
      Manifold m(a,b);

      // Solve
      m.Solve();

      //Check if there's any contact count
      if (m.contactCount)
      {
        // Append new element to the end
        contacts.emplace_back(m);
      }
    }
  }
}
void PhysicsManager::Step(float dt)
{

  ColliderCheck();

   // Clear any old information in the vector
  contacts.clear();

  RigidBodyCheck();

  // Integrate forces
  for (unsigned int i = 0; i < bodies.size(); ++i)
  {
    IntegrateForces(bodies[i], dt);
  }

  // Pre-Steps
    for (unsigned int i = 0; i < contacts.size(); ++i)
    {
      contacts[i].PreStep(dt);
    }

   //Solve collisions
  for (int j = 0; j < 10/*TEMP NUMBER*/; ++j)
  {
    for (unsigned int i = 0; i < contacts.size();++i)
    {
      contacts[i].AppyImpulse();
    }
  }

  // Integrate Velocities
  for (unsigned int i = 0; i < bodies.size(); ++i)
  {
    IntegrateVelocity(bodies[i], dt);
  }

   //Correct Positions
  for (unsigned int i = 0; i < contacts.size(); ++i)
  {
     contacts[i].CorrectPosition();
  }

  // Clear forces
  for (unsigned int i = 0; i < bodies.size(); ++i)
  {
    RigidBody *b = bodies[i];
    b->forceAccum.Clear();
  }

  for (unsigned int i = 0; i < bodies.size(); ++i)
  {
    bodies[i]->Update(dt);
  }
}

void PhysicsManager::IntegrateForces(RigidBody *body, float dt)
{
  // Skip if static
  if (body->isStatic == true)
  {
    return;
  }

  if (body->useGravity)
  {
    // Symplectic Euler w/ gravity
    body->velocity += ((body->forceAccum * body->invMass + Vector2D(0,*physics->GRAVITY)) * (dt / 2.0f));
  }
  else
  {
    // Without gravity
    body->velocity += ((body->forceAccum * body->invMass) * (dt / 2.0f));
  }
}

void PhysicsManager::IntegrateVelocity(RigidBody *body, float dt)
{
  // Skip if static
  if (body->isStatic == true)
  {
    return;
  }
  body->position += body->velocity * dt;
  body->orientation += body->angularVelocity * dt;
  body->SetOrientation(body->orientation);
  IntegrateForces(body, dt);
}
