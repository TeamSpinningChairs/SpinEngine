/****************************************************************************/
/*!
\author Henry Morgan
\par    email: henry.morgan@digipen.edu
\par    Course: GAM 200
\brief

Stores the global settings loaded from the .json file, adds to tweak bar
if necessary / enabled.

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#pragma once
#include "Factory\Serializer\JSONLoader.h"
#include "Factory\Serializer\JSONSaver.h"

class Settings
{
public:
  Settings();
  ~Settings();

  //Give them a member's value as requested
  //They provide a callback function for whenever it's changed? Maybe?

  //Hypothetical scenario:
  //1. Judy asks for gravity from Settings
  //2. Settings returns the value of Gravity
  //3. Judy sets gravity, adds that variable to tweak bar.
  //4. Ian changes gravity value in tweak bar. 

  //We get the value for that name. If it's the correct type, we set it,
  //add setting to the tweak bar, and add that name, type, and pointer to our list of stuff-to-save.

  //Any values not accessed through one of these functions will be erased when we save settings.
  bool GetIntValue(std::string name, int *&setting, bool addToBar = true);
  bool GetFloatValue(std::string name, float *&setting, bool addToBar = true);
  bool GetBoolValue(std::string name, bool *&setting, bool addToBar = true);

  
  bool *SHOWING_TWEAK_BAR;
  bool *TWEAK_BAR_ENABLED;

private:
  TwBar *myTweakBar_;
  JSONLoader loader_;
  DynamicElement *root_;

  int settingsCount_;
  

  static void TW_CALL SaveSettingsToFile(void *clientData);

  struct Setting
  {
    enum Type
    {
      //TYPE_NULL,
      //TYPE_OBJECT,
      TYPE_FLOAT,
      TYPE_BOOL,
      TYPE_STRING,
      TYPE_INT,
      //TYPE_VECTOR,
      //TYPE_COUNT
    };
    std::string name;
    Type type;
    union value
    {
      int intVal;
      float floatVal;
      bool boolVal;
    } val;
  };

  std::vector<Setting> settings_;
};

extern Settings *GlobalSettings;