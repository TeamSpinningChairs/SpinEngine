//I love this name and I refuse to change it

//A class to contain a group of heavy particles, and move them around.
//Is that what you want? You want them to move in "currents", yeah, but you also want them all to have access
//to their nearest neighbors.
//And you want a thread updating all their nearest neighbors every few frames... (separate thread)


//So, have the particles (okay, basically sprites) ALL be in here, then have certain "Parents" for each? 
//This way, you can access all the particles as one big graph, BUT you can also access each Parent and have it update its kids.
//AND you can have "unparented" sprites doing their thing.
#pragma once
#include "Precompiled.h"

class ShiftingRoilingParticle;


class ShiftingRoilingMassThing
{
public:

  void Initialize(Vector3D *wallTransform); //Creates all of the heavy particles in the Factory (and eventually, registers the thread for them)

  void ThreadUpdate();

private:
  Vector3D *wallPos;
  //Contain the second thread, and the vector of heavy particles
  std::vector<ShiftingRoilingParticle *> coolRoilingParticles;

};