/****************************************************************************/
/*!
\author Judy Cheng
\par    email: j.cheng\@digipen.edu (Optional)
\par    Course: GAM 200
\date   xx/xx/xx (Optional)
\brief

Contains the information when two objects are in contact and the resolution.


Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#include "manifold.h"
#include "Precompiled.h"

PhysicsMaterial::PhysicsMaterial()
{
  staticFriction = 0.5f;
  staticFriction2 = 0.5f;
  dynamicFriction = 0.3f;
  dynamicFriction2 = 0.3f;
  frictionCombine = 0.0f;
  //friction = 0.2f;
}

void Manifold::Solve(void)
{
  // Give the current data
  CollisionRegistry[A->bodyShape->GetID()][B->bodyShape->GetID()](A, B, this);
}

void Manifold::PreStep(float dt)
{

   const float EPSILON = 0.0001f;

  // Calculate the average restitution
  e = std::min(A->restitution, B->restitution);
 
  // Calculate the static and dynamic friction
  //staticFriction = std::sqrt( A->stFric * B->stFric );
  //dynamicFriction = std::sqrt( A->dynFric * B->dynFric );

  staticFriction = std::sqrt(A->bodyShape->material.staticFriction * 
    B->bodyShape->material.staticFriction);

  dynamicFriction = std::sqrt(A->bodyShape->material.dynamicFriction * 
    B->bodyShape->material.dynamicFriction);

  for (unsigned int i = 0; i < contactCount; ++i)
  {
    // Calculate the radii
    Vector2D radii_A = contact[i] - A->position;
    Vector2D radii_B = contact[i] - B->position;

    Vector2D relativeVel = B->velocity + Vector2D::CrossProduct(B->angularVelocity, radii_B) - 
                          A->velocity - Vector2D::CrossProduct(A->angularVelocity, radii_A);

    if(relativeVel.Magnitude() < ( (1.0f/60.0f) * (Vector2D(0,*physics->GRAVITY))).Magnitude() + EPSILON)
      e = 0.0f;
  }


}

void Manifold::AppyImpulse(void)
{
  // If static
  if (A->isStatic && B->isStatic)
  {
    A->velocity.Clear();
    B->velocity.Clear();
    return;
  }

  if (A->isGhost == true || B->isGhost == true)
  {
    return;
  }

  for (unsigned int i = 0; i < contactCount; ++i)
  {
    // Calculate radii from center of mass to contact
    Vector2D radii_a = contact[i] - A->position;
    Vector2D radii_b = contact[i] - B->position;

    // Relative velocity at contact
    Vector2D relativeVel = B->velocity + Vector2D::CrossProduct(B->angularVelocity, radii_b) -
                          A->velocity - Vector2D::CrossProduct(A->angularVelocity, radii_a);

    // Relative velocity along normal
    float contactVel = Vector2D::DotProduct(relativeVel, normal);

    // If velocities are separating do not resolve
    if (contactVel > 0)
    {
      return;
    }

    float crossA = Vector2D::CrossProduct(radii_a, normal);
    float crossB = Vector2D::CrossProduct(radii_b, normal);
    float invMassSum = A->invMass + B->invMass + (crossA * crossA) * A->invInertia + (crossB * crossB) * B->invInertia;
    e  = 0.2003f; //epsilon value
    // Calculate the impulse scalar
    float j = -(1.0f + e) * contactVel;
    j /= invMassSum;
    j /= (float)contactCount;

    // Apply the impulse
    Vector2D impulse = normal * (j/2);
    A->ApplyImpulse(-impulse, radii_a);
    B->ApplyImpulse(impulse, radii_b);

    // Friction impulse
    relativeVel = B->velocity + Vector2D::CrossProduct(B->angularVelocity, radii_b) - 
                  A->velocity - Vector2D::CrossProduct(A->angularVelocity, radii_a);

    Vector2D tangentVec = relativeVel - (normal * Vector2D::DotProduct(relativeVel, normal));
    tangentVec.Normalize();

    // Solve magnitude to apply along friction vector
    float jt = -Vector2D::DotProduct(relativeVel, tangentVec);
    jt /= invMassSum;
    jt /= (float)contactCount;

    // Not applying tiny friction impulses
    if (jt == 0.0f)
    {
      return;
    }

    // Clamp magnitude of friction and create impulse vector
    Vector2D tangentImpulse;
    tangentImpulse = tangentVec * jt;
    if (std::abs(jt)< j * staticFriction)
      tangentImpulse = tangentVec * jt;
    else
      tangentImpulse = tangentVec * -j * dynamicFriction;


    // Apply friction impulse
  /*  A->ApplyImpulse(-1 * (tangentImpulse), radii_a);
    B->ApplyImpulse((tangentImpulse), radii_b);*/
  }


}

void Manifold::CorrectPosition(void)
{
  if (A->isGhost == true || B->isGhost == true)
  {
    return;
  }

  const float slop = 0.05f;//0.01
  const float percent = 0.8f;//0.4f; // 40% //0.2

  // Allows object to penetrate slightly without position correction from occurring 
  Vector2D correction = (std::max(penetration - slop, 0.0f) / (A->invMass + B->invMass)) * normal * percent;
 
  A->position -= correction * A->invMass;
  B->position += correction * B->invMass;
 
}
