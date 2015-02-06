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
#include "Precompiled.h"//iostream
#include "Factory\Serializer\JSONLoader.h"


JSONLoader::JSONLoader(void) : archiveLoaded_(false)
{  
}
JSONLoader::~JSONLoader(void)
{
}


//open a file, load a JSON file into a DynamicElement structure, close the file
bool JSONLoader::LoadArchive(const char *filepath)
{
  if(!filepath) //With no filepath given, make the root an empty object
  {
    ObjectStack.push(&root);
    root.SetAsObject();
    return true;
  }
  fopen_s(&(archive),filepath,"r");
  if(!archive)
    return false;

  archiveLoaded_ = true;

  //Buffers for holding lines of json file, object names, and object data
  char* buffer = new char [500];
  char* objectname = new char [500];
  char* data = new char [500];

  //Go through the entire file, line by line, and load into the structure
  while(!feof(archive))
  {
    *buffer = '\0'; //Usually overwritten, but prevents a bounding error at EOF

    //Get a new line
    fgets(buffer,500,archive);
    char *c = buffer;
    SkipWhiteSpace(c);

    //COMMENT
    if (*c == '#')
      continue;

    //NEW UNNAMED ELEMENT
    if(*c == '{')
    {
      //if first element, add root to stack
      if(ObjectStack.empty())
      {
        ObjectStack.push(&root);
        root.SetAsObject();
        //If the rest of the line is empty (I.E. a file WE saved), go to next line
        if (*++c == '\n')
          continue;
        //Otherwise, we are reading in a Tiled .json export and will continue on to first variable.
        SkipWhiteSpace(c);
      }
      else 
      {
        //Create a new unnamed dynamic element object
        DynamicElement *newElement = new DynamicElement;
        newElement->SetAsObject();
        //add to current top of stack, then make it the new top of stack
        ObjectStack.top()->AddObjectMember(newElement, NULL);
        ObjectStack.push(newElement);
        //keep track of the allocated memory
        AllocatedMemory.push_back(newElement);
        continue;
      }
    }
    
    //NAMES - store the name for use later on
    if (*c == '\"')
    {
      ++c;
      char *n = objectname;
      while (*c != '\"')
      {
        *n++ = *c++;
      }
      *n = '\0';
      //Skip last quotation mark, should now be on a :
      ++c;
    }

    //DONE WITH OLD ELEMENT
    if(*c == '}' || *c == ']')
    {
      //Remove from stack (we won't need to deal with this object anymore)
      ObjectStack.pop();
      //NOW: Check for another one immediately behind it. (Tiled thing.)
      if (*++c == ']')
        ObjectStack.pop();
      continue;
    }

    //DATA
    if(*c == ':')
    {
      ++c;
      SkipWhiteSpace(c);

      //NEW NAMED ELEMENT
      //the newline is for a case in Tiled's export. It does "properties":\n\t\t\t\t{.........} instead of "properties":{\n\t\t\t\t.........}
      //(this may be the default, I was lazy and did not check the JSON standard)
      if(*c == '{' || *c == '\n') 
      {
        //create new dynamic element object,
        DynamicElement *newElement = new DynamicElement();
        newElement->SetAsObject();
        //add to current top of stack, then make it the new top of stack
        ObjectStack.top()->AddObjectMember(newElement,objectname);
        ObjectStack.push(newElement);
        //keep track of the allocated memory
        AllocatedMemory.push_back(newElement);
        if (*c == '\n')
        {
          //Move to next line and skip past next {
          fgets(buffer, 500, archive);
          char *c = buffer;
          SkipWhiteSpace(c);
          if (*c != '{')
            MessageBox(NULL, "JSONLoader thought it was getting a new object but it wasn't..", NULL, NULL);
          ++c;
          SkipWhiteSpace(c);
        }
        continue;
      }

      //ARRAY (OR NEW NAMED ELEMENT)
      //If there's a number directly after this, we make a Vector for tilemap data.
      //If there's anything else (newline), we treat it like a {
      if (*c == '[')
      {
        ++c;
        if (*c == '\n' || *c == ',')
        {
          //create new dynamic element object,
          DynamicElement *newElement = new DynamicElement;
          newElement->SetAsObject();
          //add to current top of stack, then make it the new top of stack
          ObjectStack.top()->AddObjectMember(newElement, objectname);
          ObjectStack.push(newElement);
          //keep track of the allocated memory
          AllocatedMemory.push_back(newElement);
          continue;
        }

        //NEW ARRAY
        DynamicElement *newElement = new DynamicElement;
        //Allocate vector, add all values from file
        std::vector<int> *newVector = new std::vector<int>;
        //Until we've reached the end of the array
        while (*c != ']')
        {
          //We check for reaching the end of this buffer every time we advance c.
          //ExtendBufferIfNeeded(c, buffer, archive);

          //Skip commas
          if (*c == ',')
            ++c;
          ExtendBufferIfNeeded(c, buffer, archive);

          SkipWhiteSpace(c);
          ExtendBufferIfNeeded(c, buffer, archive);

          //c is currently a number!
          //Put this number in data, convert to int
          char *s = data;
          while(*c != ',' && *c != ']')
          {
            *s++ = *c++;
            ExtendBufferIfNeeded(c, buffer, archive);

            SkipWhiteSpace(c);
            ExtendBufferIfNeeded(c, buffer, archive);
          }
          *s = '\0';
          newVector->push_back(atoi(data));
        }
        newElement->SetVectorVal(newVector);
        //Add to current top of stack
        ObjectStack.top()->AddObjectMember(newElement, objectname);
        //Keep track of allocated memory.
        AllocatedMemory.push_back(newElement);
        continue;
      }

      //STRING
      if(*c == '\"')
      {
        //Skip the quotation mark and copy the string into data
        ++c;
        char *s = data;
        while(*c != '\"')
        {
          *s++ = *c++;
        }
        *s = '\0';
        //Create new string element, add to current top of stack.
        DynamicElement *newElement = new DynamicElement();
        newElement->SetStringVal(data);
        ObjectStack.top()->AddObjectMember(newElement,objectname);
        //Keep track of allocated memory.
        AllocatedMemory.push_back(newElement);
        continue;
      }

      //INT OR FLOAT
      if((*c >= '0' && *c <= '9') || *c == '-')
      {
        bool isFloat = false;
        //Copy the number into data
        char *s = data;
        while(*c != '\n' && *c != ',')
        {
          if(*c == '.')
          {
            isFloat = true;
          }
          *s++ = *c++;
        }
        *s = '\0';
        //create new float element, add to current top of stack
        DynamicElement *newElement = new DynamicElement();
        if(isFloat)
        {
          newElement->SetFloatVal(static_cast<float>(atof(data)));
        }
        else //int
        {
          newElement->SetIntVal(atoi(data));
        }
        ObjectStack.top()->AddObjectMember(newElement,objectname);
        //Keep track of allocated memory.
        AllocatedMemory.push_back(newElement);
        continue;
      }

      //BOOL
      if(*c == 't' || *c == 'f')
      {
        //create new bool element, add to current top of stack
        DynamicElement *newElement = new DynamicElement;
        newElement->SetBoolVal(*c == 't' ? true : false);
        ObjectStack.top()->AddObjectMember(newElement,objectname);
        //Keep track of allocated memory.
        AllocatedMemory.push_back(newElement);
        continue;
      }

    }//DATA

  }
  //end of file reached

  delete[] data;
  delete[] objectname;
  delete[] buffer;
  fclose(archive);
  return true;
}

//unload the DynamicElement structure
void JSONLoader::UnloadArchive(void)
{
  archiveLoaded_ = false;

  //delete all allocated memory
  std::list<DynamicElement*>::iterator it = AllocatedMemory.begin();
  while(!AllocatedMemory.empty())
  {
    delete *it++;
    AllocatedMemory.pop_front();
  }
  //ObjectStack should already be empty
}




/////////////////////////////////////////////////////////////////////////////////////////////////
//void BeginObject(const char *name = 0)
//
//	Purpose:
//		Initiate the current data object in the archive.  If the purpose of the archive
//		is loading, this will search for a data object within the current object.
//    In both cases of loading and saving, this method will 
//		set the current object to the initiated object.
//
//	Parameters:
//		name:
//			Name of the data object to be initiate within the current data object.
//
//	Notes:
//		In the case of this assignment, we have specified that the only acceptable unnamed 
//		is the archive's root object.  In the case where unnamed child objects are
//		acceptable, we would rename this to "NextObject", where if the name parameter is
//		NULL, we initiate the next sequential object.
//
/////////////////////////////////////////////////////////////////////////////////////////////////
bool JSONLoader::BeginObject(const char *name)
{
  //We reuse the stack here.
  if(!name)
  {
    ObjectStack.push(&root);
    return true;
  }
  if(ObjectStack.top()->GetType() != ObjectStack.top()->TYPE_OBJECT)
    return false;

  //Search for the data object within the current object.
  //If we find it, we put it on the top of the stack.
  DynamicElement *temp;
  if(ObjectStack.top()->GetObjectMember(&temp,name))
  {
    ObjectStack.push(temp);
    return true;
  }
  else
    return false;
}
/////////////////////////////////////////////////////////////////////////////////////////////////
//void EndObject(void)
//
//	Purpose:
//		End the current data object in the archive. In both cases of loading and saving, 
//		this method will set the current object to its parent object.
//
/////////////////////////////////////////////////////////////////////////////////////////////////
void JSONLoader::EndObject(void)
{
  ObjectStack.pop();
}


//		If the purpose of the archive is loading, this method will load the value 
//		from the archive.  In the case of loading, if the value is not found in the
//		archive, a default value will be set.
//		
//	Parameters:
//		name:
//			Name string identifier for variable.
//		value:
//			Value variable to be archived.
//		defval:
//			Default value to be used if variable identifier is not found in archive.
//			This parameters should typically be only used for loading archives.  For
//			example, if no archive is available, the value will be set to a default
//			value.  Or in the case where an outdated archive missing data elements
//			is loaded, values will be set to default values.
//
//	Notes:
//		This function does not return a error due to the fact that if the element is
//		not located, the default value will be used.  Therefore, aside from critical
//		system errors, this function by definition cannot logically fail.  Returning
//		an error may be useful in a situation where the use of the default value
//		is required to be flagged.  
void JSONLoader::Field(const char *name, float &value, float defval)
{
  //add a variable with that name and value to the top of the stack
  DynamicElement *temp;
  //If we find the variable identifier in the archive, set it
  if(ObjectStack.top()->GetObjectMember(&temp,name))
  {
    temp->GetFloatVal(&value);
  }
  else //add it using the default value
  {
    value = defval;
    //DynamicElement *newObj = new DynamicElement;
    //newObj->SetFloatVal(defval);
    //ObjectStack.top()->AddObjectMember(newObj,name);
  }
}
void JSONLoader::Field(const char *name, bool &value, bool defval)
{
  //add a variable with that name and value to the top of the stack
  DynamicElement *temp;
  //If we find the variable identifier in the archive, set it
  if(ObjectStack.top()->GetObjectMember(&temp,name))
  {
    temp->GetBoolVal(&value);
  }
  else //add it using the default value
  {
    value = defval;
    //DynamicElement *newObj = new DynamicElement;
    //newObj->SetBoolVal(defval);
    //ObjectStack.top()->AddObjectMember(newObj,name);
  }
}
void JSONLoader::Field(const char *name, std::string &value, const std::string &defval)
{
  //add a variable with that name and value to the top of the stack
  DynamicElement *temp;
  //If we find the variable identifier in the archive, set it
  if(ObjectStack.top()->GetObjectMember(&temp,name))
  {
    temp->GetStringVal(&value);
    //std::string *val;
    //if(temp->GetStringVal(val))
     // *val = value;
  }
  else //add it using the default value
  {
    value = defval;
    //DynamicElement *newObj = new DynamicElement;
    //newObj->SetStringVal(defval.c_str());
    //ObjectStack.top()->AddObjectMember(newObj,name);
  }
}

bool JSONLoader::Field(XINPUT_GAMEPAD &input)
{
  //Get next unnamed member, get each submember, load each property into input
  DynamicElement *temp;
  if (!root.GetObjectMember(&temp, NULL))
    return false;


  DynamicElement *fields;
  int f;
  temp->GetObjectMember(&fields, "wButtons");
  fields->GetIntVal(&f);
  input.wButtons = static_cast<WORD>(f);

  temp->GetObjectMember(&fields, "bLeftTrigger");
  fields->GetIntVal(&f);
  input.bLeftTrigger = static_cast<BYTE>(f);

  temp->GetObjectMember(&fields, "bRightTrigger");
  fields->GetIntVal(&f);
  input.bRightTrigger = static_cast<BYTE>(f);

  temp->GetObjectMember(&fields, "sThumbLX");
  fields->GetIntVal(&f);
  input.sThumbLX = static_cast<SHORT>(f);

  temp->GetObjectMember(&fields, "sThumbLY");
  fields->GetIntVal(&f);
  input.sThumbLY = static_cast<SHORT>(f);

  temp->GetObjectMember(&fields, "sThumbRX");
  fields->GetIntVal(&f);
  input.sThumbRX = static_cast<SHORT>(f);

  temp->GetObjectMember(&fields, "sThumbRY");
  fields->GetIntVal(&f);
  input.sThumbRY = static_cast<SHORT>(f);
  return true;
}


//Helper functions:

DynamicElement &JSONLoader::Root(void)
{
  return root;
}

void JSONLoader::SkipWhiteSpace(char *&c)
{
  while (*c == ' ' || *c == '\t')
  {
    ++c;
  }
}

void JSONLoader::ExtendBufferIfNeeded(char *&c, char *&buffer, FILE *&archive)
{
  if (*c == '\0')
  {
    fgets(buffer, 500, archive);
    c = buffer;
  }
}

bool JSONLoader::ArchiveIsLoaded() const
{
  return archiveLoaded_;
}