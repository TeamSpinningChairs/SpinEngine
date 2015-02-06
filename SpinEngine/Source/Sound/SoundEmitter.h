/****************************************************************************/
/*!
\author Judy Cheng
\par    email: j.cheng\@digipen.edu (Optional)
\par    Course: GAM 200
\date   xx/xx/xx (Optional)
\brief

A sound emitter component header


Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#pragma once
#include "SoundManager.h"

class SoundEmitter;

class SoundEmitter : public IComponent
{
public:
  SoundEmitter() {};
  SoundEmitter(IEntity *Owner);
  ~SoundEmitter();

  bool Initialize() override;
  void Update(float dt) override;
  void Release() override;

  void Play(const std::string &name, bool loop);
  //Play(const std::string& name, bool loop, SoundInstance* instance);
  void Stop();
  void Pause();
  void Resume();
  void SetVolume(float volume);
  void SetPitch(float pitch);

  //Set in editor (optional, see segment in FactoryManager)
  bool playSoundOnStart;
  std::string startSoundName;

private:
  SoundManager *m_manager;
  SoundInstance instance;
  float volume_;
  float pitch_;
};