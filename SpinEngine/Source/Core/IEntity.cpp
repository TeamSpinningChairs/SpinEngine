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
#include "Precompiled.h" //windows.h
#include "IEntity.h"
#include "FactoryAccess.h" //So that you can get the graphics manager
#include "GraphicsManager.h" //So that you can remove sprite renderers from the graphics manager when removing/deleting sprite renderers

#pragma warning(disable:4351) //So we don't get a warning for including Components() in the initializer list.
//(I set this so it'd initialize the array to default (null), which I think it was already doing, but this is more explicit.)

ZilchDefineType(IEntity, SpinningZilch)
{
	type->HandleManager = ZilchManagerId(Zilch::PointerManager);
	ZilchBindMethodOverload(GetComponent, IComponent*, Zilch::String);
	//ZilchBindMethod(GetTransform);
	ZilchBindFieldGetSet(Transform);
	ZilchBindMethod(Destroy);
	ZilchBindFieldGetSetAs(IsVisible, "Visible");
}

ZilchDefineType(IComponent, SpinningZilch)
{
	type->HandleManager = ZilchManagerId(Zilch::PointerManager);
	ZilchBindConstructor();
	ZilchBindDestructor();
}

EntityId IEntity::total_game_object_count = 0;

IEntity::IEntity(EntityId id, Vector3D pos, Vector3D rot, Vector3D scal, std::string tag,
	std::string name, bool IsVisible) : Entity_Id(id), IsActive(true),
  IsVisible(IsVisible), Components(), IsCaptured(false)
{
  //EntityComponents[CT_TransformComponent] = new Transform(this, pos, rot, scal);
  //Entity_Id = total_game_object_count++;
  std::memset(Components, 0, sizeof(Components) );
  strcpy(this->name, name.c_str());
  strcpy(this->tag, tag.c_str());

  ITransform* new_transform_comp = reinterpret_cast<ITransform*>(
    MemoryManager::Allocate_Component(CT_TransformComponent));

  Components[CT_TransformComponent] = 
    new (new_transform_comp)ITransform(this, pos, rot, scal);
}

IEntity::~IEntity()
{
	if (Actions)
	{
		delete Actions;
		Actions = nullptr;
	}
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

Component IEntity::GetComponent(Zilch::String type)
{
	std::string name = type.c_str();

	for (auto i : Components)
	{
		if (name.compare(i->GetName()) != 0)
		{
			return i;
		}
	}
	return nullptr;
}

Zilch::Handle IEntity::GetZilchComponent(Zilch::String type)
{
	std::string name = type.c_str();

	for (auto i : ZilchComponents)
	{
		if (type == i.Type->Name)
		{
			return i;
		}
	}
	return Zilch::Handle();
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

void IEntity::AddZilchComponent(Zilch::Handle comp)
{
	if (!comp.IsNull())
	{
		ZilchComponents.push_back(comp);
	}
}

void IEntity::RemoveGameComponent(Component_Type type)
{
  //We cannot remove transform components
  if (type != CT_TransformComponent)
  {
    //If sprite renderer, remove from graphics manager
    if (type == Component_Type::CT_SpriteRenderer)
    {
      GlobalFactory->GetGraphicsManager()->RemoveSpriteRend( 
        reinterpret_cast<ISpriteRenderer*>(Components[type]) );
    }

    SPIN_ENGINE_SAFE_DELETE(Components[type]);
  }
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
  for (auto i : ZilchComponents)
  {
	 i.Delete();
  }
  ZilchComponents.clear();

  for (int i = 0; i < Component_Count; i++)
  {
    if (Components[i])
    {
      if (Components[i]->getComponentType() == CT_TransformComponent)
      {
        reinterpret_cast<ITransform*>(Components[i])->~Transform();
        MemoryManager::Free_Component(CT_TransformComponent, Components[i]);
        Components[i] = NULL;
      }
      else
      {
        SPIN_ENGINE_SAFE_DELETE(Components[i]);
      }
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
  
  
  Zilch::ExceptionReport report;
  Zilch::Array<Zilch::Type*> args;
  for (auto i : ZilchComponents)
  {
	  Function* ZilchInitialize = i.Type->FindFunction("Initialize", args, ZilchTypeId(void), Zilch::FindMemberOptions::None);
	  ErrorIf(ZilchInitialize == nullptr, "Failed to find function 'Initialize' on Zilch type ", i.Type);
	  {
		  // Invoke the Create function, which assigns this object an owner.
		  Zilch::Call call(ZilchInitialize, ZILCH->GetDependencies());
		  call.SetHandle(Zilch::Call::This, i);
		  call.Invoke(report);
	  }
  }
  
  return true;
}

//Go through every component except for the ones with core managers
void IEntity::Update(float dt)
{
  //for (unsigned i = 0; i < EntityComponents.size(); ++i )
  //{
  //  if (EntityComponents.find((Component_Type)i) != EntityComponents.end())
  //  {
  //    EntityComponents[(Component_Type)i]->Update(dt);
  //  }
  //}

  if (ZilchComponents.size())
  {
    Zilch::ExceptionReport report;
    Zilch::Array<Zilch::Type*> args;
    args.push_back(ZilchTypeId(float));
    for (auto i : ZilchComponents)
    {
      Function* ZilchInitialize = i.Type->FindFunction("Update", args, ZilchTypeId(void), Zilch::FindMemberOptions::None);
      ErrorIf(ZilchInitialize == nullptr, "Failed to find function 'Initialize' on Zilch type ", i.Type);
      {
        // Invoke the Create function, which assigns this object an owner.
        Zilch::Call call(ZilchInitialize, ZILCH->GetDependencies());
        call.SetHandle(Zilch::Call::This, i);
        call.Set(0, dt);
        call.Invoke(report);
      }
    }
  }

  for (int i = 0; i < Component_Count; ++i)
  {
    if (!Components[i])
      continue;
    if (i == CT_SpriteRenderer || i == CT_ParticleEmitter)
      continue;
    //if (i == CT_Keyboard_Control || i == CT_Player_Controller)
      //continue;
    if (i >= CT_Body && i <= CT_CircleCollider)
      continue;

	if (Components[i])
	{
	  Components[i]->Update(dt);
	}
  }
}

void IEntity::Destroy()
{
	IsActive = false;
}

ITransform* IEntity::GetTransform()
{
  //return reinterpret_cast<Transform*>(EntityComponents[CT_TransformComponent]);
  return reinterpret_cast<ITransform*>(Components[CT_TransformComponent]);
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