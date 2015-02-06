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
#include "Precompiled.h"
#include "LevelData.h"

LevelData::LevelData()
{
  //Access master.json, store a map of all level names vs file names
  JSONLoader loader;
  //Open and load the master file
  loader.LoadArchive("Assets\\master.json");
  DynamicElement *root = &loader.Root();

  //LEVELS (Assets\Levels)
  DynamicElement *levels;
  root->GetObjectMember(&levels, "levels");

  //Going through each sprite in master.json, loading its .json file,
  //and adding to our list of sprites.
  DynamicElement *sp;
  while (levels->GetObjectMember(&sp, NULL))
  {
    //Get the level's name
    DynamicElement *namedata;
    if (!sp->GetObjectMember(&namedata, "name"))
    {
      MessageBox(NULL, "One of the levels in master.json had no name given.", NULL, NULL);
      continue;
    }
    std::string levelName;
    namedata->GetStringVal(&levelName);
    
    //Get the level file's location
    DynamicElement *pathdata;
    if (!sp->GetObjectMember(&pathdata, "path"))
    {
      MessageBox(NULL, "One of the sprites in master.json had no path given.", NULL, NULL);
      continue;
    }
    std::string levelPath;
    pathdata->GetStringVal(&levelPath);

    levelPaths_[levelName] = levelPath;
  }

  loader.UnloadArchive(); //@Does this need to be part of the class?
  //Review this when you've added non-Sprite stuff to this.
}

LevelData::~LevelData()
{
  if (masterLoader_.ArchiveIsLoaded())
    masterLoader_.UnloadArchive();
}

DynamicElement* LevelData::LoadLevelData(std::string levelName)
{
  if (masterLoader_.ArchiveIsLoaded())
    masterLoader_.UnloadArchive();


  if (masterLoader_.LoadArchive(levelPaths_[levelName].c_str()))
    return &masterLoader_.Root();
  else
    return NULL; //failed loading
}

void LevelData::UnloadLevelData()
{
  if (masterLoader_.ArchiveIsLoaded())
    masterLoader_.UnloadArchive();
}

bool LevelData::LevelIsLoaded() const
{
  return masterLoader_.ArchiveIsLoaded();
}