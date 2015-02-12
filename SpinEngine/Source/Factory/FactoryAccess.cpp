/****************************************************************************/
/*!
\author Henry Morgan
\par    email: henry.morgan@digipen.edu
\par    Course: GAM 200
\brief

Factory accessor class.

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#include "Precompiled.h"
#include "FactoryAccess.h"
#include "Sprite.h" //To create sprites
#include "SpriteRenderer.h" //To create sprite renderers
#include "GraphicsManager.h" //To add sprite renderers to graphics manager
#include "Wall.h"
#include "ZInterface.h"

ZilchDefineType(FactoryAccess, SpinningZilch)
{
	type->HandleManager = ZilchManagerId(Zilch::PointerManager);
	ZilchBindMethodAs(ZCreateSprite, "CreateSprite");
	ZilchBindMethodAs(ZCreateGameObject, "CreateGameObject");
}

//We actually set this in Engine.cpp (during engine init), but we need to initialize here.
//NO WE DON'T - Josh
FactoryAccess *GlobalFactory = nullptr;

FactoryAccess::FactoryAccess(FactoryManager &factorymanager) : factory_(factorymanager)
{
  //I fixed it
	GlobalFactory = this;
	ZInterface::Factory = this;
  directXDevice_ = factorymanager.directXDevice_;
}

void FactoryAccess::LoadLevel(std::string levelName)
{
  factory_.LoadLevel(levelName);
}
void FactoryAccess::UnloadLevel()
{
  factory_.UnloadLevel();
}
void FactoryAccess::Initialize()
{
  factory_.Initialize();
}
void FactoryAccess::AddGameObject(GameObject ent)
{
  factory_.AddGameObject(ent);
}
void FactoryAccess::RemoveGameObject(EntityId id)
{
  factory_.RemoveGameObject(id);
}

int FactoryAccess::AddTile(std::string spritename, int position, int tilemapWidth)
{
  factory_.CreateBasicTile(factory_.defaults.GetIDFromSpriteName(spritename.c_str()), position, tilemapWidth, HALF_TILE_WIDTH);
  return factory_.m_TotalObjectCount - 1; //we subtract 1 because the count is incremented after the tile is created
}

void FactoryAccess::RemoveTileObject(EntityId id)
{
  factory_.RemoveTileObject(id);
}

IDirect3DDevice9 *FactoryAccess::GetDevice()
{
  return directXDevice_;
}

int FactoryAccess::GetGameObjectCount()
{
	return factory_.GetGameObjectCount(); 
}

GraphicsManager *FactoryAccess::GetGraphicsManager()
{
  return factory_.m_pGraphicsManager;
}

IEntity* FactoryAccess::ZCreateGameObject(Zilch::String name = "GameObject", Zilch::String spritename = "DefaultTile.png",
	Vector3D pos = Vector3D(), Vector4D color = Vector4D(255, 255, 255, 255), bool isUI = false)
{
		//COLOR DOES NOT WORK!
		//color.x, color.y, color.z, color.w);
	
	return CreateGameObject(name.c_str(), spritename.c_str(), pos, d3dColors::White, isUI);
}

IEntity *FactoryAccess::CreateGameObject(std::string name, std::string spritename, Vector3D pos, D3DCOLOR color, bool is_UI)
{
  IEntity *mem_obj = reinterpret_cast<GameObject>( MemoryManager::Allocate_GameObj() );
  
  IEntity *obj = new (mem_obj)IEntity(factory_.GetGameObjectCount(), pos, 
    Vector3D(0, 0, 0), Vector3D(1, 1, 0), "default", name.c_str());

  SpriteRenderer *spRend = new SpriteRenderer(obj, directXDevice_, color);
  //spRend->AddSprite(CreateSprite(spritename));
  spRend->AddSprite(CreateSprite(spritename));
  if (is_UI)
  {
    factory_.m_pGraphicsManager->AddUISpriteRend(spRend);
    obj->GetTransform()->GetPosition().z = -9.f; //Everything below 0 is "in front" of everything above 0
  }
  else
    factory_.m_pGraphicsManager->AddSpriteRend(spRend); 
  
  obj->AddGameComponent(CT_SpriteRenderer, spRend);
  
  factory_.AddGameObject(obj);
  return obj;
}

//Needs to be passed a valid sprite name (I.E. one of the ones we load in)
//Checks in object data, creates a sprite without needing to create a new texture
//(I'm working on art pipeline, ask me about this)
Sprite* FactoryAccess::ZCreateSprite(Zilch::String spritename)
{
	return CreateSprite(spritename.c_str());
}
Sprite *FactoryAccess::CreateSprite(std::string spritename)
{
  return factory_.defaults.CreateSprite(factory_.defaults.GetIDFromSpriteName(spritename));
}


ParticleEmitter &FactoryAccess::GetParticleEmitter(std::string particlename)
{
  return factory_.defaults.GetParticleEmitter(particlename);
}

//I.E. TileMapData &data = GetTileMapData();
TileMapData &FactoryAccess::GetTileMapData()
{
  return factory_.tileMap_;
}


GameObjectUmap &FactoryAccess::GetGameObjectList()
{
  return factory_.m_GameObjectList;
}

IComponent *FactoryAccess::GetWall()
{
  return factory_.GetWall();
}

GameObject FactoryAccess::CheckTargets(Vector3D TargetPoint)
{
  for (unsigned i = 0; i < factory_.m_Targets.size(); ++i)
  {
    if (factory_.m_Targets[i] == NULL)
    {
      continue;
    }

    float X1 = factory_.m_Targets[i]->GetTransform()->GetPosition().x;
    float Y1 = factory_.m_Targets[i]->GetTransform()->GetPosition().y;

    float X2 = TargetPoint.x;
    float Y2 = TargetPoint.y;

    float XDistance = X2 - X1;

    float YDistance = Y2 - Y1;

    float Total = sqrt((XDistance * XDistance) + (YDistance * YDistance));

    if (Total < 1.0 && factory_.m_Targets[i]->IsCaptured == false)
    {
      factory_.m_Targets[i]->IsCaptured = true;
      return factory_.m_Targets[i];
    }
  }

  return NULL;
}

void FactoryAccess::RemoveTarget(GameObject Target)
{
  for (unsigned i = 0; i < factory_.m_Targets.size(); ++i)
  {
    if (factory_.m_Targets[i] == Target)
    {
      factory_.m_Targets[i] = NULL;
      return;
    }
  }
}