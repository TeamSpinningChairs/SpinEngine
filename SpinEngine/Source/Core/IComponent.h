/****************************************************************************/
/*!
\author Esteban Maldonado
\par    email: esteban.maldonado@digipen.edu
\par    Course: GAM 200
\brief

Base interface for the Component class

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#pragma once
#include "Precompiled.h" //string, vector, unorderedmap


enum Component_Type
{
  //Notes on component update order: Some of these are in systems,
  //every other one will be updated in order.

  //Transform - Only mandatory component
  CT_TransformComponent = 0,

  //Graphics Components
  CT_SpriteRenderer,
  CT_CameraComponent,
  CT_ParticleEmitter,
  CT_ParallaxBG,
  CT_CoolRoilingParticle,

  // Sound Components
  CT_SoundEmitter,

  //Physics Components
  CT_Body,
  CT_BoxCollider,
  CT_CircleCollider,

  CT_TileMapCollider,
  CT_TileMapDetector,

  //Input Components
  CT_Player_Controller, //Contains keyboard and controller input for a player
  CT_Keyboard_Control, //Currently unused

  //Game Logic Components
  CT_SCROLL,
  CT_ACTIONTEST,
  CT_WALL,
  CT_CHASE,
  CT_HAND,
  CT_WALL_TARGET,
  CT_WALLDESTROY,
  CT_SWITCH,
  CT_PLAYERTRIGGER,
  CT_MOVPLATFORM,
  CT_DOOR,
  CT_WALL_TRIGGER,
  CT_EXTEND_BLOCK,
  CT_SPAWN_BLOCK,
  CT_COLLISIONACTIVATE,
  CT_DOOR2,
  CT_MENUBUTTON,
  CT_MENUCONTROLLER,
  CT_LEVELTRIGGER,
  CT_DOOR2TRIGGER,
  CT_PROXIMITYACTIVATE,
  CT_HOWTOPLAY,
  CT_CONFIRMDESTROY,
  CT_CHECKPOINT,
  CT_KILLPLAYER,
  CT_DESTROY_ON_ACTIVATE,
  CT_LASERBEAM,
  CT_WALL_CONTROLLER,
  CT_HEADLESS_CHICKEN,
  CT_HEAD_CONTROLLER,
  //future add

  //Logic or Script Components
  CT_DIGIPENLOGOFADE, //splash screen
  CT_TEAMLOGOFADE,
  CT_CREDITSDISPLAY,
  CT_ZILCHCOMPONENT,
  CT_UNDEFINEDCOMPONENT,

  //Last enum value for available component count in engine
  Component_Count
};

//Forward declaration
class IComponent;
class IEntity;
class DynamicElement;
namespace Serializer
{
	class DataNode;
};

//Component typedefs
typedef IComponent* Component;
typedef std::unordered_map<Component_Type, Component> ComponentUmap;
typedef std::unordered_map<Component_Type, Component>::iterator CompUmapIterator;
typedef std::vector<Component> ComponentVector;
typedef std::vector<Component>::iterator ComponentVec_iterator;
//typedef IComponent* ComponentArr[Component_Type::Component_Count];

class IComponent : public Zilch::IZilchObject
{
public:
  ZilchDeclareBaseType(IComponent, Zilch::TypeCopyMode::ReferenceType);
  IComponent(void){}
  IComponent(Component_Type type, IEntity* owner = NULL, const char* name = "component", bool is_active = true) :
    componentType(type), Owner(owner), is_active(is_active)
    {
      strcpy(component_name, name);
    }
  virtual ~IComponent(){}
  virtual bool Initialize(){ return false; }
  virtual void Serialize(DynamicElement* props, Serializer::DataNode* data){}
  virtual void Update(float dt){}
  virtual void Release(){}

  //For Zilch Deletion
  virtual void DeleteThis() { delete this; }

  char* GetName() { return component_name; }
  Component_Type getComponentType(){ return componentType; }
  IEntity* GetOwner(){ return Owner; }
  bool GetActive(){ return is_active; }
  void SetActive(bool is_active){ this->is_active = is_active; }
  Component_Type componentType;
 protected:
  char component_name[100];
  
  IEntity* Owner; //Pointer to the IEntity* or GameObject owner
  bool is_active;
};
