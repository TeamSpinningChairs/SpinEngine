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
#pragma once
#include "GameStateList.h"

class string;
// ---------------------------------------------------------------------------
// externs

extern std::string gCurrentLevel;
extern bool gGameStatePaused;
extern States gGameStateCurr;

// ---------------------------------------------------------------------------
// Function prototypes

// call this at the beginning and AFTER all game states are added to the manager
void GameStateMgrInit(States state, std::string level);

// update is used to set the function pointers
void GameStateLoad();
void GameStateInit();
void GameStateUnload();
