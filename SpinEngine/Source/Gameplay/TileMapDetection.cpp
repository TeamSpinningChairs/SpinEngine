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
#include "Precompiled.h"
#include "TileMapDetection.h"
#include "FactoryAccess.h"
#include "TileMapData.h"

TileMapDetection::TileMapDetection(IEntity *owner, TileMapData &tilemap, GameObjectUmap &objects) :
tilemap_(tilemap), pos_(owner->GetTransform()->GetWorldPosition()), gameObjects_(objects), lastTileID(0), lastTileIndex(0),
lastObjectIndex(0), IComponent(Component_Type::CT_TileMapDetector, Owner)
{

}

bool TileMapDetection::Initialize()
{
  return true;
}
void TileMapDetection::Update(float dt)
{

}
void TileMapDetection::Release()
{

}

//Returns the tilemap ID of the object, and sets reference to the object if it hit something.
int TileMapDetection::GetCurrentGameObject(GameObject &ref)
{
  //Divide by 2 (since all values are * tile width (which is 2.0f)
  float fx = pos_.x / 2.0f;
  float fy = pos_.y / 2.0f;
  int x = static_cast<int>(fx);
  int y = static_cast<int>(fy);

  //Preventing access outside of the array.
  //(aka what happens when/if we go beyond the map)
  if (x >= tilemap_.width || y >= tilemap_.height
    || x < 0 || y < 0)
  {
    return 0;
  }

  //The tile index of our current world position
  lastTileIndex = tilemap_.numTiles - 1 - (tilemap_.width - 1) + x - (tilemap_.width * y);
  //Whether or not we're over something (0 or 1)
  lastTileID = (*(tilemap_.tiles))[lastTileIndex];
  //The index of the object (if we're over one)
  lastObjectIndex = tilemap_.factoryIndeces[lastTileIndex];

  if (lastTileID != 0)
    ref = gameObjects_[lastObjectIndex];
  return lastTileID;
}

//
//Returns the tilemap ID of the object, and sets reference to the object if it hit something.
int TileMapDetection::GetGameObject(Vector2D position, GameObject &ref)
{
  //Divide by 2 (since all values are * tile width (which is 2.0f)
  float fx = position.x / 2.0f;
  float fy = position.y / 2.0f;
  int x = static_cast<int>(fx);
  int y = static_cast<int>(fy);

  //Preventing access outside of the array.
  //(aka what happens when/if we go beyond the map)
  if (x >= tilemap_.width || y >= tilemap_.height
    || x < 0 || y < 0)
  {
    return 0;
  }

  //I originally made separate variables for this (specTileIndex/Id/ObjectIndex)
  //but changed lastTileIndex back (it was the only part I needed.)
  //This could get messy later on, but it shouldn't, because we should only ever
  //use one version of GetGameObject in any given component.
  //(I changed it back so I wouldn't have to write another version of RemoveCurrentTileFromMap)

  //The tile index of our current world position
  lastTileIndex = tilemap_.numTiles - 1 - (tilemap_.width - 1) + x - (tilemap_.width * y);
  //Whether or not we're over something (0 or 1)
  int specTileID = (*(tilemap_.tiles))[lastTileIndex];
  //The index of the object (if we're over one)
  int specObjectIndex = tilemap_.factoryIndeces[lastTileIndex];

  if (specTileID != 0)
    ref = gameObjects_[specObjectIndex];
  return specTileID;
}

bool TileMapDetection::AddTileToMap(std::string spritename, float posX, float posY)
{
  //Create the tile object
  //From x- and y- position, get place in array
  //Divide by 2 (since all values are * tile width (which is 2.0f)
  posX /= 2.0f;
  posY /= 2.0f;
  int x = static_cast<int>(posX);
  int y = static_cast<int>(posY);

  //Preventing access outside of the array.
  //(aka what happens when/if we go beyond the map)
  if (x >= tilemap_.width || y >= tilemap_.height
    || x < 0 || y < 0)
  {
    return false;
  }

  int newTileIndex = tilemap_.numTiles - 1 - (tilemap_.width - 1) + x - (tilemap_.width * y);

  //Preventing duplicate tiles
  if (tilemap_.factoryIndeces[newTileIndex] != -1)
    return false;

  //Creating tile and recording its index (i.e. that it exists)
  tilemap_.factoryIndeces[newTileIndex] = GlobalFactory->AddTile(spritename, newTileIndex, tilemap_.width);
  //Adding new tile to collision map. (Will need to add options to add to target map)
  (*tilemap_.tiles)[newTileIndex] = 1;
  //Initialize tile
  gameObjects_[tilemap_.factoryIndeces[newTileIndex]]->Initialize();
  return true;
}

//Assumes that GetGameObject or GetCurrentGameObject have been called recently
void TileMapDetection::RemoveCurrentTileFromMap(bool andDestroy)
{
  //Remove us from the tilemap
  (*(tilemap_.tiles))[lastTileIndex] = 0;
  //Tell the tiles around us to update their sprites
  reinterpret_cast<SpriteRenderer*>(gameObjects_[tilemap_.factoryIndeces[lastTileIndex]]->GetComponent(CT_SpriteRenderer))->UpdateSurroundingTileSprites();
  //If we're being destroyed, tell the Factory to get rid of us
  if (andDestroy)
  {
    GlobalFactory->RemoveTileObject(gameObjects_[tilemap_.factoryIndeces[lastTileIndex]]->GetEntityId());
  }
  //Change our entry to show we were removed from the Factory
  tilemap_.factoryIndeces[lastTileIndex] = -1;

  //@PARTICLES: place/activate the appropriate emitter depending on whether or not we're immediately destroying the tile we remove


}

int TileMapDetection::GetCurrentGameObjectIndex()
{
  return lastObjectIndex;
}

int TileMapDetection::GetHandTarget()
{
  //

  return (std::rand() % tilemap_.numTiles);
}
