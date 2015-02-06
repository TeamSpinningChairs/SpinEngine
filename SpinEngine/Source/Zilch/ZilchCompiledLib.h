/******************************************************************************/
/*!
\file   ZilchCompiledLib.h
\author Sagnik Chowdhury
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  Container for the compiled Zilch libraries.
*/
/******************************************************************************/

#pragma once
#include "Precompiled.h"
#include "ISystem.h"
/*------------------------------------------------------------------------------
// Includes
------------------------------------------------------------------------------*/

using namespace Zilch;
ZilchStaticLibrary(SpinningZilch);

  /*------------------------------------------------------------------------------
  // Class
  ------------------------------------------------------------------------------*/


class ScriptSystem : public ISystem
  {
  public:
    ScriptSystem();
    ~ScriptSystem();
    // Called after System is Initialized
    bool Initialize(void) override;

    // Called every frame
	void Update(float dt) override;

    // Returns name of System
	std::string GetName() override;

	enum SystemType sysType = SystemType::ST_Zilch;

	SystemType GetType() { return sysType; }


	void Shutdown() {}
    //Returns pointer to the required LibraryRef
    Zilch::LibraryRef *GetZilchLib(const char *ScriptName);

    //Returns pointer to the dependency library
    Zilch::ExecutableState *GetDependencies(){ return LinkedLibs; }

	
    //Hack!!!!!!!
    Zilch::LibraryRef lib;
  private:

    int ScriptCount;

    /* This class encompasses all compilation errors that can occur when compiling 
        Zilch code. Its responsibility is to provide friendly error messages, 
        error codes, and callbacks to the user*/
    Zilch::CompilationErrors Errors;

    Zilch::Debugger * Debugger;

    /* Link all the libraries together into one ExecutableState*/
    Zilch::ExecutableState* LinkedLibs;

    //LoadAllZilchFiles into the static Library
    void LoadZilchFiles(Zilch::Project & project);
    //Compile all the scripts
    void CompileScripts(Zilch::Project& project, Zilch::Module& dependencies);

    //Unordered map of compiled Zilch libraries
    std::unordered_map<const char*, Zilch::LibraryRef>* LibList;

  };  //class ScriptSystem

  extern ScriptSystem * ZILCH;

//Framework


