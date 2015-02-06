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
#pragma once
//#include"ISystem.h"
#include "../Core/IComponent.h"
#include "ObjectAllocator.h"
#include "MemoryHelper.h"
#include <cstdio>

#define MAX_COMPONENTS_PER_PAGE 1000
#define MAX_SYSTEMS_PER_PAGE 2
#define MAX_GAMEOBJECT_PER_PAGE 1000

typedef std::unordered_map<Component_Type, ObjectAllocator*> ComponentAllocator;
//typedef std::unordered_map<SystemType, ObjectAllocator*> SystemAllocator;

typedef unsigned int OA_ID;
typedef std::unordered_map<OA_ID, ObjectAllocator*> OA_Umap;
typedef OA_Umap::iterator OA_Umap_iter;

typedef ComponentAllocator::iterator CompAllocIter;
//typedef SystemAllocator::iterator SysAllocIter;

//Forward Declarations
class IEntity;

class MemoryManager
{
public:
  static void Add_Component_Allocator(Component_Type comp_type, size_t object_size);
  //static void Add_System_Allocator(SystemType sys_type, size_t object_size);
  static void Add_GameObj_Allocator(size_t object_size);
  static ObjectAllocator* Add_OtherObj_Allocator(size_t object_size);

  static Component Allocate_Component(Component_Type comp_type);
  //static System Allocate_System(SystemType sys_type);
  static IEntity* Allocate_GameObj();

  static void Free_Component(Component_Type comp_type, void* component);
  //static void Free_System(SystemType sys_type, void* system);
  static void Free_GameObject(void* game_object);

  static void Clear_Allocators();

private:
  static ObjectAllocator* GameObj_OA;

  static ObjectAllocator* Component_OA[Component_Count];
  //static ObjectAllocator* System_OA[System_Count];
  static OA_Umap Other_OA;
};

#ifndef ADD_COMPONENT_ALLOCATOR
#define ADD_COMPONENT_ALLOCATOR(comp_type, object_size) \
{\
  MemoryManager::Add_Component_Allocator(comp_type, object_size); \
}
#endif

#ifndef ADD_GAMEOBJECT_ALLOCATOR
#define ADD_GAMEOBJECT_ALLOCATOR() \
{ \
  MemoryManager::Add_GameObj_Allocator( sizeof(IEntity) ); \
}
#endif

#ifndef ErrorIf
#define ErrorIf(predicate, ...)       \
  if (predicate)                      \
          {                           \
    printf("An error has occured! \n" \
           "Function: %s \n"          \
           "File:     %s \n"          \
           "Line #:   %i \n"          \
           "Description: "            \
           , __FUNCTION__             \
           , __FILE__                 \
           , __LINE__);               \
    fprintf(stderr, __VA_ARGS__);     \
    __debugbreak();                   \
          }
#endif