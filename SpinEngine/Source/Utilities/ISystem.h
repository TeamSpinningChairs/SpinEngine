/****************************************************************************/
/*!
\author Esteban Maldonado
\par    email: esteban.maldonado@digipen.edu
\par    Course: GAM 200
\brief

Class interface for the base system class

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#pragma once

#include "Precompiled.h"
#include <string>
#include <unordered_map>
#include "IComponent.h"
//Systems will be updated in this order.
//All comments are to consider potential conflicts in order-of-execution
//(mostly, we want input to affect player direction *in the same frame*, which requires Physics
//to be updated *after* any system that affects a rigidbody. small difference, but perceptible even at 60fps)
enum SystemType 
{
  //Core,
  
  ST_Input, //Things in Input call character controllers, which update their objects' rigidbodies
  
  ST_Factory, //Updates all game objects, which updates all components not in one of the systems
              //This includes TileMap collision (until / if we add that to the physics manager)
  ST_Physics,
  ST_Sound,
  ST_Graphics,
  ST_Zilch,
  //ST_GameStateMgr,
  //Graphics is updated last, but I think all the relevant stuff to order-of-execution is in Render,
  //which is called after all the systems update anyways
  System_Count
};

class SysComponentListener;
class ISystem;

typedef ISystem* System;
typedef std::unordered_map<SystemType, System> SystemUmap;
typedef std::unordered_map<SystemType, System>::iterator SysUmapIterator;

class ISystem 
{
	ZilchDeclareBaseType(ISystem, Zilch::TypeCopyMode::ReferenceType);

protected:
	char system_name[100];
	SystemType sysType;
  bool isInitialized;

public:
	ISystem();
  ISystem(std::string name, SystemType type): sysType(type),isInitialized(false){ strcpy(system_name, name.c_str() ); }
	virtual ~ISystem(){}
	virtual bool Initialize() = 0;
	virtual void Update(float dt) = 0;
	virtual void Shutdown() = 0;
	virtual std::string GetName() { return system_name; }
	virtual SystemType GetType() { return sysType; }

	std::list<IComponent *> Components;
	SysComponentListener * SystemComponents;
};