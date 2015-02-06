/****************************************************************************/
/*!
\author Henry Morgan
\par    email: henry.morgan@digipen.edu
\par    Course: GAM 200
\brief

Loads any saved information on to the settings file

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#pragma once
#include "Precompiled.h" //stack, list
#include "Factory\Serializer\DynamicElement.h"


class JSONLoader
{
public:

	JSONLoader(void);
	~JSONLoader(void);

	bool LoadArchive(const char *filepath = 0);	
	void UnloadArchive(void);					

	bool BeginObject(const char *name = 0);
	void EndObject(void);

  //Float also gets ints, the difference is specified in the actual dynamicelement
	void Field(const char *name, float &value, float defval);
	void Field(const char *name, bool &value, bool defval);
	void Field(const char *name, std::string &value, const std::string &defval);
  bool Field(XINPUT_GAMEPAD &input);

  //Passes root to LevelData classes, settings classes, etc... so each of them can parse
  //(prevents having a ton of .h files / dependencies in here. in exchange, you have to
  //make sure that 'root' isn't deallocated while it's being parsed elsewhere.
  //So, don't delete the factory's loader (or whatever's loader) until everything's parsed.)
  DynamicElement &Root(void);

  bool ArchiveIsLoaded() const;

private:
  FILE * archive; //The file we're loading from
  DynamicElement root;
  std::stack<DynamicElement*> ObjectStack; //Stack of all dynamic elements
  std::list<DynamicElement *> AllocatedMemory; //keeping track of all memory we need to deallocate
  bool archiveLoaded_;

  //Helper functions to skip whitespace and extend buffer (in case of very, very long lines)
  void SkipWhiteSpace(char *&c);
  void ExtendBufferIfNeeded(char *&c, char *&buffer, FILE *&archive);
};
