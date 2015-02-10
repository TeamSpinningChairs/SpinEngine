/****************************************************************************/
/*!
\author Henry Morgan
\par    email: henry.morgan@digipen.edu
\par    Course: GAM 200
\brief

Component to provide collision against the factory's tilemap. Snaps to nearest
valid space upon collision. Assumes presence of a rigidbody sibling, and
adjusts velocity/acceleration accordingly.

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#pragma once
#include "IComponent.h"
#include "FactoryManager.h"
#include "Transform.h"
#include "FactoryAccess.h"

struct TileMapData;

class TileMapCollision : public IComponent
{
public:
  TileMapCollision(IEntity *owner, TileMapData &tilemap);
  TileMapCollision(IEntity* owner);

  bool Initialize() override;
  void Serialize (DynamicElement* props, Serializer::DataNode* data) {}
  void Update(float dt) override;
  void Release() override;

  //Returns the tilemap ID of the object, and sets reference to the object if it hit something.
  /*int GetCurrentGameObject(GameObject &ref);*/
  bool TopIsColliding();
  bool BottomIsColliding();
  bool LeftIsColliding();
  bool RightIsColliding();

private:
  TileMapCollision();

  int flag_; //Our collision flag

  //In our owner's Transform:
  Transform &transform_;
  Vector3D &pos_;
  //In our owner's RigidBody:
  RigidBody &rigid_;
  Vector2D &vel_;
  Vector2D &acc_;

  GameObject currentTile;

  //Valid reference as long as FactoryManager is alive,
  //and TileMapCollision is only alive while FactoryManager is.
  TileMapData* tilemap_;
  
  //How do we get info to our owner? We need to communicate our flags to them.
  //iscollidingup/down/left/right
  
  //Transform we can just edit here

  //Helper functions:
  int CheckBinaryMapCollision(float posX, float posY); //returns our collision flags
  void SnapToCellX(float &transfcoord, float &rigidcoord); //Given a float value, snaps left 1/4 tile away from a wall
  void SnapToCellY(float &transfcoord, float &rigidcoord); //Given a float value, snaps to nearest int (i.e. border of tile)
  int GetCellValue(float fx, float fy);

};