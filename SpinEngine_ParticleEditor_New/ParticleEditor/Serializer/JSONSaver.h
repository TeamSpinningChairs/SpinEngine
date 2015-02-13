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
#pragma once
#include "DynamicElement.h"

struct JSONSaver_Privates;

class JSONSaver
{
public:

	JSONSaver(void);
	~JSONSaver(void);

	bool LoadArchive(const char *filepath = 0);
	void UnloadArchive(void);					

	bool BeginObject(const char *name = 0);
	void EndObject(void);

  void Field(const char *name, int &value);
	void Field(const char *name, float &value);
	void Field(const char *name, bool &value);
	void Field(const char *name, std::string &value);

  void FieldEmptyLine();

private:
  FILE *archive;
  unsigned tabcount;
  //Helper function to add the correct number of tabs.
  void AddTabs(void);
};
