/****************************************************************************/
/*!
\author Henry Morgan
\par    email: henry.morgan@digipen.edu
\par    Course: GAM 200
\brief

Stores the information for all levels (paths, tile count, etc.)

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#pragma once
#include "Precompiled.h"
#include "ObjectData.h"

class LevelData
{
public:
  LevelData(); //Gets map of level names to filepaths
  ~LevelData();

  DynamicElement* LoadLevelData(std::string levelName);
  void UnloadLevelData();

  bool LevelIsLoaded() const;

private:
  JSONLoader masterLoader_; //Holds loaded level data until we unload a level

  std::map<std::string, std::string> levelPaths_;
};