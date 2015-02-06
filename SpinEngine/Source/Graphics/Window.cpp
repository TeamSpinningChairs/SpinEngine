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
#include "Precompiled.h"
#include "GraphicsManager.h"
#include "Window.h"
#include "Settings.h" //to check if tweakbar is currently enabled

namespace
{
	//Create global pointer to a DXApp object
	//This will be used to forward messages from a global space to
	//our user define message procedure. This is necessary due to the fact that
	//we cant create a method defined as WNDPROC.
	Window* g_pApp = NULL;
}

//GLOBAL MESSAGE PROCEDURE
//This is used to forward messages to our used defined procedure function
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  // Send event message to AntTweakBar, return if tweak bar handled it
  if (GlobalSettings != NULL && *GlobalSettings->TWEAK_BAR_ENABLED && *GlobalSettings->SHOWING_TWEAK_BAR && TwEventWin(hwnd, msg, wParam, lParam))
    return 0;

  if (g_pApp)
		return (g_pApp->MsgProc(hwnd, msg, wParam, lParam));
  else
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

Window::Window(UINT width, UINT height, std::string title, HINSTANCE	hAppInstance, HWND handle, bool full_screen) :
m_ClientWidth(width), m_ClientHeight(height), m_AppTitle(title), m_hAppWindow(handle),
m_hAppInstance(hAppInstance), m_Paused(false), m_pDevice3D(0), m_PmainGraphicsMang(0), 
frame_rate_controlled(false), m_EnableFullscreen(full_screen)
{
	g_pApp = this;
}

Window::~Window()
{
}

//Get Width & height
UINT Window::GetWindowWidth()
{
  if (m_EnableFullscreen)
  {
    return GetSystemMetrics(SM_CXSCREEN);
  }
	return m_ClientWidth;
}

UINT Window::GetWindowHeight()
{
  if (m_EnableFullscreen)
  {
    return GetSystemMetrics(SM_CYSCREEN);
  }
	return m_ClientHeight;
}

HWND& Window::GetHandle()
{
	return m_hAppWindow;
}

DWORD Window::GetWindowStyle()
{
	return m_WindowStyle;
}

bool Window::InitMainWindow()
{
	//First step:
	//Create a window class structure to define our window
	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(WNDCLASSEX)); //ZERO it out
	wcex.cbClsExtra = 0; //no extra bytes
	wcex.cbWndExtra = 0; //no extra bytes
	wcex.cbSize = sizeof(WNDCLASSEX); //set size in bytes
	wcex.style = CS_HREDRAW | CS_VREDRAW /*| WS_OVERLAPPED*/; //Basically states that window should be redraw both HORIZ. and VERT.
	wcex.hInstance = m_hAppInstance; //Set handle to application instance;
	wcex.lpfnWndProc = MainWndProc; //Set message procedure to our globally defined one
  
  HICON hIcon = static_cast<HICON>(
    LoadImage(NULL, "Assets\\Textures\\Setup-Icon.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE));
  //wcex.hIcon = LoadIcon(m_hAppInstance, IDI_APPLICATION); //Set window icon (standard application icon)
  wcex.hIcon = hIcon;
  wcex.hCursor = LoadCursor(NULL, IDC_ARROW); //Set window arrow (standard windows arrow)
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); //Set clear background
	wcex.lpszClassName = "WIN32WINDOWCLASS"; //Name it w.e you like. 
	wcex.lpszMenuName = NULL; //We are not using a menu at this time.
  wcex.hIconSm = hIcon; 
  
  m_WindowStyle =  WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX; //wcex.style;

	//Now we must register the window class
	//Here is an example of some simple error checking
	//This can be quite useful for larger projects to debug errors
	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL, "Failed to register window class", NULL, NULL);
		return false;
	}

	//Second step:
	//Cache the correct window dimensions
	RECT r = 
  { 
    0, 
    0, 
    m_EnableFullscreen ? GetSystemMetrics(SM_CXSCREEN) : m_ClientWidth,
    m_EnableFullscreen ? GetSystemMetrics(SM_CYSCREEN) : m_ClientHeight
  };
	AdjustWindowRect(&r, m_WindowStyle, false); //Use our window style
	int width = r.right - r.left;  //correct width based on requested client size
	int height = r.bottom - r.top;  //correct height based on requested client size
	int x = GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2; //Centers window on desktop
	int y = GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2; //Centers window on desktop

	//Third step:
	//Create our window
	//lpClassName: MUST BE SAME AS ABOVE FROM WINDOW CLASS
	//lpWindowTitle: SHOULD BE m_AppTitle.c_str()
	m_hAppWindow = CreateWindow("WIN32WINDOWCLASS", m_AppTitle.c_str(), m_WindowStyle, x, y,
		width, height, NULL, NULL, m_hAppInstance, NULL);
	//Check window creation
	if (!m_hAppWindow)
	{
		MessageBox(NULL, "Failed to create window", NULL, NULL);
		return false;
	}

	//Fourth step:
	//Show window
	//SW_SHOW: Stand window display code, take the place of nCmdShow from entry point.
	ShowWindow(m_hAppWindow, SW_SHOW);

	//If all succeeded return true
	return true;
}

void Window::EnableFullscreen(bool enable)
{
	if (enable)
	{
		//Cache desktop width and height
		int width = GetSystemMetrics(SM_CXSCREEN);
		int height = GetSystemMetrics(SM_CYSCREEN);

		//=GRAPHICS=
		m_d3dpp.BackBufferWidth = width;
		m_d3dpp.BackBufferHeight = height;
    m_d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;// D3DFMT_A8R8G8B8;
		m_d3dpp.Windowed = false;

		//Set window style to fullscreen friendly
		SetWindowLongPtr(m_hAppWindow, GWL_STYLE, WS_POPUP);

		//Need to set new position for window
    SetWindowPos(m_hAppWindow, NULL, 0, 0, width, height, SWP_NOZORDER | SWP_SHOWWINDOW);
	}

  else
  {
    //Set window back to windowed mode
    RECT r = { 0, 0, m_ClientWidth, m_ClientHeight };
    AdjustWindowRect(&r, m_WindowStyle, false);
    int w = r.right - r.left;
    int h = r.bottom - r.top;

    m_d3dpp.BackBufferFormat = /*D3DFMT_UNKNOWN;*/ D3DFMT_A8R8G8B8;
    m_d3dpp.BackBufferWidth = m_ClientWidth;
    m_d3dpp.BackBufferHeight = m_ClientHeight;
    m_d3dpp.Windowed = true;

    //Change window style back to windowed friendly
    SetWindowLongPtr(m_hAppWindow, GWL_STYLE, m_WindowStyle);

    //Set window position
    SetWindowPos(m_hAppWindow, NULL,
      GetSystemMetrics(SM_CXSCREEN) / 2 - w / 2,
      GetSystemMetrics(SM_CYSCREEN) / 2 - h / 2,
      w, h, SWP_NOZORDER | SWP_SHOWWINDOW);
	}

	//Reset our device to reflect the changes
  m_PmainGraphicsMang->OnLostDevice();
  (*(m_PmainGraphicsMang->GetDevice()))->Reset(&m_d3dpp);
  m_PmainGraphicsMang->OnResetDevice();
}

//Message procedure function.
//Windows OS runs of a message based system where all application
//that run on the OS send and receive messages that tell it what to do. To receive
//these messages our application needs what is called a message procedure function.
//Using this function we can "catch" these messages and tell out application what to do.
LRESULT Window::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  float some_speed = 0.9;


	//Switch statement on the message passed to us
  switch (msg)
  {

  case WM_SETCURSOR:

#ifndef _DEBUG
    // Turn off window cursor 
    SetCursor(NULL);

    (*m_PmainGraphicsMang->GetDevice())->ShowCursor(FALSE);

    return TRUE; // prevent Windows from setting cursor to window class cursor
#endif // _DEBUG

    break;
    //CASE: WM_DESTROY, our application is told to destroy itself
  case WM_DESTROY:
    PostQuitMessage(0); //Tell the application to quit
    return 0;

    //CASE: WM_ACTIVATE, our application is the active window
  case WM_ACTIVATE:
    //CHECK IF LOW ORDER BIT = INACTIVE
    if (wParam == WA_INACTIVE)
    {
      //Application should pause itself
      ShowWindow(hwnd, SW_MINIMIZE);
      m_Paused = true;
    }
    else if (wParam == WA_ACTIVE || wParam == WA_CLICKACTIVE)//Otherwise, the application is not paused
    {
      ShowWindow(hwnd, SW_SHOW);
      m_Paused = false;
      return 0;
    }
  case WM_SYSCOMMAND:
      if ((wParam & 0xFFF0) == SC_MINIMIZE)
          m_Paused = true;

      else if ((wParam & 0xFFF0) == SC_MAXIMIZE)
          m_Paused = false;

      break;


		//CASE: WM_KEYDOWN, user pressed a key on keyboard
	case WM_KEYDOWN:

		//Switch on the WPARAM, (which stores the keycode)
		switch (wParam)
		{
			//CASE: VK_ESCAPE, user pressed the escape key
		case VK_ESCAPE:
			//PostQuitMessage(0);
			return 0;
		case VK_RBUTTON:
			return 0;

#ifdef _DEBUG
		case VK_F1:
			//m_PmainGraphicsMang->SetHandleDeviceLost(!m_PmainGraphicsMang->IsHandlingDeviceLost());
        frame_rate_controlled = !frame_rate_controlled;
      return 0;
		case VK_F2:
			  m_EnableFullscreen = !m_EnableFullscreen;
        m_PmainGraphicsMang->m_bFullScreen = !m_PmainGraphicsMang->m_bFullScreen;
        EnableFullscreen(m_EnableFullscreen);
      return 0;

    case VK_F3:
      if (m_PmainGraphicsMang->debug_draw_on)
      {
        m_PmainGraphicsMang->SetDebugDrawingDefaultOff();
      }
      else
      {
        m_PmainGraphicsMang->SetDebugDrawingDefaultOn();
      }
      break;

    case VK_F4:
      if (m_PmainGraphicsMang->debug_draw_on)
      {
        m_PmainGraphicsMang->debug_draw_texture = !m_PmainGraphicsMang->debug_draw_texture;
      }
      break;
    case VK_F5:
      if (m_PmainGraphicsMang->debug_draw_on)
      {
        m_PmainGraphicsMang->debug_draw_colliders = !m_PmainGraphicsMang->debug_draw_colliders;
      }
      break;

    case 'A':
      m_PmainGraphicsMang->GetMainCamera()->GetPosition().x -= some_speed;
      break;

    case 'D':
      m_PmainGraphicsMang->GetMainCamera()->GetPosition().x += some_speed;
      break;

    case 'W':
      m_PmainGraphicsMang->GetMainCamera()->GetPosition().y += some_speed;
      break;

    case 'S':
      m_PmainGraphicsMang->GetMainCamera()->GetPosition().y -= some_speed;
      break;

    case 'Q':
      m_PmainGraphicsMang->GetMainCamera()->GetPosition().z -= some_speed;
      break;

    case 'E':
      m_PmainGraphicsMang->GetMainCamera()->GetPosition().z += some_speed;
      break;

    case 'F':
      m_PmainGraphicsMang->GetMainCamera()->GetRotation().x += some_speed;
      break;

    case 'H':
      m_PmainGraphicsMang->GetMainCamera()->GetRotation().x -= some_speed;
      break;

    case 'T':
      m_PmainGraphicsMang->GetMainCamera()->GetRotation().y += some_speed;
      break;

    case 'G':
      m_PmainGraphicsMang->GetMainCamera()->GetRotation().y -= some_speed;
      break;
#endif
		}
		return 0;
	}

	//Always return the default window procedure if we don't catch anything
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

bool Window::IsWindowPaused()
{
	return m_Paused;
}