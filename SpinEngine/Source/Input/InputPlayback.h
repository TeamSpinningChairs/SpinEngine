/****************************************************************************/
/*!
\author Henry Morgan
\par    email: henry.morgan@digipen.edu
\par    Course: GAM 200
\brief

Input PlayBack (see .cpp for more info)

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#pragma once
#include "Precompiled.h"
#include "JSONLoader.h"

class InputPlayback
{
public:
  InputPlayback();
  ~InputPlayback();
  void Initialize();

  //Open the file we'll be playing from
  void InitializePlayback(std::string filename);
  //Load next xinput from the stored archive
  void GetNextInput(XINPUT_GAMEPAD &input);
  void UninitializePlayback();

  bool *enabled_;

private:
  JSONLoader loader_;

  std::vector<XINPUT_GAMEPAD> inputs_;
  unsigned inputCount_;

  bool playingBack_;
};
