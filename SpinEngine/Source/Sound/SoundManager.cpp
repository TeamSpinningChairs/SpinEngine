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
#include "SoundManager.h"
#include "soundList.h"
#include "SoundInstance.h"

#pragma comment(lib, "fmodL_vc.lib")
#pragma comment(lib, "fmodstudioL_vc.lib")

SoundManager *sound = nullptr;

SoundManager::SoundManager()
: ISystem("Sound", ST_Sound), masterChannel(nullptr), musicChannel(nullptr), sfxChannel(nullptr),
menuSFXChannel(nullptr), eventInstance(nullptr), masterVolume(1.0f), masterPitch(1.0f)
{
}

SoundManager::~SoundManager() {}

bool SoundManager::Initialize()
{
  CreateFMODSystem();
  LoadAllSoundsBank();
  CreateMasterChannel();
  GetAllEvents();
  CreateChannelGroups();
  return true;
}
void SoundManager::Update(float dt)
{
  ErrorCheck(soundSystem->update());
}
void SoundManager::Shutdown()
{
  UnloadAllSoundsBank();

  lowlevelsound_System->close();
  soundSystem->release();
}

void SoundManager::ErrorCheck(FMOD_RESULT result)
{
  if (result != FMOD_OK)
  {
    char buffer[200];
    MessageBox(NULL, "ERROR: Could not create Sound System",
               strcpy(buffer, strcat("Error: ", system_name)), NULL);
  }
}

void SoundManager::GetDrivers()
{
  int numDrivers;
  ErrorCheck(lowlevelsound_System->getNumDrivers(&numDrivers));

  if (numDrivers == 0)
  {
    ErrorCheck(lowlevelsound_System->setOutput(FMOD_OUTPUTTYPE_NOSOUND));
  }
}

void SoundManager::CreateFMODSystem()
{
  // create studio system
  ErrorCheck(FMOD::Studio::System::create(&soundSystem));

  // initialize fmod studio system
  ErrorCheck(soundSystem->initialize(256, FMOD_STUDIO_INIT_NORMAL,
    FMOD_INIT_NORMAL, 0));
  
  // get low level pointer
  ErrorCheck(soundSystem->getLowLevelSystem(&lowlevelsound_System));

  // get version
  unsigned int version;
  ErrorCheck(lowlevelsound_System->getVersion(&version));

  // check version
  if (version < FMOD_VERSION)
  {
    printf("Error! You are using an old version of FMOD %08x. This program requires %08x\n", version, FMOD_VERSION);
    return;
  }
  // check for sound drivers
  GetDrivers();

}

void SoundManager::LoadSoundBank(const char *bankname)
{
  char full_bankname[256];
  sprintf_s(full_bankname, sizeof(full_bankname), "%s%s", "Assets/Sound Banks/", bankname);

  FMOD::Studio::Bank* newBank = nullptr;
  ErrorCheck(soundSystem->loadBankFile(full_bankname, FMOD_STUDIO_LOAD_BANK_NORMAL, &newBank));
}

void SoundManager::LoadAllSoundsBank()
{
  LoadSoundBank("Master Bank.bank");
  LoadSoundBank("Ambience.bank");
  LoadSoundBank("Master Bank.strings.bank");
  LoadSoundBank("Music.bank");
  LoadSoundBank("Presentation.bank");
  // add sfx
  // add menu sfx
}

void SoundManager::UnloadAllSoundsBank()
{
  ErrorCheck(soundSystem->unloadAll());
}

void SoundManager::CreateChannelGroups()
{
  ErrorCheck(lowlevelsound_System->createChannelGroup("Music", &musicChannel));
  ErrorCheck(lowlevelsound_System->createChannelGroup("SFX", &sfxChannel));
  ErrorCheck(lowlevelsound_System->createChannelGroup("MenuSFX", &menuSFXChannel));
}

void SoundManager::CreateMasterChannel()
{
  bool active;

  ErrorCheck(lowlevelsound_System->createDSPByType(FMOD_DSP_TYPE_FFT, &lowPassEffect));
  ErrorCheck(lowPassEffect->getActive(&active));
  ErrorCheck(lowlevelsound_System->getMasterChannelGroup(&masterChannel));

  if (!active)
  {
    ErrorCheck(masterChannel->addDSP(0, lowPassEffect));
    ErrorCheck(lowPassEffect->setActive(true));
  }
}

void SoundManager::LoadtoSoundMap(std::string name, FMOD::Studio::System *system)
{
  Sound* newSound = new Sound(system, name);

  // push into map
  soundMap[name] = newSound;
}

void SoundManager::GetAllEvents()
{
  for (int i = 0; i < AUDIO_COUNT; ++i)
  {
    LoadtoSoundMap(soundNames[i], soundSystem);
  }
}


void SoundManager::PlayEvent(const std::string &name, SoundInstance *instance, bool loop)
{
  if (currentSound.compare(name) != 0)
  {
    soundMap[name]->Play(instance, loop);

    if (loop)
      currentSound = name;
    else
      currentSound = "";      
  }
}

void SoundManager::StopEvent(SoundInstance *instance)
{
  if (instance->active)
  {
    ErrorCheck(instance->eventInstance->stop(FMOD_STUDIO_STOP_IMMEDIATE));
    ErrorCheck(instance->eventInstance->release());
  }
}

void SoundManager::PauseEvent(SoundInstance *instance)
{
  instance->eventInstance->setPaused(true);
}

void SoundManager::ResumeEvent(SoundInstance *instance)
{
  instance->eventInstance->setPaused(false);
}

void SoundManager::SetVolume(float volume, SoundInstance *instance)
{
  instance->volume_ = volume;
  instance->eventInstance->setVolume(volume);
}

void SoundManager::SetPitch(float pitch, SoundInstance *instance)
{
  instance->pitch_ = pitch;
  instance->eventInstance->setPitch(pitch);
}

float SoundManager::GetVolume(SoundInstance *instance)
{
  return instance->volume_;
}

float SoundManager::GetPitch(SoundInstance *instance)
{
  return instance->pitch_;
}

void SoundManager::StopAll()
{
  masterChannel->stop();
  currentSound = "";
}

void SoundManager::PauseAll()
{
  masterChannel->setPaused(true);
}

void SoundManager::ResumeAll()
{
  masterChannel->setPaused(false);
}

void SoundManager::SetMasterVolume(float volume)
{
  masterVolume = volume;
  masterChannel->setVolume(masterVolume);
}

float SoundManager::GetMasterVolume()
{
  return masterVolume;
}

void SoundManager::SetMasterPitch(float pitch)
{
  masterPitch = pitch;
  masterChannel->setPitch(masterPitch);
}

float SoundManager::GetMasterPitch()
{
  return masterPitch;
}

void SoundManager::StopMusic()
{
  musicChannel->stop();
  currentSound = "";
}
void SoundManager::PauseMusic()
{
  musicChannel->setPaused(true);
}
void SoundManager::ResumeMusic()
{
  musicChannel->setPaused(false);
}
void SoundManager::SetMusicVolume(float volume)
{
  musicChannel->setVolume(volume);
}
void SoundManager::SetMusicPitch(float pitch)
{
  musicChannel->setPitch(pitch);
}
float SoundManager::GetMusicVolume()
{
  float volume;
  musicChannel->getVolume(&volume);
  return volume;
}
float SoundManager::GetMusicPitch()
{
  float pitch;
  musicChannel->getPitch(&pitch);
  return pitch;
}