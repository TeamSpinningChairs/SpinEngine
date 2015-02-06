/****************************************************************************/
/*!
\author Henry Morgan and Steven Gallwas
\par    email: henry.morgan\@digipen.edu & s.gallwas\@digipen.edu
\par    Course: GAM 200
\brief
This file contains the implementation for the base messaging system.

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#pragma once 

#include "IComponent.h"
#include "IEntity.h"
//#include "ChaseComponent.h"
#include "TileMapDetection.h"

#define RETURN_DISTANCE 1.5f
#define MAX_DISTANCE 16.0f
#define GRAB_DISTANCE 1.0f

#define HANDCOUNT 1

class WallHand : public IComponent
{
public:
	WallHand(GameObject Parent, GameObject Wall, int handNumber, int *targets);

	bool Initialize();
	void Serialize (DynamicElement* props, Serializer::DataNode* data) {}
	void Update(float dt);

	void Release();

	void SetTarget(GameObject);

	void SetStartingPoint(Vector3D &startPos);

	bool Active;
	
	bool Chasing;
	
	bool Returning;

	

  void AdjustMoveSpeed(float wallMoveSpeed);

private:

  enum HandStates
  {
    HST_CHASING,
    HST_RETURNING
  };
  HandStates myState;
  bool isEnteringState;

  int moveSpeed;

  void ChaseUpdate(float dt);
  void ReturnUpdate(float dt);

  // this is the current target of the wall
	GameObject myTarget;
  Vector3D *myTargetsPosition;
  float myTargetXUnitVector;
  float myTargetYUnitVector;

  int currObjIndex;
  float returnTimer; //so we get time to go back after returning to the wall

  //This is our starting point and the point we return to
  //(relative to the wall's position)
  Vector2D StartingPoint;

  //Choose a tile to go after, check if it's valid
  void ChooseTarget();
  bool TargetIsValid();

  int myHandNumber;
  int *allTargets;
  Vector2D myTargetVector;

  Vector2D GetRandomTargetVector();
	

	// this is how far the hand can move from the wall before returning back to the wall.
	float MaxLength;

  Transform *myTransform;

	// this is a pointer back to the main wall object will be needed for selecting additional targets
	GameObject Wall;

	//ChaseComponent * ChaseComp;

	TileMapDetection * TileCheck;

	GameObject ChildObject;
};

