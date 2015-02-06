/****************************************************************************/
/*!
\author Esteban Maldonado
\par    email: esteban.maldonado@digipen.edu
\par    Course: GAM 200
\brief

Class interface for the Graphics system, handles all rendering on the game.

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#include "Precompiled.h"
#include"GraphicsManager.h"
#include"Engine.h"
#include "Settings.h" //TWEAK_BAR_ENABLED and ShowingTweakBar
#include "MenuController.h"

//const DWORD d3dVertex::CUSTOMVERTEX::FVF = CUSTOMFVF;

GraphicsManager::GraphicsManager(UINT width, UINT height, std::string title, 
  HINSTANCE	hAppInstance, HWND handle, D3DCOLOR backgrdClr, bool full_screen) : ISystem("Graphics", ST_Graphics),
  mainWindow(new Window(width, height, title, hAppInstance, handle, full_screen)), m_pDirect3D(0),
  m_pDevice3D(0), m_bHandleDeviceLost(true), m_bDeviceLost(false), m_DevType(D3DDEVTYPE_HAL), 
  backgroundColor(backgrdClr), default_editor_camera(NULL), m_bFullScreen(full_screen),
  
  //DEBUG DRAW INITIALIZERS
  debug_draw_on(false),
  debug_draw_transform_information(false),
  debug_draw_rigid_body_information(false),
  debug_draw_colliders(false),
  debug_draw_texture(false),
  debug_draw_wireframe(false),
  debug_draw_wireframe_color(d3dColors::SimpsonsYellow),
  debug_draw_collision_wireframe_color(d3dColors::Blue),
  debug_draw_use_editor_cam(false),
  Debug_Wireframe_vBuffer(NULL),
  Debug_CircleWireframe_vBuffer(NULL)
{
  mainWindow->m_PmainGraphicsMang = this;
  main_camera = &default_editor_camera;
}

GraphicsManager::~GraphicsManager()
{
}

bool GraphicsManager::Initialize()
{
	//Initialize main window
  if (!mainWindow->InitMainWindow())
  {
    //default_editor_camera.Set_Width_Height(mainWindow->GetWindowWidth(),
    //  mainWindow->GetWindowHeight());
    return false;
  }
  if (!InitDirect3D())
    return false;



  isInitialized = true;
	//If all succeeds return true
	return true;
}

Window* GraphicsManager::GetWindow()
{
	return mainWindow;
}

bool GraphicsManager::InitDirect3D()
{
	//There are a few steps to successfully intializing a Direct3D device object
	//The first is to actually create the Direct3D interface object

	//Step 1:
	m_pDirect3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (!m_pDirect3D)
	{
		MessageBox(NULL, "Failed to create Direct3D interface object", NULL, NULL);
		return false;
	}

	//Step 2:
	//We must check the display mode. The display mode defines
	//the pixel format our application is rendering to and only certain
	//screens have certain pixel formats supported. This is why we must
	//check if the formats we want are ok.

	//Cache the adapter display mode into our member variable
	m_pDirect3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &m_Mode);
	//Check both our WINDOWED and FULLSCREEN formats
	m_pDirect3D->CheckDeviceType(D3DADAPTER_DEFAULT, m_DevType, m_Mode.Format, m_Mode.Format, true); //WINDOWED

	//NOTE: D3DFMT_X8R8G8B8 is a widely supported display format. This is what we will be using.
	m_pDirect3D->CheckDeviceType(D3DADAPTER_DEFAULT, m_DevType, D3DFMT_X8R8G8B8, D3DFMT_X8R8G8B8, false); //FULLSCREEN

	//Step 3:
	//We must check to see if our graphics device supports hardware accelerated
	//vertex processing. Another term for this is HARDWARE TRANSFORM AND LIGHTING.
	//I'm going to assume virtually everyone following these tutorials has a device capable
	//of HWTRANSLIGHT
	int vp = 0;
	D3DCAPS9 devCaps;
	//Cache device caps 
	m_pDirect3D->GetDeviceCaps(D3DADAPTER_DEFAULT, m_DevType, &devCaps);
	if (devCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		//Our device supports transformations in hardware
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	//Step 4:
	//We must initialize our present parameters structure
	//This will tell our device how it should render to the back buffer, how many
	//buffers it has, etc. It is basically a swap chain description.
	ZeroMemory(&m_d3dpp, sizeof(D3DPRESENT_PARAMETERS));
  m_d3dpp.BackBufferWidth = mainWindow->GetWindowWidth();
	m_d3dpp.BackBufferHeight = mainWindow->GetWindowHeight();
  m_d3dpp.BackBufferFormat = m_bFullScreen ? D3DFMT_X8R8G8B8 : D3DFMT_A8R8G8B8; //32 bit format
  m_d3dpp.Windowed = !m_bFullScreen; //start windowed
	m_d3dpp.BackBufferCount = 1; //Double buffered. 
	m_d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE; //No multisampling (way too intensive)
	m_d3dpp.MultiSampleQuality = 0;
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_d3dpp.hDeviceWindow = mainWindow->GetHandle();
	m_d3dpp.Flags = 0;
	m_d3dpp.EnableAutoDepthStencil = TRUE;
  m_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;// D3DFMT_D24S8;
	m_d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	//m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
  m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	//pass a copy of the present parameters to the window.
	mainWindow->m_d3dpp = m_d3dpp;

	//Step 5:
	//Create our device
  m_pDirect3D->CreateDevice(D3DADAPTER_DEFAULT, m_DevType, mainWindow->GetHandle(), vp, &m_d3dpp, &m_pDevice3D);
  
	mainWindow->m_pDevice3D = m_pDevice3D;

	//Create the sprite
	//if (FAILED(D3DXCreateSprite(m_pDevice3D, &m_sprite)))
	//{
	//	return false;
	//}

  //Initialize Vertex and Index Buffers

    //Create Vertex Vuffer
    d3dVertex::CUSTOMVERTEX verts[NUM_VERTS] =
    {
      //U V Coordinates
      //(0,0)--------(0,1)
      //
      //
      //
      //(0,1)--------(1,1)

      //Front Face
      d3dVertex::CUSTOMVERTEX(-1.0f, 1.0f, 0.0f, d3dColors::White, 0.0f, 0.0f),
      d3dVertex::CUSTOMVERTEX(1.0f, 1.0f, 0.0f, d3dColors::White, 0.25f, 0.0f),
      d3dVertex::CUSTOMVERTEX(-1.0f, -1.0f, 0.0f, d3dColors::White, 0.0f, 0.25f),
      d3dVertex::CUSTOMVERTEX(1.0f, -1.0f, 0.0f, d3dColors::White, 0.25f, 0.25f)
    };

    //Creating the vertex buffer uppon VB
    m_pDevice3D->CreateVertexBuffer(NUM_VERTS * sizeof(d3dVertex::CUSTOMVERTEX), 0,
      d3dVertex::CUSTOMVERTEX::FVF, D3DPOOL_MANAGED, &vBuffer, NULL);

    VOID* pVerts;
    //LOCK THE MEMORY TO MAKE SURE DATA IS PASSED THROUGH
    vBuffer->Lock(0, sizeof(verts), (void**)&pVerts, 0);
    memcpy(pVerts, verts, sizeof(verts));
    vBuffer->Unlock();

    WORD indices[NUM_INDICES] =
    {
      //Front face
      0, 1, 2,
      2, 1, 3
    };

    //Creating the index buffer uppon iBuffer
    m_pDevice3D->CreateIndexBuffer(NUM_INDICES * sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16,
      D3DPOOL_MANAGED, &iBuffer, NULL);

    VOID* pIndices;
    //LOCK THE MEMORY TO MAKE SURE DATA IS PASSED THROUGH
    iBuffer->Lock(0, sizeof(indices), (void**)&pIndices, 0);
    memcpy(pIndices, indices, sizeof(indices));
    iBuffer->Unlock();

  //END Initialize Vertex and Index Buffers

  //Initialize font variables
    m_FontDesc.Height = 16;
    m_FontDesc.Width = 8;
    m_FontDesc.Weight = FW_BOLD;
    m_FontDesc.MipLevels = 0;
    m_FontDesc.Italic = false;
    m_FontDesc.CharSet = DEFAULT_CHARSET;
    m_FontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
    m_FontDesc.Quality = DEFAULT_QUALITY;
    m_FontDesc.PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
    //_tcscpy(m_FontDesc.FaceName, _T("Times New Roman"));
    std::strcpy(m_FontDesc.FaceName, "Times New Roman");
    D3DXCreateFontIndirect(m_pDevice3D, &m_FontDesc, &m_Font);
  //End 

  //Initialize Debug Draw Vertex Buffer
    d3dVertex::VertexPositionColor debug_draw_verts[5] =
    {
      d3dVertex::VertexPositionColor(-1.0f, 1.0f, 0.0f, debug_draw_wireframe_color),
      d3dVertex::VertexPositionColor(-1.0f, -1.0f, 0.0f, debug_draw_wireframe_color),
      d3dVertex::VertexPositionColor(1.0f, -1.0f, 0.0f, debug_draw_wireframe_color),
      d3dVertex::VertexPositionColor(1.0f, 1.0f, 0.0f, debug_draw_wireframe_color),
      d3dVertex::VertexPositionColor(-1.0f, 1.0f, 0.0f, debug_draw_wireframe_color)
    };
    
    m_pDevice3D->CreateVertexBuffer(sizeof(debug_draw_verts), 0,
      d3dVertex::VertexPositionColor::FVF, D3DPOOL_MANAGED, &Debug_Wireframe_vBuffer, NULL);

    VOID* pDDVerts;
    //LOCK THE MEMORY TO MAKE SURE DATA IS PASSED THROUGH
    Debug_Wireframe_vBuffer->Lock(0, sizeof(debug_draw_verts), (void**)&pDDVerts, 0);
    memcpy(pDDVerts, debug_draw_verts, sizeof(debug_draw_verts));
    Debug_Wireframe_vBuffer->Unlock();
  //END Initialize Debug Draw Vertex Buffer

  //Initialize Box Debug Draw Vertex Buffer
  m_pDevice3D->CreateVertexBuffer(sizeof(d3dVertex::box_debug_draw_verts), 0,
    d3dVertex::VertexPositionColor::FVF, D3DPOOL_MANAGED, &Debug_BoxWireframe_vBuffer, NULL);

  //LOCK THE MEMORY TO MAKE SURE DATA IS PASSED THROUGH
  Debug_BoxWireframe_vBuffer->Lock(0, sizeof(d3dVertex::box_debug_draw_verts), (void**)&pDDVerts, 0);
  memcpy(pDDVerts, d3dVertex::box_debug_draw_verts, sizeof(d3dVertex::box_debug_draw_verts));
  Debug_BoxWireframe_vBuffer->Unlock();
  //END Initialize Box Debug Draw Vertex Buffer
    
  //Initialize Circle Debug Draw Vertex Buffer
  m_pDevice3D->CreateVertexBuffer(sizeof(d3dVertex::circle_debug_draw_verts), 0,
    d3dVertex::VertexPositionColor::FVF, D3DPOOL_MANAGED, &Debug_CircleWireframe_vBuffer, NULL);

  //LOCK THE MEMORY TO MAKE SURE DATA IS PASSED THROUGH
  Debug_CircleWireframe_vBuffer->Lock(0, sizeof(d3dVertex::circle_debug_draw_verts), (void**)&pDDVerts, 0);
  memcpy(pDDVerts, d3dVertex::circle_debug_draw_verts, sizeof(d3dVertex::circle_debug_draw_verts));
  Debug_CircleWireframe_vBuffer->Unlock();
  //END Initialize Circle Debug Draw Vertex Buffer

	//Initialize all graphics
	OnResetDevice();

	//If this all succeeds return true
	return true;
}

void GraphicsManager::Update(float dt)
{
  //These aren't updated in Factory, so having them all update here isn't redundant
  //Draw backgrounds
  for (unsigned i = 0; i < bgSpriteCollection.size(); ++i)
  {
    if (bgSpriteCollection[i]->GetActive())
    {
      bgSpriteCollection[i]->Update(dt);
    }
  }

  //Draw tiles and custom objects
  //(tiles go first and will stay beneath custom objects)
	for (unsigned i = 0; i < spriteCollection.size(); ++i)
	{
    if (spriteCollection[i]->GetActive() )
    {
      spriteCollection[i]->Update(dt);
    }
	}
  //Update UI sprites
  for (unsigned i = 0; i < uiSpriteCollection.size(); ++i)
  {
    if (uiSpriteCollection[i]->GetActive())
    {
      uiSpriteCollection[i]->Update(dt);
    }
  }
  

  //Draw particles (particles will stay on top of everything else)
  //Any particles that we need drawn behind objects will require a
  //particle equivalent to the background sprite collection.
  if (!gGameStatePaused)
  {
    for (unsigned i = 0; i < particleEmitterColl.size(); ++i)
    {
      if (particleEmitterColl[i]->GetActive())
      {
        particleEmitterColl[i]->Update(dt);
      }
    }
  }
}

void GraphicsManager::AddSpriteRend(SpriteRenderer* sprite)
{
	spriteCollection.push_back(sprite);
}

void GraphicsManager::RemoveSpriteRend(SpriteRenderer* sprite)
{
  for(SpriteRendVecIter iter = spriteCollection.begin(); iter != spriteCollection.end(); ++iter)
  {
    if (*iter == sprite)
    {
      spriteCollection.erase(iter);
      return;
    }
  }
}

void GraphicsManager::AddBGSpriteRend(SpriteRenderer* sprite)
{
  bgSpriteCollection.push_back(sprite);
}

void GraphicsManager::RemoveBGSpriteRend(SpriteRenderer* sprite)
{
  for (SpriteRendVecIter iter = bgSpriteCollection.begin(); iter != bgSpriteCollection.end(); ++iter)
  {
    if (*iter == sprite)
    {
      bgSpriteCollection.erase(iter);
      return;
    }
  }
}

void GraphicsManager::AddUISpriteRend(SpriteRenderer* sprite)
{
  //SpriteRendVecIter it = uiSpriteCollection.begin();
  //uiSpriteCollection.insert(it,sprite);
  uiSpriteCollection.push_back(sprite);
}
void GraphicsManager::RemoveUISpriteRend(SpriteRenderer* sprite)
{
  for (SpriteRendVecIter iter = uiSpriteCollection.begin(); iter != uiSpriteCollection.end(); ++iter)
  {
    if (*iter == sprite)
    {
      uiSpriteCollection.erase(iter);
      return;
    }
  }
}

void GraphicsManager::Render()
{
	HRESULT hr;
  //if (m_bHandleDeviceLost)
  //{
	if (m_bDeviceLost)
	{
		if ( FAILED(hr = m_pDevice3D->TestCooperativeLevel()) )
		{
      if (hr == D3DERR_DEVICELOST)
      {
        return;
      }

			else if( hr == D3DERR_DEVICENOTRESET)
			{
				OnLostDevice();

				//Reset the device
				hr = m_pDevice3D->Reset(&m_d3dpp);

				if ( FAILED(hr) )
				{
					MessageBox(NULL, "FAILED TO RESET DEVICE", NULL, NULL);
				}

				//Restore Device Objects
				OnResetDevice();
			}
		}
	}
	//}

  //SET THE VIEW MATRIX AND PROJECTION MATRIX
  D3DXMATRIX view, proj;

	//Clears the back buffer
  if (main_camera)
  {
    if (debug_draw_on)
    {
      m_pDevice3D->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, d3dColors::Black, 1.0f, 0);
    }
    else
    {
      m_pDevice3D->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, main_camera->GetBackColor(), 1.0f, 0);
    }
    
    if (m_bFullScreen)
    {
      main_camera->Set_Width_Height(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
    }
    else
    {
      main_camera->Set_Width_Height(mainWindow->GetWindowWidth(), mainWindow->GetWindowHeight());
    }

    main_camera->GetViewMatrix(view);
    main_camera->GetProjMatrix(proj);

    //Setting the view as the transform
    m_pDevice3D->SetTransform(D3DTS_VIEW, &view);

    //Setting the projection as the transform
    m_pDevice3D->SetTransform(D3DTS_PROJECTION, (D3DMATRIX*)&proj);
  }
  else
  {
    if (debug_draw_on)
    {
      m_pDevice3D->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, d3dColors::Black, 1.0f, 0);
    }
    else
    {
      m_pDevice3D->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, default_editor_camera.GetBackColor(), 1.0f, 0);
    }

    if (m_bFullScreen)
    {
      default_editor_camera.Set_Width_Height(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
    }
    else
    {
      default_editor_camera.Set_Width_Height(mainWindow->GetWindowWidth(), mainWindow->GetWindowHeight());
    }
    default_editor_camera.GetViewMatrix(view);
    default_editor_camera.GetProjMatrix(proj);

    //Setting the view as the transform
    m_pDevice3D->SetTransform(D3DTS_VIEW, &view);

    //Setting the projection as the transform
    m_pDevice3D->SetTransform(D3DTS_PROJECTION, (D3DMATRIX*)&proj);
  }
  //m_pDevice3D->SetRenderState(D3DRS_CLIPPING, true);
  
  //END SET THE VIEW MATRIX AND PROJECTION MATRIX

	m_pDevice3D->BeginScene();
  m_pDevice3D->SetRenderState(D3DRS_ZENABLE, TRUE);    // turn on the z-buffer
  m_pDevice3D->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
  #ifndef _DEBUG
  for (unsigned i = 0; i < bgSpriteCollection.size(); ++i)
  {
    if (bgSpriteCollection[i]->GetActive() && bgSpriteCollection[i]->GetOwner()->IsObjectVisible())
    {
      bgSpriteCollection[i]->Draw(&vBuffer, &iBuffer, debug_draw_on);
    }
  }
  for (unsigned i = 0; i < spriteCollection.size(); ++i)
  {
    if (spriteCollection[i]->GetActive() && spriteCollection[i]->GetOwner()->IsObjectVisible() && IsInBounds(spriteCollection[i]))
    {
      spriteCollection[i]->Draw(&vBuffer, &iBuffer, debug_draw_on);
    }
  }
  #endif

  //Debug mode for rendering sprites
  #ifdef _DEBUG
  //Draw Background
  for (unsigned i = 0; i < bgSpriteCollection.size(); ++i)
  {
    if (!debug_draw_on)
    {
      if (bgSpriteCollection[i]->GetActive() && bgSpriteCollection[i]->GetOwner()->IsObjectVisible())
      {
        bgSpriteCollection[i]->Draw(&vBuffer, &iBuffer, debug_draw_on);
      }
    }
  }
  //Draw sprites (UI is drawn after this, scroll down)
	for (unsigned i = 0; i < spriteCollection.size(); ++i)
	{
    if (!debug_draw_on)
    {
      if (spriteCollection[i]->GetActive() && spriteCollection[i]->GetOwner()->IsObjectVisible() 
        && IsInBounds(spriteCollection[i]))
      {
        spriteCollection[i]->Draw(&vBuffer, &iBuffer, debug_draw_on);
      }
    }
      
    //Additional Debug draw logic...
    //If debug drawing is on then proceed
    else //if (debug_draw_on)
    {
      //If we draw the wireframe...
      if (debug_draw_wireframe)
      {
        setWireframeTransform(spriteCollection[i]->GetOwner()->GetTransform());
        m_pDevice3D->SetStreamSource(0, Debug_Wireframe_vBuffer, 0, sizeof(d3dVertex::VertexPositionColor));
        m_pDevice3D->SetFVF(d3dVertex::VertexPositionColor::FVF);
        m_pDevice3D->SetTransform(D3DTS_WORLD, &debug_draw_wireframe_world);
        m_pDevice3D->SetTexture(0, NULL);
        m_pDevice3D->DrawPrimitive(D3DPT_LINESTRIP, 0, 4);
      }
      if(debug_draw_texture)
      {
        if (spriteCollection[i]->GetActive() && spriteCollection[i]->GetOwner()->IsObjectVisible())
        {
          spriteCollection[i]->Draw(&vBuffer, &iBuffer, debug_draw_on);
        }
      }

      if (debug_draw_colliders)
      {
        //Search for box colliders
        AABB* boxColl = reinterpret_cast<AABB*>(
          spriteCollection[i]->GetOwner()->GetComponent(CT_BoxCollider));

        if(boxColl)
        {
          setBoxCollWireframeTransform(spriteCollection[i]->GetOwner()->GetTransform(), boxColl->halfSize);
          m_pDevice3D->SetStreamSource(0, Debug_BoxWireframe_vBuffer, 0, sizeof(d3dVertex::VertexPositionColor));
          m_pDevice3D->SetFVF(d3dVertex::VertexPositionColor::FVF);
          m_pDevice3D->SetTransform(D3DTS_WORLD, &debug_draw_wireframe_world);
          m_pDevice3D->SetTexture(0, NULL);
          m_pDevice3D->DrawPrimitive(D3DPT_LINESTRIP, 0, 4);
        }

        Circle* circleColl = reinterpret_cast<Circle*>(
          spriteCollection[i]->GetOwner()->GetComponent(CT_CircleCollider));

        if(circleColl)
        {
          setCircleCollWireframeTransform(spriteCollection[i]->GetOwner()->GetTransform(), circleColl->radius);
          m_pDevice3D->SetStreamSource(0, Debug_CircleWireframe_vBuffer, 0, sizeof(d3dVertex::VertexPositionColor));
          m_pDevice3D->SetFVF(d3dVertex::VertexPositionColor::FVF);
          m_pDevice3D->SetTransform(D3DTS_WORLD, &debug_draw_wireframe_world);
          m_pDevice3D->SetTexture(0, NULL);
          m_pDevice3D->DrawPrimitive(D3DPT_LINESTRIP, 0, d3dVertex::circle_vert_count - 1);
        }

        RigidBody* rigidBody = reinterpret_cast<RigidBody*>(spriteCollection[i]->GetOwner()->GetComponent(CT_Body));
        if (rigidBody && spriteCollection[i]->GetOwner()->GetComponent(CT_Player_Controller))
        {
          Vector3D vec = rigidBody->GetOwner()->GetTransform()->GetPosition();

          D3DVIEWPORT9 pViewport;
          m_pDevice3D->GetViewport(&pViewport);

          D3DXVECTOR3  temp_pos;
          D3DXVec3Project(&temp_pos, &D3DXVECTOR3(vec.x, vec.y, vec.z), &pViewport, &proj, &view, (D3DXMATRIX*)&debug_draw_wireframe_world);

          //left top right bottom
          RECT formatRect = { temp_pos.x, temp_pos.y, temp_pos.x + 200, temp_pos.y + 200 };
          GetClientRect(mainWindow->GetHandle(), &formatRect);
          GetClientRect(NULL, &formatRect);

          std::stringstream ss;
          ss << "X = " << vec.x << "\n\n";
          ss << "Y = " << vec.y << "\n\n";
          ss << "Z = " << vec.z << "\n\n";

          m_Font->DrawText(0, ss.str().c_str(), -1, &formatRect, DT_CENTER | DT_VCENTER, d3dColors::Red);
        }
      }
    }
    //END Additional Debug draw logic...
	}
  #endif

  if (!gGameStatePaused)
  {
    for (unsigned i = 0; i < particleEmitterColl.size(); ++i)
    {
      if (particleEmitterColl[i]->GetActive()) //&& particleEmitterColl[i]->GetOwner()->IsObjectVisible())
      {
        particleEmitterColl[i]->Render(m_pDevice3D);
      }
    }
  }
  //Draw UI
  for (unsigned i = 0; i < uiSpriteCollection.size(); ++i)
  {
      if (!debug_draw_on)
      {
          if (uiSpriteCollection[i]->GetActive() && uiSpriteCollection[i]->GetOwner()->IsObjectVisible())
          {
              uiSpriteCollection[i]->Draw(&vBuffer, &iBuffer, debug_draw_on);
          }
      }
  }

  #ifdef _DEBUG
  //Draw the tweak bar (@put in a enabled/disabled check here)
  if (*GlobalSettings->TWEAK_BAR_ENABLED && *GlobalSettings->SHOWING_TWEAK_BAR)
    TwDraw();
  #endif

  m_pDevice3D->EndScene();
  
	//Present the backbuffer to our window
  hr = m_pDevice3D->Present(0, 0, 0, 0);
  if (hr == D3DERR_DEVICELOST || hr == D3DERR_DEVICEREMOVED)
	{
		m_bDeviceLost = true;
	}
}

bool GraphicsManager::IsInBounds(SpriteRenderer *rend)
{
    //if ( strcmp(rend->GetOwner()->GetName(), "WallImage") != 0 )
    //{
        if (abs(rend->GetPosition().x - GetMainCamera()->GetPosition().x) > 24.0f)
            return false;
        if (abs(rend->GetPosition().y - GetMainCamera()->GetPosition().y) > 18.0f)
            return false;

        return true;
    //}
    //return true;
  //rend->GetPosition().x
}

void GraphicsManager::Shutdown()
{
  //Release Direct3D from memory
  SPIN_ENGINE_SAFE_RELEASE(m_pDevice3D);
  SPIN_ENGINE_SAFE_RELEASE(m_pDirect3D);
  SPIN_ENGINE_SAFE_RELEASE(iBuffer);
  SPIN_ENGINE_SAFE_RELEASE(vBuffer);
  SPIN_ENGINE_SAFE_RELEASE(Debug_Wireframe_vBuffer);
  SPIN_ENGINE_SAFE_RELEASE(Debug_CircleWireframe_vBuffer);
  SPIN_ENGINE_SAFE_RELEASE(m_Font);
  SPIN_ENGINE_SAFE_DELETE(mainWindow);
  SPIN_ENGINE_SAFE_RELEASE(ParticleEmitter::v_buffer);
  SPIN_ENGINE_SAFE_RELEASE(ParticleEmitter::i_buffer);

  for (unsigned i = 0; i < spriteCollection.size(); i++)
  {
    if (spriteCollection[i])
    {
      spriteCollection[i]->Release();
      SPIN_ENGINE_SAFE_DELETE(spriteCollection[i]);
    }
  }
  spriteCollection.clear();
}

void GraphicsManager::OnLostDevice()
{
	//m_sprite->OnLostDevice();
  m_Font->OnLostDevice();

  //PAUSE ALL SOUNDS
  //  ENGINE->m_Sound->PauseAll();
  if (gGameStatePaused == false && gGameStateCurr == GS_LEVEL)
  {
      MenuMessage msg1(NULL);
      msg1.Type = UPDATE_POSITION;
      msg1.newPosition = ENGINE->m_Graphics->GetMainCamera()->GetPosition();
      GlobalSystem->MyDispatchMessage(&msg1);

      MenuMessage msg_pause_activate(NULL);
      msg_pause_activate.Type = ActivatePause;

      GlobalSystem->MyDispatchMessage(&msg_pause_activate);
      //ShowPauseMenu();
      gGameStatePaused = true;
      ENGINE->m_Sound->PauseAll();
  }

}

//Resets or re-initializes D3D
void GraphicsManager::OnResetDevice()
{
	//reset the m_sprite
	//m_sprite->OnResetDevice();

  //INITIALIZE DEFAULT CAMERA
  D3DXMATRIX view, proj;

  default_editor_camera.SetPosition(0.0f, 0.0f, -30.0f);
  default_editor_camera.GetViewMatrix(view);

  if (m_bFullScreen)
  {
    default_editor_camera.Set_Width_Height(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
  }
  else
  {
    default_editor_camera.Set_Width_Height(mainWindow->GetWindowWidth(), mainWindow->GetWindowHeight());
  }
  default_editor_camera.GetProjMatrix(proj);

  //Setting the view as the transform
  m_pDevice3D->SetTransform(D3DTS_VIEW, &view);

  //Setting the projection as the transform
  m_pDevice3D->SetTransform(D3DTS_PROJECTION, (D3DMATRIX*)&proj);

	//This code sets texture filters, which helps to smooth out distortions when you scale a texture
  m_pDevice3D->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pDevice3D->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pDevice3D->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	//disable lighting
	m_pDevice3D->SetRenderState(D3DRS_LIGHTING, false);

	//The following code specifies an alpha test and reference value
	m_pDevice3D->SetRenderState(D3DRS_ALPHAREF, 10);
	m_pDevice3D->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//Now to set up alpha blending
	m_pDevice3D->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pDevice3D->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pDevice3D->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//Indicates that we are using 2D Texture coordinates (U,V)
	m_pDevice3D->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
  
  //device->SetTransform(D3DTS_TEXTURE0, &textureTransform); //<-- TEXTURE TRANSFORM
  m_pDevice3D->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

  m_pDevice3D->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
  m_pDevice3D->SetRenderState(D3DRS_ZENABLE, true);

  //RESUME ALL SOUNDS
  if (isInitialized)
  {
    ENGINE->m_Sound->ResumeAll();
  }
  m_bDeviceLost = false;
}

bool GraphicsManager::IsDeviceLost()
{
	//Cache the state of the device
	HRESULT hr = m_pDevice3D->TestCooperativeLevel();
	if (hr == D3DERR_DEVICELOST) //If it is lost
	{
		//Sleep cpu for 1/10 of a second
		//Sleep(10);
		return true;
	}
	else if (hr == D3DERR_DRIVERINTERNALERROR) //Fatal error occured
	{
		//Display message box
		MessageBox(NULL, "FATAL INTERNAL ERROR DETECTED.\n APPLICATION QUITTING", NULL, NULL);
		PostQuitMessage(0); //Quit application
		return true;
	}
	else if (hr == D3DERR_DEVICENOTRESET) //Device available for reset
	{
		//Destroy graphics
		OnLostDevice();

		//Reset the device
		m_pDevice3D->Reset(&m_d3dpp);

		//Reset graphics
		OnResetDevice();

		//Device no longer lost
		return false;
	}
	else
		return false;
}

bool GraphicsManager::IsAppPaused()
{
	return mainWindow->IsWindowPaused();
}

IDirect3DDevice9** GraphicsManager::GetDevice()
{
	return &m_pDevice3D;
}

bool GraphicsManager::IsHandlingDeviceLost()
{
	return m_bHandleDeviceLost;
}

void GraphicsManager::SetHandleDeviceLost(bool toggle)
{
	m_bHandleDeviceLost = toggle;
}

void GraphicsManager::SetMainCamera(Camera* cam)
{
  if(cam)
  {
    main_camera = cam;
  }
}

void GraphicsManager::SetMainCameraDefault()
{
  main_camera = NULL;// &default_editor_camera;
}

Camera* GraphicsManager::GetMainCamera()
{
  if (main_camera)
  {
    return main_camera;
  }

  return &default_editor_camera;
}

bool GraphicsManager::IsDebugDrawingOn()
{
  return debug_draw_on;
}

void GraphicsManager::AddParticleEmitter(ParticleEmitter* PE)
{
  if (PE)
  {
    particleEmitterColl.push_back(PE);
  }
}

void GraphicsManager::RemoveParticleEmitter(ParticleEmitter* PE)
{
  if(PE)
  {
    for (PartEmitVecIter it = particleEmitterColl.begin(); it != particleEmitterColl.end(); ++it)
    {
      if (*it == PE)
      {
        particleEmitterColl.erase(it);
        break;
      }
    }
  }
}

void GraphicsManager::setWireframeTransform(Transform* trans)
{
  //Scale, rotation and translation matrices
  D3DXMATRIX Scale, Rx, Ry, Rz, Translation;

  Vector3D rot = trans->GetRotation();
  D3DXMatrixRotationX(&Rx, rot.x);
  D3DXMatrixRotationY(&Ry, rot.y);
  D3DXMatrixRotationZ(&Rz, rot.z);

  Vector3D scal = trans->GetScale();
  D3DXMatrixScaling(&Scale, scal.x, scal.y, scal.z);

  Vector3D& tempTanslate = trans->GetPosition();
  D3DXMatrixTranslation(&Translation, tempTanslate.x, tempTanslate.y, tempTanslate.z);

  debug_draw_wireframe_world = Scale * Rx * Ry * Rz * Translation;
}

void GraphicsManager::setBoxCollWireframeTransform(Transform* trans, Vector2D halfSize)
{
  //Scale, rotation and translation matrices
  D3DXMATRIX Scale, Rx, Ry, Rz, Translation;

  Vector3D rot = trans->GetRotation();
  D3DXMatrixRotationX(&Rx, rot.x);
  D3DXMatrixRotationY(&Ry, rot.y);
  D3DXMatrixRotationZ(&Rz, rot.z);

  Vector3D scal = trans->GetScale();
  D3DXMatrixScaling(&Scale, halfSize.x / scal.x, halfSize.y / scal.y, scal.z);

  Vector3D& tempTanslate = trans->GetPosition();
  D3DXMatrixTranslation(&Translation, tempTanslate.x, tempTanslate.y, tempTanslate.z);

  debug_draw_wireframe_world = Scale * Rx * Ry * Rz * Translation;
}

void GraphicsManager::setCircleCollWireframeTransform(Transform* trans, float radius)
{
  //Scale, rotation and translation matrices
  D3DXMATRIX Scale, Rx, Ry, Rz, Translation;

  Vector3D rot = trans->GetRotation();
  D3DXMatrixRotationX(&Rx, rot.x);
  D3DXMatrixRotationY(&Ry, rot.y);
  D3DXMatrixRotationZ(&Rz, rot.z);

  Vector3D scal = trans->GetScale();
  D3DXMatrixScaling(&Scale, radius * 2.0f, radius * 2.0f / scal.y, scal.z);

  Vector3D& tempTanslate = trans->GetPosition();
  D3DXMatrixTranslation(&Translation, tempTanslate.x, tempTanslate.y, tempTanslate.z);

  debug_draw_wireframe_world = Scale * Rx * Ry * Rz * Translation;
}

void GraphicsManager::SetDebugDrawingDefaultOn()
{
   debug_draw_on = true;
   debug_draw_transform_information = true;
   debug_draw_rigid_body_information = true;
   debug_draw_texture = true;
   debug_draw_wireframe = true;
   debug_draw_colliders = true;
   debug_draw_use_editor_cam = false;
}

void GraphicsManager::SetDebugDrawingDefaultOff()
{
  debug_draw_on = false;
  debug_draw_colliders = false;
  debug_draw_transform_information = false;
  debug_draw_rigid_body_information = false;
  debug_draw_texture = false;
  debug_draw_wireframe = false;
  debug_draw_use_editor_cam = false;
}

void GraphicsManager::prepare_ui_elements()
{
    //Put the wall (if it exists) below all other UI sprites
    for (SpriteRendVecIter it = uiSpriteCollection.begin(); it != uiSpriteCollection.end(); ++it)
    {
        if (strcmp((*it)->GetOwner()->GetName(), "WallImage") == 0)
        {
            std::iter_swap(it, uiSpriteCollection.begin());
            break;
        }
    }

    //printf("not an infinite loop");
    //std::reverse(this->uiSpriteCollection.begin(), this->uiSpriteCollection.end() );
}

void GraphicsManager::ChangeResolution(const float width, const float height)
{
  //if (main_camera)
  //{
  //  main_camera->Set_Width_Height(width, height);
  //}
  //default_editor_camera.Set_Width_Height(width, height);

  //if (m_bFullScreen)
  //{
  //  //Cache desktop width and height
  //  int width = GetSystemMetrics(SM_CXSCREEN);
  //  int height = GetSystemMetrics(SM_CYSCREEN);

  //  //=GRAPHICS=
  //  m_d3dpp.BackBufferWidth = width;
  //  m_d3dpp.BackBufferHeight = height;
  //  m_d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;// D3DFMT_A8R8G8B8;
  //  m_d3dpp.Windowed = false;

  //  //Set window style to fullscreen friendly
  //  SetWindowLongPtr(m_hAppWindow, GWL_STYLE, WS_POPUP);

  //  //Need to set new position for window
  //  SetWindowPos(m_hAppWindow, NULL, 0, 0, width, height, SWP_NOZORDER | SWP_SHOWWINDOW);
  //}

  //else
  //{
  //  //Set window back to windowed mode
  //  RECT r = { 0, 0, m_ClientWidth, m_ClientHeight };
  //  AdjustWindowRect(&r, m_WindowStyle, false);
  //  int w = r.right - r.left;
  //  int h = r.bottom - r.top;

  //  m_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;// D3DFMT_A8R8G8B8;
  //  m_d3dpp.BackBufferWidth = m_ClientWidth;
  //  m_d3dpp.BackBufferHeight = m_ClientHeight;
  //  m_d3dpp.Windowed = true;

  //  //Change window style back to windowed friendly
  //  SetWindowLongPtr(m_hAppWindow, GWL_STYLE, m_WindowStyle);

  //  //Set window position
  //  SetWindowPos(m_hAppWindow, NULL,
  //    GetSystemMetrics(SM_CXSCREEN) / 2 - w / 2,
  //    GetSystemMetrics(SM_CYSCREEN) / 2 - h / 2,
  //    w, h, SWP_NOZORDER | SWP_SHOWWINDOW);
  //}

  ////Reset our device to reflect the changes
  //m_PmainGraphicsMang->OnLostDevice();
  //(*(m_PmainGraphicsMang->GetDevice()))->Reset(&m_d3dpp);
  //m_PmainGraphicsMang->OnResetDevice();
}
