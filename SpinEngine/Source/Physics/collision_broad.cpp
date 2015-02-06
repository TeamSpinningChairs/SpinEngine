/****************************************************************************/
/*!
\author Judy Cheng
\par    email: j.cheng\@digipen.edu (Optional)
\par    Course: GAM 200
\date   xx/xx/xx (Optional)
\brief

Quick collision detection test that returns a true or false if there is a 
collision.


Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#include "collision_broad.h"
#include "Precompiled.h"

bool CollisionDetectorBroad::CircleAndCircle(Circle *a, Vector3D aPos, Circle *b, Vector3D bPos)
{
  // Calculate the distance between centre points of two Circle
  Vector3D aPosition = aPos;
  Vector3D bPosition = bPos;  
  float distance = Vector3D::Distance(aPosition, bPosition);


  if (distance <= a->radius * b->radius)
  {
    return true;
  }
  else
  {
    return false;
  }
}
bool CollisionDetectorBroad::AABBAndAABB(AABB *a, Vector3D aPos, AABB *b, Vector3D bPos)
{
  Vector3D aPosition = aPos;
  Vector3D bPosition = bPos;  

  if (aPosition.y + a->halfSize.y <= bPosition.y - b->halfSize.y || 
      bPosition.y + b->halfSize.y <= aPosition.y - a->halfSize.y ||
      aPosition.x + a->halfSize.x <= bPosition.x - b->halfSize.x || 
      bPosition.x + b->halfSize.x <= aPosition.x - a->halfSize.x)
  {
    return false;
  }
  else
  {
    return true;
  }
}

bool CollisionDetectorBroad::AABBAndCircle(AABB *a, Vector3D aPos, Circle *b, Vector3D bPos)
{
  Vector3D box = aPos;
  Vector3D circle = bPos;
  Vector3D point;
  point.x = bPos.x;
  point.y = bPos.y;

  // x-axis
  if (circle.x > box.x + a->halfSize.x)
  {
    point.x = circle.x + a->halfSize.x;
  }
  else if (circle.x < box.x - a->halfSize.x)
  {
    point.x = circle.x - a->halfSize.x;
  }

  // y-axis
  if (circle.y  > box.y + a->halfSize.y)
  {
    point.y = box.y + a->halfSize.y;
  }
  else if (circle.y < box.y - a->halfSize.y)
  {
    point.y = box.y - a->halfSize.y;
  }

  float distance = Vector3D::Distance(circle, point);

  if (distance >= b->radius * b->radius)
  {
    return false;
  }
  else
  {
    return true;
  }
}

bool CollisionDetectorBroad::CircleAndAABB(Circle *a, Vector3D aPos, AABB *b, Vector3D bPos)
{
  Vector3D box = bPos;
  Vector3D circle = aPos;
  Vector3D point;
  point.x = aPos.x;
  point.y = aPos.y;

  // x-axis
  if (circle.x > box.x + b->halfSize.x)
  {
    point.x = circle.x + b->halfSize.x;
  }
  else if (circle.x < box.x - b->halfSize.x)
  {
    point.x = circle.x - b->halfSize.x;
  }

  // y-axis
  if (circle.y  > box.y + b->halfSize.y)
  {
    point.y = box.y + b->halfSize.y;
  }
  else if (circle.y < box.y - b->halfSize.y)
  {
    point.y = box.y - b->halfSize.y;
  }

  float distance = Vector3D::Distance(circle, point);

  if (distance >= a->radius * a->radius)
  {
    return false;
  }
  else
  {
    return true;
  }
}
