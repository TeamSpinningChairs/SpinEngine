/*****************************************************************************/
/*!
\file   TreeNavigation.cpp
\author Sagnik Chowdhury
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Constains functions to navigate around the tree
*/
/*****************************************************************************/

#include <Precompiled.h>
#include "JSONSerializer.h"
#include <iostream>
#include <assert.h>


  namespace Serializer
  {
    DataNode* ZeroSerializer::FindStem(DataNode* current)
    {
      assert(current); // If current is NULL, this will break

      auto it = current;
      while (it && it->previous && it == it->previous->next)
      {
        it = it->previous;
      }
      return it->previous;
    }

    void ZeroSerializer::DumpArchive(DataNode* stem, int indentation)
    {
      //int findent = indentation;
      DataNode* it = stem;
      while (it)
      {
        indent(indentation);

        if (it->dataType == TYPE_OBJECT)
        {
          std::cout << it->objectName << " = " << std::endl;
          indent(indentation);
          std::cout << "{" << std::endl;
          DumpArchive(it->branch, indentation + 1);
        }
        else
        {
          if (it->dataType == TYPE_UINT)
            std::cout << it->typeString << " " << it->objectName << " = " << it->value_.UInt_ << "," << std::endl;
          else if (it->dataType == TYPE_INT)
            std::cout << it->typeString << " " << it->objectName << " = " << it->value_.Int_ << "," << std::endl;
          else if (it->dataType == TYPE_LONG)
            std::cout << it->typeString << " " << it->objectName << " = " << it->value_.Long_ << "," << std::endl;
          else if (it->dataType == TYPE_FLOAT)
            std::cout << it->typeString << " " << it->objectName << " = " << it->value_.Float_ << "," << std::endl;
          else if (it->dataType == TYPE_DOUBLE)
            std::cout << it->typeString << " " << it->objectName << " = " << it->value_.Double_ << "," << std::endl;
          else if (it->dataType == TYPE_BOOL)
          {
            std::cout << it->typeString << " " << it->objectName << " = ";
            if (it->value_.Bool_)
              std::cout << "true" << "," << std::endl;
            else
              std::cout << "false" << "," << std::endl;
          }
          else if (it->dataType == TYPE_STRING)
            std::cout << it->typeString << " " << it->objectName << " = \"" << *(it->value_.String_) << "\"," << std::endl;
          else if (it->dataType == TYPE_VEC2)
          {
            std::cout << it->typeString << " " << it->objectName << " = [";
            for (int i = 0; i < 2; ++i)
            {
              std::cout << (*it->value_.VecN_)[i];
              if (i < 1)
                std::cout << ", ";
            }
            std::cout << "]," << std::endl;
          }
          else if (it->dataType == TYPE_VEC3)
          {
            std::cout << it->typeString << " " << it->objectName << " = [";
            for (int i = 0; i < 3; ++i)
            {
              std::cout << (*it->value_.VecN_)[i];
              if (i < 2)
                std::cout << ", ";
            }
            std::cout << "]," << std::endl;
          }
          else if (it->dataType == TYPE_VEC4)
          {
            std::cout << it->typeString << " " << it->objectName << " = [";
            for (int i = 0; i < 4; ++i)
            {
              std::cout << (*it->value_.VecN_)[i];
              if (i < 3)
                std::cout << ", ";
            }
            std::cout << "]," << std::endl;
          }
          else if (it->dataType == TYPE_QUAT)
          {
            std::cout << it->typeString << " " << it->objectName << " = [";
            for (int i = 0; i < 4; ++i)
            {
              std::cout << (*it->value_.VecN_)[i];
              if (i < 3)
                std::cout << ", ";
            }
            std::cout << "]," << std::endl;
          }
          else if (it->dataType == TYPE_ENUM || it->dataType == TYPE_CUSTOM)
          {
            std::cout << it->typeString << " " << it->objectName << " = " << *(it->value_.String_) << "," << std::endl;
          }
        }

        it = it->next;
      }
      if (indentation > 0)
      {
        indent(indentation - 1);
        std::cout << "}," << std::endl;
      }
    }

    DataNode* ZeroSerializer::GetValue(std::initializer_list<const char*> keys)
    {
      DataNode* walk = trunk;
      /*for (auto it = keys.begin(); it != keys.end(); ++it)
      {
      if (walk)
      walk = FindElement(walk, *it);
      else
      {
      std::cout << "Branch not found";
      return NULL;
      }
      }*/
      if (walk)
        return walk;
      else
        return NULL;
    }

    void ZeroSerializer::recursiveFree(DataNode* Obj)
    {
      while (Obj)
      {
        DataNode *temp = Obj->next;
        if (Obj->dataType == TYPE_OBJECT)
        {
          recursiveFree(Obj->branch);
        }
        delete Obj;

        if (temp)
          Obj = temp;
        else
          Obj = nullptr;
      }
    }
  } // serializer
 // framework