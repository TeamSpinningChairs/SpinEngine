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
#include "Precompiled.h"
#include "Engine.h"
#include "FactoryAccess.h"
//#include "ChaseComponent.h"
//#include "Level1/Level1.h"
#include "Settings.h"
#include "PlayerController.h" //for the PlayerController constructor, feel free to remove once Factory takes care of player
#include "MenuController.h"

Engine* ENGINE = nullptr;

ZilchDefineType(ISystem, SpinningZilch)
{
	type->HandleManager = ZilchManagerId(Zilch::PointerManager);
}

Engine::Engine(std::string WindowName, UINT width, UINT height, bool IsFullScreen, HINSTANCE hInstance) : 
               window_name(WindowName),screenWidth(width), screenHeight(height), IsFullScreen(IsFullScreen),
               m_hinstance(hInstance), m_FPS(0), frameTime(1.0f / 60.0f), returningFromOutOfFocus(false)
{
  //Initialize all system pointers to NULL
  ZeroMemory(m_SystemsArr, sizeof(m_SystemsArr));

  ENGINE = this;
}

//MAIN ENGINE INITIALIZING PHASE - create and initialize all objects
bool Engine::Initialize()
{
  //Initialize 3 critical object allocators
  ADD_COMPONENT_ALLOCATOR(CT_TransformComponent, sizeof(Transform));
  ADD_COMPONENT_ALLOCATOR(CT_SpriteRenderer, sizeof(SpriteRenderer));
  ADD_COMPONENT_ALLOCATOR(CT_ParticleEmitter, sizeof(ParticleEmitter));

  //ADD_COMPONENT_ALLOCATOR(CT_TileMapCollider, sizeof(TileMapCollision));
  //ADD_COMPONENT_ALLOCATOR(CT_TileMapDetector, sizeof(TileMapDetection));

  ADD_GAMEOBJECT_ALLOCATOR();

	// Create the graphics object.  This object will handle rendering all the graphics for this application.
  m_SystemsArr[ST_Graphics] = new GraphicsManager(screenWidth, screenHeight, window_name, m_hinstance, 
    m_hwnd, d3dColors::Gray, IsFullScreen);
  if (!m_SystemsArr[ST_Graphics])
	{
		return false;
	}
  m_Graphics = reinterpret_cast<GraphicsManager*>(m_SystemsArr[ST_Graphics]);

	// Initialize the graphics object.
  if (!m_Graphics->Initialize())
	{
    MessageBox(m_hwnd, "Could not initialize the Graphics object.", "Error", MB_OK);
		return false;
	}

  // Initialize AntTweakBar
  //if (*GlobalSettings->TWEAK_BAR_ENABLED)
  //{
#ifdef _DEBUG
  TwInit(TW_DIRECT3D9, *m_Graphics->GetDevice() );
  TwWindowSize(m_Graphics->GetWindow()->GetWindowWidth(), m_Graphics->GetWindow()->GetWindowHeight());
  TwDefine(" mybar size='540 420' ");
  TwDefine(" mybar valueswidth=fit ");
  TwDefine(" mybar resizable=true "); // mybar cannot be resized
#endif
  // }
  //Create the global settings accessor
  GlobalSettings = new Settings;

  //Create and initialize the Input Manager
  m_SystemsArr[ST_Input] = new InputManager();
  if (!m_SystemsArr[ST_Input]->Initialize())
  {
    MessageBox(m_hwnd, "Could not initialize the input object.", "Error", MB_OK);
    return false;
  }
  m_Input = reinterpret_cast<InputManager*>(m_SystemsArr[ST_Input]);
 
  //Create the physics manager
  m_SystemsArr[ST_Physics] = new PhysicsManager();
  if (!m_SystemsArr[ST_Physics]->Initialize())
  {
    MessageBox(m_hwnd, "Could not initialize the PhysicsManager object.", "Error", MB_OK);
    return false;
  }
  m_physics = reinterpret_cast<PhysicsManager*>(m_SystemsArr[ST_Physics]);
  //Do whatever this is
  physics = m_physics;

  //Create the sound manager
  m_SystemsArr[ST_Sound] = new SoundManager();
  if (!m_SystemsArr[ST_Sound]->Initialize())
  {
    return false;
  }
  m_Sound = reinterpret_cast<SoundManager*>(m_SystemsArr[ST_Sound]);
  //END Create the sound manager

  //Create the factory and access to it
  m_SystemsArr[ST_Factory] = new FactoryManager(m_Graphics );
  m_Factory = reinterpret_cast<FactoryManager*>(m_SystemsArr[ST_Factory]);

  DebugSystem = new ConsoleDebug();
  GlobalSystem->RegisterListener(MESSAGE_DEBUG, DebugSystem);
  
  //INITIALIZE THE ZILCH COMPILED LIBRARY
  m_SystemsArr[ST_Zilch] = new ScriptSystem();
  if (!m_SystemsArr[ST_Zilch]->Initialize())
  {
	  MessageBox(m_hwnd, "Could not initialize the scriptsystem object.", "Error", MB_OK);
	  return false;
}
  ZilchSystem = reinterpret_cast<ScriptSystem*>(m_SystemsArr[ST_Zilch]);

  //INITIALIZE GAMESTATE MANAGER
#ifndef _DEBUG
  GameStateMgrInit(GS_MENU, "Digipen_Splash_Screen");
#else
  GameStateMgrInit(GS_LOAD, "ZilchTestLevel"); //LEVEL WE ARE LOADING
  //GameStateMgrInit(GS_LOAD, "Level_1");
  //GameStateMgrInit(GS_LOAD, "New_Wall_And_Gameplay");
#endif
  
  return true;
}

void Engine::Run()
{
  //Main message loop (MAIN APPLICATION LOOP)
  //For every application that remains persistent on a system there needs to be a loop of some kind
  //As I explained further down, windows applications use a message based system. While your application
  //is still receiving message that don't equal WM_QUIT it will be running on the system.
  //Here is how your create such a loop:

  //To create a high performance timer you must use two methods
  //QueryPerformanceFrequency() and QueryPerformanceCounter()

  //QueryPerformanceFrequency():
  //This method stores the (COUNTS PER SECOND) that your system timer is running at
  //Using this we can cache a conversion to seconds per count
  __int64 countsPerSec = 0;
  QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
  float secPerCount = 1.0f / countsPerSec; //Conversion to seconds per count

  //QueryPerformanceCounter():
  //This method stores and image of the current count of the system timer
  //Using this we can find the count before and after we update to determine
  //the CHANGE IN ELAPSED TIME (Otherwise known as deltaTime)
  __int64 prevTime = 0;
  QueryPerformanceCounter((LARGE_INTEGER*)&prevTime);
  __int64 curTime = 0;
  float dt = 0;

  //Create MSG struct
  MSG msg = { 0 }; //sets all members to null. (empty set)

  while (gGameStateCurr != GS_QUIT)
  {
    //Load and initialize the current level
    GameStateLoad();
    GameStateInit();
   
    //GameStateMgrUpdate();
    //if (gGameStateCurr != GS_RESTART)
    //{
   //   GameStateLoad();
   // }
   // else
   // {
   //   gGameStateNext = gGameStatePrev;
  //    gGameStateCurr = gGameStatePrev;
  //  }

    
   // GameStateInit();

    //the inner main game loop on the level
    while (gGameStateCurr != GS_LOAD)
    {
      if (gGameStateCurr == GS_QUIT) //Because this is set in a menu, in-game
        break;

      //Here is where things such as rendering, updating, etc. would go
      if (!m_Graphics->IsAppPaused()) //If application is not paused
      {
        if (!m_Graphics->IsDeviceLost())
        {
          //Temp code, so that we don't fall through tiles if/when we hit a break point
          //(this will mess physics up a lot if we get below 60fps)
          if (dt > frameTime)
            dt = frameTime;

          //If we're in a menu, check if we just tabbed back in (since we don't have a pause
          //menu to trigger restarting music from)
          if (returningFromOutOfFocus)
          {
            returningFromOutOfFocus = false;
            gGameStatePaused = false;
            ENGINE->m_Sound->ResumeAll();
          }


          //update all engine systems
          for (unsigned i = 0; i < System_Count; ++i)
          {
            if (m_SystemsArr[i])
            {
              if (!gGameStatePaused)
              {
                m_SystemsArr[i]->Update(dt);
              }

              else if (i == ST_Input || i == ST_Graphics || i == ST_Sound)
              {
                m_SystemsArr[i]->Update(dt);
              }
            }
          }
          #ifdef _DEBUG
          if (m_Input->IsKeyPressed(VK_BACK))
          {
            *GlobalSettings->SHOWING_TWEAK_BAR = !(*GlobalSettings->SHOWING_TWEAK_BAR);
          }
          #endif
          if (m_Input->IsKeyPressed(VK_ESCAPE))
          {
            if(gGameStateCurr == GS_LEVEL)
            {
              if (gGameStatePaused)
              {
                MenuMessage msg_pause_deactivate(NULL);
                msg_pause_deactivate.Type = DeactivatePause;

                GlobalSystem->MyDispatchMessage(&msg_pause_deactivate);
                gGameStatePaused = false;
                ENGINE->m_Sound->ResumeAll();
              }
              
              else
              {
                  MenuMessage msg1(NULL);
                  msg1.Type = UPDATE_POSITION;
                  msg1.newPosition = ENGINE->m_Graphics->GetMainCamera()->GetPosition();
                  GlobalSystem->MyDispatchMessage(&msg1);

                  MenuMessage msg_pause_activate(NULL);
                  msg_pause_activate.Type = ActivatePause;

                  GlobalSystem->MyDispatchMessage(&msg_pause_activate);
                  //ShowPauseMenu();
                  gGameStatePaused = true;
                  ENGINE->m_Sound->PauseAll();
              }
            }
          }

          //Do any level specific logic
          //GameStateUpdate(dt);

          //Render
          m_Graphics->Render();

          //Limit Frame Rate
          //Capture current time in counts
          QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&curTime));
          //Calculate delta time
          dt = (curTime - prevTime) * secPerCount;
          //Limit to 60 fps (comment out if you want to check actual frame times)
            
          if (m_Graphics->GetWindow()->frame_rate_controlled)
          {
            while (dt < frameTime)
            {
              QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&curTime));
              dt = (curTime - prevTime) * secPerCount;
            }
          }
          //Calculate FPS =GRAPHICS=
          #ifdef _DEBUG
            CalculateFPS(dt);
          #endif
          prevTime = curTime;
        }
      }
      else
      {
         if(gGameStateCurr == GS_LEVEL)
         {
          MenuMessage msg1(NULL);
          msg1.Type = UPDATE_POSITION;
          msg1.newPosition = ENGINE->m_Graphics->GetMainCamera()->GetPosition();
          GlobalSystem->MyDispatchMessage(&msg1);

          MenuMessage msg_pause_activate(NULL);
          msg_pause_activate.Type = ActivatePause;

          GlobalSystem->MyDispatchMessage(&msg_pause_activate);

          gGameStatePaused = true;
          ENGINE->m_Sound->PauseAll();
         }
         else if (gGameStateCurr == GS_MENU) //this is the only other option, but, legibility
         {
           returningFromOutOfFocus = true;
           gGameStatePaused = true;
           ENGINE->m_Sound->PauseAll();
         }

        reinterpret_cast<SoundManager*>(m_SystemsArr[ST_Sound])->PauseAll();
        Sleep(1); //Free up cpu for 1/1000 of a second
      }

      //Peek at a message on the stack,
      //store it in msg,
      //and remove it from the stack
      while (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
      {
        if (msg.message == WM_QUIT)
        {
          //quitting = true;
          gGameStateCurr = GS_QUIT;
        }
        else
        {
          TranslateMessage(&msg);
          DispatchMessage(&msg);
        }
      }
    }

    //Regardless of whether we're loading a new level or reloading this one,
    //we unload whatever's currently there.
    GameStateUnload();

  }

  //Now when the application finally finishes, we need to return
  //the error code given from our application
  //return static_cast<int>(msg.wParam); //The error code is stored in the wParam member of our msg struct
}

//Calculates frame per second
void Engine::CalculateFPS(float dt)
{
  //Create static counters
  static int frameCnt;
  static float elapsedTime;
  
  //Increment counters
  frameCnt++;
  elapsedTime += dt;

  if (elapsedTime >= 1.0f)
  {
    m_FPS = (float)frameCnt;

	std::stringstream ss;
	ss << window_name.c_str() << "  FPS: " << m_FPS;
  HWND temp_handle = reinterpret_cast<GraphicsManager*>(
    m_SystemsArr[ST_Graphics])->GetWindow()->GetHandle();
	SetWindowText(temp_handle, (LPCTSTR)ss.str().c_str());

    //Reset counters
    frameCnt = 0;
    elapsedTime = 0;
  }
}

System Engine::GetSystem(SystemType type)
{
  return m_SystemsArr[type];
}

void Engine::Shutdown()
{
  m_Factory->UnloadLevel();
  	// Show the mouse cursor.
	ShowCursor(true);

  //Uninitialize AntTweakBar
#ifdef _DEBUG
  if (*GlobalSettings->TWEAK_BAR_ENABLED)
    TwTerminate();
#endif

	// Fix the display settings if leaving full screen mode.
  if(IsFullScreen)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
  UnregisterClass(window_name.c_str(), m_hinstance);
	m_hinstance = NULL;

  for (unsigned i = 0; i < System_Count; ++i)
  {
    if (m_SystemsArr[i])
    {
      m_SystemsArr[i]->Shutdown();
    }
  }

  //DELETING ALL SYSTEMS FROM THE ARRAY
  //I TESTED IT AND IT CALLS THE PROPER DESTRUCTORS
  for (unsigned i = 0; i < System_Count; ++i)
  {
    if (m_SystemsArr[i])
    {
      SPIN_ENGINE_SAFE_DELETE(m_SystemsArr[i]);
    }
  }

  //Clear Memory Manager
  MemoryManager::Clear_Allocators();

  //Free Systems
  SPIN_ENGINE_SAFE_DELETE(GlobalSettings);
  SPIN_ENGINE_SAFE_DELETE(GlobalFactory);
  //SPIN_ENGINE_SAFE_DELETE(DebugSystem);
}