/*****************************************************************************/
/*!
\file   DataNode.h
\author Sagnik Chowdhury
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  The JSON serializer will parse JSON data files and return values.
*/
/*****************************************************************************/
//!make sure this is only included once
#pragma once;
#include "Precompiled.h"
#ifndef DATANODE_H

#define DATANODE_H

namespace Serializer
{
    /////////////////////////////////////////////////////////////////
    enum TYPE
    {
      TYPE_NONE,
      TYPE_OBJECT,
      TYPE_UINT,
      TYPE_INT,
      TYPE_LONG,
      TYPE_FLOAT,
      TYPE_DOUBLE,
      TYPE_BOOL,
      TYPE_STRING,
      TYPE_VEC2,
      TYPE_VEC3,
      TYPE_VEC4,
      TYPE_QUAT,
      TYPE_ENUM,
      TYPE_CUSTOM
    }; //enum TYPE
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    class DataNode
	{
		ZilchDeclareBaseType(DataNode, Zilch::TypeCopyMode::ReferenceType);
    public://methods
      DataNode(TYPE type, const char* name);
      ~DataNode();
	  //ZilchDeclareBaseType(DataNode, Zilch::TypeCopyMode::ReferenceType);
      //Overloaded setter/////////////////////////////////////
      void SetValue(unsigned int& value);
      void SetValue(int& value);
      void SetValue(long& value);
      void SetValue(float& value);
      void SetValue(double& value);
      void SetValue(bool& value);
	  void SetValue(std::string& value);
      void SetValue(std::vector<float>& value);
      //Overloaded getters///////////////////////////////////
      void GetValue(unsigned int* store);
      void GetValue(int* store);
      void GetValue(long* store);
      void GetValue(float* store);
      void GetValue(double* store);
      void GetValue(bool* store);
	  void GetValue(std::string* store);
      void GetValue(Vector2D* store);
	  void GetValue(Vector3D* store);
	  void GetValue(Vector4D* store);
      
      //Find specific object
      DataNode* FindElement(DataNode* branch, const char* name);

    public://data
      DataNode* previous;
      TYPE dataType;
      std::string typeString;
	  std::string objectName;
      DataNode* branch;
      DataNode* next;
      union
      {
        int Int_;
        unsigned int UInt_;
        long Long_;
        float Float_;
        double Double_;
        bool Bool_;
		std::string* String_;
        std::vector<float>* VecN_; //2, 3 or 4
        std::string* Enum_;
        std::string* Custom_;
      } value_;
    private://data

    };
} //namespace serializer


#endif