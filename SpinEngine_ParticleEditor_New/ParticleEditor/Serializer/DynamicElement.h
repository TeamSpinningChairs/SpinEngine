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
#pragma once

#include <vector>
#include <string>
#include <stack>
#include <list>
#include <map>

class DynamicElement
{
public:
	enum Type
	{
		TYPE_NULL,
		TYPE_OBJECT, 
		TYPE_FLOAT, 
		TYPE_BOOL, 
		TYPE_STRING,
    TYPE_INT,
    TYPE_VECTOR
	};

	DynamicElement(void);
	~DynamicElement(void);

	/////////////////////////////////////////////////////////////////////////////////////////////////
	//void SetVal(const val)
	//
	//	Purpose:
	//		Sets the data object's type to TYPE_OBJECT.  The object will later be used with 
	//		AddObjectMember and GetObjectMember
	//
	/////////////////////////////////////////////////////////////////////////////////////////////////
	void SetAsObject(void);

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
	bool AddObjectMember(DynamicElement *member, const char *name = 0);

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
	bool GetObjectMember(DynamicElement **member, const char *name = 0);

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
	void SetFloatVal(const float val);
	void SetBoolVal(const bool val);
	void SetStringVal(const char *val);
  void SetIntVal(const int val);
  void SetVectorVal(std::vector<int> *val);

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
	bool GetFloatVal(float *val) const;
	bool GetBoolVal(bool *val) const;
	bool GetStringVal(std::string *val) const;
  bool GetIntVal(int *val) const;
  bool GetVector(std::vector<int> **val, bool mergeWithExistingVector = false) const;

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
	Type GetType(void) const;

  std::string QueryChildName(DynamicElement *child)
	{
    #define QUERYCHILDNAME
		if(type != TYPE_OBJECT)
			return std::string("");
		if(!data.val_membersPtr)
			return std::string("");
    if(data.val_membersPtr->empty()) //@me, otherwise wouldn't recognize unnameds
      return std::string("");

		for(auto i : *(data.val_membersPtr))
		{
			if(i.second == child)
			{
				return i.first;
			}
		}
	}
	union
		{
			float val_float;
		int val_int;
			bool val_bool;
			std::string *val_stringPtr;
			std::map<std::string, DynamicElement*> *val_membersPtr;
		std::vector<int> *val_vectorPtr;
		} data;	//@mine
private:

	void PrepareForNewType(void); //@mine

	

	std::map<std::string, DynamicElement*>::iterator nextmember;

  //DynamicElement *parent;

  //Vector containing all unnamed members. (map could only store one)
  std::vector<DynamicElement*> unnamedmembers;
  unsigned unnamedmemberIndex;

	Type type; //@mine
};
