/****************************************************************************/
/*!
\author Steven Gallwas
\par    email: s.gallwas\@digipen.edu
\par    Course: GAM 250
\brief
This file contains the implementation for the base messaging system.

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#pragma once

#include "IComponent.h"
#include "IEntity.h"
#include "rigidbody.h"
#include "SpriteRenderer.h"
#include "WallControllerListener.h"
#include "collision_tilemap.h"
#include "ObjectPool.h"
#include "Wall.h"
#include "NewMessageSystem.h"
#include "FactoryAccess.h"

class WallController : public IComponent //, public CollisionDelegate
{
public:

  WallController(GameObject Parent, int ControllerNum);
 
  ~WallController();

  bool Initialize();

  void Update(float dt);

  void Release();
  
  void LaunchTentacle(Vector3D Direction);

  bool WallActive;

  GameObject PrisonPart;
  GameObject BodyPart;

private:

  int ControllerNumber;
  GameObject parent;
  WallControllerListener * WallListener;



  float movevalue;

  GameObject Runners[10];

  // the wall needs to have an array of dynamic hands
  DynamicHand * Dynamic_Hands[DYNAMIC_HANDCOUNT + RESERVED_HANDS];

  //void OnCollision(GameObject) override;

};
