/****************************************************************************/
/*!
\author Esteban Maldonado
\par    email: esteban.maldonado@digipen.edu
\par    Course: GAM 200
\brief

DirectX libraries and linkage.

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/ #pragma once  #define WIN32_LEAN_AND_MEAN  //strips away any nonessentials (i.e. winsockets, encryption, etc.) //we only really care about the standard application stuff  #include <windows.h> //Include the windows header file, This contains all you will need to create a basic window #include <string>	//needed for std::string #include <sstream> //needed for std::stringstream (check CalculateFPS()) #include <d3dx9.h> #include <d3d9.h> //needed for Direct3D #include <dxerr.h>  //#include "Precompiled.h" //windows.h, string, sstream, d3dx9.h, d3d9.h, dxerr.h //sstream is needed for std::stringstream (check CalculateFPS()) //windows.h contains all you will need to create a basic window   ////Link to necessary DIRECTX libraries #pragma comment(lib, "d3d9.lib") #pragma comment(lib, "d3dx9.lib") #pragma comment(lib, "dxerr.lib") //used for our HR macro. DxTraceW is located here   //#if defined(_DEBUG) /*| defined(_DEBUG) */ //#ifndef HR //#define HR(x) \ //{ \ //	HRESULT hr = x; \ //	if(FAILED(hr)) \ //  { \ //    DXTraceW(__FILEW__, __LINE__, hr, L#x, TRUE); \ //  } \ //}  //#endif //#ifndef HR //#define HR(x) x; //#endif //#endif 