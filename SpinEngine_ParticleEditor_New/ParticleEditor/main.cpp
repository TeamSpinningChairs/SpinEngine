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

#include <windows.h>
#include <windowsx.h>
#include <time.h>
#include "Core/IEntity.h"
#include "Graphics/ParticleEmitter.h"
#include <AntTweakBar.h>
#include <fstream>

using std::fstream;
using std::ios;

// define the screen resolution and keyboard macros
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

// include the Direct3D Library files
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

IEntity object;
TwBar* bar;
bool show_bar = true;

// global declarations
LPDIRECT3D9 d3d;    // the pointer to our Direct3D interface
IDirect3DDevice9* d3ddev;    // the pointer to the device class
LPDIRECT3DVERTEXBUFFER9 t_buffer = NULL;    // the pointer to the particle's vertex buffer
LPDIRECT3DVERTEXBUFFER9 g_buffer = NULL;    // the pointer to the grid's vertex buffer
float camx, camy, camz;    //camera position
D3DXCOLOR back_color = d3dColors::Black;

// texture declarations
LPDIRECT3DTEXTURE9 texture;

void TW_CALL SetMyStdStringCB(const void *value, void *clientData);
void TW_CALL GetMyStdStringCB(void *value, void * clientData);

void TW_CALL SetFileNameCB(const void *value, void *clientData);
void TW_CALL GetFileNamegCB(void *value, void * clientData);
void TW_CALL SaveParticleFile(void * clientData);
void TW_CALL LoadParticleFile(void * clientData);

// function prototypes
void initD3D(HWND hWnd);    // sets up and initializes Direct3D
void render_frame(void);    // renders a single frame
void cleanD3D(void);    // closes Direct3D and releases memory
void init_graphics(void);    // 3D declarations

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
// the entry point for any Windows program

DWORD starting_point , timey;
std::string save_load_file_name = "particle_emitter";
fstream file_json, file_zilch;

// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  // Send event message to AntTweakBar
  if (TwEventWin(hWnd, message, wParam, lParam))
    return 0;   // Event has been handled by AntTweakBar

  switch (message)
  {
  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;
    break;

  case WM_CHAR:
    if (wParam == VK_RETURN)
      show_bar = !show_bar;
    break;
  }

  return DefWindowProc(hWnd, message, wParam, lParam);
}

// this is the main message handler for the program
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow)
{
  __int64 countsPerSec = 0;
  QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
  float secPerCount = 10.0f / countsPerSec; //Conversion to seconds per count

  __int64 prevTime = 0;
  QueryPerformanceCounter((LARGE_INTEGER*)&prevTime);
  __int64 curTime = 0;
  
  float dt = 0;
  float frameTime(1.0f / 30.0f);

  HWND hWnd;
  WNDCLASSEX wc;

  ZeroMemory(&wc, sizeof(WNDCLASSEX));

  wc.cbSize = sizeof(WNDCLASSEX);
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc = (WNDPROC)WindowProc;
  wc.hInstance = hInstance;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.lpszClassName = "WindowClass";

  RegisterClassEx(&wc);

  hWnd = CreateWindowEx(NULL, "WindowClass", "Spin Engine - Particle Editor",
    WS_OVERLAPPEDWINDOW, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, NULL,
    NULL, hInstance, NULL);

  ShowWindow(hWnd, nCmdShow);

  // set up and initialize Direct3D
  initD3D(hWnd);

  //Initialize Ant Tweak Bar
  //################################
  ParticleEmitter* pe = reinterpret_cast<ParticleEmitter*>(
    object.GetComponent(CT_ParticleEmitter));

  TwInit(TW_DIRECT3D9, d3ddev);
  TwWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);

  bar = TwNewBar("Particle Properties YAYYYY!!!!!");

  // Message added to the help bar.
  TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar in a DirectX9 application.' ");
  // Change TweakBar color and use dark text
  TwDefine(" TweakBar color='128 224 160' text=dark ");

  TwAddVarRW(bar, "particle_lifetime", TW_TYPE_FLOAT, &pe->particle_lifetime, " step=0.01 label='particle_lifetime' ");
  TwAddVarRW(bar, "lifetime invariant", TW_TYPE_FLOAT, &pe->lifetime_invariant, " step=0.01 label='lifetime invariant' ");

  char buff[15]; itoa(MAX_PARTICLE_COUNT, buff, 10);
  std::string label = std::string("min=0 max=") + std::string(buff) + (" step=1 label='particle_count");
  TwAddVarRW(bar, "particle_count", TW_TYPE_INT32, &pe->particle_count, label.c_str());// " step=0.01 label='particle_count' ");


  TwEnumVal emitter_typesEV[] = { { ET_NORMAL, "Point" }, { ET_SQUARE, "Square" }, { ET_DIAMOND, "Diamond" }, { ET_CIRCLE, "Circle" } };
  TwType emitter_types = TwDefineEnum("EmitterType", emitter_typesEV, 4);
  TwAddVarRW(bar, "EmitterType", emitter_types, &pe->emit_type, NULL /* " min=0 max=3 step=1 label='emitter_type' "*/);

  TwAddVarRW(bar, "Back Color", TW_TYPE_COLOR4F, &back_color, "label='background color' "); //colororder=argb
  TwAddVarRW(bar, "Color", TW_TYPE_COLOR4F, &pe->my_color, "label='color' "); //colororder=argb
  TwAddVarRW(bar, "End Color", TW_TYPE_COLOR4F, &pe->my_color_end, "label='end_color' "); //colororder=argb
  TwAddVarRW(bar, "Blend Colors", TW_TYPE_BOOLCPP, &pe->blend_colors, " label='blend_colors' ");
  TwAddVarRW(bar, "Use operation", TW_TYPE_BOOLCPP, &pe->use_blending_operation, " label='use_blending_operation' ");

  TwEnumVal BFT_types_EV[] =
  {
    //ZERO / ONE
    { BFT_ZERO, "Zero" }, { BFT_ONE, "One" },
    //ALPHA VALUES
    { BFT_SRC_ALPHA, "Source Alpha" }, { BFT_INV_SRC_ALPHA, "Source Inv Alpha" }, { BFT_DEST_ALPHA, "Dest Alpha" }, { BFT_INV_DEST_ALPHA, "Dest Inv Alpha" },
    //COLOR VALUES
    { BFT_SRC_COLOR, "Source Color" }, { BFT_INV_SRC_COLOR, "Source Inv Color" }, { BFT_DEST_COLOR, "Dest Color" }, { BFT_INV_DEST_COLOR, "Dest Inv Color" }
  };
  TwType bft_types = TwDefineEnum("Blend Factor", BFT_types_EV, 10);
  TwAddVarRW(bar, "Use source factor", TW_TYPE_BOOLCPP, &pe->use_source_factor, " label='use_source_factor' ");
  TwAddVarRW(bar, "Source Factor", bft_types, &pe->source_factor, NULL);

  TwEnumVal blend_typesEV[] = { { BT_ADD, "Add" }, { BT_SUBSTRACT, "Substract" }, { BT_REVSUBSTRACT, "Rev_Substract" }, { BT_MIN, "Min" }, { BT_MAX, "Max" } };
  TwType blend_types = TwDefineEnum("Blend Operation", blend_typesEV, 5);
  TwAddVarRW(bar, "Blend Operation", blend_types, &pe->blend_type, NULL);
  TwAddVarRW(bar, "Use dest factor", TW_TYPE_BOOLCPP, &pe->use_dest_factor, " label='use_dest_factor' ");
  TwAddVarRW(bar, "Dest Factor", bft_types, &pe->dest_factor, NULL);

  TwAddVarCB(bar, "texture name", TW_TYPE_STDSTRING, SetMyStdStringCB, GetMyStdStringCB, &pe->texture_name, "texture_name"); // set clientData to NULL if no client data

  TwAddVarRW(bar, "scale", TW_TYPE_FLOAT, &pe->scale, " step=0.005 label='scale' ");
  TwAddVarRW(bar, "end_scale", TW_TYPE_FLOAT, &pe->end_scale, " step=0.005 label='end_scale' ");

  TwAddVarRW(bar, "angle_1", TW_TYPE_FLOAT, &pe->angle_1, " step=0.01 label='angle_1' ");
  TwAddVarRW(bar, "engle_2", TW_TYPE_FLOAT, &pe->angle_2, " step=0.01 label='angle_2' ");

  TwAddVarRW(bar, "speed_1", TW_TYPE_FLOAT, &pe->speed_1, " step=0.01 label='speed_1' ");
  TwAddVarRW(bar, "speed_2", TW_TYPE_FLOAT, &pe->speed_2, " step=0.01 label='speed_2' ");

  TwAddVarRW(bar, "acceleration_1", TW_TYPE_FLOAT, &pe->acceleration_1, " step=0.01 label='acceleration_1' ");
  TwAddVarRW(bar, "acceleration_2", TW_TYPE_FLOAT, &pe->acceleration_2, " step=0.01 label='acceleration_2' ");

  TwAddVarRW(bar, "spin", TW_TYPE_FLOAT, &pe->spin, " step=0.01 label='spin' ");
  TwAddVarRW(bar, "spin_variance", TW_TYPE_FLOAT, &pe->spin_variance, " step=0.01 label='spin_variance' ");

  TwAddVarRW(bar, "Square width", TW_TYPE_FLOAT, &pe->width, " step=0.01 label='Square width' ");
  TwAddVarRW(bar, "Square height", TW_TYPE_FLOAT, &pe->height, " step=0.01 label='Square height' ");

  TwAddVarRW(bar, "Circle radius", TW_TYPE_FLOAT, &pe->radius, " step=0.01 label='Circle radius' ");

  TwAddVarCB(bar, "file name", TW_TYPE_STDSTRING, SetFileNameCB, GetFileNamegCB, &save_load_file_name, "file name");
  TwAddButton(bar, "Serialize", SaveParticleFile, NULL, " label='Serialize File' ");
  TwAddButton(bar, "Load", LoadParticleFile, NULL, " label='Load File' ");
  //################################

  // enter the main loop:
  MSG msg = { 0 };
  starting_point = GetTickCount();
  while (TRUE)
  {
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      if (msg.message == WM_QUIT)
        break;

      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }

    object.Update( static_cast<float>( GetTickCount() - starting_point) / 1000.0f  );
    render_frame();

    // check the 'escape' key
    if (KEY_DOWN(VK_ESCAPE))
      PostMessage(hWnd, WM_DESTROY, 0, 0);
    
    //Limit Frame Rate
    //Capture current time in counts
    QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&curTime));
    //Calculate delta time
    dt = (curTime - prevTime) * secPerCount;
    //Limit to 60 fps (comment out if you want to check actual frame times)

    while (dt < frameTime)
    {
      QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&curTime));
      dt = (curTime - prevTime) * secPerCount;
    }
    
    Sleep(1);
  }

  // Terminate AntTweakBar
  TwTerminate();

  // clean up DirectX and COM
  cleanD3D();

  return msg.wParam;
}

void init_graphics(void)
{
  // load the texture we will use
  D3DXCreateTextureFromFile(d3ddev, "fire.png", &texture);

  // create the vertices using the CUSTOMVERTEX
  struct CUSTOMVERTEX t_vert[] =
  {
    { -1.0f, 1.0f, 0.0f, D3DCOLOR_XRGB(118, 89, 55), 1, 0, },
    { -1.0f, -1.0f, 0.0f, D3DCOLOR_XRGB(118, 89, 55), 0, 0, },
    { 1.0f, 1.0f, 0.0f, D3DCOLOR_XRGB(118, 89, 55), 1, 1, },
    { 1.0f, -1.0f, 0.0f, D3DCOLOR_XRGB(118, 89, 55), 0, 1, },
  };

  // create a vertex buffer interface called t_buffer
  d3ddev->CreateVertexBuffer(4 * sizeof(CUSTOMVERTEX), 0, CUSTOMFVF, D3DPOOL_MANAGED,
    &t_buffer, NULL);

  VOID* pVoid; // a void pointer

  // lock t_buffer and load the vertices into it
  t_buffer->Lock(0, 0, (void**)&pVoid, 0);
  memcpy(pVoid, t_vert, sizeof(t_vert));
  t_buffer->Unlock();

  return;
}


// this function initializes and prepares Direct3D for use
void initD3D(HWND hWnd)
{
  d3d = Direct3DCreate9(D3D_SDK_VERSION);

  D3DPRESENT_PARAMETERS d3dpp;

  ZeroMemory(&d3dpp, sizeof(d3dpp));
  d3dpp.Windowed = TRUE;
  d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
  d3dpp.hDeviceWindow = hWnd;
  d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
  d3dpp.BackBufferWidth = SCREEN_WIDTH;
  d3dpp.BackBufferHeight = SCREEN_HEIGHT;
  d3dpp.EnableAutoDepthStencil = TRUE;
  d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

  d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, 
    D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3ddev);

  init_graphics();

  d3ddev->SetRenderState(D3DRS_LIGHTING, FALSE);
  d3ddev->SetRenderState(D3DRS_ZENABLE, TRUE);

  d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);    // turn on the color blending
  //d3ddev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTCOLOR);    // set source factor
  //d3ddev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);    // set dest factor
  //d3ddev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);    // set the operation

  //give the object a particle emitter
  object.AddGameComponent(CT_ParticleEmitter, new ParticleEmitter(&object, d3ddev, emitter_types::ET_CIRCLE, BT_ADD, 500, "HHUE", d3dColors::Blue));
  
  ParticleEmitter* pe = reinterpret_cast<ParticleEmitter*>(
    object.GetComponent(CT_ParticleEmitter) );
  pe->Initialize();
  pe->particle_lifetime = 1.0f;
}

// this is the function used to render a single frame
void render_frame(void)
{
  d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, back_color, 1.0f, 0);

  d3ddev->BeginScene();

  // calculate the camera position
  static float index = 0.0f; index += 0.01f;
  camx = cos(index) * 15.0f; camy = 2.0f; camz = sin(index) * 15.0f;

  // set the view transform
  D3DXMATRIX matView;
  D3DXMatrixLookAtLH(&matView,
    //&D3DXVECTOR3(camx, camy, camz),//position of camera
    &D3DXVECTOR3(0.0f, 0.0f, -20.0f),
    &D3DXVECTOR3(0.0f, 0.0f, 0.0f), //view target
    &D3DXVECTOR3(0.0f, 1.0f, 0.0f)); //up vector
  d3ddev->SetTransform(D3DTS_VIEW, &matView);

  // set the projection transform
  D3DXMATRIX matProjection;
  D3DXMatrixPerspectiveFovLH(&matProjection, D3DXToRadian(45),
    (FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT, 1.0f, 100.0f);

  //D3DXMatrixOrthoLH(&matProjection, SCREEN_WIDTH, SCREEN_HEIGHT, 1.0f, 100.0f);

  d3ddev->SetTransform(D3DTS_PROJECTION, &matProjection);

  // render the grid
  //d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);


  reinterpret_cast<ParticleEmitter*>(object.GetComponent(CT_ParticleEmitter))->Render(d3ddev);

  d3ddev->EndScene();

  // Draw tweak bars
  if (show_bar)
    TwDraw();

  d3ddev->Present(NULL, NULL, NULL, NULL);

  return;
}

// this is the function that cleans up Direct3D and COM
void cleanD3D(void)
{
  SPIN_ENGINE_SAFE_RELEASE(t_buffer);    // close and release the particle vertex buffer
  SPIN_ENGINE_SAFE_RELEASE(g_buffer);    // close and release the grid vertex buffer
  SPIN_ENGINE_SAFE_RELEASE(texture);    // close and release the texture
  SPIN_ENGINE_SAFE_RELEASE(d3ddev);    // close and release the 3D device
  SPIN_ENGINE_SAFE_RELEASE(d3d);    // close and release Direct3D
}

void TW_CALL SetMyStdStringCB(const void *value, void *clientData)
{
  // Set: copy the value of texture name from AntTweakBar
  const std::string *srcPtr = static_cast<const std::string *>(value);
  reinterpret_cast<ParticleEmitter*>(object.GetComponent(CT_ParticleEmitter))->change_texture(*srcPtr);

}

void TW_CALL GetMyStdStringCB(void *value, void * clientData)
{
  // Get: copy the value of texture name to AntTweakBar
  std::string *destPtr = static_cast<std::string *>(value);
  TwCopyStdStringToLibrary(*destPtr, 
    (reinterpret_cast<ParticleEmitter*>(object.GetComponent(CT_ParticleEmitter))->texture_name));
}

void TW_CALL SetFileNameCB(const void *value, void *clientData)
{
  // Set: copy the value of texture name from AntTweakBar
  const std::string *srcPtr = static_cast<const std::string *>(value);
  save_load_file_name = *srcPtr;
}

void TW_CALL GetFileNamegCB(void *value, void * clientData)
{
  // Get: copy the value of texture name to AntTweakBar
  std::string *destPtr = static_cast<std::string *>(value);
  TwCopyStdStringToLibrary(*destPtr, save_load_file_name);
}

void TW_CALL SaveParticleFile(void * clientData)
{
  ParticleEmitter* pe = reinterpret_cast<ParticleEmitter*>(object.GetComponent(CT_ParticleEmitter));

  file_json.open((save_load_file_name + ".json").c_str(), std::fstream::out | std::fstream::trunc);
  file_zilch.open((save_load_file_name + ".data").c_str(), std::fstream::out | std::fstream::trunc);
  
  //If zilch file is open then save it as a .data file
  if (file_zilch.is_open() )
  {
    file_zilch << save_load_file_name << " = \n{\n";
    file_zilch << "\tfloat current_PE_lifespan = " << pe->current_PE_lifespan<<",\n";
    file_zilch << "\tfloat PE_lifespan = " << pe->PE_lifespan << ",\n";
    
    file_zilch << "\tfloat scale = " << pe->scale << ",\n";
    file_zilch << "\tfloat end_scale = " << pe->end_scale << ",\n";

    file_zilch << "\tfloat start_delay = " << pe->scale << ",\n";

    file_zilch << "\tfloat particle_lifetime = " << pe->particle_lifetime << ",\n";
    file_zilch << "\tfloat lifetime_invariant = " << pe->lifetime_invariant << ",\n";

    file_zilch << "\tfloat angle_1 = " << pe->angle_1 << ",\n"; file_zilch << "\tfloat angle_2 = " << pe->angle_2 << ",\n";
    file_zilch << "\tfloat speed_1 = " << pe->speed_1 << ",\n"; file_zilch << "\tfloat speed_2 = " << pe->speed_2 << ",\n";
    file_zilch << "\tfloat acceleration_1 = " << pe->acceleration_1 << ",\n"; file_zilch << "\tfloat acceleration_2 = " << pe->acceleration_2 << ",\n";
    file_zilch << "\tfloat spin = " << pe->spin << ",\n"; file_zilch << "\tfloat spin_variance = " << pe->spin_variance << ",\n";

    file_zilch << "\tfloat gravity = " << pe->gravity << ",\n";

    file_zilch << "\tVec4 my_color = [" << 255.0f * pe->my_color.r << "," << 255.0f * pe->my_color.g << "," << 255.0f * pe->my_color.b << "," << 255.0f * pe->my_color.a << "],\n";
    file_zilch << "\tVec4 my_color_end = [" << 255.0f * pe->my_color_end.r << "," << 255.0f * pe->my_color_end.g << "," << 255.0f * pe->my_color_end.b << "," << 255.0f * pe->my_color_end.a << "],\n";
    file_zilch << "\tbool blend_colors = " << pe->blend_colors << ",\n";

    file_zilch << "\tint particle_count = " << pe->particle_count << ",\n";

    file_zilch << "\tfloat x_diff_from_owner = " << pe->x_diff_from_owner << ",\n";
    file_zilch << "\tfloat y_diff_from_owner = " << pe->y_diff_from_owner << ",\n";

    file_zilch << "\tint emit_type = " << pe->emit_type << ",\n";
    file_zilch << "\tint blend_type = " << pe->blend_type << ",\n";
    file_zilch << "\tint source_factor = " << pe->source_factor << ",\n";
    file_zilch << "\tint dest_factor = " << pe->dest_factor << ",\n";

    file_zilch << "\tfloat width = " << pe->width << ",\n";
    file_zilch << "\tfloat height = " << pe->height << ",\n";
    file_zilch << "\tfloat radius = " << pe->radius << ",\n";

    file_zilch << "\tstring texture_name = \"" << pe->texture_name << "\",\n";

    file_zilch << "},";
    file_zilch.close();

    //MessageBox(NULL, "Settings saved succesfully to .data file", NULL, NULL);
  }
  
  //If json file is open then save it as a .json file
  if( file_json.is_open() )
  {
    file_json << "{\n";
    
    file_json << "\t\"current_PE_lifespan\": " << pe->current_PE_lifespan << "\n";
    file_json << "\t\"PE_lifespan\": " << pe->PE_lifespan << "\n";

    file_json << "\t\"scale\": " << pe->scale << "\n";
    file_json << "\t\"end_scale\": " << pe->end_scale << "\n";

    file_json << "\t\"start_delay\": " << pe->start_delay << "\n";

    file_json << "\t\"particle_lifetime\": " << pe->particle_lifetime << "\n";
    file_json << "\t\"lifetime_invariant\": " << pe->lifetime_invariant << "\n";

    file_json << "\t\"angle_1\": " << pe->angle_1 << "\n"; file_json << "\t\"angle_2\": " << pe->angle_2 << "\n";
    file_json << "\t\"speed_1\": " << pe->speed_1 << "\n"; file_json << "\t\"speed_2\": " << pe->speed_2 << "\n";
    file_json << "\t\"acceleration_1\": " << pe->acceleration_1 << "\n"; file_json << "\t\"acceleration_2\": " << pe->acceleration_2 << "\n";
    file_json << "\t\"spin\": " << pe->spin << "\n"; file_json << "\t\"spin_variance\": " << pe->spin_variance << "\n";

    file_json << "\t\"gravity\": " << pe->gravity << "\n";

    file_json << "\t\"my_color\": " << pe->my_color << "\n";
    file_json << "\t\"my_color_end\": " << pe->my_color_end << "\n";
    file_json << "\t\"blend_colors\": " << pe->blend_colors << "\n";

    file_json << "\t\"particle_count\": " << pe->particle_count << "\n";

    file_json << "\t\"x_diff_from_owner\": " << pe->x_diff_from_owner << "\n"; 
    file_json << "\t\"y_diff_from_owner\": " << pe->y_diff_from_owner << "\n";

    file_json << "\t\"emit_type\": " << pe->emit_type << "\n";
    file_json << "\t\"blend_type\": " << pe->blend_type << "\n";
    file_json << "\t\"source_factor\": " << pe->source_factor << "\n";
    file_json << "\t\"blend_type\": " << pe->blend_type << "\n";

    file_json << "\t\"width\": " << pe->width << "\n"; file_json << "\t\"height\": " << pe->height << "\n";
    file_json << "\t\"radius\": " << pe->radius << "\n";

    file_json << "\t\"texture_name\": \"" << pe->texture_name << "\"\n";

    file_json << "}";

    file_json.close();
  }
}

void TW_CALL LoadParticleFile(void * clientData)
{

}