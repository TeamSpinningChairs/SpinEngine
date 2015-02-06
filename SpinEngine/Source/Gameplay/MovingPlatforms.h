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

#include"IComponent.h"
#include"IEntity.h"
#include"rigidbody.h"
#include"SwitchComponent.h"
#include"mathlib.h"

//enumeration which is used to determine how a platform moves 
enum MoveDirection {PLATFORM_LEFT, PLATFORM_RIGHT, PLATFORM_UP, PLATFORM_DOWN, PLATFORM_PACE_HORIZONTAL, PLATFORM_PACE_VERTICAL};

class MovingPlatform : public IComponent, public CollisionDelegate
{
public:

	// default constructor makes a pacing vertical platform 
	MovingPlatform(GameObject parent);

  ~MovingPlatform();

	// create a moving platform of any type
	MovingPlatform(GameObject parent, int type, float distance = 5.0f, float movespeed = 4.0f);
	
	// initializes platform body to point at the owners rigid body 
	bool Initialize();
	void Serialize (DynamicElement* props, Serializer::DataNode* data) {}
	// moves the platform based on its type
	void Update(float dt);
	
	// frees all allocated memory which there shouldn't be any
	void Release();
	
	// incase we want a moving platform that changes to a different one at runtime
	void ChangeType(int type);
	
	// allows us to change the speed of a platform at runtime
	void SetSpeeed(float speed);
	
	// allows us to change how far a platform moves at runtime
	void SetPaceDistance(float Distance);

	bool PlatformActive;

	bool MovingLeft;

	bool MovingRight;

  bool MovingUp;

  bool MovingDown;

  bool StartAtTop;

  bool PaceDownThenUp;

	float PreviousVel;

  void SetPlatformType(int Type);

  void OnCollision(GameObject) override;

  Vector3D PreviousPosition;

private:

	// set this if you want the platform to stop moving for a set amount of time
	float MoveDelay;
	
	// pointer to the rigid body component on the player
	RigidBody * PlatformBody;
	
	// integer representign the type of the platform
	int PlatformType;
	
	// float representing how fast the platform moves
	float MoveSpeed;
	
	// determines how far the object moves in one direction before moving to the next
	float PaceDistance;

	// used to keep track of where the object started from
	Vector2D StartingPoint;

  void SnapToCellX(float &transfcoord, float &rigidcoord); //Given a float value, snaps platform away from a wall
  void SnapToCellY(float &transfcoord, float &rigidcoord); //Given a float value, snaps to nearest int (i.e. border of tile)

  SwitchComponent * Switch;
};

