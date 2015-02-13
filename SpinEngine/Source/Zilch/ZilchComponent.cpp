/******************************************************************************/
/*!
\file   WindowSystem.h
\author Micah Rust, Sagnik Chowdhury
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Contains all of the framework for creating and maintaining a window.
*/
/******************************************************************************/

#include <Precompiled.h>
#include "ZilchCompiledLib.h"
#include "ScriptComponent.h"
#include "DynamicElement.h"

#pragma warning(disable:4413)

using namespace Zilch;

  ZilchDefineType(ZilchComponent, SpinningZilch)
  {
	  type->HandleManager = ZilchManagerId(Zilch::PointerManager);
	  ZilchBindConstructor();
	  ZilchBindDestructor();
	  ZilchBindMethod(Create);
	  ZilchBindMethod(Serialize);
	  ZilchBindMethod(Initialize);
	  ZilchBindFieldGet(Owner);
  }

  //CONSTRUCTOR
  ZilchComponent::ZilchComponent() : IComponent()
  {
  }

  void ZilchComponent::Create(IEntity* owner) 
  {
	  Owner = owner;
	  componentType = CT_ZILCHCOMPONENT;
	  std::cout << "CREATING ZILCH COMPONENT" << std::endl;
  }

  void ZilchComponent::DeleteThis()
  {
		Zilch::ExceptionReport report;
		Zilch::Array<Zilch::Type*> args;
		Function* ZilchInitialize = ThisHandle.Type->FindFunction("Destroy", args, ZilchTypeId(void), Zilch::FindMemberOptions::None);
		ErrorIf(ZilchInitialize == nullptr, "Failed to find function 'Initialize' on Zilch type ", ThisHandle.Type);
		{
			// Invoke the Create function, which assigns this object an owner.
			Zilch::Call call(ZilchInitialize, ZILCH->GetDependencies());
			call.SetHandle(Zilch::Call::This, ThisHandle);
			call.Invoke(report);
		}

	  ThisHandle.Delete();
  }

  ZilchComponent::~ZilchComponent()
  {
	  
  }


  void ZilchComponent::Serialize(DynamicElement* props, Serializer::DataNode* data)
  {
	  //std::cout << "SERIALIZING ZILCH COMPONENT" << std::endl;
	  //Serializer::DataNode* temp = componentData->FindElement(componentData, "OHai");
	  //temp->GetValue(&ohai);
	  Zilch::ExceptionReport report;
	  auto fields = ThisHandle.Type->InstanceFields.values();
	  while (!fields.empty())
	  {

		  Field* field = fields.front();
		  Serializer::DataNode* value = data->FindElement(data, field->Name.c_str());
		  fields.popFront();

		  if (value != nullptr)
		  {
			  Call call(field->Set, ZILCH->GetDependencies());
			  call.SetHandle(Zilch::Call::This, ThisHandle);
			  switch (value->dataType)
			  {
			  case Serializer::TYPE_INT:
			  case Serializer::TYPE_UINT:
			  {
				  call.Set(0, value->value_.Int_);
				  break;
			  }
			  case Serializer::TYPE_BOOL:
			  {
				  call.Set(0, value->value_.Bool_);
				  break;
			  }
			  case Serializer::TYPE_FLOAT:
			  {
				  call.Set(0, value->value_.Float_);
				  break;
			  }
			  case Serializer::TYPE_STRING:
			  {
				  call.Set(0, String(value->value_.String_->c_str()));
				  break;
			  }
			  case Serializer::TYPE_VEC2:
			  {
				  std::vector<float>* vec = value->value_.VecN_;
				  call.Set(0, Real2(vec->at(0), vec->at(1)));
				  break;
			  }
			  case Serializer::TYPE_VEC3:
			  {
				  std::vector<float>* vec = value->value_.VecN_;
				  call.Set(0, Real3(vec->at(0), vec->at(1), vec->at(2)));
				  break;
			  }
			  case Serializer::TYPE_VEC4:
			  {
				  std::vector<float>* vec = value->value_.VecN_;
				  call.Set(0, Real4(vec->at(0), vec->at(1), vec->at(2), vec->at(3)));
				  break;
			  }
			  default:
			  {
				  std::string msg = "UNRECOGNIZED TYPE ";
				  msg.append(value->typeString);
				  msg.append(" FOR VARIABLE ");
				  msg.append(value->objectName);
				  std::cout << msg << std::endl;
			  }
			  }

			  call.Invoke(report);
		  }
	  }
  }

  void ZilchComponent::Update(float dt)
  {

  }


  //INITIALIZE
  bool ZilchComponent::Initialize()
  {
	  //std::cout << "INITIALIZING ZILCH COMPONENT" << std::endl;
	  /*
	  //Get pointer to appropriate library
    library = &(ZILCH->lib);
    //Bind the Zilch class
    ZilchClass = (*library)->BoundTypes.findValue(ZilchClassName->c_str(), nullptr);
    ErrorIf(ZilchClass == nullptr, "Failed to find a Zilch type named ", ZilchClassName);

    //Create an array of arguments
    Zilch::Array<Zilch::Type*> args;
	//args.push_back(ZilchTypeId(ObjectSystem*));
	args.push_back(ZilchTypeId(GameObject*));
    //Find the Initialize function
    // We pass in an array of types to specify the arguments we want, in this case its an empty array
    // We also pass in the void type because we don't expect a return value
    ZilchInitialize = ZilchClass->FindFunction("Initialize", args, ZilchTypeId(void), Zilch::FindMemberOptions::None);
    ErrorIf(ZilchInitialize == nullptr, "Failed to find function 'Initialize' on Zilch type ", ZilchClass);

    // The exception report stores any exceptions that may have occurred while executing code
    // Exceptions include accessing arrays out of bounds, dereferencing null, etc
    Zilch::ExceptionReport report;
    Zilch::ExecutableState* state = ZILCH->GetDependencies();
    ActiveScript = state->AllocateDefaultConstructedHeapObject(ZilchClass, report, Zilch::HeapFlags::NonReferenceCounted);
	*/
	//var Owner : GameObject;
	//owner->PropertyType == ZilchTypeId(Real);
	/*
	Field* owner = ZilchClass->InstanceFields["Owner"];
	
	{
		Zilch::Call call(owner->Set, ZILCH->GetDependencies());
		call.SetHandle(Zilch::Call::This, bob);
		call.Set(0, gameObject);
	}
	*/
	//Trevor touched this stuff
	//this->gameObject = (GameObject*)0x12345678;
	  return true;
  }
  

  void ZilchComponent::Release()
  {

  }