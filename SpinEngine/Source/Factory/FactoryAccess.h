/****************************************************************************/
/*!
\author Henry Morgan
\par    email: henry.morgan@digipen.edu
\par    Course: GAM 200
\brief

An extension of the factory manager class- provides global access to
certain factory functions.

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#pragma once
#include "FactoryManager.h"


//A class for accessing the factory without being able to accidentally touch
//potentially dangerous functionality like Shutdown()
class ParticleEmitter;

class FactoryAccess
{
  friend class FactoryManager;
public:
  ZilchDeclareBaseType(FactoryAccess, Zilch::TypeCopyMode::ReferenceType);
  FactoryAccess(FactoryManager &factorymanager);

  //We only allow level & object creation
  
  void LoadLevel(std::string levelName);
  
  void UnloadLevel();

  void Initialize();
  
  void AddGameObject(GameObject ent);
  
  void RemoveGameObject(EntityId id);

  int AddTile(std::string spritename, int position, int tilemapWidth);

  void RemoveTileObject(EntityId id);
  
  IEntity* ZCreateGameObject(Zilch::String name, Zilch::String spritename, Vector3D pos, Vector4D color, bool isUI);
  IEntity *CreateGameObject(std::string name, std::string spritename, Vector3D pos, D3DCOLOR color = d3dColors::White, bool is_UI = false);
  
  GameObject CheckTargets(Vector3D);
  void RemoveTarget(GameObject);

  Sprite* ZCreateSprite(Zilch::String spritename);
  Sprite *CreateSprite(std::string spritename);

  ParticleEmitter &GetParticleEmitter(std::string particlename);
  
  IDirect3DDevice9 *GetDevice();

  GraphicsManager *GetGraphicsManager();

  int GetGameObjectCount();
  TileMapData &GetTileMapData();

  GameObjectUmap &GetGameObjectList();

  IComponent *GetWall();
  FactoryManager &factory_;
private:
  
  IDirect3DDevice9 *directXDevice_;
  

};

//Global pointer, access is given wherever FactoryAccess.h is included.
extern FactoryAccess *GlobalFactory;
