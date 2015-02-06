/****************************************************************************/
/*!
\author Judy Cheng
\par    email: j.cheng\@digipen.edu (Optional)
\par    Course: GAM 200
\date   xx/xx/xx (Optional)
\brief

Collision Detector Broad class that checks for quick collision


Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#pragma once
#include "primitives.h"

// Quick intersection tests;

class CollisionDetectorBroad
{
public:
  static bool CircleAndCircle(Circle *a, Vector3D aPos, Circle *b, Vector3D bPos);
  static bool AABBAndAABB(AABB *a, Vector3D aPos, AABB *b, Vector3D bPos);
  static bool AABBAndCircle(AABB *a, Vector3D aPos, Circle *b, Vector3D bPos);
  static bool CircleAndAABB(Circle *a, Vector3D aPos, AABB *b, Vector3D bPos);

private:

};