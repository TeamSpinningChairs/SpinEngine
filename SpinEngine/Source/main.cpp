/****************************************************************************/
/*!
\author Esteban Maldonado
\par    email: esteban.maldonado@digipen.edu
\par    Course: GAM 200
\brief

Main file for the entire game

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#include"Precompiled.h"
#include<Windows.h>
#include"Core\Engine.h"
#include "Console.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pScmdline, int iCmdshow)
{

#ifdef _DEBUG
	//THIS CREATES THE CONSOLE WINDOW
	///###############################################COMPLETELY LEGIT
	/*AllocConsole();

	HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
	int hCrt = _open_osfhandle((long)handle_out, _O_TEXT);
	FILE* hf_out = _fdopen(hCrt, "w");
	setvbuf(hf_out, NULL, _IONBF, 1);
	*stdout = *hf_out;

	HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
	hCrt = _open_osfhandle((long)handle_in, _O_TEXT);
	FILE* hf_in = _fdopen(hCrt, "r");
	setvbuf(hf_in, NULL, _IONBF, 128);
	*stdin = *hf_in;*/
	///###############################################COMPLETELY LEGIT
	SpinningConsole::Create_Spinning_Console("Spinning Console Window");
	//NOW USING THE SPINNING CONSOLE FROM MICAH RUST


#endif
	// use the console just like a normal one - printf(), getchar(), ...

  srand( time(NULL) );

  // Create the system object.
  Engine* System = new Engine("Vessoul", 1024, 768, false, hInstance);
  if (!System)
  {
    MessageBox(NULL, "ERROR: MAIN SYSTEM COULD NOT BE ALLOCATED\n\n", NULL, NULL);
    return 1;
  }

  // Initialize and run the system object.
  if(System->Initialize())
  {
    System->Run();
  }

  // Shutdown and release the system object.
  System->Shutdown();
  SPIN_ENGINE_SAFE_DELETE(System);
  SpinningConsole::Free_Spinning_Console();
  return 0;
}

// PARTICLE EMITTER TUTORIAL
//
//// include the basic windows header files and the Direct3D header file
//#include"Precompiled.h"
//// include the basic windows header files and the Direct3D header file
//#include <windows.h>
//#include <windowsx.h>
//#include <d3d9.h>
//#include <d3dx9.h>
//
//// define the screen resolution and keyboard macros
//#define SCREEN_WIDTH 800
//#define SCREEN_HEIGHT 600
//#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
//#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)
//
//// include the Direct3D Library files
//#pragma comment (lib, "d3d9.lib")
//#pragma comment (lib, "d3dx9.lib")
//
//// the PARTICLE class
//class PARTICLE
//{
//public:
//  PARTICLE()
//  {
//    reset_particle();
//  }
//
//  void render_particle();
//  void set_particle(float camx, float camy, float camz);
//  void run_particle(float seconds);
//  void reset_particle();
//
//  bool active;
//
//protected:
//  D3DXVECTOR3 position;
//  D3DXVECTOR3 velocity;
//  D3DXVECTOR3 acceleration;
//  float radius;
//  float lifespan;
//  float life;
//  D3DXMATRIX matRotateX;
//  D3DXMATRIX matRotateY;
//};
//
//// global declarations
//LPDIRECT3D9 d3d;    // the pointer to our Direct3D interface
//LPDIRECT3DDEVICE9 d3ddev;    // the pointer to the device class
//LPDIRECT3DVERTEXBUFFER9 t_buffer = NULL;    // the pointer to the particle's vertex buffer
//LPDIRECT3DVERTEXBUFFER9 g_buffer = NULL;    // the pointer to the grid's vertex buffer
//float camx, camy, camz;    // camera position
//// PARTICLE particle;
//static const int PARTICLECOUNT = 300;
//PARTICLE particle[PARTICLECOUNT];
//
//struct CUSTOMVERTEX { FLOAT X, Y, Z; DWORD COLOR; FLOAT U, V; };
//#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)
//struct GRIDVERTEX { D3DXVECTOR3 position; DWORD color; };
//#define GRIDFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)
//
//// texture declarations
//LPDIRECT3DTEXTURE9 texture;
//
//// function prototypes
//void initD3D(HWND hWnd);    // sets up and initializes Direct3D
//void render_frame(void);    // renders a single frame
//void cleanD3D(void);    // closes Direct3D and releases memory
//void init_graphics(void);    // 3D declarations
//void render_grid();
//void run_particles();
////float random_number(float low, float high);
//
//// the WindowProc function prototype
//LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
//
//// the entry point for any Windows program
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow)
//{
//  HWND hWnd;
//  WNDCLASSEX wc;
//
//  ZeroMemory(&wc, sizeof(WNDCLASSEX));
//
//  wc.cbSize = sizeof(WNDCLASSEX);
//  wc.style = CS_HREDRAW | CS_VREDRAW;
//  wc.lpfnWndProc = (WNDPROC)WindowProc;
//  wc.hInstance = hInstance;
//  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
//  wc.lpszClassName = "WindowClass";
//
//  RegisterClassEx(&wc);
//
//  hWnd = CreateWindowEx(NULL, "WindowClass", "Our Direct3D Program",
//    WS_OVERLAPPEDWINDOW, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, NULL,
//    NULL, hInstance, NULL);
//
//  ShowWindow(hWnd, nCmdShow);
//
//  // set up and initialize Direct3D
//  initD3D(hWnd);
//
//  // enter the main loop:
//  MSG msg;
//
//  while (TRUE)
//  {
//    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
//    {
//      if (msg.message == WM_QUIT)
//        break;
//
//      TranslateMessage(&msg);
//      DispatchMessage(&msg);
//    }
//
//    render_frame();
//
//    // check the 'escape' key
//    if (KEY_DOWN(VK_ESCAPE))
//      PostMessage(hWnd, WM_DESTROY, 0, 0);
//  }
//
//  // clean up DirectX and COM
//  cleanD3D();
//
//  return msg.wParam;
//}
//
//// this is the main message handler for the program
//LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//  switch (message)
//  {
//  case WM_DESTROY:
//  {
//    PostQuitMessage(0);
//    return 0;
//  } break;
//  }
//
//  return DefWindowProc(hWnd, message, wParam, lParam);
//}
//
//// this function initializes and prepares Direct3D for use
//void initD3D(HWND hWnd)
//{
//  d3d = Direct3DCreate9(D3D_SDK_VERSION);
//
//  D3DPRESENT_PARAMETERS d3dpp;
//
//  ZeroMemory(&d3dpp, sizeof(d3dpp));
//  d3dpp.Windowed = TRUE;
//  d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
//  d3dpp.hDeviceWindow = hWnd;
//  d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
//  d3dpp.BackBufferWidth = SCREEN_WIDTH;
//  d3dpp.BackBufferHeight = SCREEN_HEIGHT;
//  d3dpp.EnableAutoDepthStencil = TRUE;
//  d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
//
//  d3d->CreateDevice(D3DADAPTER_DEFAULT,
//    D3DDEVTYPE_HAL,
//    hWnd,
//    D3DCREATE_SOFTWARE_VERTEXPROCESSING,
//    &d3dpp,
//    &d3ddev);
//
//  init_graphics();
//
//  d3ddev->SetRenderState(D3DRS_LIGHTING, FALSE);
//  d3ddev->SetRenderState(D3DRS_ZENABLE, TRUE);
//
//  d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);    // turn on the color blending
//  d3ddev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTCOLOR);    // set source factor
//  d3ddev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);    // set dest factor
//  d3ddev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);    // set the operation
//
//  return;
//}
//
//// this is the function used to render a single frame
//void render_frame(void)
//{
//  d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
//  //d3ddev->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
//
//  d3ddev->BeginScene();
//
//  // calculate the camera position
//  static float index = 0.0f; index += 0.01f;
//  camx = cos(index) * 15.0f; camy = 2.0f; camz = sin(index) * 15.0f;
//
//  // set the view transform
//  D3DXMATRIX matView;
//  D3DXMatrixLookAtLH(&matView,
//    &D3DXVECTOR3(camx, camy, camz),//position of camera
//    &D3DXVECTOR3(0.0f, 0.0f, 0.0f), //view target
//    &D3DXVECTOR3(0.0f, 1.0f, 0.0f)); //up vector
//  d3ddev->SetTransform(D3DTS_VIEW, &matView);
//
//  // set the projection transform
//  D3DXMATRIX matProjection;
//  D3DXMatrixPerspectiveFovLH(&matProjection,
//    D3DXToRadian(45),
//    (FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT,
//    1.0f,
//    100.0f);
//  d3ddev->SetTransform(D3DTS_PROJECTION, &matProjection);
//
//  // render the grid
//  d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
//  render_grid();
//
//  run_particles();
//
//  d3ddev->EndScene();
//
//  d3ddev->Present(NULL, NULL, NULL, NULL);
//
//  return;
//}
//
//// this is the function that cleans up Direct3D and COM
//void cleanD3D(void)
//{
//  t_buffer->Release();    // close and release the particle vertex buffer
//  g_buffer->Release();    // close and release the grid vertex buffer
//  texture->Release();    // close and release the texture
//  d3ddev->Release();    // close and release the 3D device
//  d3d->Release();    // close and release Direct3D
//
//  return;
//}
//
//// this is the function that puts the 3D models into video RAM
//void init_graphics(void)
//{
//  // load the texture we will use
//  D3DXCreateTextureFromFile(d3ddev,
//    "fire.png",
//    &texture);
//
//  // create the vertices using the CUSTOMVERTEX
//  struct CUSTOMVERTEX t_vert[] =
//  {
//    { -1.0f, 1.0f, 0.0f, D3DCOLOR_XRGB(118, 89, 55), 1, 0, },
//    { -1.0f, -1.0f, 0.0f, D3DCOLOR_XRGB(118, 89, 55), 0, 0, },
//    { 1.0f, 1.0f, 0.0f, D3DCOLOR_XRGB(118, 89, 55), 1, 1, },
//    { 1.0f, -1.0f, 0.0f, D3DCOLOR_XRGB(118, 89, 55), 0, 1, },
//  };
//
//  // create a vertex buffer interface called t_buffer
//  d3ddev->CreateVertexBuffer(4 * sizeof(CUSTOMVERTEX),
//    0,
//    CUSTOMFVF,
//    D3DPOOL_MANAGED,
//    &t_buffer,
//    NULL);
//
//  VOID* pVoid;    // a void pointer
//
//  // lock t_buffer and load the vertices into it
//  t_buffer->Lock(0, 0, (void**)&pVoid, 0);
//  memcpy(pVoid, t_vert, sizeof(t_vert));
//  t_buffer->Unlock();
//
//  return;
//}
//
//// this is the function that positions, rotates, scales and renders the particle
//void PARTICLE::set_particle(float camx, float camy, float camz)
//{
//  // Before setting the world transform, do the intense mathematics!
//  // a. Calculate the Differences
//  static float difx, dify, difz;
//  difx = camx - position.x;
//  dify = camy - position.y;
//  difz = camz - position.z;
//
//  // b. Calculate the Distances
//  static float FlatDist, TotalDist;
//  FlatDist = sqrt(difx * difx + difz * difz);
//  TotalDist = sqrt(FlatDist * FlatDist + dify * dify);
//
//  // c. Y Rotation
//  D3DXMatrixIdentity(&matRotateY);
//  matRotateY._11 = matRotateY._33 = difz / FlatDist;    // cosY
//  matRotateY._31 = difx / FlatDist;    // sinY
//  matRotateY._13 = -matRotateY._31;    // -sinY
//
//  // d. X Rotation
//  D3DXMatrixIdentity(&matRotateX);
//  matRotateX._22 = matRotateX._33 = FlatDist / TotalDist;    // cosX
//  matRotateX._32 = dify / TotalDist;   // sinX
//  matRotateX._23 = -matRotateX._32;    // -sinX
//
//  // e. Tranlation
//  static D3DXMATRIX matTranslate;
//  D3DXMatrixTranslation(&matTranslate, position.x, position.y, position.z);
//
//  // f. Scaling
//  static D3DXMATRIX matScale;
//  D3DXMatrixIdentity(&matScale);
//  matScale._11 = matScale._22 = matScale._33 = radius;
//
//
//  // Now build the world matrix and set it
//  d3ddev->SetTransform(D3DTS_WORLD, &(matScale * matRotateX* matRotateY* matTranslate));
//
//  return;
//}
//
//// this function renders the particle
//void PARTICLE::render_particle()
//{
//  d3ddev->SetFVF(CUSTOMFVF);
//
//  d3ddev->SetStreamSource(0, t_buffer, 0, sizeof(CUSTOMVERTEX));
//
//  d3ddev->SetTexture(0, texture);
//
//  d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
//
//  return;
//}
//
//// this function updates the particle
//void PARTICLE::run_particle(float seconds)
//{
//  // handle lifespan
//  life += seconds;
//  if (life > lifespan)
//  {
//    reset_particle();
//    return;
//  }
//
//  velocity += acceleration * seconds;
//  position += velocity * seconds;
//
//}
//
//// this function sets the particle back to its original state
//void PARTICLE::reset_particle()
//{
//  active = false;
//  position.x = 0.0f;
//  position.y = 0.0f;
//  position.z = 0.0f;
//
//  velocity.x = -2.0f;
//  velocity.y = 3.0f;
//  velocity.z = -2.0f;
//
//  acceleration.x = 0.0f;
//  acceleration.y = 15.0f;
//  acceleration.z = 0.0f;
//  radius = 1.0f;
//  lifespan = 0.5f;
//  life = 0.0f;
//
//  return;
//}
//
//
//
//// this function goes through and runs each active particle
//void run_particles()
//{
//  // calculate elapsed time
//  static DWORD starting_point = GetTickCount(), time;
//  time = GetTickCount() - starting_point;
//  starting_point = GetTickCount();
//
//  // turn on alpha blending
//  d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
//
//  // run and render all active particles
//  d3ddev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
//  for (int index = 0; index < PARTICLECOUNT; index++)
//  {
//    if (particle[index].active)
//    {
//      particle[index].run_particle(time / 1000.0f);
//      particle[index].set_particle(camx, camy, camz);
//      particle[index].render_particle();
//    }
//  }
//  d3ddev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
//
//  for (int index = 0; index < time / 10.0f; index++)
//  {
//    for (int index = 0; index < PARTICLECOUNT; index++)
//    {
//      if (!particle[index].active) // if the particle is inactive...
//      {
//        particle[index].active = true;// ...activate it
//        break;    // then stop
//      }
//    }
//  }
//
//  return;
//}
//
//// this function renders the grid
//void render_grid()
//{
//  static bool InitNeeded = true;
//  static int GridSize = 10;
//  static D3DXMATRIX matIdentity;
//
//  d3ddev->SetFVF(GRIDFVF);
//
//  if (InitNeeded)
//  {
//    d3ddev->CreateVertexBuffer(sizeof(GRIDVERTEX) * GridSize * 9,
//      0,
//      GRIDFVF,
//      D3DPOOL_MANAGED,
//      &g_buffer,
//      0);
//
//    GRIDVERTEX* pGridData = 0;
//    g_buffer->Lock(0, 0, (void**)&pGridData, 0);
//
//    int index = 0;
//    for (; index <= GridSize * 4 + 1; index++)
//    {
//      float x = (index % 2) ? GridSize : -GridSize;
//      float y = 0.0f;
//      float z = index / 2 - GridSize;
//
//      pGridData[index].position = D3DXVECTOR3(x, y, z);
//      pGridData[index].color = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);
//    }
//
//    for (; index <= GridSize * 8 + 4; index++)
//    {
//      static int half = index;
//      float x = (index - half) / 2 - GridSize;
//      float y = 0.0f;
//      float z = (index % 2) ? -GridSize : GridSize;
//
//      pGridData[index].position = D3DXVECTOR3(x, y, z);
//      pGridData[index].color = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);
//    }
//
//    g_buffer->Unlock();
//
//    D3DXMatrixIdentity(&matIdentity);
//    InitNeeded = false;
//  }
//
//  d3ddev->SetTransform(D3DTS_WORLD, &matIdentity);
//  d3ddev->SetStreamSource(0, g_buffer, 0, sizeof(GRIDVERTEX));
//  d3ddev->SetTexture(0, NULL);
//  d3ddev->DrawPrimitive(D3DPT_LINELIST, 0, GridSize * 4 + 2);
//
//  return;
//}
//
////// this function generates random float values
////float rand_number(float low, float high)
////{
////  return low + ((float)((float)rand() / (float)RAND_MAX) * (float)((high)-(low)));
////}