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
#include "IEntity.h"
#include <Windows.h>
#include "../Utilities/MemoryHelper.h"
//#include "FactoryAccess.h" //So that you can get the graphics manager
//#include "GraphicsManager.h" //So that you can remove sprite renderers from the graphics manager when removing/deleting sprite renderers

EntityId IEntity::total_game_object_count = 0;

IEntity::IEntity(EntityId id, Vector3 pos, Vector3 rot, Vector3 scal, std::string tag,
	std::string name, bool IsVisible) : Entity_Id(id), IsActive(true),
                 IsVisible(IsVisible)
{
  //EntityComponents[CT_TransformComponent] = new Transform(this, pos, rot, scal);
  //Entity_Id = total_game_object_count++;
  std::memset(Components, 0, sizeof(Components) );
  strcpy(this->name, name.c_str());
  strcpy(this->tag, tag.c_str());

  //Transform* new_transform_comp = reinterpret_cast<Transform*>(
  //  MemoryManager::Allocate_Component(CT_TransformComponent));

  Components[CT_TransformComponent] = 
    new /*(new_transform_comp)*/Transform(this, pos, rot, scal);
}

IEntity::~IEntity()
{
  CleanupComponents();
}

EntityId IEntity::GetEntityId() const
{ 
  return Entity_Id;
}

Component IEntity::GetComponent(Component_Type type)
{
  return Components[type];
}

bool IEntity::IsObjectActive()
{
  return IsActive;
}

void IEntity::AddGameComponent(Component_Type type,Component comp)
{ 
  if(comp)
  {
    if (!Components[type])
    {
      Components[type] = comp;
    }
    else
    {
      char temp[500] = "ERROR: COMPONENT ";
      std::strcat(temp, comp->GetName());
      std::strcat(temp, " ALREADY ADDED TO ");
      std::strcat(temp, this->name);

      MessageBox(NULL, temp, NULL, NULL);
    }
  }
  else
  {
    char temp[500];
    std::strcat(temp, "ERROR: NULL COMPONENT TO ");
    std::strcat(temp, this->name);
    MessageBox(NULL, temp, NULL, NULL);
  }
}

void IEntity::RemoveGameComponent(Component_Type type)
{
  ////We cannot remove transform components
  //if (type != CT_TransformComponent)
  //{
  //  //If sprite renderer, remove from graphics manager
  //  if (type == Component_Type::CT_SpriteRenderer)
  //  {
  //    GlobalFactory->GetGraphicsManager()->RemoveSpriteRend( 
  //      reinterpret_cast<SpriteRenderer *>(Components[type]) );
  //  }

  //  SPIN_ENGINE_SAFE_DELETE(Components[type]);
  //}
}

void IEntity::CleanupComponents()
{

  //for (CompUmapIterator it = EntityComponents.begin(); it != EntityComponents.end(); ++it )
  //{
  //  if( it->second)
  //  {
  //    delete it->second;
  //  }
  //}

  //Remove components from systems (I think SpriteRenderer is the only one guaranteed)
  /*SpriteRenderer *TestRend = reinterpret_cast<SpriteRenderer *>(GetComponent(CT_SpriteRenderer));
  GlobalFactory->GetGraphicsManager()->RemoveSpriteRend(TestRend);*/
  //We actually remove the sprite renderer in its own destructor.

  for (int i = 0; i < Component_Count; i++)
  {
    if (Components[i])
    {
      SPIN_ENGINE_SAFE_DELETE(Components[i]);
    }
  }
}

bool IEntity::Initialize()
{
  //for (CompUmapIterator it = EntityComponents.begin(); it != EntityComponents.end(); ++it )
  //{
  //  if( !it->second->Initialize())
  //  {
  //    return false;
  //  }
  //}

  for (int i = 0; i < Component_Count; ++i)
  {
    if (Components[i])
    {
      if (!Components[i]->Initialize())
      {
        return false;
      }
    }
  }

  return true;
}

//Go through every component except for the ones with core managers
void IEntity::Update(float dt)
{
  for (int i = 0; i < Component_Count; ++i)
  {
    //if (!Components[i])
    //  continue;
    //if (i == CT_SpriteRenderer || i == CT_ParticleEmitter)
    //  continue;
    ////if (i == CT_Keyboard_Control || i == CT_Player_Controller)
    //  //continue;
    //if (i >= CT_Body && i <= CT_CircleCollider)
    //  continue;

	  if (Components[i])
	  {
	    Components[i]->Update(dt);
	  }
  }
}

Transform* IEntity::GetTransform()
{
  //return reinterpret_cast<Transform*>(EntityComponents[CT_TransformComponent]);
  return reinterpret_cast<Transform*>(Components[CT_TransformComponent]);
}

char* IEntity::GetName()
{
  return name;
}

char* IEntity::GetTag()
{
  return tag;
}

bool IEntity::IsObjectVisible()
{
  return IsVisible;
}


void IEntity::SetVisible()
{
    IsVisible = true;
}

void IEntity::SetInVisible()
{
    IsVisible = false;
}

void IEntity::SetName(char * newname)
{
    strcpy(this->name, newname);
}