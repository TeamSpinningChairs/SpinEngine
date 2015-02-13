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
#include "Precompiled.h"
#include "Sprite.h"
#include "Vertices.h"

ZilchDefineType(Sprite, SpinningZilch)
{
	type->HandleManager = ZilchManagerId(Zilch::PointerManager);
	ZilchBindMethodAs(ZGetTextureName, "GetName");
}

Sprite::Sprite(IDirect3DTexture9 *texture, std::string textureName, UINT start_i,
  UINT end_i, unsigned int total_frames, float timePerFrame, std::string state_name) :
  m_texture(texture), m_textureName(textureName), m_currentFrame(start_i), start_i(start_i),
    m_currentTimeOnFrame(0), m_timePerFrame(timePerFrame), end_i(end_i),
    m_stateName(state_name), m_totalFrames(total_frames), paused(false)
{
  //m_spriteCenter.x = float(frame_w)/2.0f;
  //m_spriteCenter.y = float(frame_h)/2.0f;
  //m_spriteCenter.z = 0.0f;

  m_rows = std::ceil(static_cast<float>(total_frames) 
    / static_cast<float>(row_column_count));

  if (total_frames < row_column_count)
  {
    m_columns = total_frames;
  }
  else
  {
    m_columns = row_column_count;
  }

  m_current_row = start_i / row_column_count;
  m_current_column = start_i % row_column_count;
}

Sprite::~Sprite()
{
  //Don't need to free the texture, they'll all be freed in ObjectData
}

bool Sprite::Initialize(IDirect3DDevice9* device)
{
  return true;
}

void Sprite::Update(float dt)
{
  //Update if you have more than one frame
  if (m_totalFrames > 1)
  {
    if (!paused)
    {
      m_currentTimeOnFrame += dt;
      if (m_currentTimeOnFrame >= m_timePerFrame)
      {
        m_currentTimeOnFrame = 0.0f;
        m_currentFrame++;
        if (m_currentFrame >= m_totalFrames)
        {
          m_currentFrame = 0;
        }

        m_current_row = m_currentFrame / m_columns; // Spritesheet row
        m_current_column = m_currentFrame % (m_columns); //Spritesheet column
      }
    }
  }
}

void Sprite::Draw(IDirect3DDevice9* device, D3DXMATRIX transform, 
  D3DCOLOR color, IDirect3DVertexBuffer9** m_ppVBuffer, 
    IDirect3DIndexBuffer9** m_ppIBuffer)
{
  VOID* pVerts;
  //LOCK THE MEMORY TO MAKE SURE DATA IS PASSED THROUGH
  (*m_ppVBuffer)->Lock(0, sizeof(d3dVertex::CUSTOMVERTEX) * 4/*NUM_VERTS*/, (void**)&pVerts, 0);
  d3dVertex::CUSTOMVERTEX* temp = reinterpret_cast<d3dVertex::CUSTOMVERTEX*>(pVerts);

  //D3DCAPS9 caps;
  //int max_tex_size;

  //device->GetDeviceCaps(&caps);

  //max_tex_size = caps.MaxTextureWidth;

  //if (caps.TextureCaps & D3DPTEXTURECAPS_SQUAREONLY)
  //{
  //  //Textures have to be square
  //  int i = 0;
  //}
  //else
  //{
  //  //Texture do not have to be square
  //  int i = 0;
  //}

  //if (caps.TextureCaps & D3DPTEXTURECAPS_POW2)
  //{
  //  //Textures must be a power of 2 in size
  //  int i = 0;

  //  if (caps.TextureCaps & D3DPTEXTURECAPS_NONPOW2CONDITIONAL)
  //  {
  //    //But, in certain cases textures can ignore the power of 2 limitation
  //    int i = 0;
  //  }
  //}
  //else
  //{
  //  //Textures do not need to be a power of 2 in size
  //  int i = 0;
  //}

  //If the animation is a single sprite draw the entire texture.
  if (m_totalFrames == 1)
  {
    temp[0].u = temp[0].v = 0.0f;

    temp[1].u = 1.0f;
    temp[1].v = 0.0f;

    temp[2].u = 0.0f;
    temp[2].v = 1.0f;

    temp[3].u = temp[3].v = 1.0f;
  }
  else
  {
    const float offset = 0.005f;
    temp[0].u = (m_current_column * 0.2500) + offset;
    temp[0].v = (m_current_row * 0.2500) + offset;

    temp[1].u = ((m_current_column + 1) * 0.2500f) - offset;
    temp[1].v = (m_current_row * 0.2500f) + offset;

    temp[2].u = (m_current_column * 0.2500f) + offset;
    temp[2].v = ((m_current_row + 1)* 0.2500f) - offset;

    temp[3].u = ((m_current_column + 1) * 0.2500f) - offset;
    temp[3].v = ((m_current_row + 1) * 0.2500f) - offset;
  }

  temp[0].color = temp[1].color = temp[2].color = temp[3].color = color;

  (*m_ppVBuffer)->Unlock();

  //if(FAILED(sprite->Draw(m_texture, &m_currentFrameRect, &m_spriteCenter, &position, d3dColors::White)))
  //{
  //  MessageBox(NULL, ("ERROR: COULD NOT DRAW STATE " + m_textureName + " FROM STATE " + m_stateName).c_str(),
  //    NULL, NULL);
  //}
  //sprite->Flush();
  device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
  //device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  //device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  //device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

  device->SetStreamSource(0, *m_ppVBuffer, 0, sizeof(d3dVertex::CUSTOMVERTEX));
  device->SetIndices(*m_ppIBuffer);
  device->SetFVF( d3dVertex::CUSTOMVERTEX::FVF);
  device->SetTransform(D3DTS_WORLD, &transform);
  
  if (m_texture)
  {
    device->SetTexture(0, m_texture);
  }
  else
  {
    device->SetTexture(0, NULL);
  }

  //device->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
  device->SetSamplerState(0, D3DSAMP_ADDRESSU,  D3DTADDRESS_CLAMP);
  device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
  //device->SetTransform(D3DTS_TEXTURE0, &textureTransform); //<-- TEXTURE TRANSFORM
  device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, /*NUM_INDICES*/ 6, 0, /*NUM_INDICES = 6 / 3 */ 2 );
  //device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2 );//0, /*NUM_INDICES*/ 6, 0, /*NUM_INDICES*/ 6 / 3);
  //device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

IDirect3DTexture9* Sprite::GetTexture()
{
  return m_texture;
}

const std::string &Sprite::GetStateName() const
{
  return m_stateName;
}

Zilch::String Sprite::ZGetTextureName()
{
	return Zilch::String(m_textureName.c_str());
}

const std::string &Sprite::GetTextureName() const
{
  return m_textureName;
}

void Sprite::Release()
{
  SPIN_ENGINE_SAFE_RELEASE(m_texture);
}

D3DXVECTOR3 Sprite::GetSpriteCenter()
{
	return m_spriteCenter;
}

bool Sprite::isPaused()
{
  return paused;
}

void Sprite::Set_Paused(bool p)
{
  paused = p;
}

void Sprite::PauseAt(unsigned int frame)
{
  if (frame >= start_i && frame <= end_i)
  {
    m_currentFrame = frame;
    m_current_row = m_currentFrame / m_columns; // Spritesheet row
    m_current_column = m_currentFrame % (m_columns); //Spritesheet column
    paused = true;
  }
  else
  {
    std::cout << "ERROR: Unacceptable frame number on PauseAt("<<frame<<")\n\n";
  }
}

void Sprite::SetFrame(unsigned int frame)
{
  m_currentFrame = frame;
  m_current_row = m_currentFrame / m_columns; // Spritesheet row
  m_current_column = m_currentFrame % (m_columns); //Spritesheet column
}
