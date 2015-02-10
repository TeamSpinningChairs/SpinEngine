/****************************************************************************/
/*!
\author Henry Morgan
\par    email: henry.morgan@digipen.edu
\par    Course: GAM 200
\brief

Factory class (see .cpp for more info)

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#pragma once

#include"Utilities\ISystem.h"
#include"Core\IEntity.h"
#include"Factory\Serializer\JSONLoader.h"
#include "Factory\Serializer\DynamicElement.h"
#include "Factory\ObjectData.h"
#include "Factory\LevelData.h"
#include "Camera.h"
#include "ComponentCreator.h"
#include "TileMapData.h"

//forward declaration
class GraphicsManager;
class FactoryAccess;

class FactoryManager : public ISystem
{
  friend class FactoryAccess;

public:
	ZilchDeclareDerivedType(FactoryManager, ISystem);
  //Constructor & Destructor
  FactoryManager(GraphicsManager* m_pGraphicsManager);
  ~FactoryManager();
  
  //Generic System Functions
  bool Initialize() override;
  void Update(float dt) override;
  void Shutdown() override;

  //Getters
  std::string GetName();
  GameObjectUmap* GetObjectList();
  unsigned int GetGameObjectCount();

  void SetPlayerNum(int);

  //Methods
  void AddEmptyGameObject();
  void AddGameObject(GameObject ent);
  void RemoveGameObject(EntityId id);
  void RemoveTileObject(EntityId id);
  void CreatePlayer();

  //Level & object creation
  void LoadLevel(std::string levelName);
  void UnloadLevel();
  IComponent *GetWall();
  void SetWall(IComponent *wall);
  void ClearEverythingInFactory();
  GameObject FindObjectByName(std::string);

  TileMapData tileMap_;

  GameObjectUmap m_Targets;
  FactoryAccess* FactAccess;
  
private:
  GameObjectUmap m_GameObjectList;
  //Graphics - related properties'


  GraphicsManager *m_pGraphicsManager;
  //counters that will not ever decrease:
  unsigned int m_TotalObjectCount;
  unsigned int m_DeletedObjectCount;
  //This goes up and down all the time, though!
  unsigned int m_CurrentObjectCount;

  //For giving all game objects access to the wall.
  IComponent *wall_;
  std::unordered_map<std::string, ComponentCreator *> SerialMap;
  void FactoryManager::AddComponentCreator(std::string name, ComponentCreator* creator);

  GameObject PlayerSpawn;

  int TotalPlayers;

  //Here so that FactoryAccess can grab it without knowing what GraphicsManager is
  IDirect3DDevice9 *directXDevice_;

  //Used to so that when we're creating a Player, we can add a pointer of their Transform to the camera.
  //Cleared once we finish loading a level.
  std::vector<Transform*> tempPlayerTransformRefs;
  //Used so that we can hold on to the "current" camera until we've finished loading.
  //(otherwise we wouldn't know whether or not we were using the "default" camera, I think.
  Camera *currentCameraRef;

  
  //JSONLoader loader;
  ObjectData defaults; //Will load all prefab properties in constructor.
  LevelData levels; //Will load all paths to level files, but not any level data itself

  bool PopulateLevel(DynamicElement &root);
  //Helpers for PopulateLevel
  void CreateBasicTile(int tileID, int position, int mapWidth, int halfTileWidth);
  void FactoryManager::CreateGameObject(DynamicElement *object);

};
