/****************************************************************************/
/*!
\author Esteban Maldonado
\par    email: esteban.maldonado@digipen.edu
\par    Course: GAM 200
\brief

Extern function pointer for handling different cases.

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#include "Precompiled.h"
#include"GameStateManager.h"
#include "PauseMenu.h" //createPauseMenu()

//INCLUDE SPLASH SCREEN
//#include "Levels/EngineIntro/EngineIntro.h"

//INCLUDE LEVEL FILES
//#include "Levels/Level1/Level1.h"
//#include "Level2/Level2.h"
//#include "LevelMenu.h"

#include "FactoryAccess.h"

// variables to keep track the current game state and if we're quitting
std::string gCurrentLevel;
bool gGameStatePaused;
States gGameStateCurr;

void GameStateMgrInit(States state, std::string level)
{
  gGameStatePaused = false;

  // set the initial game state
  gGameStateCurr = state;
  gCurrentLevel = level;
}

void GameStateLoad()
{
  if (GlobalFactory)
  {
    //This needs to happen before LoadLevel, because menu-specific and level-specific
    //components check to see if they should be present. (currently, just ParallaxBG)
    if (gCurrentLevel == "Digipen_Splash_Screen" || gCurrentLevel == "TeamLogo_Splash_Screen" ||
      gCurrentLevel == "Level_Menu" || gCurrentLevel == "Credits")
    {
      gGameStateCurr = GS_MENU;
    }
    else
    {
      gGameStateCurr = GS_LEVEL;
    }

    GlobalFactory->LoadLevel(gCurrentLevel);

    //If we're having a pause menu, create it after all other game objects.
    if (gGameStateCurr == GS_LEVEL)
      createPauseMenu();
  }
}

void GameStateInit()
{
  if (GlobalFactory)
    GlobalFactory->Initialize();

  ENGINE->m_Input->InitializeAnyPlayback(gCurrentLevel);
  ENGINE->m_Input->StartAnyRecording(gCurrentLevel);
  ENGINE->m_Graphics->prepare_ui_elements();
}

void GameStateUnload()
{
  if (GlobalFactory)
  {
    ENGINE->m_Input->UninitializeAnyPlayback();
    ENGINE->m_Input->StopAnyRecording();
    GlobalFactory->UnloadLevel();
    ENGINE->m_physics->Shutdown();
    GlobalSystem->ClearMessageSystem();
    ENGINE->m_Graphics->SetMainCameraDefault();
  }
}
