/******************************************************************************/
/*!
\file   ZInterface.h
\author Joshua Biggs
\par    Course: GAM205
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  This file contains several helper macros for programming in ZILCH!
*/
/******************************************************************************/

#pragma once
#include "Precompiled.h"
#include "ZilchCompiledLib.h"
#include "SoundManager.h"
#include "Engine.h"

using namespace std;
  class Sound;

	class ZilchFile
	{
	public:
		ZilchDeclareBaseType(ZilchFile, Zilch::TypeCopyMode::ReferenceType);
		
		ZilchFile(ifstream* file);
		~ZilchFile();

		
		Zilch::String ReadLine(Zilch::Integer line);
		

	private:
		ifstream* File;
		vector<std::string> Data;
	};

	class ZInterface
	{
	public:
		ZilchDeclareBaseType(ZInterface, Zilch::TypeCopyMode::ReferenceType);
		static Engine* Engine;
		static FactoryAccess* Factory;
		static SoundManager* Sound;
		static PhysicsManager* Physics;
		static InputManager* Input;

		static void QuitGame();


		//static CreateGameObject(String Name, String SpriteName);
		static IEntity* CreateAtPosition(String filepath, Real3 position);
		//static Sound* GetSound(String soundName);
		static void TogglePaused();
		static Boolean IsPaused();
		static void ZInterface::SetPaused(bool state);
		static void SetSceneShader(String shader);

		static IEntity* FindObjectByName(Zilch::String name);

		static int DT;
		
		static Real2* VecToReal(Vector2D* vec);
		static Real3* VecToReal(Vector3D* vec);
		static Real4* VecToReal(Vector4D* vec);

		static Vector2D RealToVec(Real2* real);
		static Vector3D RealToVec(Real3* real);
		static Vector4D RealToVec(Real4* real);
		
		static ZilchFile* OpenFile(Zilch::String filepath);
		
	};

	class ZArray
	{
	public:
		ZilchDeclareBaseType(ZArray, Zilch::TypeCopyMode::ReferenceType);
		ZArray(Zilch::Array<IEntity*>* arr);
		~ZArray();
		unsigned Length;

		IEntity* Get(unsigned index);

		Zilch::Array<IEntity*> Objects;
	private:

	};

	
// namespace Framework