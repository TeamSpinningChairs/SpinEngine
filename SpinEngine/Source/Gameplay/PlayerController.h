/****************************************************************************/
/*!
\author Henry Morgan, Steven Galwas
\par    email: henry.morgan@digipen.edu
\par    Course: GAM 200
\brief

Manages player controls. Contains a KeyboardListener and ControllerListener
that directly affect horizontal velocity and call the jump functions within
this class. This class controls jump and movement states, all options and
settings for double jump, variable jump height, jump speed, etc... as well
as maximum fall speed and all OnCollision events that should affect the
player object.

(Steven's code/contributions are throughout, but some parts are noted
specifically.)

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#pragma once

#include "IComponent.h"
#include "IEntity.h"
#include "rigidbody.h"
#include "SpriteRenderer.h"
#include "PlayerControllerListener.h"
#include "PlayerKeyboardListener.h"
#include "collision_tilemap.h"
#include "ObjectPool.h"
#include "LaserBeam.h"

#define BIT_MORE_THAN_ONE_FRAME 0.02f
#define MAX_LASER_COUNT 20 //Not a gameplay thing, just an arbitrary player-cannot-possibly-have-more-than-this-in-the-air-at-once count

//PlayerKeyboardListeners and/or PlayerControllerListeners will call the public functions in here.
//This should access the player object's rigidbody, sprite, etc...

//Not all that much will happen in Update, this is mostly a common point of reference
//for our Keyboard and Controller inputs.
//Changes made to the rigidbody, etc... here can be stopped later on by tilemap collision
//or box colliders. @Check with Judy to see if there's anything in place to guarantee
//rigidbodies are updated AFTER any boxcollider they have.

//Potentially: Have the listeners affect our acceleration/jump rigidbodies.


enum JumpStates //Used in Jump()
{
  JS_Grounded, //Or falling without having jumped
  JS_VariableJumping, //If we have variable jump height enabled, this is the initial "button held down" jump
  JS_Jumping,
  JS_PLATFORM
};

class PlayerController : public IComponent, public CollisionDelegate
{
public:

	PlayerController(GameObject Parent, int ControllerNum);
  ~PlayerController();

	bool Initialize();
	void Serialize (DynamicElement* props, Serializer::DataNode* data) {}
	void Update(float dt);
	
	void Release();

  //Called by imput listeners:
  void PressJump();
  void HoldJump();
  void ReleaseJump();

  //Can be modified, but should only be modified by tweak bar at runtime
  //(unless we introduce mechanics to slow the player down, or something)
  float *playerRunSpeed;
  float *playerJumpVelocity;

  int getJumpState(void);
  float WalkTimer;
  float getJumpTimer(void);

  void KillPlayer();

  void KBFireLaser(bool isFacingRight);
  void ControllerFireLaser();
  void ReturnLaser(LaserBeam *);

  bool Active;

private:
  Vector3D PreviousPlatformPosition;
  GameObject Platform;
	RigidBody *playerBody;
  SpriteRenderer *playerSpriteRend;
  TileMapCollision *playerTileCollision;

  void AlignToNearestPixel();

  //Both of these have listeners that register with the messaging system.
  PlayerControllerListener *const controller;
  PlayerKeyboardListener *const keyboard;

  GameObject parent;

  JumpStates myJumpState;

  bool *jetpackEnabled;
  bool *variableJumpHeightEnabled;
  unsigned jumpCount;
  float jumpTimer;
  float *variableJumpTime;
  float *variableJumpPower;
  float *maxDownwardsVelocity;
  float *maxUpwardsVelocity;
  bool jumpButtonReleased; //if the jump button has been released, after jumping, before hitting the ground

  int fuel;
  int *maxFuel;
  float *fuelRegenDelay;
  float *fuelRegenSpeed;
  float fuelRegenTimer;
  float *thrust;
  float lastFrameDT;

  float laserTimer;
  float *laserFiringSpeed;
  float *laserSpeed;

  float airbornTimer;
  float *timeToJumpAfterLeftGround;
  void OnCollision(GameObject) override;

  bool playerIsAlive;
  float deathTimer;

  bool GetLaser(LaserBeam**);

 // ObjectPool<LaserBeam*> laserBeams;
  std::pair<bool, LaserBeam*> pool_[MAX_LASER_COUNT];
  //DONE//In constructor for PlayerController, create all LaserBeams, and add them to the Factory. (This will require FactoryAccess.)
  //DONE//While creating them, add them to the ObjectPool. (Just Get each pointer and point it to a LaserBeam, then return them all at once afterwards.)
  //DONE//Then, the Factory will get rid of them on level change, and you can use the Pool during gameplay to access any given laser beam.

  //They're in the Factory so that the Physics system will update and they can have OnCollision behavior.

  //What else? Need to actually spawn them, rotate, calculate appropriate velocity, etc..., so get the prototype laser info
  //Have the Jetpack tweaks ready so you can do Jetpack stuff once Steven enables tracking both sticks
  //@@@:
  //Basically, have laser and jetpack functionality working with the arrow keys. Jetpack with arrows after jumping and releasing key, lasers by hitting spacebar and launching in current direction.

  //Additional things to do:
  //Give laser beams access to the tilemap array. Have them "Collide" if they run into anything.
  //Assign laser beams a velocity and angle (and launch position) as appropriate. (Wait for Steven's thing)
  //Create "destructible" tiles, have laser impacts change their sprite
  //Create the "scrap" pickup
  

  //Current priority: Limit jetpack uses to individual "bursts". Each burst gives one frame of thrust (read: adds to the player's velocity) -> think of how it works in spacewalks.
  //The player will end up pressing the A button a bunch of times, in different directions, to get where they want. Makes it less ponderous, more action-y.
  //Require multiple bursts to build up speed, allowing for rapid movement or precise movement (not both) and encouraging lots of urgent adjustments and hectic mid-air activity.
  //Disable explicit left-right movement while in the air? Or slow it down? (probably that). Goal would be to make the player want left/right thrust as well as up/down.

};
