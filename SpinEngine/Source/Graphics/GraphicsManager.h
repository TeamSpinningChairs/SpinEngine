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
#pragma once

//const bool FULL_SCREEN = true;
//const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

#include "Precompiled.h" //vector, ISystem, DXUtil, MemoryHelper
#include"ParticleEmitter.h"
#include "Core/SpriteRenderer.h"
#include "Window.h"
#include "Camera.h"

class GraphicsManager : public ISystem
{
public:
	GraphicsManager(UINT width, UINT height, std::string title, HINSTANCE	hAppInstance, HWND handle,
		D3DCOLOR backgrdClr = d3dColors::Black, bool full_screen = false);
	~GraphicsManager();

	//Overloaded methods
	bool Initialize() override;
	void Update(float dt) override;
	void Shutdown() override;

	//Methods
	Window* GetWindow();
	bool IsAppPaused();
	void Render();
	IDirect3DDevice9** GetDevice();

  void SetMainCamera(Camera* cam = NULL);
  void SetMainCameraDefault();
  Camera* GetMainCamera();
  bool IsDebugDrawingOn();
  void SetDebugDrawing(bool on_or_off);
  void ResetMainCamera();

	//Handles lost device
	bool IsDeviceLost();
	bool IsHandlingDeviceLost();
	void SetHandleDeviceLost(bool toggle);
	void AddSpriteRend(SpriteRenderer* sprite);
  void RemoveSpriteRend(SpriteRenderer* sprite);
  void AddBGSpriteRend(SpriteRenderer* sprite);
  void RemoveBGSpriteRend(SpriteRenderer* sprite);
  void AddUISpriteRend(SpriteRenderer* sprite);
  void RemoveUISpriteRend(SpriteRenderer* sprite);

  void ChangeResolution(const float width, const float height);

  void AddParticleEmitter(ParticleEmitter* PE = NULL);
  void RemoveParticleEmitter(ParticleEmitter* PE = NULL);

	void OnLostDevice();
	void OnResetDevice();

  //DEBUG DRAWING SETTINGS

  bool debug_draw_on;
  bool debug_draw_transform_information;
  bool debug_draw_rigid_body_information;
  bool debug_draw_texture;
  bool debug_draw_wireframe;
  bool debug_draw_colliders;
  bool debug_draw_use_editor_cam;
  D3DCOLOR debug_draw_wireframe_color;
  D3DCOLOR debug_draw_collision_wireframe_color;
  D3DMATRIX debug_draw_wireframe_world;

  void SetDebugDrawingDefaultOn();
  void SetDebugDrawingDefaultOff();
  //END DEBUG DRAWING SETTINGS

  bool m_bFullScreen;

  //THIS IS AWEFUL - WE NEED TO FIX THIS
  void prepare_ui_elements(); // <- YES, THIS THING
  //OH GOD FIX IT ^^^^^^^^^^^


private:
	//Methods
	//Initialize direct3D
	bool InitDirect3D();
	bool Initialize_Sprites();

  //Debug Draw helper functions
  void setWireframeTransform(Transform* trans);
  void setBoxCollWireframeTransform(Transform* trans, Vector2D halfSize);
  void setCircleCollWireframeTransform(Transform* trans, float radius);
  bool IsInBounds(SpriteRenderer *rend);

	//DirectX members
	IDirect3D9*	m_pDirect3D;			//Direct3D interface
	IDirect3DDevice9* m_pDevice3D; //Direct3D device interface
	D3DPRESENT_PARAMETERS m_d3dpp;				//Direct3D present parameters struct
	D3DDISPLAYMODE m_Mode;				//Direct3D display mode struct
	D3DDEVTYPE m_DevType;			//Device Type (SHOULD BE DEVTYPE_HAL)
	D3DCOLOR backgroundColor;
	ID3DXSprite *m_sprite;

  //Buffers
	IDirect3DVertexBuffer9 *vBuffer; //Vertex Buffer
	IDirect3DIndexBuffer9 *iBuffer; //Index Buffer
  IDirect3DVertexBuffer9 *Debug_Wireframe_vBuffer;
  IDirect3DVertexBuffer9 *Debug_BoxWireframe_vBuffer;
  IDirect3DVertexBuffer9 *Debug_CircleWireframe_vBuffer;


  //Text Variables for Debug Drawing
  ID3DXFont* m_Font; // Font pointer
  D3DXFONT_DESC m_FontDesc; //Font description

  //Cameras
  Camera* main_camera;
  Camera default_editor_camera;

	//Members
	SpriteRendVector spriteCollection;
  ParticleEmitVector particleEmitterColl;
  SpriteRendVector bgSpriteCollection;
  SpriteRendVector uiSpriteCollection;

	Window* mainWindow;
	bool m_bHandleDeviceLost;
	bool m_bDeviceLost;
	float m_FPS;					//Frames per second of our application
};

#pragma once