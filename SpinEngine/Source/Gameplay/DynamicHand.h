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

#include "IComponent.h"
#include "Wall.h"
#include "TileMapDetection.h"
#include "CollisionDelegate.h"

#define RETURN_SPEED 1.0f
#define BASE_AMPLITUDE 1.0f
#define BASE_DURATION  1.2f
#define BASE_RANGE 15.0f
#define DYNAMIC_GRAB 2.0f
#define LINK_COUNT 25

enum HandState
{
	HAND_CHASING,
	HAND_RETURNING,
	HAND_INACTIVE
};

class DynamicHand : public IComponent, public CollisionDelegate
{
public:

	// constructor takes a pointer to the parent obj and a pointer to the wall
	DynamicHand(GameObject Parent, GameObject Wall);

	// initializes the tile deteactor object
	bool Initialize();

	void Serialize (DynamicElement* props, Serializer::DataNode* data) {}

	// we will have two versions of Re_Initialize one for target game objects, and 1 for a specific point on the map.
	void Re_Initialize(GameObject newTarget);

	void Re_Initialize(Vector3D);

	// this function handles how the hand moves
	void Update(float dt);

	// deletes all allocated data
	void Release();

	// function to set the target of the wall we may not need this, but i'll leave it here just in case
	void SetTarget(GameObject);

	// we'll have the starting point for the dyanamic hands be at the center of the wall
	void SetStartingPoint(Vector3D &startPos);

  void CheckTargets(void);

	// function to set the hand as active
	void ActivateHand(void);

	void DeActivateHand(void);

	bool GetHandActive(void);

	// the number of the target in the wall's list of targets this will be used to remove the target, or set it as targetable again.
	int TargetNumber;

	int currentState;

  // how far the hand can travel before returning to the wall
  float ChaseRange;

  RigidBody * HandBody;

private:

	// variables for sin wave calculations
		// a variable to track the angle between the wall and the target
		float angle;

		// variable to determine how high we want the peaks and valleys for the wave to be
		float WaveAmplitude;

		// variable to 
		float WavePeriodScalar;

    Vector3D OwnerPosition;
	// variables required to calculate the hands movement

		// vector representing the origin of the hand
		Vector3D StartingPoint;

		// the x position of the object, we'll need to increment this as the hand moves forward
		float xPosition;

		// variable for how long we want it to take the hand to reach its target
		float HandDuration;
	
		// the speed at which the hand will return back to the wall
		float ReturnSpeed;
		
		
		// variable to track how far the target is from the hand
		float TotalDistance;
		
		// variable to track how long the hand has been active used to determine x position
		float TimeActive;
	
	// Variables to track when to create links

		// variable to track when a link is created 
		float TimeBetweenLinks;

		// timer variable to be used when determining when to create links
		float LinkTimer;
		
		// array of child links
		GameObject Links[LINK_COUNT];

		int ActiveLinks;

	// general private variables

		// pointer to the wall
		GameObject WallOwner;

		// flag to determine if the hand has contacted its target
		bool HitObject;
	
		// this is the target that the wall is currently chasing
		GameObject Target;

		GameObject CapturedObj;

		// if a hand is not active, and there are no other available targets, the hand will be launched in a random direction.
		bool HandActive;

		// a pointer to the tilemapdetector component that will be on hand game object.
		TileMapDetection * TileDetection;

    WallComp * WallComponent;

	// Helper functions 

		// helper function to move the hand in a sine wave motion
		void DynamicHandMovement();

		// calculates the distance between the hand and another point
		float CalculateDistance(GameObject);

		// calculates the distance between the hand and another point
		float CalculateDistance(Vector3D);

		float CalculateDistance(GameObject TargetObject, GameObject StartObj);

		// calculates the hands next position based on where it started, where is needs to end, and how long it should take
		float CalculateNextPosition(float starting, float ending, float TotalTime, float Duration);

		void CheckforTile(void);

		void CreateLink(Vector3D position, float angle);

		void LinkReturn(float dt);

    void StraightenLink(void);

		void HandReturn();

    void OnCollision(GameObject) override;
};