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
#include "Precompiled.h"
#include "ActionSequence.h"
#include "ZilchCompiledLib.h"
#include "ScriptComponent.h"
#include "SpriteRenderer.h"

//constants values
const std::string defaultName = "GameObject";
const std::string defaultTag = "Untagged";
const int sizeof_name_tag = 100;

//Forward Declaration
class IEntity;

//typedefs
typedef IEntity* GameObject;

typedef Transform ITransform;
typedef SpriteRenderer ISpriteRenderer;

typedef unsigned int EntityId; //unique game object id
typedef std::vector<GameObject> GameObjectUmap;
typedef std::unordered_map<EntityId, GameObject>::iterator GObjUmapIterator;

#define ComponentPointer(Name) Name* Name = nullptr

class IEntity : public Zilch::IZilchObject
{
public:
	ZilchDeclareBaseType(IEntity, Zilch::TypeCopyMode::ReferenceType);
  //Constructor & Destructor
  IEntity(EntityId id = 0, Vector3D pos = Vector3D(), Vector3D rot= Vector3D(), Vector3D scal= Vector3D(1.0f,1.0f,1.0f), 
          std::string tag = defaultTag, std::string name = defaultName, bool IsVisible = true);
  ~IEntity();

  //static variables
  static EntityId total_game_object_count;

  

  //Getters
  EntityId GetEntityId() const;
  Component GetComponent(Component_Type type);
  Component GetComponent(Zilch::String type);
  Zilch::Handle GetZilchComponent(Zilch::String type);
  bool IsObjectActive();
  bool IsObjectVisible();
  ITransform* GetTransform();
  char* GetName();
  char* GetTag();
  void SetName(char *);
  //Setters
  void AddGameComponent(Component_Type type, Component comp);
  void AddZilchComponent(Zilch::Handle comp);
  void RemoveGameComponent(Component_Type type);
  void SetVisible();
  void SetInVisible();
  void Destroy();
  //Action Sequence
  ActionSequence* Actions = nullptr;

  //Methods
  virtual bool Initialize();
  virtual void Update(float dt);

  //Component Pointers For Easy Access

  ComponentPointer(Transform);
  ComponentPointer(SpriteRenderer);

  bool IsCaptured;

protected:
  EntityId Entity_Id;
  Component Components[Component_Count];
  std::vector<Zilch::Handle> ZilchComponents;


  bool IsVisible;
  bool IsActive;
  char tag[sizeof_name_tag];
  char name[sizeof_name_tag];
  void CleanupComponents();
};
