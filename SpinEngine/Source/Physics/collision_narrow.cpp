/****************************************************************************/
/*!
\author Judy Cheng
\par    email: j.cheng\@digipen.edu (Optional)
\par    Course: GAM 200
\date   xx/xx/xx (Optional)
\brief

Refine collision detection that returns several information when two objects
are in contact


Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/

#include "collision_narrow.h"
#include "Precompiled.h"

CollisionTest CollisionRegistry[Primitive::pCount][Primitive::pCount] =
{
  {
    CircleAndCircle, CircleAndAABB
  },

  {
    AABBAndCircle, AABBAndAABB
  },
};

float CalculateRestitution(RigidBody *a, RigidBody *b)
{
  return (std::min(a->restitution, b->restitution));
}

void CircleAndCircle(RigidBody *a, RigidBody *b, Manifold *data)
{
   // Create pointers
  Circle *Circle1 = reinterpret_cast<Circle *>(a->bodyShape);
  Circle *Circle2 = reinterpret_cast<Circle *>(b->bodyShape);

   // Calculate the vector between Circle1 and Circle2
   Vector2D midline = Circle2->body->position - Circle1->body->position;

   // Calculate the distance squared
   float distSqr = midline.SqrMagnitude();
   
   // Calculated added radii
   float radius = Circle1->radius + Circle2->radius;

    // Case 0: No Intersection
   if (distSqr > radius * radius)
   {
     data->contactCount = 0;
     return;
   }

   float distance = sqrt(distSqr);
   // Update contact
   data->contactCount = 1;

   Vector2D normal;

   // Case 1: if circles are on top of each other
   if (distance == 0.0f)
   {
     data->penetration = Circle1->radius;
     data->normal = Vector2D(1, 0);
     data->contact[0] = a->position;
   }

   // Case 2: if circles intersects
   else
   {
     data->penetration = radius - distance;
     data->normal = midline.Normalize();
     data->contact[0] = data->normal * Circle1->radius + a->position;
   }

   // Add contact
   data->A = Circle1->body;
   data->B = Circle2->body;

   data->restitution = CalculateRestitution(Circle1->body, Circle2->body);
}

void AABBAndAABB(RigidBody *a, RigidBody *b, Manifold *data)
{
  AABB *AABB1 = reinterpret_cast<AABB *>(a->bodyShape);
  AABB *AABB2 = reinterpret_cast<AABB *>(b->bodyShape);

   // Calculate the vector between the objects
   Vector2D midline = AABB1->body->position  - AABB2->body->position;

   float xOverlap = (AABB1->halfSize.x) + (AABB2->halfSize.x) - fabs(midline.x);
   data->contactCount = 1;
   if (xOverlap > 0)
   {
     data->contactCount = 1;
     float yOverlap = (AABB1->halfSize.y) + (AABB2->halfSize.y) - fabs(midline.y);

     if (yOverlap > 0)
     {
       if (xOverlap < yOverlap)
       {
         Vector2D normal = midline.x < 0 ? Vector2D(1,0) : Vector2D(-1,0);
         data->A = AABB1->body;
         data->B = AABB2->body;
         data->normal = normal;
         data->penetration = xOverlap;
         data->restitution = CalculateRestitution(AABB1->body, AABB2->body);
         data->contact[0] = data->normal * a->bodyShape->halfSize.x + a->position;
         return;
       }
       else
       {
         Vector2D normal = midline.y < 0 ? Vector2D(0,1) : Vector2D(0,-1);
         data->A = AABB1->body;
         data->B = AABB2->body;
         data->normal = normal;
         data->penetration = yOverlap;
         data->restitution = CalculateRestitution(AABB1->body, AABB2->body);
         data->contact[0] = data->normal * a->bodyShape->halfSize.y + a->position;
         return;
       }
     }
   }
   // No Collision
   data->contactCount = 0;

}

void AABBAndCircle(RigidBody *a, RigidBody *b, Manifold *data)
{
  AABB *aabb;
  Circle *circle;
  // Create pointers based on what is what
  if(a->bodyShape->GetID() == Primitive::pCircle)
  {
    aabb = reinterpret_cast<AABB *>(b->bodyShape);
    circle = reinterpret_cast<Circle *>(a->bodyShape);
  }
  else
  {
    aabb = reinterpret_cast<AABB *>(a->bodyShape);
    circle = reinterpret_cast<Circle *>(b->bodyShape);
  }

  //  Calculate the vector between the objects
  Vector2D midline = circle->body->position - aabb->body->position;

  // Closest point on A to the center of B
  Vector2D closest = midline;

  // Calculate half extent for each axis
  float x_extent = aabb->halfSize.x;
  float y_extent = aabb->halfSize.y;

  // Clamp t0=o point to edges of AABB
  closest.x = Clamp(-x_extent, x_extent, closest.x);
  closest.y = Clamp(-y_extent, y_extent, closest.y);

  bool inside = false;

  // If circle is inside, clamp the circle's center to the closest
  // AABB's edge

  if (midline == closest)
  {
    inside = true;

    // Find the closest AABB's axis
    if (abs(midline.x) > abs(midline.y))
    {
      // Clamp to the closest extent
      if (closest.x > 0)
        closest.x = x_extent;
      else
        closest.x = -x_extent;
    }
    // y axis is shorter
    else
    {
      // Clamp to closest extent
      if (closest.y > 0)
        closest.y = y_extent;
      else
        closest.y = -y_extent;
    }
  }

  Vector2D normal = midline - closest;
  float distance = normal.Magnitude();
  float radius = circle->radius;

  // Circle not inside the AABB
  if (distance > radius * radius && !inside)
  {
    data->contactCount = 0;
    return;
  }

  distance = sqrtf(distance);

  // If circle is inside AABB,
  // Collision normal flipped to point outside
  if (inside)
  {
    data->normal = -midline;
    data->penetration = radius - distance;
    data->restitution = CalculateRestitution(aabb->body, circle->body);
  }
  else
  {
    data->normal = midline;
    data->penetration = radius - distance;
    data->restitution = CalculateRestitution(aabb->body, circle->body);
  }

  // Update contact
  data->contactCount = 1;
  data->A = aabb->body;
  data->B = circle->body;
}

void CircleAndAABB(RigidBody *b, RigidBody *a, Manifold *data)
{
  AABBAndCircle(b, a, data);
}

float Clamp(float min, float max, float x)
 {
    if (x < min)
      return min;
    else if (x > max)
      return max;
    return x;
 }
