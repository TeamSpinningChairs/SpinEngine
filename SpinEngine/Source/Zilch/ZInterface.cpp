/******************************************************************************/
/*!
\file   ZInterface.cpp
\author Joshua Biggs
\par    Course: GAM205
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  This file contains several helper macros for programming in ZILCH!
*/
/******************************************************************************/

#include "Precompiled.h"
#include "ZInterface.h"
#include "Zilch.hpp"
#include "FactoryAccess.h"


	Engine* ZInterface::Engine = ENGINE;
	FactoryAccess* ZInterface::Factory = GlobalFactory;
	SoundManager* ZInterface::Sound = sound;
	PhysicsManager* ZInterface::Physics = physics;
	InputManager* ZInterface::Input = nullptr;

	ZilchDefineType(ZilchFile, SpinningZilch)
	{
		type->HandleManager = ZilchManagerId(Zilch::PointerManager);
		ZilchBindMethod(ReadLine);
	}

	ZilchDefineType(ZArray, SpinningZilch)
	{
		type->HandleManager = ZilchManagerId(Zilch::PointerManager);
		ZilchBindMethod(Get);
		ZilchBindFieldGet(Length);
		//ZilchBindFieldGet(Objects);
	}


	ZilchDefineType(ZInterface, SpinningZilch)
	{
		type->HandleManager = ZilchManagerId(Zilch::PointerManager);

		ZilchBindStaticMethod(OpenFile);
		ZilchBindStaticMethod(QuitGame);
		ZilchBindStaticFieldGetAs(Input, "InputManager");
		//ZilchBindStaticFieldGetAs(ObjectSys, "ObjectSystem");
		//ZilchBindStaticFieldGetAs(WindowSys, "WindowSystem");
		//ZilchBindStaticFieldGetAs(Physics, "Physics");
		//ZilchBindStaticFieldGet(ResourceSystem);
		ZilchBindStaticMethod(TogglePaused);
		ZilchBindStaticMethod(IsPaused);
		ZilchBindStaticMethod(SetPaused);
		ZilchBindStaticMethod(SetSceneShader);
		
		
	}

	IEntity* CreateAtPosition(String filepath, Real3 position)
	{

		//IEntity* obj = ZInterface::Factory->CreateGameObject()
		return nullptr;
	}

	ZilchFile* ZInterface::OpenFile(Zilch::String filepath)
	{
		ifstream myfile;

		myfile.open(filepath.c_str(), std::ios::out);
		
		ErrorIf(myfile.is_open() == false, "Could not open Level File");
		/*Zilch::Array<Zilch::String>* returnfile = new Zilch::Array<Zilch::String>;
		if (myfile.is_open())
		{
		while (getline(myfile, line))
		{
		returnfile->append(Zilch::String(line.c_str()));
		}
		}
		else
		{
		std::cout << "ERROR COULD NOT OPEN FILE" << std::endl;
		}*/

		return new ZilchFile(&myfile);


	}

	void ZInterface::SetSceneShader(String shader)
	{
		
	}

	

	void ZInterface::TogglePaused()
	{
		//CORE->TogglePaused();
	}

	Boolean ZInterface::IsPaused()
	{
		return false;//CORE->IsPaused();
	}

	void ZInterface::SetPaused(bool state)
	{
		return; //CORE->SetPaused(state);
	}
	
	Real2* ZInterface::VecToReal(Vector2D* vec)
	{
		return &Real2(vec->x, vec->y);
	}
	Real3* ZInterface::VecToReal(Vector3D* vec)
	{
		return &Real3(vec->x, vec->y, vec->z);
	}
	Real4* ZInterface::VecToReal(Vector4D* vec)
	{
		return &Real4(vec->x, vec->y, vec->z, vec->w);
	}

	Vector2D ZInterface::RealToVec(Real2* real)
	{
		Vector2D vec;
		vec.x = real->x;
		vec.y = real->y;
		return vec;
	}
	Vector3D ZInterface::RealToVec(Real3* real)
	{
		Vector3D vec;
		vec.x = real->x;
		vec.y = real->y;
		vec.z = real->z;
		return vec;
	}
	Vector4D ZInterface::RealToVec(Real4* real)
	{
		Vector4D vec;
		vec.x = real->x;
		vec.y = real->y;
		vec.z = real->z;
		vec.w = real->w;
		return vec;
	}
	


	ZilchFile::ZilchFile(ifstream* file)
	{
		File = file;
		std::string line;
		
		while (getline(*File, line))
		{
			Data.push_back(line);
		}
	}

	ZilchFile::~ZilchFile()
	{
		File->close();

		delete File;
    File = nullptr;
	}

	Zilch::String ZilchFile::ReadLine(Zilch::Integer line)
	{
		if ((unsigned)line > Data.size() - 1)
		{
			return Zilch::String('\0');
		}

		return Zilch::String(Data.at(line).c_str());
		//File->
	}

	void ZInterface::QuitGame()
	{
		Engine->Shutdown();
	}

	ZArray::ZArray(Zilch::Array<IEntity*>* arr)
	{
		Objects = *arr;
		Length = Objects.size();
	}

	IEntity* ZArray::Get(unsigned index)
	{
		return Objects[index];
	}

	ZArray::~ZArray()
	{
		Objects.deallocate();
	}