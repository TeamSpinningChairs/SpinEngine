/*****************************************************************************/
/*!
\file   GetData.cpp
\author Sagnik Chowdhury
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  The overloaded functions for getting data.
*/
/*****************************************************************************/

#include "Precompiled.h"
#include "JSONSerializer.h"
#include "DataNode.h"
//#include "GraphicsCommon.h"

#include <cstdarg> //for variadic crap


  namespace Serializer
  {
    void ZeroSerializer::GetData(unsigned int* store, std::initializer_list<const char*> keys)
    {
      DataNode* found = GetValue(keys);
      *store = found->value_.UInt_;
    }
    void ZeroSerializer::GetData(int* store, std::initializer_list<const char*> keys)
    {
      DataNode* found = GetValue(keys);
      *store = found->value_.Int_;
    }
    void ZeroSerializer::GetData(long* store, std::initializer_list<const char*> keys)
    {
      DataNode* found = GetValue(keys);
      *store = found->value_.Long_;
    }
    void ZeroSerializer::GetData(float* store, std::initializer_list<const char*> keys)
    {
      DataNode* found = GetValue(keys);
      *store = found->value_.Float_;
    }
    void ZeroSerializer::GetData(double* store, std::initializer_list<const char*> keys)
    {
      DataNode* found = GetValue(keys);
      *store = found->value_.Double_;
    }
    void ZeroSerializer::GetData(bool* store, std::initializer_list<const char*> keys)
    {
      DataNode* found = GetValue(keys);
      *store = found->value_.Bool_;
    }
	void ZeroSerializer::GetData(std::string* store, std::initializer_list<const char*> keys)
    {
      DataNode* found = GetValue(keys);
      switch (found->dataType)
      {
      case TYPE_STRING:
      {
        *store = *(found->value_.String_);
        break;
      }
      case TYPE_ENUM:
      {
        *store = *(found->value_.Enum_);
      }
      case TYPE_CUSTOM:
      {
        *store = *(found->value_.Custom_);
        break;
      }
      }
    }
    void ZeroSerializer::GetData(std::vector<float>* store, std::initializer_list<const char*> keys)
    {
      DataNode* found = GetValue(keys);
      if (store)
      {
        store->clear();
        *store = *(found->value_.VecN_);
      }
      else
      {
        store = new std::vector<float>(*(found->value_.VecN_));
      }
    } //function GetData

    DataNode* DataNode::FindElement(DataNode* branch, const char* key)
    {
      auto it = branch;
      std::string name(key);
      while (it && it->objectName.compare(name) != 0)
      {
        it = it->next;
      }
      //ErrorIf(it == nullptr, "The data field was not found");
      return it;
    }

  const char* SkipHash(const char* str)
  {
    const char* temp = str;
    while (*temp != ':' && *temp != '\0')
      ++temp;
    return temp + 1;
  }
  } //Serializer
//Framework