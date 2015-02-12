/****************************************************************************/
/*!
\author Henry Morgan, Esteban Maldonado.
\par    email: henry.morgan@digipen.edu
\par    Course: GAM 200
\brief

Main factory class. Detections and constructions of individual components in
CreateObject are written by the authors of those components.

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/

#include "Precompiled.h"
#include "FactoryManager.h"
//For creating components:
#include "Core\SpriteRenderer.h"
#include "Graphics\Sprite.h" //to create sprites to pass to sprite renderer
#include "Factory\ObjectData.h"
#include "Graphics\GraphicsManager.h"
#include "Input/ControllerComponent.h"
#include "PlayerController.h"
#include "WallTarget.h"
#include "SoundEmitter.h"
#include "MovingPlatforms.h"
#include "SwitchComponent.h"
#include "PlayerTrigger.h"
#include "DoorComponent.h"
#include "ParallaxBG.h"
#include "WallTriggerComp.h"
#include "ExtendBlock.h"
#include "CollisionActivate.h"
#include "MenuController.h"
#include "LevelChangeTrigger.h"
#include "ProximityActivate.h"
#include "DigipenLogoFade.h"
#include "TeamLogoFade .h"
#include "CreditsDisplay.h"
#include "Checkpoint.h"
#include "KillPlayer.h"
#include "DestroyOnWallActivate.h"
#include "DoorType2Comp.h"
#include "ActionTest.h"
#include "ChaseComponent.h"
#include "DestroyOnWall.h"
#include "HowToPlayComponent.h"
#include "SpawnedBlock.h"
#include "DataNode.h"
#include "JSONSerializer.h"
#include "TileMapData.h"
#include "WallController.h"
#include "FactoryAccess.h"


ZilchDefineType(FactoryManager, SpinningZilch)
{
	type->HandleManager = ZilchManagerId(Zilch::PointerManager);

}

FactoryManager::FactoryManager(GraphicsManager* m_pGraphicsManager) :ISystem("Factory", SystemType::ST_Factory),
                                 m_GameObjectList( GameObjectUmap()), m_TotalObjectCount(0),
                                 m_CurrentObjectCount(0), m_DeletedObjectCount(0), m_pGraphicsManager(m_pGraphicsManager), PlayerSpawn(NULL), m_Targets(GameObjectUmap())
{
  directXDevice_ = *(m_pGraphicsManager->GetDevice());
  GlobalFactory = new FactoryAccess(*this);
  defaults.AddIDirect3DDevice9(directXDevice_);
  defaults.Initialize();

 /* size_t wordsize = sizeof(WORD);
  size_t shortsize = sizeof(short);
  size_t bytesize = sizeof(BYTE);
  size_t bleh = wordsize;*/

  //REGISTERING ALL OF THE COMPONENTS SO THAT WE DON'T HAVE TO FIND THEM BY NAME
  RegisterComponent(ActionTest);
  RegisterComponent(Transform);
  RegisterComponent(ChaseComponent);
  //RegisterComponent(ParticleEmitter);
  RegisterComponent(CollisionActivate);
  //RegisterComponent(DestroyOnWall);
  RegisterComponent(DestroyOnActivate);
  RegisterComponent(DoorComponent);
  //RegisterComponent(DoorType2Comp);
  //RegisterComponent(DynamicHand);
  RegisterComponent(ExtendBlock);
  RegisterComponent(HowToPlayComponent);
  RegisterComponent(KillPlayer);
  //RegisterComponent(LaserBeam);
  //RegisterComponent(LevelChangeTrigger);
  RegisterComponent(MovingPlatform);
  //RegisterComponent(PlayerController);
  RegisterComponent(PlayerTrigger);
  RegisterComponent(ProximityTrigger);
  //RegisterComponent(ScrollComponent);
  //RegisterComponent(SpawnedBlock);
  RegisterComponent(SwitchComponent);
  RegisterComponent(TileMapCollision);
  RegisterComponent(WallComp);
  //RegisterComponent(WallHand);
  RegisterComponent(WallTarget);
  RegisterComponent(WallTriggerComp);
  RegisterComponent(Circle);
  RegisterComponent(AABB);
  //RegisterComponent(SpriteRend);
}

FactoryManager::~FactoryManager()
{
	//delete FactAccess;

	std::unordered_map<std::string, ComponentCreator *>::iterator iterator;
	for (iterator = SerialMap.begin(); iterator != SerialMap.end(); ++iterator)
	{
		delete iterator->second;
	}
	SerialMap.clear();
	//Delete objects in serial map?
}

void FactoryManager::AddComponentCreator(std::string name, ComponentCreator* creator)
{
	SerialMap[name] = creator;
}

bool FactoryManager::Initialize()
{
  
  for(unsigned i = 0; i < this->m_GameObjectList.size(); i++)
  {
    if (!m_GameObjectList[i]->Initialize())
    {
      return false;
    }
  }

  

  return true;
}

void FactoryManager::Update(float dt)
{
  for(unsigned i = 0; i < this->m_GameObjectList.size(); i++)
  {
    if (m_GameObjectList[i])
      m_GameObjectList[i]->Update(dt);
  }
}

void FactoryManager::Shutdown()
{
  UnloadLevel();
}

std::string FactoryManager::GetName() 
{ 
  return system_name; 
}

GameObjectUmap* FactoryManager::GetObjectList()
{ 
  return &m_GameObjectList;
}

unsigned int FactoryManager::GetGameObjectCount()
{ 
  return m_GameObjectList.size(); 
}

void FactoryManager::AddEmptyGameObject()
{ 
  GameObject mem_ = MemoryManager::Allocate_GameObj();
  GameObject obj = new (mem_) IEntity(m_TotalObjectCount++);
  m_GameObjectList.push_back(obj);
  m_CurrentObjectCount++;
}

//Shouldn't we be accessing these by CurrentObjectCount? Yes, so I changed it
void FactoryManager::AddGameObject(GameObject ent)
{ 
  m_GameObjectList.push_back(ent);
  m_CurrentObjectCount++;
  m_TotalObjectCount++;

  //Not needed
  //GameObjectUmap bleh = m_GameObjectList;
  //int q = bleh.size();
  //q++;
}

//We really, REALLY assume that this won't be called on tiles during gameplay
//(we rely on the tiles being the first however-many elements in the array,
//in order, and removing one / moving the rest of the elements over would
//mess the order up.
//Maybe make a RemoveGameObject function.
void FactoryManager::RemoveGameObject(EntityId id)
{
  //SpriteRenderer *TestRend = reinterpret_cast<SpriteRenderer *>(m_GameObjectList[id]->GetComponent(CT_SpriteRenderer));
  //m_pGraphicsManager->RemoveSpriteRend(TestRend);

  m_GameObjectList[id]->~IEntity();
  MemoryManager::Free_GameObject(m_GameObjectList[id]);
  m_GameObjectList[id] = NULL;
  //SPIN_ENGINE_SAFE_DELETE(m_GameObjectList[id]);

    /*
  for (GameObjectUmap::iterator iter = m_GameObjectList.begin();
    iter != m_GameObjectList.end(); iter++)
  {
    if ( (*iter)->GetEntityId() == id)
    {
      SPIN_ENGINE_SAFE_DELETE(m_GameObjectList[id]);
      //m_GameObjectList.erase(iter);
      break;
    }
  }
  */
  //m_GameObjectList.erase(m_GameObjectList.begin() + id);
  m_CurrentObjectCount--;
  m_DeletedObjectCount++;
}

void FactoryManager::RemoveTileObject(EntityId id)
{
  //for (GameObjectUmap::iterator iter = m_GameObjectList.begin();
  //  iter != m_GameObjectList.end(); iter++)
  //{
  //  if ((*iter)->GetEntityId() == id)
  //  {
  //    SPIN_ENGINE_SAFE_DELETE(*iter);
  //    m_GameObjectList.erase(iter);
  //  }
  //}

  ////SPIN_ENGINE_SAFE_DELETE(m_GameObjectList[id]);

  ////we don't clear that spot in the array
  //m_CurrentObjectCount--;
  //m_DeletedObjectCount++;

  //Does the same logic
  RemoveGameObject(id);
}

void FactoryManager::LoadLevel(std::string levelName)
{
  if (levels.LevelIsLoaded())
    UnloadLevel();

  DynamicElement *data = levels.LoadLevelData(levelName);

  if (data == NULL)
  {
    MessageBox(NULL, "Factory given incorrect level name.", NULL, NULL);
    return;
  }
  if (!PopulateLevel(*data))
  {
    MessageBox(NULL, "Failure in FactoryManager creating game objects for level.", NULL, NULL);
    return;
  }

  //Not needed
  //gameobjectumap bleh = m_gameobjectlist;
  //int i = bleh.size();
  //i++;
}

//If you don't unload, LoadLevel will reload the same level.
void FactoryManager::UnloadLevel()
{
  if (!levels.LevelIsLoaded())
    return;

  levels.UnloadLevelData();

  tileMap_.width = -1;
  tileMap_.height = -1;
  tileMap_.numTiles = -1;
  tileMap_.tiles = NULL;

  for (unsigned i = 0; i < m_GameObjectList.size(); i++)
    {
      if (m_GameObjectList[i])
      {
        m_GameObjectList[i]->~IEntity();
        MemoryManager::Free_GameObject(m_GameObjectList[i]);
        m_GameObjectList[i] = NULL;
        //SPIN_ENGINE_SAFE_DELETE(m_GameObjectList[i]);
        //m_GameObjectList.erase(iter);
        --m_CurrentObjectCount;
        ++m_DeletedObjectCount;
      }
    }

  //}//GameObjectList is now empty
  //m_GameObjectList.clear();
  if (m_DeletedObjectCount != m_TotalObjectCount)
      MessageBox(NULL, "There are still live game objects after level unload. Possible memory leak.", NULL, NULL);
  else
      this->ClearEverythingInFactory();
}

bool FactoryManager::PopulateLevel(DynamicElement &root)
{
  /****************************************
  *********WIDTH/HEIGHT/PIXEL SIZE*********
  ****************************************/
  //SERIALIZING TILEMAPDATA STRUCT
  int height;
  //Get "height" and "width" objects, then get the int values from them.
  DynamicElement *heightObject;
  if (!root.GetObjectMember(&heightObject, "height"))
  {
    MessageBox(NULL, "LoadLevelData failed: No 'height' object.", NULL, NULL);
    return false;
  }
  if (!heightObject->GetIntVal(&height))
  {
    MessageBox(NULL, "LoadLevelData failed: 'height' object did not contain an int.", NULL, NULL);
    return false;
  }
  tileMap_.height = height;

  int width;
  DynamicElement *widthObject;
  if (!root.GetObjectMember(&widthObject, "width"))
  {
    MessageBox(NULL, "LoadLevelData failed: No 'width' object.", NULL, NULL);
    return false;
  }
  if (!widthObject->GetIntVal(&width))
  {
    MessageBox(NULL, "LoadLevelData failed: 'width' object did not contain an int.", NULL, NULL);
    return false;
  }
  tileMap_.width = width;
  tileMap_.numTiles = width * height;
  
  int tileWidth;
  DynamicElement *tileWidthObject;
  if (!root.GetObjectMember(&tileWidthObject, "tilewidth"))
  {
    return false;
  }
  if (!tileWidthObject->GetIntVal(&tileWidth))
  {
    return false;
  }
  int halfTileWidth = tileWidth / 2;

  /****************************************
  ***************TILE LAYERS***************
  ****************************************/

  //Get the "Layers" Object, then the tile layer(s) and object layer(s)
  DynamicElement *layers;
  if (!root.GetObjectMember(&layers, "layers"))
  {
    MessageBox(NULL, "LoadLevelData failed: No 'layers' object.", NULL, NULL);
    return false;
  }

  //Go through each layer in a loop.
  //Keep the target layer and object layers separate from the rest.
  std::vector<DynamicElement *> nonTargetLayers;
  DynamicElement *currLayer, *TargetLayer, *ObjectLayer;
  while (layers->GetObjectMember(&currLayer, NULL))
  {
    DynamicElement *n;
    if(!currLayer->GetObjectMember(&n, "name"))
      MessageBox(NULL, "A layer didn't have a name? What?", NULL, NULL);
    std::string name;
    n->GetStringVal(&name);
    
    if (name == "TargetLayer")
    {
      TargetLayer = currLayer;
    }
    else if (name == "Objects")
    {
      ObjectLayer = currLayer;
    }
    else
    {
      nonTargetLayers.push_back(currLayer);
    }
  }

  //We go through the target layer and give it its own vector.
  //Then we copy it and merge the other layers into the copy, and use the copy as the overall tilemap vector.
  //The original will remain the "targetTiles" vector.
  if (!TargetLayer->GetObjectMember(&TargetLayer, "data"))
  {
    MessageBox(NULL, "LoadLevelData failed: TargetLayer did not contain a 'data' vector.", NULL, NULL);
    return false;
  }
  std::vector<int> *TileMapVec;
  if (!TargetLayer->GetVector(&TileMapVec))
  {
    MessageBox(NULL, "The target layer's 'data' object did not contain a vector or wasn't TYPE_VECTOR.", NULL, NULL);
    return false;
  }
  
  //Copy the vector of targetable tiles for reference later.
  tileMap_.targetTiles = *TileMapVec;

  //Add each non-target layer's tiles to the original vector (so that we get one vector with all tiles)
  for (unsigned i = 0; i < nonTargetLayers.size(); ++i)
  {
    nonTargetLayers[i]->GetObjectMember(&currLayer, "data");
    currLayer->GetVector(&TileMapVec, true);
  }
  tileMap_.tiles = TileMapVec;

  //And create a copy of the full vector to hold the original state of the map.
  tileMap_.origTiles = *TileMapVec;
  tileMap_.numTiles = tileMap_.origTiles.size();
  tileMap_.factoryIndeces.resize(tileMap_.origTiles.size());


  //Create all tile objects.
  for (unsigned i = 0; i < TileMapVec->size(); ++i)
  {
    //Skip empty tile spaces (represented by 0)
    if ((*TileMapVec)[i] == 0)
    {
      tileMap_.factoryIndeces[i] = -1;
      continue;
    }

    //Add game objects according to the "basic tile" formula
    CreateBasicTile((*TileMapVec)[i], i, width, halfTileWidth);
  }

  /****************************************
  **************OBJECT LAYER***************
  ****************************************/
  //The object layers are a series of unnamed objects. We will get them until there are no more.
  if(!ObjectLayer->GetObjectMember(&ObjectLayer, "objects"))
    MessageBox(NULL, "Failed to get custom objects.", NULL, NULL);

  //Clear out the temp player transform and camera references
  currentCameraRef = nullptr;
  tempPlayerTransformRefs.clear();
  tempPlayerTransformRefs.resize(0); //cludgy, but I'm on a plane and that makes it okay for some reason

  DynamicElement *currObject;
  //Going through every custom object
  while (ObjectLayer->GetObjectMember(&currObject, NULL))
  {
    CreateGameObject(currObject);
  }

  //Provide player transform references to the current camera.
  if (currentCameraRef != nullptr)
    for (auto it = tempPlayerTransformRefs.begin(); it != tempPlayerTransformRefs.end(); ++it)
    {
      currentCameraRef->AddPlayerPosition(*it);
    }

  //Everything is loaded and in the Factory's array.
  return true;
}

//Creates a basic, immobile tile sprite. (Needs to have BoxCollider component added.)
//mapWidth is in tiles
void FactoryManager::CreateBasicTile(int tileID, int position, int mapWidth, int halfTileWidth)
{
  //@Consider getting rid of halfTileWidth- you don't (or shouldn't) use it since almost all
  //tiles will be 512x512 and scaled down.

  //Get the stats for our new IEntity
  //pos.x = (position % width) * 64
  //pos.y = (position / width) * 64

  Vector3D pos = Vector3D(static_cast<float>((position % mapWidth) * 2 /* * 64 + 32*/),
    ((tileMap_.height - 1) * 2.0f) - (static_cast<float>((position / mapWidth) * 2) /* * 64 + 32*/), 0.0f);

  //getridofmelater
  Vector3D rot = Vector3D(0.0f, 0.0f, 0.0f);
  Vector3D scale = Vector3D(1.0f, 1.0f, 1.0f); // 1/8th of 512x512 -> 64x64
  //Create the new game object

  GameObject mem_newTile = MemoryManager::Allocate_GameObj();
  GameObject newTile = new (mem_newTile) IEntity(m_TotalObjectCount, pos, rot, scale, "DefaultTile");

  //Create the sprite renderer
  SpriteRenderer *renderer = new SpriteRenderer(newTile, *(m_pGraphicsManager->GetDevice()) ,d3dColors::White,"idle");
  //renderer->ToggleAsTile();

  //Graphics manager needs it to render. Factory updates sprite renderers, but graphics manager actually renders
  m_pGraphicsManager->AddSpriteRend(renderer);
  renderer->AddSprite(defaults.CreateSprite(tileID));
  renderer->GetCurrentSprite()->Set_Paused(true);
  newTile->AddGameComponent(CT_SpriteRenderer, renderer);

  //If we're using the Desert_Tiles tileset, toggle us to not use scaling/rotation/translation (and to flag us as a tile)
  //This is to recognize later on that we should change frames in the Desert_Tiles tileset to look good/appropriate for our position.
  //This is hardcoded and kinda bad, sorry
  if (tileID == 1 || tileID == 2)
  {
    //We disable this in SpriteRenderers' inits if they are rotated at all during the frame changes
    renderer->ToggleAsTile();
    //This also means that we no longer toggle non-Desert tiles, but there shouldn't BE any non-Desert tiles after this.
    //Except for debris tiles, but I'll just add those as the second case.
  }

  //Add box collider
  //@Later, only add these to edges, or have a tile-specific layer. Having 700 box colliders brings us from 210fps to 150fps.
  //if (defaults.)
  /*AABB *coll = new AABB(newTile);
  coll->SetHalfSize(1, 1);
  RigidBody *body = new RigidBody(newTile, reinterpret_cast<Primitive*>(coll));
  body->SetStatic();
  newTile->AddGameComponent(CT_Body, body);*/
  tileMap_.factoryIndeces[position] = m_TotalObjectCount;
  AddGameObject(newTile);
}

//Looks for every component. For each found, adds that components and their properties.
void FactoryManager::CreateGameObject(DynamicElement *object)
{
  //TRANSFORM: (mandatory)
  DynamicElement *properties; //Used to access each of object's properties
  
  //POSITION
  //Is in Pixels for 512x512 tiles.
  if (!object->GetObjectMember(&properties, "x"))
    return;
  float posX;
  if (!properties->GetFloatVal(&posX)) //Sometimes it will be an integer value
  {
    int bleh;
    properties->GetIntVal(&bleh);
    posX = static_cast<float>(bleh);
  }
  posX = (posX / 512.0f) * 2.0f;

  if (!object->GetObjectMember(&properties, "y"))
    return;
  float posY;
  if (!properties->GetFloatVal(&posY))
  {
    int bleh;
    properties->GetIntVal(&bleh);
    posY = static_cast<float>(bleh);
  }
  posY = (tileMap_.height - (posY / 512.0f)) * 2.0f;
  Vector3D pos(posX, posY, 0.0f);

  //ROTATION - clockwise from the positive x-axis, in degrees
  if (!object->GetObjectMember(&properties, "rotation"))
    return;
  float rot;
  if (!properties->GetFloatVal(&rot))
  {
    int bleh;
    properties->GetIntVal(&bleh);
    rot = static_cast<float>(bleh);
  }

  if (rot < 0.0f)
    rot = 360.0f + rot;

  rot = rot * 180.0f / 3.141593f; //@is now in counterclockwise radians.
  Vector3D rotation(rot,rot,rot); //@@ Is this what to do?
  Vector3D scale(1.0f, 1.0f, 1.0f);

  //Create the new game object

  GameObject mem_newObject = MemoryManager::Allocate_GameObj();
  GameObject newObject = new (mem_newObject) IEntity(m_TotalObjectCount, pos, rotation, scale, "DefaultTile");
  
  
  //COMPONENTS: Here, we check for each component type. If it's detected, we get its variables.
  //If a variable isn't found, currently we complain, but in the future we'll use default
  //values, which we'll obtain from text files. (I.E. "coin.json", "enemy.json" etc...)
  if(!object->GetObjectMember(&properties, "properties"))
    MessageBox(NULL, "Failed to get object properties.", NULL, NULL);
  //We assume everything has a sprite renderer. SEEMS like a safe assumption for now.
  //@If it's an issue, make a "nosprite" tag in the editor and check for that.
  DynamicElement *currProperty;

  if (!object->GetObjectMember(&currProperty, "name"))
	MessageBox(NULL, "Failed to get object Name.", NULL, NULL);

  newObject->SetName(currProperty->data.val_stringPtr->c_str());

  //newObject->SetName(object)
  if (!properties->GetObjectMember(&currProperty, "nosprite"))
  {
    //Create the sprite renderer
    SpriteRenderer *renderer = new SpriteRenderer(newObject, *(m_pGraphicsManager->GetDevice()), d3dColors::White, "idle");
    m_pGraphicsManager->AddSpriteRend(renderer);

    //Set visibility
    //Get whether or not the object should be visible (meaning, have a sprite renderer)
    bool objIsVisible;
    if (!object->GetObjectMember(&currProperty, "visible"))
      MessageBox(NULL, "Failed to get object visibility.", NULL, NULL);
    currProperty->GetBoolVal(&objIsVisible);
    if (!objIsVisible)
      newObject->SetInVisible();

    //@@ADD SPRITE HERE
    if (!object->GetObjectMember(&currProperty, "gid"))
      MessageBox(NULL, "Failed to get object ID.", NULL, NULL);
    int ID;
    currProperty->GetIntVal(&ID);
    renderer->AddSprite(defaults.CreateSprite(ID));

    //Check for additional sprites
    if (properties->GetObjectMember(&currProperty, "AdditionalSprites"))
    {
      std::string toadd;
      currProperty->GetStringVal(&toadd);
      std::istringstream stream(toadd);
      while (true)
      {
        int extraID;
        stream >> extraID;
        if (!stream)
          break;
        renderer->AddSprite(defaults.CreateSprite(extraID));
      }
    }

    newObject->AddGameComponent(CT_SpriteRenderer, renderer);
  }

  if(object->GetObjectMember(&properties,"properties"))
  {
    DynamicElement *custom;
    if (properties->GetObjectMember(&custom, "RigidBody"))
    {
      AABB *coll = new AABB(newObject);
      coll->SetHalfSize(1, 1);
      RigidBody *body = new RigidBody(newObject, reinterpret_cast<Primitive*>(coll));
      body->SetStatic();
      newObject->AddGameComponent(CT_Body, body);
    }

    if (properties->GetObjectMember(&custom, "Scale"))
    {
      std::string scalestr;
      custom->GetStringVal(&scalestr);
      float sc = std::stof(scalestr);
      newObject->GetTransform()->GetWorldScale() *= sc;
    }
    
    //Player (Includes input, tilemap collision, box collider, rigidbody)
    if (properties->GetObjectMember(&custom, "PlayerController"))
    {
      //Box collider and rigidbody
      AABB *box = new AABB(newObject);
      box->SetHalfSize(0.2f, 0.9f);
      newObject->AddGameComponent(CT_BoxCollider, box);
      RigidBody *body = new RigidBody(newObject, reinterpret_cast<Primitive*>(box));
      body->set(5.0f);
      //Make the body not collide with other bodies
      body->isGhost = true;
      //body->useGravity = false;
      body->restitution = 0;
      newObject->AddGameComponent(CT_Body, body);
      //Tilemap collider
      TileMapCollision *tcoll = new TileMapCollision(newObject, tileMap_);
      newObject->AddGameComponent(CT_TileMapCollider, tcoll);
      //Sound Emitter
      
      SoundEmitter *sEmitter = new SoundEmitter(newObject);

      newObject->AddGameComponent(CT_SoundEmitter, reinterpret_cast<Component>(sEmitter));
      
      //Player Controller (includes keyboard & controller listeners)
      std::string contr;
      custom->GetStringVal(&contr);
      int controllerNumber = atoi(contr.c_str());
      PlayerController *cont = new PlayerController(newObject, controllerNumber);
      newObject->AddGameComponent(CT_Player_Controller, reinterpret_cast<Component>(cont));

      //Wall target (reference to the wall will be added during initialization)
      WallTarget *TestTarget = new WallTarget(newObject);
      newObject->AddGameComponent(CT_WALL_TARGET, TestTarget);
      newObject->SetName("MainPlayer");

      //Save pointer to player transform, so we can give it to the camera after construction
      tempPlayerTransformRefs.push_back(newObject->GetTransform());

      m_Targets.push_back(newObject);
    }

    //Camera
    if (properties->GetObjectMember(&custom, "MainCamera"))
    {
      Camera* cam = new Camera(newObject);
      if (ENGINE->m_Graphics->m_bFullScreen)
      {
        cam->Set_Width_Height(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
      }
      else
      {
        cam->Set_Width_Height(ENGINE->m_Graphics->GetWindow()->GetWindowWidth(),
          ENGINE->m_Graphics->GetWindow()->GetWindowHeight());
      }

      //cam->SetBackColor(d3dColors::DarkSlateGray);
      cam->SetBackColor(d3dColors::LightBlue);
      newObject->AddGameComponent(CT_CameraComponent, cam);
      //m_pGraphicsManager->SetMainCamera(reinterpret_cast<Camera*>(newObject->GetComponent(CT_CameraComponent)));
      m_pGraphicsManager->SetMainCamera(cam);
      currentCameraRef = cam;

      //If we're supposed to be following the wall (at least initially), follow the wall
      if (properties->GetObjectMember(&custom, "CameraIsOnWall"))
      {
        cam->SetToFollowWall();
      }

    }

	//ActionTest
	if (properties->GetObjectMember(&custom, "ActionTest"))
	{
		ActionTest* cam = new ActionTest(newObject);
		newObject->AddGameComponent(CT_ACTIONTEST, cam);
	}

    //Wall
    if (properties->GetObjectMember(&custom, "Wall"))
    {
      WallComp *wall = new WallComp(newObject);
      newObject->AddGameComponent(CT_WALL, wall);
      SetWall(reinterpret_cast<IComponent*>(wall));
      
      WallController * walltest = new WallController(newObject, 0);
      newObject->AddGameComponent(CT_WALL_CONTROLLER, walltest);
      //walltest->Initialize();

      //Add parallaxing background
      if (gGameStateCurr == GS_LEVEL)
      {
        ParallaxBG *par = new ParallaxBG(newObject);
        newObject->AddGameComponent(CT_ParallaxBG, par);
      }
    }

    //Platform
    if (properties->GetObjectMember(&custom, "PlatformController"))
    {
      AABB *box = new AABB(newObject);
      box->SetHalfSize(1, 1);
      newObject->AddGameComponent(CT_BoxCollider, box);
      
      RigidBody *body = new RigidBody(newObject, reinterpret_cast<Primitive*>(box));
      newObject->AddGameComponent(CT_Body, body);
      body->useGravity = false;

      MovingPlatform *plat = new MovingPlatform(newObject, PLATFORM_RIGHT, 6.0f);
      
      if(properties->GetObjectMember(&custom, "PaceVertical"))
      {
        plat->SetPlatformType(PLATFORM_PACE_VERTICAL); 
      }

      if(properties->GetObjectMember(&custom, "PaceHorizontal"))
      {
        plat->SetPlatformType(PLATFORM_PACE_HORIZONTAL); 
      }

      if(properties->GetObjectMember(&custom, "StartAtTop"))
      {
        plat->StartAtTop = true;
      }


      newObject->AddGameComponent(CT_MOVPLATFORM, plat);

      SwitchComponent *SwitchComp = new SwitchComponent(newObject, false);
      newObject->AddGameComponent(CT_SWITCH, SwitchComp);
      
      PlayerTrigger * TriggerComp = new PlayerTrigger(newObject);
      newObject->AddGameComponent(CT_PLAYERTRIGGER, TriggerComp);

      TileMapCollision *tcoll = new TileMapCollision(newObject, tileMap_);
      newObject->AddGameComponent(CT_TileMapCollider, tcoll);

      //For now, just automatically moves up, later on change direction according to properties
      //TranslateUp == vertical movement, positive is up, negative is down
      if (properties->GetObjectMember(&custom, "MoveRange"))
      {
        std::string dist;
        custom->GetStringVal(&dist);
        int distance = atoi(dist.c_str());
        plat->SetPaceDistance(2 * distance);
        //@Steven, apply distance here.
      }
      //else if (properties->GetObjectMember(&custom, "TranslatesLeft"))
    }

    //Door
    if (properties->GetObjectMember(&custom, "Door"))
    {
      m_Targets.push_back(newObject);

        // scale the door to the correct size;
        newObject->GetTransform()->GetWorldScale().x = 2;
        newObject->GetTransform()->GetWorldScale().y = 5;
        newObject->GetTransform()->GetWorldPosition().y -= 4;

        AABB *box = new AABB(newObject);
        box->SetHalfSize(2, 5);
        newObject->AddGameComponent(CT_BoxCollider, box);

        RigidBody *body = new RigidBody(newObject, reinterpret_cast<Primitive*>(box));
        body->SetStatic();
        newObject->AddGameComponent(CT_Body, body);

        DoorComponent *DoorComp = new DoorComponent(newObject);
        newObject->AddGameComponent(CT_DOOR, DoorComp);
       
        SwitchComponent *SwitchComp = new SwitchComponent(newObject, false);
        newObject->AddGameComponent(CT_SWITCH, SwitchComp);

        if (properties->GetObjectMember(&custom, "Locked"))
        {
            std::string boolcheck;
            custom->GetStringVal(&boolcheck);

            if (boolcheck == "True")
            {
                DoorComp->Locked = true;
            }
        }


        // add the collision trigger;
    }

    if (properties->GetObjectMember(&custom, "HeavyDoor"))
    {
        // scale the door to the correct size;
        newObject->GetTransform()->GetWorldScale().x = 2;
        newObject->GetTransform()->GetWorldScale().y = 5;
        newObject->GetTransform()->GetWorldPosition().y -= 4;

        AABB *box = new AABB(newObject);
        box->SetHalfSize(2, 5);
        newObject->AddGameComponent(CT_BoxCollider, box);

        RigidBody *body = new RigidBody(newObject, reinterpret_cast<Primitive*>(box));
        body->SetStatic();
        newObject->AddGameComponent(CT_Body, body);

        //DoorComponent *DoorComp = new DoorComponent(newObject);
        //newObject->AddGameComponent(CT_DOOR, DoorComp);

        DoorType2Comp * dt2 = new DoorType2Comp(newObject, DOORCLOSE_RIGHT);
        newObject->AddGameComponent(CT_DOOR2, dt2);

        SwitchComponent *SwitchComp = new SwitchComponent(newObject, false);
        newObject->AddGameComponent(CT_SWITCH, SwitchComp);

        body->position.y += 5;
        newObject->GetTransform()->GetWorldPosition().y += 5;
    }


    if (properties->GetObjectMember(&custom, "OnCollideTurnOnWall"))
    {
        AABB *box = new AABB(newObject);
        box->SetHalfSize(1, 1);
        newObject->AddGameComponent(CT_BoxCollider, box);

        RigidBody *body = new RigidBody(newObject, reinterpret_cast<Primitive*>(box));
        body->SetStatic();
        newObject->AddGameComponent(CT_Body, body);

        newObject->SetName("Iamwalltrigger");

        body->isGhost = true;

        WallTriggerComp * WallTrigger = new WallTriggerComp(newObject);
        newObject->AddGameComponent(CT_WALL_TRIGGER, WallTrigger);
        // add the collision trigger;
    }
	
    if (properties->GetObjectMember(&custom, "SpawnUp") || 
        properties->GetObjectMember(&custom, "SpawnDown") || 
        properties->GetObjectMember(&custom, "SpawnLeft") || 
        properties->GetObjectMember(&custom, "SpawnRight"))
    {
        AABB *box = new AABB(newObject);
        box->SetHalfSize(1, 1);
        newObject->AddGameComponent(CT_BoxCollider, box);

        RigidBody *body = new RigidBody(newObject, reinterpret_cast<Primitive*>(box));
        body->SetStatic();
        newObject->AddGameComponent(CT_Body, body);

        SwitchComponent *SwitchComp = new SwitchComponent(newObject, false);
        newObject->AddGameComponent(CT_SWITCH, SwitchComp);

       CollisionActivate * CA = new CollisionActivate(newObject);
       newObject->AddGameComponent(CT_COLLISIONACTIVATE, CA);

       ProximityTrigger* PA = new ProximityTrigger(newObject);
       newObject->AddGameComponent(CT_PROXIMITYACTIVATE, PA);

        ExtendBlock * EB = new ExtendBlock(newObject);
        newObject->AddGameComponent(CT_EXTEND_BLOCK, EB);

        TileMapDetection * TileCheck = new TileMapDetection(newObject, tileMap_,m_GameObjectList);
        newObject->AddGameComponent(CT_TileMapDetector, TileCheck);

        if (properties->GetObjectMember(&custom, "SpawnUp"))
            EB->ActivateDirection(SpawnDIR::SPAWN_UP);

        if (properties->GetObjectMember(&custom, "SpawnDown"))
            EB->ActivateDirection(SpawnDIR::SPAWN_DOWN);

        if(properties->GetObjectMember(&custom, "SpawnLeft"))
            EB->ActivateDirection(SpawnDIR::SPAWN_LEFT);

        if(properties->GetObjectMember(&custom, "SpawnRight"))
            EB->ActivateDirection(SpawnDIR::SPAWN_RIGHT);
        

        // add the collision trigger;
    }
    
    if (properties->GetObjectMember(&custom, "MenuController"))
    {
      MenuController * TestMenu = new MenuController(newObject);
      newObject->AddGameComponent(CT_MENUCONTROLLER, TestMenu);
      reinterpret_cast<SpriteRenderer*>(newObject->GetComponent(CT_SpriteRenderer))->AddSprite(defaults.CreateSprite(16));
      reinterpret_cast<SpriteRenderer*>(newObject->GetComponent(CT_SpriteRenderer))->ChangeState("cursor");
      newObject->GetTransform()->GetWorldScale() *= 1.2f;
    }
                           
    if (properties->GetObjectMember(&custom, "MenuButton"))
    {
        std::string lvlname;
        custom->GetStringVal(&lvlname);
        //int Level = atoi(dist.c_str());

        MenuButtons * Testbutton = new MenuButtons(newObject, lvlname);
      newObject->AddGameComponent(CT_MENUBUTTON, Testbutton);

      newObject->GetTransform()->GetWorldScale() *= 3.0f;
    }

    if (properties->GetObjectMember(&custom, "OnCollideGoToLevel"))
    {
        std::string lvlname;
        custom->GetStringVal(&lvlname);

        AABB *box = new AABB(newObject);
        box->SetHalfSize(1, 1);
        newObject->AddGameComponent(CT_BoxCollider, box);

        RigidBody *body = new RigidBody(newObject, reinterpret_cast<Primitive*>(box));
        body->SetStatic();
        body->isGhost = true;
        newObject->AddGameComponent(CT_Body, body);

        LevelChangeTrigger *LevelChange = new LevelChangeTrigger(newObject, lvlname);
        newObject->AddGameComponent(CT_LEVELTRIGGER, LevelChange);
    }

    if (properties->GetObjectMember(&custom, "Checkpoint"))
    {
      //(decided to have people do this through the editor.
      //Bring back if you want the tile for this to be visible in the editor
      //but invisible in-game.
      //newObject->SetInVisible(); 

      std::string checkpointname;
      custom->GetStringVal(&checkpointname);

      AABB *box = new AABB(newObject);
      box->SetHalfSize(1, 1);
      newObject->AddGameComponent(CT_BoxCollider, box);

      RigidBody *body = new RigidBody(newObject, reinterpret_cast<Primitive*>(box));
      body->SetStatic();
      body->isGhost = true;
      newObject->AddGameComponent(CT_Body, body);

      Checkpoint *check = new Checkpoint(newObject, checkpointname);
      newObject->AddGameComponent(CT_CHECKPOINT, check);
    }

    if (properties->GetObjectMember(&custom, "DigipenLogoFade"))
    {
      DigipenLogoFade *logo = new DigipenLogoFade(newObject);
      newObject->AddGameComponent(CT_DIGIPENLOGOFADE, logo);
    }

    if (properties->GetObjectMember(&custom, "TeamLogoFade"))
    {
      TeamLogoFade *logo = new TeamLogoFade(newObject);
      newObject->AddGameComponent(CT_TEAMLOGOFADE, logo);
    }

    if (properties->GetObjectMember(&custom, "CreditsDisplay"))
    {
      CreditsDisplay *credits = new CreditsDisplay(newObject);
      newObject->AddGameComponent(CT_CREDITSDISPLAY, credits);
    }

    if (properties->GetObjectMember(&custom, "MainMenuBackground"))
    {
      Sprite *mmb = defaults.CreateSprite(defaults.GetIDFromSpriteName("MainMenuBackground.png"));
      reinterpret_cast<SpriteRenderer*>(newObject->GetComponent(CT_SpriteRenderer))->AddSprite(mmb);
      reinterpret_cast<SpriteRenderer*>(newObject->GetComponent(CT_SpriteRenderer))->ChangeState("mainmenubg");
      newObject->GetTransform()->GetWorldScale().x *= 2.06f; //Correct the aspect ratio
      newObject->GetTransform()->GetWorldScale() *= 13.0f; //Scale it way up (it's a tiny rectangle)
    }

    if (properties->GetObjectMember(&custom, "KillPlayer"))
    {
      AABB *box = new AABB(newObject);
      box->SetHalfSize(1, 1);
      newObject->AddGameComponent(CT_BoxCollider, box);

      RigidBody *body = new RigidBody(newObject, reinterpret_cast<Primitive*>(box));
      body->SetStatic();
      newObject->AddGameComponent(CT_Body, body);

      KillPlayer *IMUSTTASTEBLOOD = new KillPlayer(newObject);
      newObject->AddGameComponent(CT_KILLPLAYER, IMUSTTASTEBLOOD);
    }


    //Currently, if we add a SoundEmitter to an object specifically, it's so we can
    //check one of various possible settings (currently just "StartingMusic" for what
    //to play on Initialization). Miiiight expand or revamp this later.
    if (properties->GetObjectMember(&custom, "SoundEmitter"))
    {
      SoundEmitter *sEmitter = new SoundEmitter(newObject);
      newObject->AddGameComponent(CT_SoundEmitter, reinterpret_cast<Component>(sEmitter));

      if (properties->GetObjectMember(&properties, "StartingMusic"))
      {
        std::string music;
        properties->GetStringVal(&music);
        sEmitter->playSoundOnStart = true;
        sEmitter->startSoundName = music;
      }
    }

    if (properties->GetObjectMember(&custom, "DestroyOnActivate"))
    {
        
        if (newObject->GetComponent(CT_BoxCollider) == NULL && newObject->GetComponent(CT_Body) == NULL)
        {
            AABB *box = new AABB(newObject);
            box->SetHalfSize(1, 1);
            newObject->AddGameComponent(CT_BoxCollider, box);
        
            RigidBody *body = new RigidBody(newObject, reinterpret_cast<Primitive*>(box));
            body->SetStatic();
            newObject->AddGameComponent(CT_Body, body);
        }
        

        DestroyOnActivate * destroy = new DestroyOnActivate(newObject);
        newObject->AddGameComponent(CT_DESTROY_ON_ACTIVATE, destroy);
    }

	if (properties->GetObjectMember(&custom, "DataFile"))
	{

		std::string fileName;
		std::string objName;
		DynamicElement* objEle;
		custom->GetStringVal(&fileName);
		object->GetObjectMember(&objEle, "name");
		objEle->GetStringVal(&objName);

		

		Serializer::ZeroSerializer file;
		file.open(fileName.c_str());
		file.CreateArchive();
		Serializer::DataNode* Trunk = file.GetTrunk();
		
		//newobj->Name = *Trunk->branch->next->branch->value_.String_;
		//Skip 1st 3 objects
		auto ct = Trunk->branch;
		//Ct = "Cog"
		while (ct)
		{
			//currName = "Object Name as specified by the name in the 'Named' component."
			std::string currName;
			ct->branch->branch->GetValue(&currName);
			if (currName.compare(objName) == 0)
			{
				//compName = "ComponentName"
				auto obj = ct->branch->next;
				//If the component is registered, serialize it. If it isn't, it is a Zilch Component.
				while (obj)
				{
					std::string compName = obj->objectName;
					if (SerialMap.find(compName) != SerialMap.end())
					{
						IComponent* comp = SerialMap[compName]->Create(newObject);
						comp->Serialize(properties, obj->branch);
						newObject->AddGameComponent(comp->getComponentType(), comp);
					}
					else
					{
						//Creating a ZilchComponent;
						Zilch::LibraryRef* library = &(ZILCH->lib);
						//Bind the Zilch class
						Zilch::BoundType* ZilchClass = (*library)->BoundTypes.findValue(compName.c_str(), nullptr);
						ErrorIf(ZilchClass == nullptr, "Failed to find a Zilch type named ", compName);

						Zilch::ExecutableState* state = ZILCH->GetDependencies();
						Zilch::Handle ActiveScript = state->AllocateDefaultConstructedHeapObject(ZilchClass, ZILCH->Report, Zilch::HeapFlags::NonReferenceCounted);

						ZilchComponent* script = (ZilchComponent*)ActiveScript.Dereference();
						script->ThisHandle = ActiveScript;

						//Create an array of arguments
						Zilch::Array<Zilch::Type*> args;
						args.push_back(ZilchTypeId(IEntity*));
						//Find the Create function
						// We pass in an array of types to specify the arguments we want.
						// We also pass in the void type because we don't expect a return value

						Function* ZilchCreate = ZilchClass->FindFunction("Create", args, ZilchTypeId(void), Zilch::FindMemberOptions::None);
						ErrorIf(ZilchCreate == nullptr, "Failed to find function 'Create' on Zilch type ", ZilchClass);
						{
							// Invoke the Create function, which assigns this object an owner.
							Zilch::Call call(ZilchCreate, ZILCH->GetDependencies());
							call.SetHandle(Zilch::Call::This, ActiveScript);
							call.SetHandle(0, newObject);
							call.Invoke(ZILCH->Report);
						}

						Zilch::Array<Zilch::Type*> args2;
						args2.push_back(ZilchTypeId(DynamicElement*));
						args2.push_back(ZilchTypeId(Serializer::DataNode*));
						//Find theSerialize function
						// We pass in an array of types to specify the arguments we want.
						// We also pass in the void type because we don't expect a return value

						Function* ZilchSerialize = ZilchClass->FindFunction("Serialize", args2, ZilchTypeId(void), Zilch::FindMemberOptions::None);
						ErrorIf(ZilchSerialize == nullptr, "Failed to find function 'Serialize' on Zilch type ", ZilchClass);
						{
							// Invoke the Create function, which assigns this object an owner.
							Zilch::Call call(ZilchSerialize, ZILCH->GetDependencies());
							call.SetHandle(Zilch::Call::This, ActiveScript);
							call.SetHandle(0, properties);
							call.SetHandle(1, obj->branch);
							call.Invoke(ZILCH->Report);
						}
						newObject->AddZilchComponent(ActiveScript);
					}
					obj = obj->next;
				}
				break;
			}
			//Component* newcomp = nullptr;// newObject->AddComponent(ct->objectName);
			//if (newcomp)
			//{
				//newcomp->gameObject = newobj;
				//newcomp->Serialize(ct->branch);
				//newcomp->Initialize();
			//}
			ct = ct->next;
		} //while
		

		//Get pointer to appropriate library
		/**/
		//newObject->AddGameComponent(CT_ZILCHCOMPONENT, comp);
	}

    /* Got rid of this since you can just use any object's "Visible" checkbox in Tiled now
    if (properties->GetObjectMember(&custom, "Invisible"))
    {
        newObject->SetInVisible();
    }*/
  }

  //All possible components accounted for. Add the game object!
  AddGameObject(newObject);
}

//It should be preeeeetty clear when we haven't loaded a level
TileMapData::TileMapData()
{
  width = -1;
  height = -1;
  tiles = NULL;
}

IComponent *FactoryManager::GetWall()
{
  return wall_;
}
void FactoryManager::SetWall(IComponent *wall)
{
  wall_ = wall;
}

void FactoryManager::ClearEverythingInFactory()
{
    m_GameObjectList.clear();
    m_TotalObjectCount = 0;
    m_DeletedObjectCount = 0;
    m_CurrentObjectCount = 0;
    m_Targets.clear();
	
    /*
    for (GameObjectUmap::iterator iter = m_GameObjectList.begin();
        iter != m_GameObjectList.end(); iter++)
    {
        SPIN_ENGINE_SAFE_DELETE(m_GameObjectList[(*iter)->GetEntityId()]);
        m_GameObjectList.erase(iter);
        --m_CurrentObjectCount;
        ++m_DeletedObjectCount;
    }
    */
}

void FactoryManager::CreatePlayer()
{
    for (int i = 0; i < 4; ++i)
    {
        GameObject mem_ = MemoryManager::Allocate_GameObj();
        GameObject obj = new (mem_)IEntity(m_TotalObjectCount++, PlayerSpawn->GetTransform()->GetWorldPosition());
		obj->SetName("Player");
        m_GameObjectList.push_back(obj);
        m_CurrentObjectCount++;
    }
}




GameObject FactoryManager::FindObjectByName(std::string Target)
{
  for(unsigned int i = 0; i < m_GameObjectList.size(); ++i)
  {
    std::string test =  *(m_GameObjectList[i]->GetName());

    if(test == Target)
    {
      return m_GameObjectList[i];
    }
  }
  return NULL;
}