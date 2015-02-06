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
#include "SoundInstance.h"

struct SoundInstance;

class SoundManager : public ISystem
{
public:
  SoundManager();
  ~SoundManager();

  bool Initialize() override;
  void Update(float dt) override;
  void Shutdown() override;

  // INDIVIDUALS
  void PlayEvent(const std::string &name, SoundInstance *instance, bool loop);
  void StopEvent(SoundInstance *instance);
  void PauseEvent(SoundInstance *instance);
  void ResumeEvent(SoundInstance *instance);
  void SetVolume(float volume, SoundInstance *instance);
  void SetPitch(float pitch, SoundInstance *instance);
  float GetVolume(SoundInstance *instance);
  float GetPitch(SoundInstance *instance);

  // MASTER CHANNEL
  void StopAll();
  void PauseAll();
  void ResumeAll();
  void SetMasterVolume(float volume);
  void SetMasterPitch(float pitch);
  float GetMasterVolume();
  float GetMasterPitch();

  //////////////////////////////////////////////////////////////////////////
  // Rest of these channels are untested!

  // MUSIC CHANNEL
  void StopMusic();
  void PauseMusic();
  void ResumeMusic();
  void SetMusicVolume(float volume);
  void SetMusicPitch(float pitch);
  float GetMusicVolume();
  float GetMusicPitch();

  // SFX CHANNEL
  void StopSFX();
  void PauseSFX();
  void RESUMESFX();
  void SetSFXVolume(float volume);
  void SetSFXPitch(float pitch);
  float GetSFXVolume();
  float GetSFXPitch();

  // MENUSFX CHANNEL
  void StopMenuSFX();
  void PauseMenuSFX();
  void RESUMEMenuSFX();
  void SetMenuSFXVolume(float volume);
  void SetMenuSFXPitch(float pitch);
  float GetMenuSFXVolume();
  float GetMenuSFXPitch();

private:
  void CreateFMODSystem();
  void GetAllEvents();
  void CreateChannelGroups();
  void CreateMasterChannel();
  void ErrorCheck(FMOD_RESULT result);
  void GetDrivers(); //private

  void LoadtoSoundMap(std::string name, FMOD::Studio::System *system);
  void LoadAllSoundsBank(); //private
  void LoadSoundBank(const char *bankname); //private
  void UnloadAllSoundsBank(); //private

  FMOD::Studio::System* soundSystem;
  FMOD::System* lowlevelsound_System;
  FMOD::DSP* lowPassEffect;

  // containers
  std::unordered_map<std::string, Sound*> soundMap;

  // channel groups
  FMOD::ChannelGroup* masterChannel;
  FMOD::ChannelGroup* musicChannel;
  FMOD::ChannelGroup* sfxChannel;
  FMOD::ChannelGroup* menuSFXChannel;

  // instances
  FMOD::Studio::EventInstance* eventInstance;
  std::string currentSound;

  float masterVolume; // 0.0 ~ 1.0
  float masterPitch;  // 0.0 ~ 1.0

};

extern SoundManager *sound;