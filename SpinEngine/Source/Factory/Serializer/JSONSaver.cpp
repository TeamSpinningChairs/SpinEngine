/****************************************************************************/
/*!
\author Henry Morgan
\par    email: henry.morgan@digipen.edu
\par    Course: GAM 200
\brief

Saves any needed information on to the settings file

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#include "Precompiled.h"
#include "Factory\Serializer\JSONSaver.h"

JSONSaver::JSONSaver(void) : tabcount(0), archive(nullptr)
{
  
}
JSONSaver::~JSONSaver(void)
{
}

void JSONSaver::AddTabs(void)
{
  for (unsigned i = 0; i < tabcount; ++i)
    fprintf(archive, "\t");
}

//Loads the archive file for writing, returns true if fopen succeeded.
bool JSONSaver::LoadArchive(const char *filepath)
{
  fopen_s(&archive,filepath,"wt");
  if(archive)
  {
    tabcount = 0;
    return true;
  }

  //otherwise, failed to open
  return false;
}
void JSONSaver::UnloadArchive(void)
{
  fclose(archive);
}

bool JSONSaver::BeginObject(const char *name)
{
  AddTabs();

  if(!name)
    fprintf(archive,"{\n");
  else
    fprintf(archive,"\"%s\": {\n",name);

  ++(tabcount);

  return true;
}
void JSONSaver::EndObject(void)
{
  --(tabcount);
  AddTabs();
  fprintf(archive,"}\n");
}

void JSONSaver::Field(const char *name, int &value)
{
  AddTabs();
  fprintf(archive, "\"%s\": %d\n", name, value);
}

void JSONSaver::Field(const char *name, float &value)
{
  AddTabs();
  fprintf(archive,"\"%s\": %f\n",name,value);
}
void JSONSaver::Field(const char *name, bool &value)
{
  AddTabs();
  if(value)
    fprintf(archive,"\"%s\": true\n",name,value);
  else
    fprintf(archive,"\"%s\": false\n",name,value);
}
void JSONSaver::Field(const char *name, std::string &value)
{
  AddTabs();
  fprintf(archive,"\"%s\": \"%s\"\n",name,value.c_str());
}
void JSONSaver::Field(XINPUT_GAMEPAD &input)
{
  BeginObject();
  fprintf(archive, "\"%s\": %d\n","wButtons", input.wButtons);
  fprintf(archive, "\"%s\": %d\n", "bLeftTrigger", input.bLeftTrigger);
  fprintf(archive, "\"%s\": %d\n", "bRightTrigger", input.bRightTrigger);
  fprintf(archive, "\"%s\": %d\n", "sThumbLX", input.sThumbLX);
  fprintf(archive, "\"%s\": %d\n", "sThumbLY", input.sThumbLY);
  fprintf(archive, "\"%s\": %d\n", "sThumbRX", input.sThumbRX);
  fprintf(archive, "\"%s\": %d\n", "sThumbRY", input.sThumbRY);
  EndObject();
}

void JSONSaver::FieldEmptyLine()
{
  fprintf(archive, "\n");
}
