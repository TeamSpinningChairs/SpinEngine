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
#include "Precompiled.h"
#include "Settings.h"

Settings *GlobalSettings = nullptr;

#define MAX_POSSIBLE_SETTINGS 50

Settings::Settings() : settingsCount_(0)
{
  loader_.LoadArchive("Assets\\settings.json");
  root_ = &loader_.Root();

  //We specify this instead of using push_back so that it doesn't grow itself / so that its elements don't change location in memory
  settings_.resize(MAX_POSSIBLE_SETTINGS); 

  float *temp = NULL;
  GetFloatValue("___ TWEAK BAR SETTINGS ___", temp, false);
  GetBoolValue("Tweak Bar Enabled", TWEAK_BAR_ENABLED, false);
  GetBoolValue("Showing Tweak Bar", SHOWING_TWEAK_BAR, false);
  //It's set to false when we save it, since it'd always be true otherwise. So for now, we force it to be true
  *TWEAK_BAR_ENABLED = true; 
  *SHOWING_TWEAK_BAR = false;

  //Initialize tweak bar
#ifdef _DEBUG
  myTweakBar_ = TwNewBar("Custom Settings");
  TwAddButton(myTweakBar_, "Save", this->SaveSettingsToFile, reinterpret_cast<void *>(this), NULL);
#endif
}

Settings::~Settings()
{
  loader_.UnloadArchive();
}

bool Settings::GetIntValue(std::string name, int *&setting, bool addToBar)
{
  //Check Settings to see if we've gotten the value in the previous level.
  //for (Setting s : settings_) //DON'T DO THIS. s isn't made reference to each entry during each iteration- it's apparently by value?
  //When you did it this way, every setting pointer (playerRunSpeed, etc...) in a given class would point to the same address, because they would all be
  //pointed to &s.val.intVal- that address would be the same every time, then once we were done loading settings, that memory on the stack would
  //be overwritten and we'd have junk. So, we just iterate through the vector normally, since it's treated like an array in every other respect.
  //Should I just make it an array?
  for (int i = 0; i < settingsCount_; ++i)
  {
    if (settings_[i].name == name)
    {
      setting = &settings_[i].val.intVal;
      return true;
    }
  }

  //Get the value (or return false if not successful)
  DynamicElement *temp;
  if (!root_->GetObjectMember(&temp, name.c_str()))
  {
    MessageBox(NULL, ("FAILED TO GET SETTING: " + name).c_str(), NULL, NULL);
    return false;
  }
  Setting set;
  if (!temp->GetIntVal(&set.val.intVal))
  {
    MessageBox(NULL, ("TRIED TO LOAD NON-INT SETTING AS INT: " + name).c_str(), NULL, NULL);
    return false;
  }
  set.name = name;
  set.type = Setting::Type::TYPE_INT;

  //We've got the value, now add it to our settings.
  if (settingsCount_ < MAX_POSSIBLE_SETTINGS)
  {
    settings_[settingsCount_] = set;
    setting = &settings_[settingsCount_].val.intVal;
    ++settingsCount_;
  }
  else
  {
    MessageBox(NULL, "TOO MANY SETTINGS IN SETTINGS.JSON, CURRENT MAX IS " + MAX_POSSIBLE_SETTINGS, NULL, NULL);
  }

  if (!addToBar)
    return true;

  //And add the value to our tweak bar (if it's enabled)
#ifdef _DEBUG
  if (*TWEAK_BAR_ENABLED)
    TwAddVarRW(myTweakBar_, name.c_str(), TwType::TW_TYPE_INT32, setting, NULL);
#endif
  return true;
}

//If setting is a NULL pointer, it will assume we're getting one of the labels
//(a junk field solely to divide the stuff in settings.json by category)
bool Settings::GetFloatValue(std::string name, float *&setting, bool addToBar)
{
  //Check Settings to see if we've gotten the value in the previous level.
  for (int i = 0; i < settingsCount_; ++i)
  {
    if (settings_[i].name == name)
    {
      setting = &settings_[i].val.floatVal;
      return true;
    }
  }

  //Get the value (or return false if not successful)
  DynamicElement *temp;
  if (!root_->GetObjectMember(&temp, name.c_str()))
  {
    MessageBox(NULL, ("FAILED TO GET SETTING: " + name).c_str(), NULL, NULL);
    return false;
  }
  Setting set;
  if (!temp->GetFloatVal(&set.val.floatVal))
  {
    MessageBox(NULL, ("TRIED TO LOAD NON-FLOAT SETTING AS FLOAT: " + name).c_str(), NULL, NULL);
    return false;
  }
  
  set.name = name;
  set.type = Setting::Type::TYPE_FLOAT;

  //We've got the value, now add it to our settings.
  settings_[settingsCount_] = set;
  setting = &settings_[settingsCount_].val.floatVal;
  ++settingsCount_;
  
  if (!addToBar)
    return true;

#ifdef _DEBUG
  if (*TWEAK_BAR_ENABLED)
    TwAddVarRW(myTweakBar_, name.c_str(), TwType::TW_TYPE_FLOAT, setting, "step = .001");
#endif
  return true;
}

bool Settings::GetBoolValue(std::string name, bool *&setting, bool addToBar)
{
  //Check Settings to see if we've gotten the value in the previous level.
  for (int i = 0; i < settingsCount_; ++i)
  {
    if (settings_[i].name == name)
    {
      setting = &settings_[i].val.boolVal;
      return true;
    }
  }

  //Get the value (or return false if not successful)
  DynamicElement *temp;
  if (!root_->GetObjectMember(&temp, name.c_str()))
  {
    MessageBox(NULL, ("FAILED TO GET SETTING: " + name).c_str(), NULL, NULL);
    return false;
  }
  Setting set;
  if (!temp->GetBoolVal(&set.val.boolVal))
  {
    MessageBox(NULL, ("TRIED TO LOAD NON-INT SETTING AS BOOL: " + name).c_str(), NULL, NULL);
    return false;
  }
  
  set.name = name;
  set.type = Setting::Type::TYPE_BOOL;

  //We've got the value, now add it to our settings.
  settings_[settingsCount_] = set;
  setting = &settings_[settingsCount_].val.boolVal;
  ++settingsCount_;
  
  if (!addToBar)
    return true;

  //And add the value to our tweak bar (if it's enabled)
#ifdef _DEBUG
  if (*TWEAK_BAR_ENABLED)
    TwAddVarRW(myTweakBar_, name.c_str(), TwType::TW_TYPE_BOOLCPP, setting, NULL);
#endif
  return true;
}

//Gameplay recording: Grab each segment of recorded input, play it back. When you near the end of one, grab next chunk in a thread.
//Alternatively, if it's not too big, grab everything for every frame.

//Activated when we hit the Save button in the tweak bar
//Go through our list of settings, get their values from their pointers, and save them with their names and types.
//However, this will result in us not saving any values that were loaded from settings.json but never accessed.
//Oh! Maybe access the rest of the values in root's map (I.E. any that don't have a name in settings)
void TW_CALL Settings::SaveSettingsToFile(void *clientData)
{
  Settings *my = reinterpret_cast<Settings*>(clientData);

  JSONSaver saver;
  saver.LoadArchive("Assets\\settings.json");
  saver.BeginObject(nullptr);
  for (int i = 0; static_cast<size_t>(i) < my->settings_.size(); ++i)
  {
    if (my->settings_[i].name == "")
      continue;

    switch (my->settings_[i].type)
    {
    case Setting::TYPE_INT:
      saver.Field(my->settings_[i].name.c_str(), my->settings_[i].val.intVal);
      break;
    case Setting::TYPE_FLOAT:
      if (my->settings_[i].name.c_str()[0] == '_')
        saver.FieldEmptyLine();
      saver.Field(my->settings_[i].name.c_str(), my->settings_[i].val.floatVal);
      break;
    case Setting::TYPE_BOOL:
      if (my->settings_[i].name == "Showing Tweak Bar")
      {
        //(If we didn't force this to be false, it would always be true,
        //since we can only save while the tweak bar is shown)
        bool temp = false; // function needs a reference, can't just enter "false"
        saver.Field("Showing Tweak Bar", temp);
      }
      else
        saver.Field(my->settings_[i].name.c_str(), my->settings_[i].val.boolVal);
      break;
    }
  }
  saver.EndObject();
  saver.UnloadArchive();
  //If we've saved fewer files 
}