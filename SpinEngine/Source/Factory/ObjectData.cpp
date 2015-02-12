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
    sprites_[i].texture_->Release();
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
  DynamicElement *setting; //A reused pointer set towards individual particle settings

  //Create a particle emitter and add it to our array of particle emitters
  //This got hacky because, apparently, ParticleEmitter is big enough to cause stack overflow. (...???)
  ParticleEmitter *pe = new ParticleEmitter();
  particleEmitters_.push_back(*pe);
  delete pe;
  pe = &particleEmitters_.back();
  //Also add the name to our map of IDs
  particleEmitterIDs_[particleName] = emittercount++;  

  //Go through and get everything on the long and intimidating list of settings:
  if (!loader.Root().GetObjectMember(&setting, "emit_type"))
    MessageBox(NULL, "Error getting emitter type of particle emitter.", NULL, NULL);
  setting->GetIntVal(reinterpret_cast<int*>(&pe->emit_type));

  if (!loader.Root().GetObjectMember(&setting, "blend_type"))
    MessageBox(NULL, "Error getting blend type of particle emitter.", NULL, NULL);
  setting->GetIntVal(reinterpret_cast<int*>(&pe->blend_type));

  if (!loader.Root().GetObjectMember(&setting, "particle_count"))
    MessageBox(NULL, "Error getting particle count of particle emitter.", NULL, NULL);
  setting->GetIntVal(reinterpret_cast<int*>(&pe->particle_count));

  if (!loader.Root().GetObjectMember(&setting, "texture_name"))
    MessageBox(NULL, "Error getting texture name of particle emitter.", NULL, NULL);
  setting->GetStringVal(&pe->texture_name);

  if (!loader.Root().GetObjectMember(&setting, "my_color_r"))
    MessageBox(NULL, "Error getting start color of particle emitter.", NULL, NULL);
  setting->GetFloatVal(&pe->my_color.r);
  if (!loader.Root().GetObjectMember(&setting, "my_color_g"))
    MessageBox(NULL, "Error getting start color of particle emitter.", NULL, NULL);
  setting->GetFloatVal(&pe->my_color.g);
  if (!loader.Root().GetObjectMember(&setting, "my_color_b"))
    MessageBox(NULL, "Error getting start color of particle emitter.", NULL, NULL);
  setting->GetFloatVal(&pe->my_color.b);
  if (!loader.Root().GetObjectMember(&setting, "my_color_a"))
    MessageBox(NULL, "Error getting start color of particle emitter.", NULL, NULL);
  setting->GetFloatVal(&pe->my_color.a);

  if (!loader.Root().GetObjectMember(&setting, "my_color_end_r"))
    MessageBox(NULL, "Error getting start color of particle emitter.", NULL, NULL);
  setting->GetFloatVal(&pe->my_color_end.r);
  if (!loader.Root().GetObjectMember(&setting, "my_color_end_g"))
    MessageBox(NULL, "Error getting start color of particle emitter.", NULL, NULL);
  setting->GetFloatVal(&pe->my_color_end.g);
  if (!loader.Root().GetObjectMember(&setting, "my_color_end_b"))
    MessageBox(NULL, "Error getting start color of particle emitter.", NULL, NULL);
  setting->GetFloatVal(&pe->my_color_end.b);
  if (!loader.Root().GetObjectMember(&setting, "my_color_end_a"))
    MessageBox(NULL, "Error getting start color of particle emitter.", NULL, NULL);
  setting->GetFloatVal(&pe->my_color_end.a);

  if (!loader.Root().GetObjectMember(&setting, "scale"))
    MessageBox(NULL, "Error getting scale of particle emitter.", NULL, NULL);
  setting->GetFloatVal(&pe->scale);
  if (!loader.Root().GetObjectMember(&setting, "end_scale"))
    MessageBox(NULL, "Error getting end scale of particle emitter.", NULL, NULL);
  setting->GetFloatVal(&pe->end_scale);

  if (!loader.Root().GetObjectMember(&setting, "angle_1"))
    MessageBox(NULL, "Error getting angle 1 of particle emitter.", NULL, NULL);
  setting->GetFloatVal(&pe->angle_1);
  if (!loader.Root().GetObjectMember(&setting, "angle_2"))
    MessageBox(NULL, "Error getting angle 2 of particle emitter.", NULL, NULL);
  setting->GetFloatVal(&pe->angle_2);

  if (!loader.Root().GetObjectMember(&setting, "speed_1"))
    MessageBox(NULL, "Error getting speed 1 of particle emitter.", NULL, NULL);
  setting->GetFloatVal(&pe->speed_1);
  if (!loader.Root().GetObjectMember(&setting, "speed_2"))
    MessageBox(NULL, "Error getting speed 2 of particle emitter.", NULL, NULL);
  setting->GetFloatVal(&pe->speed_2);

  if (!loader.Root().GetObjectMember(&setting, "acceleration_1"))
    MessageBox(NULL, "Error getting acceleration 1 of particle emitter.", NULL, NULL);
  setting->GetFloatVal(&pe->acceleration_1);
  if (!loader.Root().GetObjectMember(&setting, "acceleration_2"))
    MessageBox(NULL, "Error getting acceleration 2 of particle emitter.", NULL, NULL);
  setting->GetFloatVal(&pe->acceleration_2);

  if (!loader.Root().GetObjectMember(&setting, "PE_lifespan"))
    MessageBox(NULL, "Error getting lifespan of particle emitter.", NULL, NULL);
  setting->GetFloatVal(&pe->PE_lifespan);

  if (!loader.Root().GetObjectMember(&setting, "start_delay"))
    MessageBox(NULL, "Error getting start delay of particle emitter.", NULL, NULL);
  setting->GetFloatVal(&pe->start_delay);

  if (!loader.Root().GetObjectMember(&setting, "particle_lifetime"))
    MessageBox(NULL, "Error getting particle lifetime of particle emitter.", NULL, NULL);
  setting->GetFloatVal(&pe->particle_lifetime);

  if (!loader.Root().GetObjectMember(&setting, "lifetime_invariant"))
    MessageBox(NULL, "Error getting lifetime invariant of particle emitter.", NULL, NULL);
  setting->GetFloatVal(&pe->lifetime_invariant);

  if (!loader.Root().GetObjectMember(&setting, "spin"))
    MessageBox(NULL, "Error getting spin of particle emitter.", NULL, NULL);
  setting->GetFloatVal(&pe->spin);

  if (!loader.Root().GetObjectMember(&setting, "spin_variance"))
    MessageBox(NULL, "Error getting spin variance of particle emitter.", NULL, NULL);
  setting->GetFloatVal(&pe->spin_variance);

  if (!loader.Root().GetObjectMember(&setting, "gravity"))
    MessageBox(NULL, "Error getting gravity of particle emitter.", NULL, NULL);
  setting->GetFloatVal(&pe->gravity);

  if (!loader.Root().GetObjectMember(&setting, "blend_colors"))
    MessageBox(NULL, "Error getting blend_colors of particle emitter.", NULL, NULL);
  setting->GetBoolVal(&pe->blend_colors);

  if (!loader.Root().GetObjectMember(&setting, "x_diff_from_owner"))
    MessageBox(NULL, "Error getting blend_colors of particle emitter.", NULL, NULL);
  setting->GetFloatVal(&pe->x_diff_from_owner);
  if (!loader.Root().GetObjectMember(&setting, "y_diff_from_owner"))
    MessageBox(NULL, "Error getting blend_colors of particle emitter.", NULL, NULL);
  setting->GetFloatVal(&pe->y_diff_from_owner);

  int sourceFactor;
  if (!loader.Root().GetObjectMember(&setting, "source_factor"))
    MessageBox(NULL, "Error getting source factor of particle emitter.", NULL, NULL);
  setting->GetIntVal(&sourceFactor);
  pe->source_factor = static_cast<blend_factor_types>(sourceFactor);

  if (!loader.Root().GetObjectMember(&setting, "width"))
    MessageBox(NULL, "Error getting width of particle emitter.", NULL, NULL);
  setting->GetFloatVal(&pe->width);

  if (!loader.Root().GetObjectMember(&setting, "height"))
    MessageBox(NULL, "Error getting height of particle emitter.", NULL, NULL);
  setting->GetFloatVal(&pe->height);

  if (!loader.Root().GetObjectMember(&setting, "radius"))
    MessageBox(NULL, "Error getting radius of particle emitter.", NULL, NULL);
  setting->GetFloatVal(&pe->radius);

  //Initialize our particle emitter (this will load and create the texture, if any)
  pe->Initialize();

  loader.UnloadArchive();
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