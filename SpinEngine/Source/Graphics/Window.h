/****************************************************************************/
/*!
\author Esteban Maldonado
\par    email: esteban.maldonado@digipen.edu
\par    Course: GAM 200
\brief

Class interface for the Window object. Handles basic OS messages and creates
the main window.

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#pragma once

#include "Precompiled.h" //Windows.h, string

class GraphicsManager;

class Window
{
public:

	Window(UINT width, UINT height, std::string title, HINSTANCE	hAppInstance, HWND handle, bool full_screen = false);
	~Window();

	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam); //message procedure function

	//Initializes main application window
	bool InitMainWindow();

	//return the window handle
	HWND& GetHandle();

	//Get Width & height
	UINT GetWindowWidth();
	UINT GetWindowHeight();

	//Get the window style
	DWORD GetWindowStyle();

	//Get if window is paused
	bool IsWindowPaused();

	//Enables fullscreen
	void EnableFullscreen(bool enable);

	//D3D Specific public properties
	D3DPRESENT_PARAMETERS m_d3dpp;
	IDirect3DDevice9* m_pDevice3D;			//Direct3D device interface
	GraphicsManager* m_PmainGraphicsMang;

  bool frame_rate_controlled;

private:
	HWND m_hAppWindow;				//HANDLE to application window
	HINSTANCE	m_hAppInstance;			//HANDLE to application instance
	UINT m_ClientWidth;			//Requested client width
	UINT m_ClientHeight;			//Requested client height
	std::string	m_AppTitle;				//Application title (window title bar)
	DWORD	m_WindowStyle;			//Window style (e.g. WS_OVERLAPPEDWINDOW)


	bool m_Paused;				//True if application pause, false otherwise
	bool m_EnableFullscreen;		//True to enable fullscreen, false otherwise
};