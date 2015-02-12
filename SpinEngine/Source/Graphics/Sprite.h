/****************************************************************************/
/*!
\author Esteban Maldonado
\par    email: esteban.maldonado@digipen.edu
\par    Course: GAM 200
\brief

Sprite object used by the sprite renderers. Represent a texture.

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#pragma once
#include "Precompiled.h" 

//2D Constants for rectangle /*Code is subject to change for efficiency*/
const std::string defaultTexName = "DefaultTexture.png";
const UINT defaultWidth = 512;
const UINT defaultHeight = 512;
const UINT defaultTotalFrames = 1;
const UINT defaultStartFrame = 0;
const UINT defaultStopFrame = 1;
const int defaultRows = 1;
const int defaultColumns = 1;
const int row_column_count = 4;
const float defaultTPF = 0.0f;
const std::string defaultStateName = "idle";

//Forward Declarations
class Sprite;

typedef Sprite* Animation;
typedef std::unordered_map< std::string, Animation> SpriteCollection;
typedef std::unordered_map< std::string, Animation>::iterator SpriteCollIterator;

class Sprite
{
public:
	ZilchDeclareBaseType(Sprite, Zilch::TypeCopyMode::ReferenceType);
	Sprite(IDirect3DTexture9 *texture = NULL, std::string textureName = defaultTexName, UINT start_i = 0,
    UINT end_i = 0, unsigned int total_frames = defaultTotalFrames, float timePerFrame = defaultTPF,
    std::string state_name = defaultStateName );
  ~Sprite();

  //Methods
  bool Initialize(IDirect3DDevice9* device);
  void Update(float dt);
  void Draw(IDirect3DDevice9* device, D3DXMATRIX transform, D3DCOLOR color,
    IDirect3DVertexBuffer9** m_ppVBuffer, IDirect3DIndexBuffer9** m_ppIBuffer);
  void Release();
  //Getters
  IDirect3DTexture9* GetTexture();
  const std::string &GetStateName() const;
  const std::string &GetTextureName() const;
  Zilch::String ZGetTextureName();
  D3DXVECTOR3 GetSpriteCenter();
  bool isPaused();
  
  //Setters
  void Set_Paused(bool p);
  void PauseAt(unsigned int frame);
  void SetFrame(unsigned int frame);
  void SetStateName(Zilch::String);

private:

  //Texture properties
  std::string m_textureName;
  IDirect3DTexture9 *m_texture;
  UINT m_width, m_height;
  
  //Name of the state of the sprite
  std::string m_stateName;
  
  //Animation states
  bool paused;

  //Frame Specific properties
  D3DXVECTOR3 m_spriteCenter;
  unsigned int m_currentFrame;
  unsigned int m_totalFrames;
  UINT frame_w, frame_h;

  UINT start_i, end_i;

  unsigned int m_rows;
  unsigned int m_current_row;
  unsigned int m_columns;
  unsigned int m_current_column;

  RECT m_currentFrameRect;
  float m_timePerFrame;
  float m_currentTimeOnFrame;
};