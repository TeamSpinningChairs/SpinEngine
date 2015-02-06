#include "Precompiled.h"
#include "ShiftingRoilingParticle.h"
#include "FactoryAccess.h" //To add individual heavy particles
#include "mathlib.h" //inexpensive sqrt

//Miiiight eventually need to have these scale (depending on resolution, that sort of thing)
#define PARTICLE_FIELD_WIDTH 3.5
#define PARTICLE_FIELD_HEIGHT 15

#define MAX_PARTICLE_TRAVEL_TIME 0.5f

ShiftingRoilingParticle::ShiftingRoilingParticle() : IComponent(CT_CoolRoilingParticle){}

bool ShiftingRoilingParticle::Initialize(){ return true; }
void ShiftingRoilingParticle::Release(){}

//Move everything here to the other Update function once you've set up the separate thread
void ShiftingRoilingParticle::Update(float dt)
{ 
  //Keep us up with the wall's movement
  myPos->x += wallPos->x - wallsPrevXPos;
  startPos.x += wallPos->x - wallsPrevXPos;

  //If it's time to, choose a new random vector for movement
  if (currTravelTime < 0)
  {
    if (abs(myPos->x - startPos.x) < 0.2f && abs(myPos->y - startPos.y) < 0.2f)
    {
      mySpeed = static_cast<float>(rand() % 8);
      if (mySpeed < .01f) //I should probably use a float comparison function, but it's all the way over theeeeeeeeere
        mySpeed = 4.0f;

      currVec.x = (static_cast<float>(rand() % 10) / mySpeed) * static_cast<float>(std::rand() % 2 == 0 ? -1 : 1);
      currVec.y = (static_cast<float>(rand() % 10) / mySpeed) * static_cast<float>(std::rand() % 2 == 0 ? -1 : 1);
      currTravelTime = 0;
    }
  }

  //Move along our vector
  currTravelTime += dt;
  myPos->x += currVec.x * dt;
  myPos->y += currVec.y * dt;

  //If we're too far from our origin, head back
  if (/*myPos->x - wallPos->x > static_cast<float>(PARTICLE_FIELD_WIDTH) ||*/ abs(myPos->x - startPos.x) > 2.0f || abs(myPos->y - startPos.y) > 2.0f)
  {
    currVec = startPos - Vector2D(myPos->x,myPos->y);
    currTravelTime = -20.0f;
  }
  

  //Save our current position so that we can "catch up" to the wall appropriately, next frame
  wallsPrevXPos = wallPos->x;
}


void ShiftingRoilingParticle::Initialize(Vector3D *pos, Vector3D *wallpos)
{
  myPos = pos;
  wallPos = wallpos;

  //Give the particle a random position within PARTICLE_FIELD_WIDTH and HEIGHT
  Vector3D randPos;
  randPos.x = wallPos->x - 1.0f + static_cast<float>((std::rand() % static_cast<int>(PARTICLE_FIELD_WIDTH)) * (std::rand() % 2 == 0 ? -1 : 1));
  randPos.y = wallPos->y - 4.0f + static_cast<float>((std::rand() % PARTICLE_FIELD_HEIGHT) * (std::rand() % 2 == 0 ? -1 : 1));
  *myPos = randPos;

  startPos.x = randPos.x;
  startPos.y = randPos.y;
  wallsPrevXPos = wallPos->x;
  currTravelTime = -1.0f; //Negative == "choose new random direction to move"
  mySpeed = static_cast<float>(rand() % 8);
  if (mySpeed < .01f) //I should probably use a float comparison function, but it's all the way over theeeeeeeeere
    mySpeed = 4.0f;
}

void ShiftingRoilingParticle::Update()
{

}