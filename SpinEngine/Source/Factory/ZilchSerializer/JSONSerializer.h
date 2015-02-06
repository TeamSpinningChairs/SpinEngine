/*****************************************************************************/
/*!
\file   JSONSerializer.h
\author Sagnik Chowdhury
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  The JSON serializer will parse JSON data files and return values.
*/
/*****************************************************************************/
//!make sure this is only included once
#ifndef SERIALIZER_H
#define SERIALIZER_H
#pragma once
#include "DataNode.h"
#include <fstream>

  namespace Serializer
  {


    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    class ZeroSerializer
    {
    public:
      //------------------------------------------------
      //CONSTRUCTOR
      //------------------------------------------------
      ZeroSerializer();


      /*-----------------------------------------------------------------------
      // Public Variables
      -----------------------------------------------------------------------*/
#pragma region Public Variables

#pragma endregion

      /*---------------------------------------------------------------------*/
      // Public Structs
      /*---------------------------------------------------------------------*/
#pragma region Public Structs

	  std::unordered_map<std::string, TYPE> TypeList;
#pragma endregion

      /*-----------------------------------------------------------------------
      // Public Functions
      -----------------------------------------------------------------------*/
#pragma region Public Functions
      //GETTERS
      //Open file
      bool open(const char* filepath);
      //Close file
      void close();
      //Get trunk
      DataNode* GetTrunk(){ return trunk; }

      //Find value from tree===========================================================
      //Overloaded functions
      void GetData(unsigned int* store, std::initializer_list<const char*> keys);
      void GetData(int* store, std::initializer_list<const char*> keys);
      void GetData(long* store, std::initializer_list<const char*> keys);
      void GetData(float* store, std::initializer_list<const char*> keys);
      void GetData(double* store, std::initializer_list<const char*> keys);
      void GetData(bool* store, std::initializer_list<const char*> keys);
	  void GetData(std::string* store, std::initializer_list<const char*> keys);
      void GetData(std::vector<float>* store, std::initializer_list<const char*> keys);
      //===============================================================================

      //Load archive
      void CreateArchive();

      //Print archive for debugging
      void DumpArchive(DataNode* stem, int indentation = 0); //DONE

      //Get object name

      //is the file open
      bool is_open() { return dataFile.is_open(); }
      //Skip data

#pragma endregion

      //------------------------------------------------
      //DESTRUCTOR
      //------------------------------------------------
      ~ZeroSerializer();
      //------------------------------------------------

    private:
#pragma region Private Variables
      //Data file
      std::ifstream dataFile;
      //Name of current object
      std::string CurrentLine;
      //Type of current object
      char prev;
      //Is it an object or subobject ...
      int inObject;
      //Did the program exit a branch?
      bool exitted;

      //Data tree
      DataNode* trunk;
      DataNode* CurrentNode;
      DataNode* CurrentStem;


#pragma endregion

#pragma region Private Functions    
      //recursively free the entire tree
      void recursiveFree(DataNode* Obj);
      //void FindElement(DataNode* list, const char* key);
      void ReadLine();
      void ParseLine();

	  void InterpretData(TYPE currentdatatype, std::vector<std::string>* tokens, DataNode** node);
	  TYPE DetermineType(std::string* typestring);

      //Tree navigation
      //implemented at the end of this header
      template<typename T>
      DataNode* AddNode(DataNode* at, TYPE type, const char* name, T value);

      //walk current list

      //find item in current list (list, string)
      //Run this as many times as necessary
      //find stem of a list
      DataNode* FindStem(DataNode* current);

      //Use like this: GetValue({"helo", "fuck off", "you", "stupid", "bitch"});
      DataNode* GetValue(std::initializer_list<const char*> keys);
      //--------------------------------------------------------
      //Helper function
	  std::vector<std::string> Tokenize();
      void indent(int indentation);


#pragma endregion
    }; //class JSONSerializer

    //template function AddNode-----------------------------------
    template<typename T>
    DataNode* ZeroSerializer::AddNode(DataNode* at, TYPE type, const char* name, T value)
    {
      at = new DataNode(type, name);
      at->SetValue(value);
      return at;
    }

	const char* SkipHash(const char* str);

  }  //namespace Serializer
  //namespace Framework

#endif
