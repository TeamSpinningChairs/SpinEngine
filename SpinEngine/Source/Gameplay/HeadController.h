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
#include "SpriteRenderer.h"

class HeadController : public IComponent, public CollisionDelegate
{
public:

  HeadController(GameObject Parent, int ControllerNum);

  ~HeadController();

  bool Initialize();

  void Update(float dt);

  void Release();

private:

  TileMapCollision * HeadCollision;
  SpriteRend HeadSprite;
  RigidBody * HeadBody;
  int ControllerNumber;
  GameObject parent;

  void OnCollision(GameObject) override;
};