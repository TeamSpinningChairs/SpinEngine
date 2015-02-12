/****************************************************************************/
/*!
\author Henry Morgan
\par    email: henry.morgan@digipen.edu
\par    Course: GAM 200
\brief

Represents properties loaded from .json files

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#include "DynamicElement.h"

DynamicElement::DynamicElement(void) : type(TYPE_NULL), unnamedmemberIndex(0)
{
}

//Destructor- deallocates any dynamically allocated memory
DynamicElement::~DynamicElement(void)
{
  if(type == TYPE_STRING)
    delete data.val_stringPtr;

  if (type == TYPE_VECTOR)
    delete data.val_vectorPtr;

  if(type == TYPE_OBJECT)
  {
    //Clear and delete map (destructors of any sub-objects will be called)
    data.val_membersPtr->clear();
    delete data.val_membersPtr;
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//void SetVal(const val)
//
//	Purpose:
//		Sets the data object's type to TYPE_OBJECT.  The object will later be used with 
//		AddObjectMember and GetObjectMember
//
/////////////////////////////////////////////////////////////////////////////////////////////////
void DynamicElement::SetAsObject(void)
{
  if (type == TYPE_OBJECT)
  {
    return;
  }
  type = TYPE_OBJECT;
  data.val_membersPtr = new std::map<std::string, DynamicElement*>;
  nextmember = data.val_membersPtr->begin();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//bool AddObjectMember(const char *name, DynamicElement *member)
//
//	Purpose:
//		Adds a child  object to the this-object.  This is only valid if the this-object  
//		is of type TYPE_OBJECT.
//
//	Parameters:
//		name:
//			The archival name of the child object being added.  If the name is null, you
//			should add the object as a nameless member.
//		*member:
//			Pointer to the new child object 
//
//	Return:
//		Returns false if this-object's type is not TYPE_OBJECT.  In all other cases,
//		returns true;
//
/////////////////////////////////////////////////////////////////////////////////////////////////
bool DynamicElement::AddObjectMember(DynamicElement *member, const char *name) 
{
  if(type != TYPE_OBJECT)
    return false;

  if(name == NULL)
    unnamedmembers.push_back(member);
    //(*data.val_membersPtr)[""] = member;
  else
    (*data.val_membersPtr)[name] = member;

  //Resetting iterator each time the map is expanded
  nextmember = data.val_membersPtr->begin();

  return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//bool GetObjectMember(const char *name, DynamicElement **member) const
//
//	Purpose:
//		Gets a child to the this-object.  This is only valid if the this-object is of 
//		type TYPE_OBJECT.
//
//	Parameters:
//		**member:
//			Pointer to child object pointer to be set as located child
//		name:
//			The archival name of the child object being added.  If the name is null, the
//			next object should be queried
//
//	Return:
//		Returns false if this-object's type is not TYPE_OBJECT.  In all other cases,
//		returns true;
//
/////////////////////////////////////////////////////////////////////////////////////////////////
bool DynamicElement::GetObjectMember(DynamicElement **member, const char *name)
{
  if(type != TYPE_OBJECT)
    return false;

  if(name == NULL)
  {
    /*This was the version you had for normal JSON file, before you realized
    that "your" JSON files had no unnamed variables and that Tiled's export
    was the only one you'd have to deal with.*/
    /*
    //When getting null names, go through all named ones, then all unnamed ones
    //currently you're only getting the DynamicElement pointer pointer
    if(!data.val_membersPtr->empty() && nextmember != data.val_membersPtr->end())
    { I EAT POOP
      *member = nextmember->second;
      ++nextmember;
      return true;
    }*/
    if(unnamedmemberIndex < unnamedmembers.size())
    {
      *member = unnamedmembers[unnamedmemberIndex];
      ++unnamedmemberIndex;
      return true;
    }
    
    return false;
  }

  //name != NULL
  if(data.val_membersPtr->find(name) == data.val_membersPtr->end()) //if key not found
    return false;

  *member = (*data.val_membersPtr)[name];
  return true;
  
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//void SetVal(const val)
//
//	Purpose:
//		Sets the data object's type and value
//
//	Parameters:
//		val:
//			The value to be set to the object
//
//	Notes:
//		In order for maximum flexibility, this function does not logically fail.  
//		If a value and type is already set, the function should override the
//		previous data.  For example, if the object is initially set as a string value,
//		and then is later set to a float value, the object should deallocate the
//		string and function as a float.  Returning an error may be used in a situation
//		where type reassignment occurs to alert the user.
//
/////////////////////////////////////////////////////////////////////////////////////////////////
void DynamicElement::SetFloatVal(const float val)
{
  PrepareForNewType();
  type = TYPE_FLOAT;
  data.val_float = val;
}
void DynamicElement::SetBoolVal(const bool val)
{
  PrepareForNewType();
  type = TYPE_BOOL;
  data.val_bool = val;
}
void DynamicElement::SetStringVal(const char *val)
{
  PrepareForNewType();
  type = TYPE_STRING;
  data.val_stringPtr = new std::string(val);
}
void DynamicElement::SetIntVal(const int val)
{
  PrepareForNewType();
  type = TYPE_INT;
  data.val_int = val;
}
void DynamicElement::SetVectorVal(std::vector<int> *val)
{
  PrepareForNewType();
  type = TYPE_VECTOR;
  data.val_vectorPtr = val;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//void GetVal(float &val) const
//
//	Purpose:
//		Get the data object's value
//
//	Parameters:
//		&val:
//			The value variable to be set.
//
//	Return:
//		If the value variable does not match the object's type, the function will return
//		with fail.  In the case where the function is successful, the function will
//		with true.
//
/////////////////////////////////////////////////////////////////////////////////////////////////
bool DynamicElement::GetFloatVal(float *val) const
{
  if(type != TYPE_FLOAT)
    return false;

  *val = data.val_float;
  return true;
}
bool DynamicElement::GetBoolVal(bool *val) const
{
  if(type != TYPE_BOOL)
    return false;

  *val = data.val_bool;
  return true;
}
bool DynamicElement::GetStringVal(std::string *val) const
{
  if(type != TYPE_STRING)
    return false;

  *val = *data.val_stringPtr;
  return true;
}
bool DynamicElement::GetIntVal(int *val) const
{
  if(type != TYPE_INT)
    return false;

  *val = data.val_int;
  return true;
}
bool DynamicElement::GetVector(std::vector<int> **val, bool mergeWithExistingVector) const
{
  if (type != TYPE_VECTOR)
    return false;
  if (!mergeWithExistingVector)
    *val = data.val_vectorPtr;
  else
  {
    //Go through the existing vector. Anywhere that the existing vector is 0 and our value is different,
    //change the existing vector.
    for (unsigned i = 0; i < (*val)->size(); ++i)
    {
      if ((**val)[i] == 0 && (*(data.val_vectorPtr))[i] != 0)
        (**val)[i] = (*(data.val_vectorPtr))[i];
    }
  }
  return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//Type GetType(void) const
//
//	Purpose:
//		Get the data type of the object
//
//	Return:
//		Type value regarding the data type of the object
//
/////////////////////////////////////////////////////////////////////////////////////////////////
DynamicElement::Type DynamicElement::GetType(void) const
{
  return type;
}



//Deallocates string or object if necessary.
void DynamicElement::PrepareForNewType(void)
{
  if(type == TYPE_STRING)
    delete data.val_stringPtr;

  if (type == TYPE_VECTOR)
    delete data.val_vectorPtr;

  if(type == TYPE_OBJECT)
  {
    /*Loop through map, deallocating all Object members
    for(nextmember = data.val_membersPtr->begin();
        nextmember != data.val_membersPtr->end();
        ++nextmember)
    {
      if(nextmember->second->type == TYPE_OBJECT)
        data.val_membersPtr->erase(nextmember);
    }*/

    //Delete map
    delete data.val_membersPtr;
  }
}
