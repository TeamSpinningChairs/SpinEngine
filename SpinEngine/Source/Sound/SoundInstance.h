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
#pragma once
#include "fmod.hpp"
#include "fmod_studio.hpp"


struct SoundInstance
{
  SoundInstance();
  SoundInstance(float volume, float pitch);
  
  float volume_;
  float pitch_;
  int channel_;

  // events from banks
  FMOD::Studio::EventInstance *eventInstance;
  bool active;
};

class Sound
{
public:
  Sound() {}
  Sound(FMOD::Studio::System *system, std::string name);
  ~Sound() {};

  void ErrorCheck(FMOD_RESULT result);
  void Play(SoundInstance *instance, bool loop);
  std::string GetName(void);

private:
  Sound(const Sound&);
  Sound operator=(const Sound&);

  void PlayOnce(SoundInstance *instance);
  void PlayLoop(SoundInstance *instance);
  
  FMOD::Studio::EventDescription *eventDescription;
  std::string soundname;
  bool loop;
};