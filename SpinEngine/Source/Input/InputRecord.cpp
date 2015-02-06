/****************************************************************************/
/*!
\author Henry Morgan
\par    email: henry.morgan@digipen.edu
\par    Course: GAM 200
\brief

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#include "Precompiled.h"
#include "InputRecord.h"
#include "JSONSaver.h"
#include "Settings.h"

#define PST (-8)

InputRecord::InputRecord() : recording_(false)
{
}

InputRecord::~InputRecord()
{
  EndRecording();
}


void InputRecord::Initialize()
{
  float *temp = NULL;
  GlobalSettings->GetFloatValue("___ INPUT RECORD/PLAYBACK SETTINGS ___", temp, false);
  GlobalSettings->GetBoolValue("Record Input", enabled_, true);
}

void InputRecord::InitializeRecording(std::string levelname)
{
  if (!*enabled_ || recording_)
    return;
  recording_ = true;

  //Open the file we'll be saving to
  //recording = fopen("recording.json", "rw");
  const time_t now = std::time(0);
  char filename[80];
  std::tm *t = std::gmtime(&now);
  //strftime(filename, 10, "%F", t);
  //sprintf(filename, "%s-%d:%02d.json", levelname, (t->tm_hour + PST) % 24, t->tm_min);
  sprintf(filename, (levelname + "Playback.json").c_str());

  if (!saver_.LoadArchive(filename))
  {
    MessageBox(NULL, "InputRecord couldn't open a file to save to.", NULL, NULL);
    recording_ = false;
    return;
  }
  else
    saver_.BeginObject();
}

void InputRecord::EndRecording()
{
  if (!recording_)
    return;
  saver_.EndObject();
  recording_ = false;
}

void InputRecord::Record(XINPUT_GAMEPAD &input)
{
  if (!recording_)
    return;
  saver_.Field(input);
}
