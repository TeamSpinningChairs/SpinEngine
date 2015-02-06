/****************************************************************************/
/*!
\author Steven Gallwas
\par    email: s.gallwas\@digipen.edu
\par    Course: GAM 200
\brief
This file contains the implementation for the base messaging system.

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#pragma once

#include "Precompiled.h"
#include "IComponent.h"
#include "IEntity.h"
#include "mathlib.h"
#include "WallHand.h"
#include "Settings.h"
#include "mathlib.h"
#include "ShiftingRoilingMassThing.h" //Forward declaration wouldn't work, since it's not pointed to

#define TargetCount 100
#define DYNAMIC_HANDCOUNT 40
#define RESERVED_HANDS 5
#define LAUNCH_DELAY .15f
#define PARTICLE_EM_COUNT 3

//Forward Declarations
class DynamicHand;
class WallListener;
class ParticleEmitter;

struct Target
{
	Target();

	GameObject TargetObject;
	bool Targeted;
};

class WallComp : public IComponent
{
	public:
	//  Constructor
	WallComp(IEntity * Parent);
  ~WallComp();

	// function to choose a random target from the vector of targets
	GameObject ChooseTarget(void);

	// launches a Tentacle at a target
	void LaunchTentacle(void);

	// inherited from icomponent
	bool Initialize();
	void Serialize (DynamicElement* props, Serializer::DataNode* data) {}
	void Update(float dt);
	void Release();
  void GrabImmediately(GameObject);
  void WallStartup(void);
  GameObject CreateHand(void);

	Target Targets[TargetCount];

	int ActiveTentacleCount;

	bool WallActive;

	Vector3D Position;

  //First hand puts its current tilemap index target in [0], 2nd in [1], etc...
  int activeTargets[HANDCOUNT];
  
  float GetMoveSpeed(void);

  float WallDelay;

  float ParticleCreateTimer;

  GameObject WallImage;
  GameObject WallImage2;

  private:
  
    GameObject Player;
    GameObject parent;

    

  WallListener*  Wall_Listener;

  float *moveSpeed;
  float prevMoveSpeed;
  float LaunchTimer;

  ShiftingRoilingMassThing particleSwarm;


  ParticleEmitter* WallParticleEmitters[PARTICLE_EM_COUNT];

  Vector3D ChooseRandomDirection(void);

  Vector2D StartingPoint;

  WallHand * Hands[HANDCOUNT];
  DynamicHand * Dynamic_Hands[DYNAMIC_HANDCOUNT + RESERVED_HANDS];
};