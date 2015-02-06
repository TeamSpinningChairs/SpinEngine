#include "Precompiled.h"
#include "ParticleEmitter.h"
#include "GraphicsManager.h" //to access graphics manager
#include "FactoryAccess.h" //To access the graphics manager during destruction

IDirect3DIndexBuffer9* ParticleEmitter::i_buffer = NULL;
IDirect3DVertexBuffer9* ParticleEmitter::v_buffer = NULL;

ParticleEmitter::ParticleEmitter(GameObject Owner, emitter_types type, blend_types b_type,
  unsigned int particle_count, std::string t_name, D3DXCOLOR color, D3DXCOLOR  end_color) :
  IComponent(CT_ParticleEmitter, Owner), particle_count(particle_count), my_color(color), my_color_end(end_color),
  texture(NULL), scale(0.50f), end_scale(0.50f), gravity(0.0f), PE_lifespan(PE_LOOPING), current_PE_lifespan(0)
  , angle_1(0.0f), angle_2(360.0f), speed_1(1.0), speed_2(0), acceleration_1(-1.0f), acceleration_2(5.0f),
  x_diff_from_owner(0.0f), y_diff_from_owner(0.0f), blend_colors(true), use_blending_operation(true), emit_type(type), blend_type(b_type), kill_pe(false),
  spin(0.0f), spin_variance(0.0f),
  //lifetime variables
  particle_lifetime(1.0f),
  lifetime_invariant(0.20f),

  //FOR SQUARE EMITTERS
  height(1.0f), width(1.0f),

  //FOR CIRCLE EMITTERS
  radius(1.0f),

  //BLEND FACTORS
  source_factor(BFT_SRC_COLOR),
  dest_factor(BFT_DEST_COLOR)
{

  device = GlobalFactory->GetDevice();
  if (!Owner)
  {
    //I disabled this, since ObjectData keeps several of these without itself being a GameObject
    //MessageBox(NULL, "ERROR: Particle Emitter does not have an Game Object assigned!", "Particle Emitter Error", NULL);
  }

  if (particle_count > MAX_PARTICLE_COUNT)
  {
    this->particle_count = MAX_PARTICLE_COUNT;
  }

  texture_name = t_name;
}

ParticleEmitter::~ParticleEmitter()
{
  //GlobalFactory->GetGraphicsManager()->RemoveParticleEmitter(this);

  SPIN_ENGINE_SAFE_RELEASE(texture);
  //SPIN_ENGINE_SAFE_DELETE_ARR(texture_name);
}

bool ParticleEmitter::Initialize()
{
  D3DXCreateTextureFromFile(device, ("Assets\\Textures\\" + texture_name).c_str(), &texture);

  if (!v_buffer)
  {
    // create the vertices using the CUSTOMVERTEX
    d3dVertex::CUSTOMVERTEX t_vert[4] =
    {
      d3dVertex::CUSTOMVERTEX(-1.0f, 1.0f, 0.0f, my_color, 1.0f, 0),
      d3dVertex::CUSTOMVERTEX(-1.0f, -1.0f, 0.0f, my_color, 0, 0),
      d3dVertex::CUSTOMVERTEX(1.0f, 1.0f, 0.0f, my_color, 1, 1),
      d3dVertex::CUSTOMVERTEX(1.0f, -1.0f, 0.0f, my_color, 0, 1),
    };

    // create a vertex buffer interface called t_buffer
    if (FAILED(device->CreateVertexBuffer(sizeof(t_vert), 0, CUSTOMFVF, D3DPOOL_MANAGED, &v_buffer, 0)))
    {
      MessageBox(0, "ERROR: Could not create Vertex Buffer for the particle emitter.", "Particle Emmitter Error", 0);
      return false;
    }

    VOID* pVoid; // a void pointer

    // lock t_buffer and load the vertices into it
    v_buffer->Lock(0, 0, (void**)&pVoid, 0);
    std::memcpy(pVoid, t_vert, sizeof(t_vert));
    v_buffer->Unlock();
  }

  Vector3D initPos = Owner->GetTransform()->GetPosition();
  initPos.x += x_diff_from_owner;
  initPos.y += y_diff_from_owner;

  for (unsigned int i = 0; i < particle_count; ++i)
  {
    particles[i].reset_particle(initPos, angle_1, angle_2, speed_1, speed_2, acceleration_1, acceleration_2);
  }

  return true;
}

void ParticleEmitter::Update(float dt)
{
  if (PE_lifespan != PE_LOOPING)
  {
    if ((current_PE_lifespan += dt) > PE_lifespan)
    {
      SetActive(false);
    }
  }
}

void ParticleEmitter::Release()
{
  SPIN_ENGINE_SAFE_RELEASE(texture);
  //SPIN_ENGINE_SAFE_DELETE_ARR(texture_name);
}

void ParticleEmitter::SetParticleCount(unsigned int pc)
{
  if (pc > MAX_PARTICLE_COUNT)
  {
    particle_count = MAX_PARTICLE_COUNT;
    return;
  }

  particle_count = pc;
}

unsigned int& ParticleEmitter::GetParticleCount()
{
  return particle_count;
}

void ParticleEmitter::Render(IDirect3DDevice9* device)
{
  //update the color
  //end update the color

  device->SetRenderState(D3DRS_ALPHABLENDENABLE, blend_colors);
  device->SetRenderState(D3DRS_ZENABLE, false);
  device->SetRenderState(D3DRS_ZWRITEENABLE, false);

  device->SetRenderState(D3DRS_SRCBLEND, source_factor);// D3DBLEND_SRCCOLOR/*D3DBLEND_SRCALPHA*/ /*D3DBLEND_INVDESTCOLOR*/);    // set source factor
  device->SetRenderState(D3DRS_DESTBLEND, dest_factor);// D3DBLEND_DESTCOLOR /*D3DBLEND_INVSRCALPHA*/ /*D3DBLEND_ONE*/);    // set dest factor
  if (use_blending_operation)
  {
    device->SetRenderState(D3DRS_BLENDOP, blend_type);    // set the operation
  }

  device->SetFVF(CUSTOMFVF);

  // calculate elapsed time
  static DWORD starting_point = GetTickCount(), time;
  time = GetTickCount() - starting_point;
  starting_point = GetTickCount();
  //alive_particles = 0;

  for (unsigned int i = 0; i < particle_count; ++i)
  {
    if (particles[i].active)
    {
      alive_particles++;
      Vector3D pos = Owner->GetTransform()->GetPosition();
      pos.x += x_diff_from_owner;
      pos.y += y_diff_from_owner;

      switch (emit_type)
      {
      case ET_NORMAL:
      default:
        particles[i].run_particle(time / 1000.0f, particle_lifetime, lifetime_invariant, pos,
          angle_1, angle_2, speed_1, speed_2, acceleration_1, acceleration_2, spin, spin_variance);
        break;

      case ET_SQUARE:
        particles[i].run_particle_square(time / 1000.0f, particle_lifetime, lifetime_invariant, pos, width, height,
          speed_1, speed_2, acceleration_1, acceleration_2, angle_1, spin, spin_variance);
        break;

      case ET_DIAMOND:
        particles[i].run_particle_diamond(time / 1000.0f, particle_lifetime, pos, width, height,
          speed_1, speed_2, acceleration_1, acceleration_2, angle_1);
        break;

      case ET_CIRCLE:
        particles[i].run_particle_circle(time / 1000.0f, particle_lifetime, lifetime_invariant, pos, radius,
          speed_1, speed_2, acceleration_1, acceleration_2, angle_1, spin, spin_variance);
        break;
      }

      particles[i].set_particle(device, scale, end_scale);
      particles[i].render_particle(device, &v_buffer, texture, my_color, my_color_end);
    }
  }

  for (int index = 0; index < time / 2.0f; index++)
  {
    for (unsigned int index = 0; index < particle_count; index++)
    {
      if (!particles[index].active) // if the particle is inactive...
      {
        particles[index].active = true;// ...activate it
        break;    // then stop
      }
    }
  }

  //Now to set up alpha blending
  device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
  device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
  device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

  //Indicates that we are using 2D Texture coordinates (U,V)
  device->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);

  //device->SetTransform(D3DTS_TEXTURE0, &textureTransform); //<-- TEXTURE TRANSFORM
  device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

  device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
  device->SetRenderState(D3DRS_ZENABLE, true);
  device->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

void ParticleEmitter::ReAnimate_Emitter(float time /*= 0.0f*/)
{
  current_PE_lifespan = time;
}

void ParticleEmitter::change_texture(std::string new_name)
{
  texture_name = new_name;
  SPIN_ENGINE_SAFE_RELEASE(texture);
  D3DXCreateTextureFromFile(device, ("Assets\\Textures\\" + texture_name).c_str(), &texture);
}
