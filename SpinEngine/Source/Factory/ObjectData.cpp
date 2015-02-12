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
#include "Precompiled.h" //cstring
#include "Factory\ObjectData.h"
#include "ParticleEmitter.h" //For our array of particle emitters


ObjectData::ObjectData() : hasInitialized(false)
{
  //Moved everything out of constructor to an initialize function because
  //the order of calls is Making FactoryManager -> FactoryManager's initialization list ->
  //ObjectData's constructor -> FactoryManager's constructor code
}

void ObjectData::Initialize()
{
  if (hasInitialized)
    return;
  hasInitialized = true;

  JSONLoader loader;
  //Open and load the master file
  loader.LoadArchive("Assets\\master.json");
  DynamicElement *root = &loader.Root();

  //SPRITES (Assets\Animations)
  DynamicElement *sprites;
  root->GetObjectMember(&sprites, "sprites");

  //Going through each sprite in master.json, loading its .json file,
  //and adding to our list of sprites.
  unsigned spritecount = 0;
  DynamicElement *sp;
  while (sprites->GetObjectMember(&sp, NULL))
  {
    DynamicElement *pathdata;
    if (!sp->GetObjectMember(&pathdata, "path"))
    {
      MessageBox(NULL, "One of the sprites in master.json had no path given.", NULL, NULL);
      continue;
    }
    std::string spritePath;
    pathdata->GetStringVal(&spritePath);
    LoadSpriteData(spritePath, spritecount); //this will check and report if path was invalid.
  }

  //PARTICLE EMITTERS
  DynamicElement *particles;
  root->GetObjectMember(&particles, "particle emitters");
  //Go through each particle emitter, load its .json file, and add it to our list of particle emitters
  unsigned particleemittercount = 0;
  DynamicElement *pe;
  while (particles->GetObjectMember(&pe, NULL))
  {
    DynamicElement *emitterName;
    if (!pe->GetObjectMember(&emitterName, "name"))
    {
      MessageBox(NULL, "One of the particle emitters in master.json had no name given.", NULL, NULL);
      continue;
    }
    std::string particleName;
    emitterName->GetStringVal(&particleName);

    DynamicElement *pathdata;
    if (!pe->GetObjectMember(&pathdata, "path"))
    {
      MessageBox(NULL, "One of the sprites in master.json had no path given.", NULL, NULL);
      continue;
    }
    std::string particlePath;
    pathdata->GetStringVal(&particlePath);
    LoadParticleEmitterData(particlePath, particleemittercount, particleName); //this will check and report if path was invalid.
  }

  loader.UnloadArchive(); //@Does this need to be part of the class?
  //Review this when you've added non-Sprite stuff to this.
}

ObjectData::~ObjectData()
{
  for (unsigned i = 0; i < sprites_.size(); ++i)
  {
    SPIN_ENGINE_SAFE_RELEASE(sprites_[i].texture_);
  }
}

//Helper function. Given a filepath, loads a .json file into a SpriteData,
//puts in our sprite_ vector for later use.
//REQUIRES A JSON FILE THAT HAS *SPRITE* DATA
void ObjectData::LoadSpriteData(std::string &filepath, unsigned &spritecount)
{
  JSONLoader loader;
  loader.LoadArchive(filepath.c_str());

  DynamicElement *sp;
  SpriteData data;

  //Get each member of SpriteData
  if (!loader.Root().GetObjectMember(&sp, "statename"))
    MessageBox(NULL, "Error getting statename of sprite.", NULL, NULL);
  sp->GetStringVal(&(data.stateName_));
  
  if (!loader.Root().GetObjectMember(&sp, "texturepath"))
    MessageBox(NULL, "Error getting texture path of sprite.", NULL, NULL);
  sp->GetStringVal(&(data.texturePath_));
  HRESULT result = D3DXCreateTextureFromFile(m_pDevice, data.texturePath_.c_str(), &(data.texture_));
  if (FAILED(result))
  {
	  MessageBox(NULL, ("FAILED TO LOAD TEXTURE: " + data.texturePath_).c_str(), NULL, NULL);
	  //return false;
  }
  if (!loader.Root().GetObjectMember(&sp, "texturename"))
    MessageBox(NULL, "Error getting texture name of sprite.", NULL, NULL);
  sp->GetStringVal(&(data.textureName_));
  spriteIDs_[data.textureName_] = ++spritecount;
  //we don't use textureName_ anywhere else
  //(can add change Sprite later to add this if we want, otherwise
  //we keep give sprite the texture's path.)

  if (!loader.Root().GetObjectMember(&sp, "start_i"))
    MessageBox(NULL, "Error getting start_i of sprite.", NULL, NULL);
  sp->GetIntVal(&(data.start_i));

  if (!loader.Root().GetObjectMember(&sp, "end_i"))
    MessageBox(NULL, "Error getting end_i of sprite.", NULL, NULL);
  sp->GetIntVal(&(data.end_i));

  //if (!loader.Root().GetObjectMember(&sp, "width"))
  //  MessageBox(NULL, "Error getting width of sprite.", NULL, NULL);
  //sp->GetIntVal(&(data.width_));

  //if (!loader.Root().GetObjectMember(&sp, "height"))
  //  MessageBox(NULL, "Error getting height of sprite.", NULL, NULL);
  //sp->GetIntVal(&(data.height_));

  if (!loader.Root().GetObjectMember(&sp, "totalframes"))
    MessageBox(NULL, "Error getting totalframes of sprite.", NULL, NULL);
  sp->GetIntVal(&(data.totalFrames_));

  //if (!loader.Root().GetObjectMember(&sp, "framewidth"))
  //  MessageBox(NULL, "Error getting framewidth of sprite.", NULL, NULL);
  //sp->GetIntVal(&(data.frameWidth_));

  //if (!loader.Root().GetObjectMember(&sp, "frameheight"))
  //  MessageBox(NULL, "Error getting frameheight of sprite.", NULL, NULL);
  //sp->GetIntVal(&(data.frameHeight_));

  //if (!loader.Root().GetObjectMember(&sp, "rows"))
  //  MessageBox(NULL, "Error getting rows of sprite.", NULL, NULL);
  //sp->GetIntVal(&(data.rows_));

  //if (!loader.Root().GetObjectMember(&sp, "columns"))
  //  MessageBox(NULL, "Error getting columns of sprite.", NULL, NULL);
  //sp->GetIntVal(&(data.columns_));

  if (!loader.Root().GetObjectMember(&sp, "timeperframe"))
    MessageBox(NULL, "Error getting timeperframe of sprite.", NULL, NULL);
  sp->GetFloatVal(&(data.timePerFrame_));

  loader.UnloadArchive();
  //Create the actual sprite, which we'll pass to everything.
  sprites_.push_back(data);
}

void ObjectData::LoadParticleEmitterData(std::string &filepath, unsigned &emittercount, std::string &particleName)
{
  JSONLoader loader;
  loader.LoadArchive(filepath.c_str());
  DynamicElement *pe;
  ParticleEmitter *nPE;
  //I put this in a scope because this function was causing stack overflow.
  //This might be unnecessary since I made nPE go on the heap for the duration of this function.
  {
    int emitType;
    if (!loader.Root().GetObjectMember(&pe, "emit_type"))
      MessageBox(NULL, "Error getting emitter type of particle emitter.", NULL, NULL);
    pe->GetIntVal(&emitType);

    int blendType;
    if (!loader.Root().GetObjectMember(&pe, "blend_type"))
      MessageBox(NULL, "Error getting blend type of particle emitter.", NULL, NULL);
    pe->GetIntVal(&blendType);

    int particleCount;
    if (!loader.Root().GetObjectMember(&pe, "particle_count"))
      MessageBox(NULL, "Error getting particle count of particle emitter.", NULL, NULL);
    pe->GetIntVal(&particleCount);

    std::string texName;
    if (!loader.Root().GetObjectMember(&pe, "texture_name"))
      MessageBox(NULL, "Error getting texture name of particle emitter.", NULL, NULL);
    pe->GetStringVal(&texName);

    //@@@Apparently start and endColor are structs with four floats in them? What and how is this supposed to get serialized as an int?
    int startColor; //should be unsigned (this becomes a D3DXCOLOR), but won't matter if it's an int during the pipeline
    if (!loader.Root().GetObjectMember(&pe, "my_color"))
      MessageBox(NULL, "Error getting start color of particle emitter.", NULL, NULL);
    pe->GetIntVal(&startColor);
    // D3DXCOLOR stColor;
    //memcpy(&stColor, &startColor, sizeof(D3DXCOLOR));
    int endColor;
    if (!loader.Root().GetObjectMember(&pe, "my_color_end"))
      MessageBox(NULL, "Error getting end color of particle emitter.", NULL, NULL);
    pe->GetIntVal(&endColor);

    //Now we've got everything required for the constructor, buuuuut there are kinda like twenty other parameters.
    //Let's get those too!
    nPE = new ParticleEmitter(nullptr, static_cast<emitter_types>(emitType),
      static_cast<blend_types>(blendType), static_cast<unsigned>(particleCount), texName);//@@ADD COLORS
  }

  if (!loader.Root().GetObjectMember(&pe, "scale"))
    MessageBox(NULL, "Error getting scale of particle emitter.", NULL, NULL);
  pe->GetFloatVal(&nPE->scale);
  if (!loader.Root().GetObjectMember(&pe, "end_scale"))
    MessageBox(NULL, "Error getting end scale of particle emitter.", NULL, NULL);
  pe->GetFloatVal(&nPE->end_scale);

  if (!loader.Root().GetObjectMember(&pe, "angle_1"))
    MessageBox(NULL, "Error getting angle 1 of particle emitter.", NULL, NULL);
  pe->GetFloatVal(&nPE->angle_1);
  if (!loader.Root().GetObjectMember(&pe, "angle_2"))
    MessageBox(NULL, "Error getting angle 2 of particle emitter.", NULL, NULL);
  pe->GetFloatVal(&nPE->angle_2);

  if (!loader.Root().GetObjectMember(&pe, "speed_1"))
    MessageBox(NULL, "Error getting speed 1 of particle emitter.", NULL, NULL);
  pe->GetFloatVal(&nPE->speed_1);
  if (!loader.Root().GetObjectMember(&pe, "speed_2"))
    MessageBox(NULL, "Error getting speed 2 of particle emitter.", NULL, NULL);
  pe->GetFloatVal(&nPE->speed_2);

  if (!loader.Root().GetObjectMember(&pe, "acceleration_1"))
    MessageBox(NULL, "Error getting acceleration 1 of particle emitter.", NULL, NULL);
  pe->GetFloatVal(&nPE->acceleration_1);
  if (!loader.Root().GetObjectMember(&pe, "acceleration_2"))
    MessageBox(NULL, "Error getting acceleration 2 of particle emitter.", NULL, NULL);
  pe->GetFloatVal(&nPE->acceleration_2);

  if (!loader.Root().GetObjectMember(&pe, "PE_lifespan"))
    MessageBox(NULL, "Error getting lifespan of particle emitter.", NULL, NULL);
  pe->GetFloatVal(&nPE->PE_lifespan);

  if (!loader.Root().GetObjectMember(&pe, "start_delay"))
    MessageBox(NULL, "Error getting start delay of particle emitter.", NULL, NULL);
  pe->GetFloatVal(&nPE->start_delay);

  if (!loader.Root().GetObjectMember(&pe, "particle_lifetime"))
    MessageBox(NULL, "Error getting particle lifetime of particle emitter.", NULL, NULL);
  pe->GetFloatVal(&nPE->particle_lifetime);

  if (!loader.Root().GetObjectMember(&pe, "lifetime_invariant"))
    MessageBox(NULL, "Error getting lifetime invariant of particle emitter.", NULL, NULL);
  pe->GetFloatVal(&nPE->lifetime_invariant);

  if (!loader.Root().GetObjectMember(&pe, "spin"))
    MessageBox(NULL, "Error getting spin of particle emitter.", NULL, NULL);
  pe->GetFloatVal(&nPE->spin);

  if (!loader.Root().GetObjectMember(&pe, "spin_variance"))
    MessageBox(NULL, "Error getting spin variance of particle emitter.", NULL, NULL);
  pe->GetFloatVal(&nPE->spin_variance);

  if (!loader.Root().GetObjectMember(&pe, "gravity"))
    MessageBox(NULL, "Error getting gravity of particle emitter.", NULL, NULL);
  pe->GetFloatVal(&nPE->gravity);

  int blendc;
  if (!loader.Root().GetObjectMember(&pe, "blend_colors"))
    MessageBox(NULL, "Error getting blend_colors of particle emitter.", NULL, NULL);
  pe->GetIntVal(&blendc);
  if (blendc)
    nPE->blend_colors = true;

  //"x_diff_from_owner": 0
  //"y_diff_from_owner" : 0

  int sourceFactor;
  if (!loader.Root().GetObjectMember(&pe, "source_factor"))
    MessageBox(NULL, "Error getting source factor of particle emitter.", NULL, NULL);
  pe->GetIntVal(&sourceFactor);
  nPE->source_factor = static_cast<blend_factor_types>(sourceFactor);

  if (!loader.Root().GetObjectMember(&pe, "width"))
    MessageBox(NULL, "Error getting width of particle emitter.", NULL, NULL);
  pe->GetFloatVal(&nPE->width);

  if (!loader.Root().GetObjectMember(&pe, "height"))
    MessageBox(NULL, "Error getting height of particle emitter.", NULL, NULL);
  pe->GetFloatVal(&nPE->height);

  if (!loader.Root().GetObjectMember(&pe, "radius"))
    MessageBox(NULL, "Error getting radius of particle emitter.", NULL, NULL);
  pe->GetFloatVal(&nPE->radius);

  //FINALLY: reate a particle emitter and add it to our array of particle emitters
 // ParticleEmitter newPart = ParticleEmitter()
  //If there's a texture, call ChangeTexture
  particleEmitters_.push_back(*nPE);
  particleEmitterIDs_[particleName] = emittercount++;

  loader.UnloadArchive();
  delete nPE;
}


ParticleEmitter &ObjectData::GetParticleEmitter(std::string particlename)
{
  return particleEmitters_[particleEmitterIDs_[particlename]];
}

Sprite *ObjectData::CreateSprite(int id)
{
  //Creating the new sprite. (scaled down as needed in FactoryManager)
  SpriteData *sp = &sprites_[id - 1]; //we subtract 1 because a 0 in Tiled is empty space
  Sprite *newSprite = new Sprite(sp->texture_,sp->textureName_,
  static_cast<unsigned>(sp->start_i),
  static_cast<unsigned>(sp->end_i),
  static_cast<unsigned>(sp->totalFrames_),
  sp->timePerFrame_, 
  sp->stateName_);
 
  return newSprite;
}
void ObjectData::AddIDirect3DDevice9(IDirect3DDevice9 *devicePointer)
{
  m_pDevice = devicePointer;
}

int ObjectData::GetIDFromSpriteName(std::string spritename)
{
  //@@@When we're doing final optimization, just have it fetch the value directly
  //from an unordered map. This will be faster and we'll know that we'll only
  //be fetching correct values.
  //std::map<std::string, int>::iterator it = spriteIDs_.find(spritename);
 // if (it == spriteIDs_.end())
  //{
  //  MessageBox(NULL, ("GAVE INCORRECT SPRITE NAME IN DYNAMIC OBJECT CREATION: " + spritename).c_str(), NULL, NULL);
 //   return 1;
  //}
  
  //return it->second;

  return spriteIDs_[spritename];
}