/****************************************************************************/
/*!
\author Esteban Maldonado
\par    email: esteban.maldonado@digipen.edu
\par    Course: GAM 200
\brief

The main engine file where all systems initialize, update, render and shutdown

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#pragma once

#include "Precompiled.h"
#include"Graphics\GraphicsManager.h"
#include"Input\InputManager.h"
#include"Factory\FactoryManager.h"
#include"Input\ControllerComponent.h"
#include"KeyboardComponent.h"
#include "Physics/PhysicsManager.h"
#include "Gameplay\ScrollComponent.h"
#include "DebugMessages.h"
#include "MovingPlatforms.h"
#include "Sound\SoundManager.h"
#include "Wall.h"
#include "WallTarget.h"
#include "collision_tilemap.h"
#include "ZilchCompiledLib.h"

class Engine;
extern Engine* ENGINE;

class Engine
{
public:
  Engine(std::string WindowName, UINT width, UINT height, bool IsFullScreen, HINSTANCE hInstance);
  ~Engine(){}
  bool Initialize();
  void Run();
  void Shutdown();

  //System pointer variables
  System m_SystemsArr[System_Count];
  InputManager * m_Input;
  PhysicsManager* m_physics;
  SystemUmap m_Systems;
  SoundManager* m_Sound;
  GraphicsManager *m_Graphics;
  FactoryManager* m_Factory;
  ConsoleDebug* DebugSystem;
  ScriptSystem* ZilchSystem;
  //ChaseComponent* TestChase;
  System GetSystem(SystemType type);
private:
  std::string window_name;
  UINT screenWidth;
  UINT screenHeight;
  bool IsFullScreen;

  bool returningFromOutOfFocus;

	HINSTANCE m_hinstance;
	HWND m_hwnd;

  //FOR TESTING
  SpriteRenderer* tempSprite, *tempSprite2, *tempSprite3, *tempSprite4, *tempSprite5, *tempSprite6;
  GameObject first_object, second_object, third_object, fourth_object, fifth_object, sixth_object;
  TileMapCollision *tcoll;
  //ControllerComp * TestController;
  //KeyboardComponent * TestKeyboard;
  ScrollComponent * TestScroll;
  MovingPlatform * TestPlatform;
  WallComp * TestWall;
  WallTarget * TestTarget;
  //END FOR TESTING

  //Calculates FPS
  void CalculateFPS(float dt);
  float m_FPS;					//Frames per second of our application
  float frameTime;
};