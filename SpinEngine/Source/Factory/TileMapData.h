//Henry Morgan
//Only used to contain the declaration for TileMapData, since it's used by several classes
//and I didn't want to unnecessarily include, say, the entirety of FactoryManager.h just for this.
#pragma once
#include "Precompiled.h"


//Tilemap data
struct TileMapData
{
  int width;
  int height;
  int numTiles;
  std::vector<int> *tiles;
  std::vector<int> origTiles;
  std::vector<int> targetTiles;
  std::vector<int> factoryIndeces;

  TileMapData();
};