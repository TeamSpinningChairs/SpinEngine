/****************************************************************************/
/*!
\author Henry Morgan
\par    email: henry.morgan@digipen.edu
\par    Course: GAM 200
\brief

Plays back input

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#include "Precompiled.h"
#include "InputPlayback.h"
#include "Settings.h"

#define PST (-8)

InputPlayback::InputPlayback() : inputCount_(0), playingBack_(false)
{
}

InputPlayback::~InputPlayback()
{
}


void InputPlayback::Initialize()
{
  float *temp = NULL;
  GlobalSettings->GetFloatValue("___ INPUT RECORD/PLAYBACK SETTINGS ___", temp, false);
  GlobalSettings->GetBoolValue("Playback Input", enabled_, true);
}

//Open the file we'll be playing from
void InputPlayback::InitializePlayback(std::string filename)
{
  if (!*enabled_ || playingBack_)
    return;
  playingBack_ = true;

  inputCount_ = 0;

  if (!loader_.LoadArchive(filename.c_str()))
  {
    MessageBox(NULL, ("InputPlayback couldn't open " + filename).c_str(), NULL, NULL);
    playingBack_ = false;
    return;
  }

  //Copy everything from archive into a vector of XINPUT_GAMEPADs
  int i = -1;
  do
  {
    ++i;
    inputs_.push_back(XINPUT_GAMEPAD());
  } while (loader_.Field(inputs_[i]));

  loader_.UnloadArchive();
}

void InputPlayback::GetNextInput(XINPUT_GAMEPAD &input)
{
  if (!playingBack_)
    return;

  //Take from inputs_ until we've played the entire recording,
  //then use the gamepad as normal.
  if (inputCount_ < inputs_.size())
    input = inputs_[inputCount_++];
  else
    input = XINPUT_GAMEPAD();
}

void InputPlayback::UninitializePlayback()
{
  if (!playingBack_)
    return;
  inputs_.clear();
  playingBack_ = false;
}
