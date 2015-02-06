/****************************************************************************/
/*!
\author Henry Morgan
\par    email: henry.morgan@digipen.edu
\par    Course: GAM 200
\brief

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#pragma once
#include "Precompiled.h"
#include "JSONSaver.h"

//The definition for a class that stores playback data- one XINPUT_GAMEPAD struct for each frame at 60fps.
//In terms of actual data throughput, it's a little bit over 42 megabytes per minute,
//but we save the data in JSON as chars, and relatively few of them, so it's not that big.

class InputRecord
{
public:
  InputRecord();
  ~InputRecord();

  void Initialize();

  void InitializeRecording(std::string levelname);
  void Record(XINPUT_GAMEPAD &input);
  void EndRecording();

  bool *enabled_;
  

private:
  JSONSaver saver_;
  bool recording_;
};


//The struct, w/ sizeof values I got
//typedef struct _XINPUT_GAMEPAD {
//  WORD  wButtons; //2 BYTES (unsigned short)
//  BYTE  bLeftTrigger; //1 BYTE (unsigned char)
//  BYTE  bRightTrigger;
//  SHORT sThumbLX; //2 BYTES
//  SHORT sThumbLY;
//  SHORT sThumbRX;
//  SHORT sThumbRY;
//} XINPUT_GAMEPAD, *PXINPUT_GAMEPAD;
