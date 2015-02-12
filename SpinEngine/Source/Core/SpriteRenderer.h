/****************************************************************************/
/*!
\author Esteban Maldonado
\par    email: esteban.maldonado@digipen.edu
\par    Course: GAM 200
\brief

Class interface for Sprite renderer components, contain all of the necessary
objects for rendering our objects.

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#pragma once

#include"Core\Transform.h"
#include"Vertices.h"
#include"Sprite.h"
#include "Precompiled.h" //Utilities\DXUtil.h
#include "ZilchCompiledLib.h"

//2D Constants for rectangle /*Code is subject to change for efficiency*/
const int NUM_FACES = 1;
const int NUM_VERTS = 4;
const int NUM_INDICES = 6;
const std::string initStateName = "idle";

//Forward declarations
class Camera;
class SpriteRenderer;
struct TileMapData;

typedef SpriteRenderer* SpriteRend;
typedef std::vector<SpriteRenderer*> SpriteRendVector;
typedef std::vector<SpriteRenderer*>::iterator SpriteRendVecIter;

//typedefs
typedef IDirect3DVertexBuffer9* VertexBuffer; //Vertex Buffer typedef
typedef IDirect3DIndexBuffer9* IndexBuffer; //Index Buffer typedef

class SpriteRenderer : public IComponent
{
public:
	ZilchDeclareDerivedType(SpriteRenderer, IComponent);
  SpriteRenderer(IEntity* Owner, IDirect3DDevice9* device,
                 D3DCOLOR m_color = d3dColors::White, std::string initStateName = initStateName);
  ~SpriteRenderer()  override;

  bool Initialize() override;
  void Serialize (DynamicElement* props, Serializer::DataNode* data) {}
  void Update(float dt) override;
  void Draw(IDirect3DVertexBuffer9** m_ppVBuffer, IDirect3DIndexBuffer9** m_ppIBuffer, 
    bool debug_drawing = false);

  //Getters
  SpriteCollection* GetSpriteCollection();
  const Vector3D &GetPosition();

  //Setters
  void AddSprite(std::string textureName, UINT start_i,UINT end_i, unsigned int total_frames, 
    float timePerFrame, std::string state_name,bool isDefaultNow = false);
  void ToggleAsTile();

  //void AddSprite( UINT width, UINT height,
  //  UINT frame_w, UINT frame_h);
  //Overloaded to add sprites already allocated. (Mostly for use later, when all sprites are kept in ObjectData
  //and renderers only access constant references. If that's possible; it seems like a thing)
  void AddSprite(Sprite *newSprite);
  void ZChangeState(Zilch::String name);
  void ChangeState(std::string new_State_Name);
  void Release();
  Sprite* GetCurrentSprite(void); 


  D3DCOLOR& GetSpriteColor();


  void UpdateTileSprite();
  void UpdateSurroundingTileSprites();
private:
  //helper functions
  D3DMATRIX GetWorld();
  void UpdateMatrices();

  //Tile-specific stuff
  TileMapData *tiles;
  //The indexes of our four adjacent neighbors
  //(only used if we're a tile)
  int myTileID, rightNeighborIndex,
    leftNeighborIndex,
    topNeighborIndex,
    bottomNeighborIndex;

  //Members
  SpriteCollection m_spriteStates;
  std::string currentStateName;
  D3DCOLOR color;
  IDirect3DVertexBuffer9** m_ppVBuffer;
  IDirect3DIndexBuffer9** m_ppIBuffer;
  D3DMATRIX world;
  UINT currentStride;
  ID3DXSprite *m_sprite;
  IDirect3DDevice9* spriteDrawingDevice;
  bool isStationary;

  //Other components' members
  const Vector3D &myPos;
};