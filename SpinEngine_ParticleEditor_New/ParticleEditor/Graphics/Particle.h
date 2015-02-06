/****************************************************************************/
/*!
\author Esteban Maldonado
\par    email: esteban.maldonado@digipen.edu
\par    Course: GAM 200
\brief

The basic particle object for the particle emitter.

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#pragma once
#include"Vertices.h"
#include "Math/mathlib.h"

enum emitter_types
{
  ET_NORMAL,
  ET_SQUARE,
  ET_DIAMOND,
  ET_CIRCLE,
  ET_RETRACTIVE,
  ET_COUNT
};

typedef enum blend_types
{
  BT_ADD = D3DBLENDOP_ADD,
  BT_SUBSTRACT = D3DBLENDOP_SUBTRACT,
  BT_REVSUBSTRACT = D3DBLENDOP_REVSUBTRACT,
  BT_MIN = D3DBLENDOP_MIN,
  BT_MAX = D3DBLENDOP_MAX
};

typedef enum blend_factor_types
{
  //ZERO / ONE
  BFT_ZERO = D3DBLEND_ZERO,
  BFT_ONE = D3DBLEND_ONE,

  //COLOR OPTIONS
  BFT_SRC_COLOR = D3DBLEND_SRCCOLOR,
  BFT_INV_SRC_COLOR = D3DBLEND_INVSRCCOLOR,
  BFT_DEST_COLOR = D3DBLEND_DESTCOLOR,
  BFT_INV_DEST_COLOR = D3DBLEND_INVDESTCOLOR,

  //ALPHA OPTIONS
  BFT_SRC_ALPHA = D3DBLEND_SRCALPHA,
  BFT_DEST_ALPHA = D3DBLEND_DESTALPHA,
  BFT_INV_SRC_ALPHA = D3DBLEND_INVSRCALPHA,
  BFT_INV_DEST_ALPHA = D3DBLEND_INVDESTALPHA
};

using namespace::d3dVertex;

class Particle
{
public:
  Particle(Vector3 initial_pos = Vector3(), float lifespan = 1.0f);
  ~Particle();

  void render_particle(IDirect3DDevice9* device, IDirect3DVertexBuffer9** t_buffer, 
    IDirect3DTexture9* texture, D3DXCOLOR color, D3DXCOLOR end_color);
  
  void set_particle(IDirect3DDevice9* device, float scal, float end_scal/*, float camx, float camy, float camz*/);
  
  //NORMAL
  void run_particle(float dt, float p_lifetime, float lifetime_invariant, Vector3 pos, float ang_1 = 90.0f, float ang_2 = 90.0f,
    float spd1 = 1.0f, float spd2 = 4.0f, float acc1 = 1.0f, float acc2 = 20.0f, float spin = 0.0f, float spin_invariance = 0.0f);
  void reset_particle(Vector3 pos, float p_lifetime, float ang_1 = 90.0f, float ang_2 = 90.0f,
    float spd1 = 1.0f, float spd2 = 4.0f, float acc1 = 1.0f, float acc2 = 20.0f, float new_spin = 0.0f);

  //SQUARE
  void run_particle_square(float dt, float p_lifetime, float lifetime_invariant, Vector3 pos, float width = 1.0f, float height = 1.0f,
    float spd1 = 1.0f, float spd2 = 4.0f, float acc1 = 1.0f, float acc2 = 20.0f, float angle = 0.0f, float spin = 0.0f, float spin_invariance = 0.0f);
  void reset_particle_square(Vector3 pos, float p_lifetime, float width = 1.0f, float height = 1.0f,
    float spd1 = 1.0f, float spd2 = 4.0f, float acc1 = 1.0f, float acc2 = 20.0f, float angle = 0.0f, float new_spin = 0.0f);

  //DIAMOND
  void run_particle_diamond(float dt, float p_lifetime, Vector3 pos, float width = 1.0f, float height = 1.0f,
    float spd1 = 1.0f, float spd2 = 4.0f, float acc1 = 1.0f, float acc2 = 20.0f, float angle = 0.0f);
  void reset_particle_diamond(Vector3 pos, float width = 1.0f, float height = 1.0f,
    float spd1 = 1.0f, float spd2 = 4.0f, float acc1 = 1.0f, float acc2 = 20.0f, float angle = 0.0f);

  //CIRCLE
  void run_particle_circle(float dt, float p_lifetime, float lifetime_invariant, Vector3 pos, float radius = 1.0f,
    float spd1 = 1.0f, float spd2 = 4.0f, float acc1 = 1.0f, float acc2 = 20.0f, float angle = 0.0f, float spin = 0.0f, float spin_invariance = 0.0f);
  void reset_particle_circle(Vector3 pos, float p_lifetime, float radius = 1.0f,
    float spd1 = 1.0f, float spd2 = 4.0f, float acc1 = 1.0f, float acc2 = 20.0f, float angle = 0.0f, float new_spin = 0.0f);

  bool active;

  D3DXVECTOR3 position;
  D3DXVECTOR3 initial_position;
  D3DXVECTOR3 velocity;
  D3DXVECTOR3 acceleration;
  float mass;
  //float radius;
  float lifespan;
  float current_life;
  D3DXMATRIX matRotateX;
  D3DXMATRIX matRotateY;
  D3DXMATRIX matRotateZ;
  D3DCOLOR color;
  D3DCOLOR end_color;

  float spin;
  float angle;

  // this function generates random float values
  float random_number(float low, float high)
  {
    return low + ((float)((float)rand() / (float)RAND_MAX) * (float)((high)-(low)));
  }
};