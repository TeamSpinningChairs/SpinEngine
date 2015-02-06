/****************************************************************************/
/*!
\author Judy Cheng
\par    email: j.cheng\@digipen.edu (Optional)
\par    Course: GAM 200
\date   xx/xx/xx (Optional)
\brief

A sound emitter component.


Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#include "Precompiled.h"
#include "SoundEmitter.h"
#include "Engine.h"

SoundEmitter::SoundEmitter(IEntity *Owner) : 
IComponent(Component_Type::CT_SoundEmitter, Owner), playSoundOnStart(false)
{
}

void SoundEmitter::Update(float dt)
{
}

void SoundEmitter::Release()
{
}

bool SoundEmitter::Initialize()
{
  //Get a pointer to the main sound system
  m_manager = ENGINE->m_Sound;

  if (playSoundOnStart)
  {
    Play(startSoundName, true);
  }
  return true;
}

SoundEmitter::~SoundEmitter()
{
  if (playSoundOnStart)
  {
    Stop();
  }    
}

void SoundEmitter::Play(const std::string &name, bool loop)
{
  m_manager->PlayEvent(name, &instance, loop);
}

void SoundEmitter::Stop()
{
  m_manager->StopEvent(&instance);
}
void SoundEmitter::Pause()
{
  m_manager->PauseEvent(&instance);
}

void SoundEmitter::Resume()
{
  m_manager->ResumeEvent(&instance);
}
void SoundEmitter::SetVolume(float volume)
{
  m_manager->SetVolume(volume, &instance);
}
void SoundEmitter::SetPitch(float pitch)
{
  m_manager->SetPitch(pitch, &instance);
}
