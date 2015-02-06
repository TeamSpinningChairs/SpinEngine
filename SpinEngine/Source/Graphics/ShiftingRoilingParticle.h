//I also love this name and refuse to change it

//A class for the individual "heavy" particles (basically just sprites)

#pragma once

#include "Precompiled.h"


class ShiftingRoilingParticle : public IComponent
{
public:

  ShiftingRoilingParticle();

  //These are required, but unused
  bool Initialize() override;
  void Serialize (DynamicElement* props, Serializer::DataNode* data) {}
  void Update(float dt) override;
  void Release() override;


  void Initialize(Vector3D *pos, Vector3D *wallpos); //Gets nearest neighbor links, stores position
  void Update(); //The actual "update", updated by the wall thread

private:
  Vector3D *myPos; //Our transform
  Vector3D *wallPos; //The wall's transform (so we can stay relative to it)
  float wallsPrevXPos; //Our horizontal position relative to the wall (so that we "follow" it while doing everything else)

  Vector2D startPos; //vector from the wall's current position to our starting position
  Vector2D currVec; //Our random vector to move along
  float currTravelTime; //How long we've been moving away from our "origin"
  float mySpeed;
  //Graph stuff
};