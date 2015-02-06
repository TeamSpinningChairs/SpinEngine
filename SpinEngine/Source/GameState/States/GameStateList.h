/****************************************************************************/
/*!
\author Esteban Maldonado & Steven Gallwas
\par    email: esteban.maldonado@digipen.edu
\par    Course: GAM 200
\brief

Contains all enums for the specific game cases.

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#pragma once

enum States
{
  //Menus (or anything NOT a level, I.E. splash screens / credits)
  GS_MENU = 0,
  
  //Levels (or anything else where we should be able to access the pause menu)
  GS_LEVEL,

  // special game state. Do not change
  GS_LOAD, //Loading new level (or reloading current level)
  GS_QUIT, //Quitting the game
  GS_NUM
};