/****************************************************************************/
/*!
\author Henry Morgan
\par    email: henry.morgan@digipen.edu
\par    Course: GAM 200
\brief

Contains the classes that store game object information, specifically for
tile maps

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
//A class that loads and stores data for each prefab
#pragma once
#include "Factory\Serializer\JSONLoader.h"
#include "Graphics\Sprite.h"
#include "Precompiled.h"

class ParticleEmitter;

struct SpriteData
{
  std::string stateName_;
  std::string texturePath_;
  std::string textureName_;
  IDirect3DTexture9 *texture_;
  //int width_;
  //int height_;
  int start_i, end_i; //<- start and end indexes on the texture 0 based
  int totalFrames_;
  //int frameWidth_;
  //int frameHeight_;
  //int rows_;
  //int columns_;
  float timePerFrame_;
};

class ObjectData
{
public:
  ObjectData(); 
  ~ObjectData();
  void Initialize();//Loads all sprite data, etc... from master file
  //Gets appropriate sprite from sprites_, uses that data to create
  //a new sprite for something in FactoryManager.
  //(id is from level editor- the order of sprites in the level editor's
  //spritesheet and in master.json must be the same, which is why we
  //append to both whenever there's a new sprite, and why you can delete
  //from anywhere in the list as long as you remove that section 
  Sprite *CreateSprite(int id);

  //So that ObjectData can give this pointer to the Sprites it creates.
  void AddIDirect3DDevice9(IDirect3DDevice9 *m_pDevice);

  int GetIDFromSpriteName(std::string spritename);

  ParticleEmitter &GetParticleEmitter(std::string particlename);

private:
  IDirect3DDevice9 *m_pDevice;

  //The data we use to create a Sprite when FactoryManager asks for one.
  std::vector<SpriteData> sprites_;
  //Map of sprite names to IDs
  std::map < std::string, int > spriteIDs_;

  std::vector<ParticleEmitter> particleEmitters_;
  std::map<std::string, int> particleEmitterIDs_; //Enter a name, get the index within particleEmitters_

  bool hasInitialized;
  //Helper functions:
  void LoadSpriteData(std::string &filepath, unsigned &spritecount);
  void LoadParticleEmitterData(std::string &filepath, unsigned &spritecount, std::string &particleName);
};