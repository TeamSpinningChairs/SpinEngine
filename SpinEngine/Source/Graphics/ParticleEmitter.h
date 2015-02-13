/****************************************************************************/
/*!
\author Esteban Maldonado
\par    email: esteban.maldonado@digipen.edu
\par    Course: GAM 200
\brief

Class interface for the Particle Emitter component

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#pragma once
#include"Precompiled.h"
#include"Particle.h"

#define MAX_PARTICLE_COUNT 5000
#define PE_LOOPING 0

//Forward Declarations
class ParticleEmitter;
class IEntity;
typedef IEntity *GameObject;

typedef std::vector<ParticleEmitter*> ParticleEmitVector;
typedef ParticleEmitVector::iterator PartEmitVecIter;


class ParticleEmitter : public IComponent
{
public:
  ParticleEmitter(GameObject owner, emitter_types type = ET_NORMAL, blend_types b_type = BT_ADD,
    unsigned int particle_count = MAX_PARTICLE_COUNT, std::string t_name = NULL,
    D3DXCOLOR color = d3dColors::White, D3DXCOLOR end_color = d3dColors::White);

  ParticleEmitter();
  ParticleEmitter(GameObject owner, ParticleEmitter &rhs);

  ~ParticleEmitter();

  //Inherited methods
  bool Initialize() override;
  void Update(float dt) override;
  void Release() override;

  //Methods
  void Render(IDirect3DDevice9* device);

  //Getters
  unsigned int& GetParticleCount();


  //Setters
  void SetParticleCount(unsigned int pc);
  void ReAnimate_Emitter(float time = 0.0f);
  void change_texture(std::string new_name = "");

  Particle particles[MAX_PARTICLE_COUNT];

  static IDirect3DVertexBuffer9* v_buffer;
  static IDirect3DIndexBuffer9* i_buffer;

  //lifepspan for the paarticle emitter itself
  float current_PE_lifespan;
  float PE_lifespan;

  //particle scale
  float scale;
  float end_scale;

  //start delay for the PE
  float start_delay;

  //lifetime of each particle
  float particle_lifetime;
  float lifetime_invariant;

  //Particles will spawn anywhere between these angles.
  float angle_1, angle_2;
  float speed_1, speed_2;
  float acceleration_1, acceleration_2;
  float spin, spin_variance;

  //gravity for each particle
  float gravity;

  //Intial and end color for each particle
  D3DXCOLOR my_color;
  D3DXCOLOR my_color_end;
  bool blend_colors;
  bool use_blending_operation;

  unsigned int particle_count;

  float x_diff_from_owner;
  float y_diff_from_owner;

  emitter_types emit_type;
  blend_types blend_type;
  blend_factor_types source_factor;
  blend_factor_types dest_factor;

  //FOR SQUARE EMITTERS
  float width, height;

  //FOR CIRCLE EMITTERS
  float radius;

  std::string texture_name;
private:
  IDirect3DDevice9* device;

  IDirect3DTexture9* texture;
  IDirect3DVertexBuffer9* t_buffer;

  unsigned int alive_particles;
  bool kill_pe;

  //bool AreAllParticlesDead();
};