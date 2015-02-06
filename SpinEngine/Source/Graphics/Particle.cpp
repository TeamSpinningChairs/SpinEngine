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
#include"Precompiled.h"
#include "Particle.h"

//const DWORD CUSTOMVERTEX::FVF = CUSTOMFVF;

Particle::Particle(Vector3D initial_pos, float lifespan) : current_life(0),
lifespan(lifespan), position(D3DXVECTOR3(initial_pos.x, initial_pos.y, initial_pos.z)),
initial_position(D3DXVECTOR3(initial_pos.x, initial_pos.y, initial_pos.z)), active(false),
spin(0.0f), angle(0.0f)
{
}

Particle::~Particle()
{
}

void Particle::render_particle(IDirect3DDevice9* device, IDirect3DVertexBuffer9** t_buffer,
  IDirect3DTexture9* texture, D3DXCOLOR color, D3DXCOLOR end_color)
{
  VOID* pVerts;
  //LOCK THE MEMORY TO MAKE SURE DATA IS PASSED THROUGH
  (*t_buffer)->Lock(0, sizeof(d3dVertex::CUSTOMVERTEX) * 4/*NUM_VERTS*/, (void**)&pVerts, 0);
  d3dVertex::CUSTOMVERTEX* temp = reinterpret_cast<d3dVertex::CUSTOMVERTEX*>(pVerts);
  //set_particle(device);
  if (color != end_color)
  {
    D3DXCOLOR c;

    D3DXColorLerp(&c, &color, &end_color, (current_life / lifespan));

    temp[0].color = temp[1].color = temp[2].color = temp[3].color = c;
  }
  else
    temp[0].color = temp[1].color = temp[2].color = temp[3].color = color;

  (*t_buffer)->Unlock();

  device->SetStreamSource(0, (*t_buffer), 0, sizeof(CUSTOMVERTEX));

  if (texture)
  {
    device->SetTexture(0, texture);
  }
  else
  {
    device->SetTexture(0, NULL);
  }

  device->DrawPrimitive(/*D3DPT_LINESTRIP*/ D3DPT_TRIANGLESTRIP, 0, 2);
  //device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 24, 0, 12);
}

void Particle::set_particle(IDirect3DDevice9* device, float scal, float end_scal /*float camx, float camy, float camz*/)
{

  //JUST KEEP SCROLLING...
  //################################
  /*int h = 90;
  int *hp = &h;
  int **hpp = (&hp);
  int ***hppp = (&hpp);
  int hhh = (*(*(*hppp)));*/
  //################################

  // Before setting the world transform, do the intense mathematics!
  // a. Calculate the Differences
  //static float difx, dify, difz;
  //difx = camx - position.x;
  //dify = camy - position.y;
  //difz = camz - position.z;

  //// b. Calculate the Distances
  //static float FlatDist, TotalDist;
  //FlatDist = sqrt(difx * difx + difz * difz);
  //TotalDist = sqrt(FlatDist * FlatDist + dify * dify);

  // c. Y Rotation
  D3DXMatrixIdentity(&matRotateY);
  //matRotateY._11 = matRotateY._33 = difz / FlatDist;    // cosY
  //matRotateY._31 = difx / FlatDist;    // sinY
  //matRotateY._13 = -matRotateY._31;    // -sinY

  // d. X Rotation
  D3DXMatrixIdentity(&matRotateX);
  //matRotateX._22 = matRotateX._33 = FlatDist / TotalDist;    // cosX
  //matRotateX._32 = dify / TotalDist;    // sinX
  //matRotateX._23 = -matRotateX._32;    // -sinX

  D3DXMatrixRotationZ(&matRotateZ, angle);

  // e. Tranlation
  static D3DXMATRIX matTranslate;
  D3DXMatrixTranslation(&matTranslate, position.x, position.y, position.z);

  // f. Scaling
  static D3DXMATRIX matScale;
  D3DXMatrixIdentity(&matScale);

  if (end_scal != scal)//abs(end_radius - radius) > 0.001)
  {
    matScale._11 = matScale._22 = matScale._33 = scal + ((end_scal - scal) * (current_life / lifespan));
  }

  else
  {
    matScale._11 = matScale._22 = matScale._33 = scal;
  }

  // Now build the world matrix and set it
  device->SetTransform(D3DTS_WORLD, &(matScale * matRotateX * matRotateY * matRotateZ * matTranslate));
}

void Particle::run_particle(float dt, float p_lifetime, float lifetime_invariant, Vector3D pos, float ang_1,
  float ang_2, float spd1, float spd2, float acc1, float acc2, float spin, float spin_invariance)
{
  // handle lifespan
  current_life += dt;
  if (current_life > p_lifetime)
  {

    const float new_time = random_number(p_lifetime - lifetime_invariant, p_lifetime + lifetime_invariant);
    const float new_spin = random_number(spin - spin_invariance, spin + spin_invariance);
    reset_particle(pos, new_time, ang_1, ang_2, spd1, spd2, acc1, acc2, new_spin);
    return;
  }

  velocity += acceleration * dt;
  position += velocity * dt;
  this->angle += this->spin * dt;

}

void Particle::reset_particle(Vector3D pos, float p_lifetime, float ang_1, float ang_2, float spd1,
  float spd2, float acc1, float acc2, float spin)
{
  active = false;
  lifespan = p_lifetime;

  position.x = pos.x;
  position.y = pos.y;
  position.z = pos.z;

  float angle_new = random_number(std::min(ang_1, ang_2), std::max(ang_1, ang_2));
  float speed = random_number(std::min(spd1, spd2), std::max(spd1, spd2));
  float acc = random_number(std::min(acc1, acc2), std::max(acc1, acc2));

  velocity.x = cos((angle_new / 180.0f) * PI) * speed;
  velocity.y = sin((angle_new / 180.0f) * PI) * speed;// random_number(-2.0f, 2.0f);
  velocity.z = 0.0f;

  acceleration.x = cos((angle_new / 180.0f) * PI) * acc;
  acceleration.y = sin((angle_new / 180.0f) * PI) * acc;
  acceleration.z = 0.0f;//random_number(-15.0f, 25.0f);

  this->spin = spin;
  this->angle = 0.0f;

  current_life = 0.0f;
}

void Particle::run_particle_square(float dt, float p_lifetime, float lifetime_invariant, Vector3D pos, float width,
  float height, float spd1 /*= 1.0f*/, float spd2, float acc1,
  float acc2, float angle, float spin, float spin_invariance)
{
  // handle lifespan
  current_life += dt;
  if (current_life > p_lifetime)
  {
    const float new_time = random_number(p_lifetime - lifetime_invariant, p_lifetime + lifetime_invariant);
    const float new_spin = random_number(spin - spin_invariance, spin + spin_invariance);

    reset_particle_square(pos, new_time, width, height, spd1, spd2, acc1, acc2, angle, new_spin);
    return;
  }

  velocity += acceleration * dt;
  position += velocity * dt;
  this->angle += this->spin * dt;
}

void Particle::reset_particle_square(Vector3D pos, float p_lifetime, float width /*= 1.0f*/, float height /*= 1.0f*/,
  float spd1 /*= 1.0f*/, float spd2 /*= 4.0f*/, float acc1 /*= 1.0f*/, float acc2 /*= 20.0f*/, float angle, float new_spin)
{
  active = false;
  lifespan = p_lifetime;
  position.x = pos.x + random_number(-(width / 2.0f), (width / 2.0f));
  position.y = pos.y + random_number(-(height / 2.0f), (height / 2.0f));
  position.z = pos.z;

  //float angle_new = random_number(std::min(ang_1, ang_2), std::max(ang_1, ang_2));
  float speed = random_number(std::min(spd1, spd2), std::max(spd1, spd2));
  float acc = random_number(std::min(acc1, acc2), std::max(acc1, acc2));

  velocity.x = cos((angle / 180.0f) * PI) * speed;
  velocity.y = sin((angle / 180.0f) * PI) * speed;// random_number(-2.0f, 2.0f);
  velocity.z = 0.0f;

  acceleration.x = cos((angle / 180.0f) * PI) * acc;
  acceleration.y = sin((angle / 180.0f) * PI) * acc;
  acceleration.z = 0.0f;//random_number(-15.0f, 25.0f);

  this->angle = 0.0f;
  spin = new_spin;

  current_life = 0.0f;
}

void Particle::run_particle_circle(float dt, float p_lifetime, float lifetime_invariant, Vector3D pos, float radius /*= 1.0f*/,
  float spd1 /*= 1.0f*/, float spd2 /*= 4.0f*/, float acc1 /*= 1.0f*/, float acc2 /*= 20.0f*/, float angle, float spin, float spin_invariance)
{
  // handle lifespan
  current_life += dt;
  if (current_life > p_lifetime)
  {

    const float new_time = random_number(p_lifetime - lifetime_invariant, p_lifetime + lifetime_invariant);
    const float new_spin = random_number(spin - spin_invariance, spin + spin_invariance);
    reset_particle_circle(pos, new_time, radius, spd1, spd2, acc1, acc2, angle, new_spin);
    return;
  }

  velocity += acceleration * dt;
  position += velocity * dt;
  this->angle += this->spin * dt;
}

void Particle::reset_particle_circle(Vector3D pos, float p_lifetime, float radius, float spd1 /*= 1.0f*/,
  float spd2, float acc1, float acc2, float angle, float new_spin)
{
  //t = 2 * pi*random()
  //  u = random() + random()
  //  r = if u > 1 then 2 - u else u
  //  [r*cos(t), r*sin(t)]
  active = false;
  lifespan = p_lifetime;

  float t = 2.0f * PI * random_number(0, 1.0f) * radius;
  float u = random_number(0, radius/*1.0f*/) + random_number(0, radius/*1.0f*/);
  float r = u;
  if (u > radius/*1.0f*/)
  {
    r = radius + radius - u;
  }

  position.x = pos.x + (r * cos(t));
  position.y = pos.y + (r * sin(t));
  position.z = pos.z;

  //float angle_new = random_number(std::min(ang_1, ang_2), std::max(ang_1, ang_2));
  float speed = random_number(std::min(spd1, spd2), std::max(spd1, spd2));
  float acc = random_number(std::min(acc1, acc2), std::max(acc1, acc2));

  velocity.x = cos((angle / 180.0f) * PI) * speed;
  velocity.y = sin((angle / 180.0f) * PI) * speed;// random_number(-2.0f, 2.0f);
  velocity.z = 0.0f;

  acceleration.x = cos((angle / 180.0f) * PI) * acc;
  acceleration.y = sin((angle / 180.0f) * PI) * acc;
  acceleration.z = 0.0f;//random_number(-15.0f, 25.0f);

  this->angle = 0.0f;
  this->spin = new_spin;

  current_life = 0.0f;
}

void Particle::run_particle_diamond(float dt, float p_lifetime, Vector3D pos, float width /*= 1.0f*/,
  float height /*= 1.0f*/, float spd1 /*= 1.0f*/, float spd2 /*= 4.0f*/, float acc1 /*= 1.0f*/,
  float acc2 /*= 20.0f*/, float angle /*= 0.0f*/)
{

}

void Particle::reset_particle_diamond(Vector3D pos, float width /*= 1.0f*/, float height /*= 1.0f*/,
  float spd1 /*= 1.0f*/, float spd2 /*= 4.0f*/, float acc1 /*= 1.0f*/, float acc2 /*= 20.0f*/,
  float angle /*= 0.0f*/)
{
  ///point_from_diamond(x,y,width,height)
  //var r1, r2; r1 = random(1); r2 = random(1);
  //global.x = argument0 + argument2*0.5*(r2 - r1 + 1); 
  //global.y = argument1 + argument3*0.5*(r1 + r2);
}
