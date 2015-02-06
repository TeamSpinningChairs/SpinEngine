/******************************************************************************/
  /*!
  \file   Precompiled.h
  \author Henry Morgan
  \par    email: henry.morgan\@digipen.edu
  \par    DigiPen login: henry.morgan

  Only add things that rarely change. If one header file in here is modified,
  the entire thing will need to be recompiled.

  Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once

//Debug:
#define _NO_DEBUG_HEAP
//Zilch (this has to go above Windows.h)
#include "Zilch.hpp"
//#pragma comment(lib, "Zilch_2013_x86_MTd.lib") //multi-threaded debug
//Replace with Zilch_2013_x86_MT (multi-threaded) for release
//I don't think we'll use MD (multi-threaded DLL) or MDd, but they can be put in

//Windows:
#define NOMINMAX
#define _WINSOCKAPI_
#include <Windows.h>

//DirectX: (check to see if you need these, Esteban)
//#include <d3d9.h>
//#include <d3dx9.h>

//#include "DXUtil.h"

#include <windows.h> //Include the windows header file, This contains all you will need to create a basic window
#include <string>	//needed for std::string
#include <sstream> //needed for std::stringstream (check CalculateFPS())
#include <d3dx9.h>
#include <d3d9.h> //needed for Direct3D
#include <dxerr.h>
#include <d3d9types.h>
#include <dxerr.h>

//Standard Library:
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <stack>
#include <list>
#include <iostream>
#include <math.h>
#include <sstream>
#include <ctime>
#include <random>
#include <utility>
#include <thread> //THANK YOU, C++ 11, THANK YOU VERY MUCH

//Our utilities:
#include "Utilities\Colors.h"
#include "MemoryManager.h"


//Our math library:
#include "mathlib.h"

//Our Graphics:
//#include"Graphics\GraphicsManager.h"
////Link to necessary DIRECTX libraries
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxerr.lib") //used for our HR macro. DxTraceW is located here

// Our physics:
#include "Physics\PhysicsManager.h"
#include "Physics\primitives.h"
#include "Physics\rigidbody.h"
#include "Physics\collision_broad.h"
#include "Physics\collision_narrow.h"

//Input:
#include <Xinput.h>
#pragma comment (lib, "XInput.lib")

//AntTweakBar:
#include "AntTweakBar.h"
#pragma comment (lib, "AntTweakBar.lib")

//Serializer
#include "DataNode.h"


//Game state management:
#include "GameState\GameStateManager.h"
#pragma warning( disable : 4244 )



///////////////////////////////////////////////////////////////////////////
//Ignore the stuff below this. It's junk that I might double-check later.//
///////////////////////////////////////////////////////////////////////////
//#include <Windows.h>
//#include <WindowsX.h> 

// stack trace
//#include <DbgHelp.h>
//#pragma comment(lib, "DbgHelp.lib")

//debug
//#include "DebugDiagnostic.h"

// stl
//#include <iostream>
//#include <iomanip>
//#include <sstream>
//#include <string>
//#include <algorithm>
//#include <math.h>
//#include <vector>
//#include <set>
//#include <hash_map>
//#include <map>
//#include <unordered_map>
//#include <unordered_set>
//#include <list>
//#include "StdTypedef.h"
//typedef std::string String;
//#include <fstream>
//#include <functional>

/*
#include <stdlib.h> // rand, srand
#include <time.h>   // time
#include <ctime> //other time
*/


// math
//#include "GMath.h"

// utilities
//#include "Utilities.h"

//#include <cstdarg> // Variadic macros
//#include "Macro.h"
//#include "consts.h"

//#include "XMLSerializer.h"

//tweakbar
//#include "AntTweakBar.h"


// interfaces
//#include "System.h"
//#include "Component.h" pure interface files for components and system


// xbox controller
//#include <XInput.h>
//#pragma comment(lib, "XInput.lib")

