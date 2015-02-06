/*****************************************************************************/
/*!
\file   DataNode.cpp
\author Sagnik Chowdhury
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  The overloaded functions for setting the value of a node.
*/
/*****************************************************************************/
#include "Precompiled.h"
#include "DataNode.h"


  namespace Serializer
  {
	  ZilchDefineType(DataNode, SpinningZilch)
	  {
		  type->HandleManager = ZilchManagerId(Zilch::PointerManager);
	  }
    DataNode::DataNode(TYPE type, const char* name) 
      : previous(nullptr)
      , next(nullptr)
      , branch(nullptr)
      , dataType(type)
      , objectName(std::string(name))
    {}

    DataNode::~DataNode()
    {
      if (dataType == TYPE_STRING)
      {
        delete value_.String_;
        value_.String_ = nullptr;
      }
      else if ( dataType == TYPE_VEC2
             || dataType == TYPE_VEC3
             || dataType == TYPE_VEC4
             || dataType == TYPE_QUAT)
      {
        delete value_.VecN_;
        value_.VecN_ = nullptr;
      }
    }

    //////////////////////////////////////////////////////////////////
    //SetValue
    //////////////////////////////////////////////////////////////////
    void DataNode::SetValue(unsigned int& value)
    {
      value_.UInt_ = value;
    }

    void DataNode::SetValue(int& value)
    {
      value_.Int_ = value;
    }

    void DataNode::SetValue(long& value)
    {
      value_.Long_ = value;
    }

    void DataNode::SetValue(float& value)
    {
      value_.Float_ = value;
    }

    void DataNode::SetValue(double& value)
    {
      value_.Double_ = value;
    }

    void DataNode::SetValue(bool& value)
    {
      value_.Bool_ = value;
    }
    
    void DataNode::SetValue(std::string& value)
    {
      value_.String_ = new std::string(value);
    }

    void DataNode::SetValue(std::vector<float>& value)
    {
      value_.VecN_ = new std::vector<float>(value);
    }

    //////////////////////////////////////////////////////////////////////////
    //GetValue
    //////////////////////////////////////////////////////////////////////////
    void DataNode::GetValue(unsigned int* store)
    {
      ErrorIf(!store, "Pointer Uninitialized");
      *store = value_.UInt_;
    }
    void DataNode::GetValue(int* store)
    {
      ErrorIf(!store, "Pointer Uninitialized");
      *store = value_.Int_;
    }
    void DataNode::GetValue(long* store)
    {
      ErrorIf(!store, "Pointer Uninitialized");
      *store = value_.Long_;
    }
    void DataNode::GetValue(float* store)
    {
      ErrorIf(!store, "Pointer Uninitialized");
      *store = value_.Float_;
    }
    void DataNode::GetValue(double* store)
    {
      ErrorIf(!store, "Pointer Uninitialized");
      *store = value_.Double_;
    }
    void DataNode::GetValue(bool* store)
    {
      ErrorIf(!store, "Pointer Uninitialized");
      *store = value_.Bool_;
    }
	void DataNode::GetValue(std::string* store)
    {
      ErrorIf(!store, "Pointer Uninitialized");
      *store = *value_.String_;
    }
    void DataNode::GetValue(Vector2D* store)
    {
      ErrorIf(value_.VecN_->size() < 2, "Wrong data type");
      store->x = (*value_.VecN_)[0];
      store->y = (*value_.VecN_)[1];
    }
    void DataNode::GetValue(Vector3D* store)
    {
      ErrorIf(value_.VecN_->size() < 3, "Wrong data type");
      store->x = (*value_.VecN_)[0];
      store->y = (*value_.VecN_)[1];
      store->z = (*value_.VecN_)[2];
    }
	void DataNode::GetValue(Vector4D* store)
    {
      ErrorIf(value_.VecN_->size() < 4, "Wrong data type");
      store->x = (*value_.VecN_)[0];
      store->y = (*value_.VecN_)[1];
      store->z = (*value_.VecN_)[2];
      store->w = (*value_.VecN_)[3];
    }
    /////////////////////////////////////////////////////////////////
  }