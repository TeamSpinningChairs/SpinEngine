/****************************************************************************/
/*!
\author Judy Cheng
\par    email: j.cheng\@digipen.edu (Optional)
\par    Course: GAM 200
\date   xx/xx/xx (Optional)
\brief

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#include "Precompiled.h"
#include "SoundInstance.h"

SoundInstance::SoundInstance() : volume_(1.0f), pitch_(1.0f), channel_(0), active(false)
{}

SoundInstance::SoundInstance(float volume, float pitch) : volume_(volume), pitch_(pitch), channel_(0)
{}

//////////////////////////////////////////////////////////////////////////
Sound:: Sound(FMOD::Studio::System *system, std::string name)
{
  // get event description
  std::string fullname = "event:/" + name;
  system->getEvent(fullname.c_str(), &eventDescription);
  soundname = fullname;

  // set loop to false
  loop = false;
}

void Sound::ErrorCheck(FMOD_RESULT result)
{
  if (result != FMOD_OK)
  {
    MessageBox(NULL, "ERROR: Could not create Sound System", NULL, NULL);
  }
}

std::string Sound::GetName(void)
{
  return soundname;
}

void Sound::PlayOnce(SoundInstance *instance)
{
  if (!instance)
  {
    SoundInstance tempInst;
    ErrorCheck(eventDescription->createInstance(&(tempInst.eventInstance)));

    tempInst.eventInstance->setPitch(tempInst.pitch_);
    tempInst.eventInstance->setVolume(tempInst.volume_);

    ErrorCheck(tempInst.eventInstance->start());
    ErrorCheck(tempInst.eventInstance->release());
  }
  else
  {
    ErrorCheck(eventDescription->createInstance(&instance->eventInstance));
    instance->eventInstance->setPitch(instance->pitch_);
    instance->eventInstance->setVolume(instance->volume_);

    instance->active = true;

    ErrorCheck(instance->eventInstance->start());
    //ErrorCheck(instance->eventInstance->release());
  }
}

void Sound::PlayLoop(SoundInstance *instance)
{
  ErrorCheck(eventDescription->createInstance(&instance->eventInstance));
  instance->eventInstance->setPitch(instance->pitch_);
  instance->eventInstance->setVolume(instance->volume_);
  ErrorCheck(instance->eventInstance->start());
  instance->active = true;
}

void Sound::Play(SoundInstance *instance, bool loop)
{
  // test
  if (loop)
  {
    PlayLoop(instance);
  }
  else
  {
    PlayOnce(instance);
  }
}
