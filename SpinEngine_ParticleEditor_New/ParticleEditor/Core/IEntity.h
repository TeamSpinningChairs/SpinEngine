/****************************************************************************/
/*!
\author Esteban Maldonado
\par    email: esteban.maldonado@digipen.edu
\par    Course: GAM 200
\brief

Class interface for the Game Object

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#pragma once
#include "../Utilities/MemoryHelper.h"
#include "Transform.h"
#include <vector>

//constants values
const std::string defaultName = "GameObject";
const std::string defaultTag = "Untagged";
const int sizeof_name_tag = 100;

//Forward Declaration
class IEntity;

//typedefs
typedef IEntity* GameObject;
typedef unsigned int EntityId; //unique game object id
typedef std::vector<GameObject> GameObjectUmap;
typedef std::unordered_map<EntityId, GameObject>::iterator GObjUmapIterator;

class IEntity
{
public:
  //Constructor & Destructor
  IEntity(EntityId id = 0, Vector3 pos = Vector3(), Vector3 rot= Vector3(), Vector3 scal= Vector3(1.0f,1.0f,1.0f), 
          std::string tag = defaultTag, std::string name = defaultName, bool IsVisible = true);
  ~IEntity();

  //static variables
  static EntityId total_game_object_count;

  //Getters
  EntityId GetEntityId() const;
  Component GetComponent(Component_Type type);
  bool IsObjectActive();
  bool IsObjectVisible();
  Transform* GetTransform();
  char* GetName();
  char* GetTag();
  void SetName(char *);
  //Setters
  void AddGameComponent(Component_Type type, Component comp);
  void RemoveGameComponent(Component_Type type);
  void SetVisible();
  void SetInVisible();

  //Methods
  virtual bool Initialize();
  virtual void Update(float dt);

protected:
  EntityId Entity_Id;
  Component Components[Component_Count];

  bool IsVisible;
  bool IsActive;
  char tag[sizeof_name_tag];
  char name[sizeof_name_tag];
  void CleanupComponents();
};
