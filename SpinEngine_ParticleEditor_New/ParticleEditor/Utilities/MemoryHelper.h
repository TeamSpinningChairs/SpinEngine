/****************************************************************************/
/*!
\author Esteban Maldonado
\par    email: esteban.maldonado@digipen.edu
\par    Course: GAM 200
\brief

Templated functions for safe deletion.

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#pragma once

namespace Memory
{
	template<typename T>
	void SafeDelete(T& t)
	{
		if (t)
		{
			delete t;
			t = 0;
		}
	}

	template<typename T>
	void SafeDeleteArr(T& t)
	{
		if (t)
		{
			delete[] t;
			t = 0;
		}
	}

	template<typename T>
	void SafeRelease(T& t)
	{
		if (t)
		{
			t->Release();
			t = 0;
		}
	}
}

#ifndef SPIN_ENGINE_SAFE_DELETE
#define SPIN_ENGINE_SAFE_DELETE(X) \
{ \
  Memory::SafeDelete(X); \
}
#endif

#ifndef SPIN_ENGINE_SAFE_DELETE_ARR
#define SPIN_ENGINE_SAFE_DELETE_ARR(X) \
{ \
  Memory::SafeDeleteArr(X); \
} 
#endif

#ifndef SPIN_ENGINE_SAFE_RELEASE
#define SPIN_ENGINE_SAFE_RELEASE(X) \
{ \
  Memory::SafeRelease(X); \
} 
#endif