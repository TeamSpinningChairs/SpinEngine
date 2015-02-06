/****************************************************************************/
/*!
\author Esteban Maldonado
\par    email: esteban.maldonado@digipen.edu
\par    Course: GAM 200
\brief

Static class that handles component and game object allocations with the
CS280 object allocator

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#include "Precompiled.h"
#include "MemoryManager.h"

//Set every Object Allocator pointer to NULL initially
ObjectAllocator* MemoryManager::GameObj_OA = 0; //For IEntity objects
ObjectAllocator* MemoryManager::System_OA[System_Count] = { 0 }; //For systems, not currently  used
ObjectAllocator* MemoryManager::Component_OA[Component_Count] = { 0 }; // For components
OA_Umap MemoryManager::Other_OA = OA_Umap();

void MemoryManager::Add_Component_Allocator(Component_Type comp_type, size_t object_size)
{
  //If we don't have a specific component allocator then proceed
  if (!Component_OA[comp_type])
  {
    OAConfig config(OAConfig::hbBasic);
    config.ObjectsPerPage_ = MAX_COMPONENTS_PER_PAGE;
    config.UseCPPMemManager_ = false;
    config.DebugOn_ = false;
    config.MaxPages_ = 0;

    Component_OA[comp_type] = new ObjectAllocator(object_size, config);
  }
}

void MemoryManager::Add_System_Allocator(SystemType sys_type, size_t object_size)
{
  //If we don't have a specific system allocator then proceed
  if (!System_OA[sys_type])
  {
    OAConfig config(OAConfig::hbBasic);
    config.ObjectsPerPage_ = MAX_SYSTEMS_PER_PAGE;
    config.UseCPPMemManager_ = false;
    config.DebugOn_ = false;
    config.MaxPages_ = 0;

    System_OA[sys_type] = new ObjectAllocator(object_size, config);
  }
}

void MemoryManager::Add_GameObj_Allocator(size_t object_size)
{
  if (!GameObj_OA)
  {
    OAConfig config(OAConfig::hbBasic);
    config.ObjectsPerPage_ = MAX_GAMEOBJECT_PER_PAGE;
    config.UseCPPMemManager_ = false;
    config.DebugOn_ = false;
    config.MaxPages_ = 0;

    GameObj_OA = new ObjectAllocator(object_size, config);
  }
}

Component MemoryManager::Allocate_Component(Component_Type comp_type)
{
  //If we have an object allocator of the specific component type
  if (Component_OA[comp_type])
  {
    return reinterpret_cast<Component>( Component_OA[comp_type]->Allocate() );
  }

  //If not then return null
  return NULL;
}

System MemoryManager::Allocate_System(SystemType sys_type)
{
  //If we have an object allocator of the specific system type
  if (System_OA[sys_type])
  {
    return reinterpret_cast<System>( System_OA[sys_type]->Allocate() );
  }

  //If not then return null
  return NULL;
}

GameObject MemoryManager::Allocate_GameObj()
{
  //If we have a GameObject Allocator
  if (GameObj_OA)
  {
    return reinterpret_cast<GameObject>( GameObj_OA->Allocate() );
  }

  //if not then return 0
  return NULL;
}

void MemoryManager::Free_Component(Component_Type comp_type, void* component)
{
  //If we have an object allocator of the specific component type
  if (Component_OA[comp_type])
  {
    Component_OA[comp_type]->Free(component);
  }
}

void MemoryManager::Free_System(SystemType sys_type, void* system)
{
  //If we have an object allocator of the specific component type
  if (System_OA[sys_type])
  {
    System_OA[sys_type]->Free(system);
  }
}

void MemoryManager::Free_GameObject(void* game_object)
{
  if (GameObj_OA)
  {
    GameObj_OA->Free(game_object);
  }
}

void MemoryManager::Clear_Allocators()
{
  //CLEAR COMPONENT ALLOCATOR
  for (int i = 0; i < Component_Count; i++)
  {
    SPIN_ENGINE_SAFE_DELETE(Component_OA[i]);
  }

  //CLEAR SYSTEM ALLOCATOR
  for (int i = 0; i < System_Count; i++)
  {
    SPIN_ENGINE_SAFE_DELETE(System_OA[i]);
  }

  //CLEAR GAME OBJECT ALLOCATOR
  SPIN_ENGINE_SAFE_DELETE(GameObj_OA);

  //CLEAR OTHER OBJECT ALLOCATOR
  for (OA_Umap_iter it = Other_OA.begin(); it != Other_OA.end(); it++)
  {
    SPIN_ENGINE_SAFE_DELETE(it->second);
  }
}

ObjectAllocator* MemoryManager::Add_OtherObj_Allocator(size_t object_size)
{
  OA_ID new_OA_ID = Other_OA.size();
  
  OAConfig config(OAConfig::hbBasic);
  config.ObjectsPerPage_ = MAX_COMPONENTS_PER_PAGE;
  config.UseCPPMemManager_ = false;
  config.DebugOn_ = false;
  config.MaxPages_ = 0;

  Other_OA[new_OA_ID] = new ObjectAllocator(object_size, config);

  return Other_OA[new_OA_ID];
}
