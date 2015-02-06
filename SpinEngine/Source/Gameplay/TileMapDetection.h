/****************************************************************************/
/*!
\author Henry Morgan
\par    email: henry.morgan@digipen.edu
\par    Course: GAM 200
\brief

Provides access to the tilemap, without resolving any collision against
it. Primarily so that the wall's claws can remove tiles from the tilemap
without ever needing to collide with it.

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#pragma once
#include "IComponent.h"
#include "FactoryManager.h"


#define TILE_WIDTH 2.0f
#define HALF_TILE_WIDTH 1.0f
#define QUARTER_TILE_WIDTH 0.5f

class TileMapDetection : public IComponent
{
public:
  TileMapDetection(IEntity *owner, TileMapData &tilemap, GameObjectUmap &objects);

  bool Initialize() override;
  void Serialize (DynamicElement* props, Serializer::DataNode* data) {}
  void Update(float dt) override;
  void Release() override;

  //Returns the tilemap ID of the object, and sets reference to the object if it hit something.
  //If 0, we're not over any tiles.
  int GetCurrentGameObject(GameObject &ref);
  int GetGameObject(Vector2D position, GameObject &ref);
  void RemoveCurrentTileFromMap(bool andDestroy = false);

  int GetCurrentGameObjectIndex();

  bool AddTileToMap(std::string spritename, float posX, float posY);


  //Returns a random index
  //NOT USING THIS
  int GetHandTarget();

private:
  TileMapDetection();
  Vector3D &pos_;
  //GameObject currentTile;
  int lastTileIndex;
  int lastObjectIndex;
  int lastTileID;

  //Valid reference as long as FactoryManager is alive,
  //and TileMapDetection is only alive while FactoryManager is.
  TileMapData &tilemap_;
  GameObjectUmap &gameObjects_;
};