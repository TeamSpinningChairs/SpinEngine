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
#include "Precompiled.h"
#include "PlayerController.h"
#include "Settings.h"
#include "NewMessageSystem.h"
#include "DebugMessages.h"
#include "FactoryAccess.h" //To add our lasers to the factory
#include "rigidbody.h"
#include "primitives.h"
#include "TileMapDetection.h"

//Note: Gonna need a way to assign unique controller numbers once we have multiple players.
//Do this in the input system/whatever


#define PIXELS_PER_UNIT 64.0f
#define TIME_TO_DIE 1.5f

#define FREE true //for space in the laser array
#define TAKEN false

PlayerController::PlayerController(GameObject Parent, int ControllerNum) : parent(Parent),
IComponent(Component_Type::CT_Player_Controller, Parent),
CollisionDelegate(Parent),
controller(new PlayerControllerListener(Parent, this, ControllerNum)),
keyboard(new PlayerKeyboardListener(Parent, this)),
myJumpState(JS_Grounded), jumpCount(0), jumpTimer(0), playerIsAlive(true), deathTimer(0),
lastFrameDT(1.0f / 60.0f), fuelRegenTimer(0), jumpButtonReleased(false)
{
  float *temp = NULL;
  GlobalSettings->GetFloatValue("___ PLAYER MOVEMENT SETTINGS ___", temp, false);
  GlobalSettings->GetFloatValue("Player Run Speed", playerRunSpeed, true);
  GlobalSettings->GetFloatValue("Player Jump Velocity", playerJumpVelocity, true);
  GlobalSettings->GetBoolValue("Jetpack Enabled", jetpackEnabled, true);
  GlobalSettings->GetBoolValue("Variable Jump Height Enabled", variableJumpHeightEnabled, true);
  GlobalSettings->GetFloatValue("Variable Jump Time", variableJumpTime, true);
  GlobalSettings->GetFloatValue("Variable Jump Power", variableJumpPower, true);
  GlobalSettings->GetFloatValue("Time To Jump After Leaving Ground", timeToJumpAfterLeftGround, true);
  GlobalSettings->GetFloatValue("Max Downwards Velocity", maxDownwardsVelocity, true);
  GlobalSettings->GetFloatValue("Max Upwards Velocity", maxUpwardsVelocity, true);

  GlobalSettings->GetFloatValue("___ COOL JETPACK/LASER SETTINGS ___", temp, false);
  GlobalSettings->GetIntValue("Max Fuel", maxFuel, true);
  GlobalSettings->GetFloatValue("Fuel Regen Delay", fuelRegenDelay, true);
  GlobalSettings->GetFloatValue("Fuel Regen Speed", fuelRegenSpeed, true);
  GlobalSettings->GetFloatValue("Thrust", thrust, true);
  fuel = *maxFuel;

  GlobalSettings->GetFloatValue("Laser Firing Speed", laserFiringSpeed, true);
  laserTimer = *laserFiringSpeed + 0.1f; //So that we can fire immediately after the level loads
  GlobalSettings->GetFloatValue("Laser Speed", laserSpeed, true);

  Platform = NULL;
  WalkTimer = 0.0f;
  PreviousPlatformPosition.x = 0;
  PreviousPlatformPosition.y = 0;
  PreviousPlatformPosition.z = 0;

  //Create our lasers and add them to the Factory. (They're set inactive while hidden, so there aren't any wasted collision messages)
  Vector3D outofsight = Vector3D(-400.0f, -400.0f, 0.0f);
  for (int i = 0; i < MAX_LASER_COUNT; ++i)
  {
    IEntity *laserObj = GlobalFactory->CreateGameObject("Laser", "Laser.png", outofsight);
    laserObj->GetTransform()->GetScale().x = 0.3f;
    laserObj->GetTransform()->GetScale().y = 0.1f;
    //Create shape and rigidbody
    //@Ask Judy if you're doing this right, you forget what SetStatic does and what to do with SetHalfSize so that the collision box is small enough
    AABB *coll = new AABB(laserObj);
    coll->SetHalfSize(.3, .1); //((2 * 0.3) / 2), ((2 * 0.1) / 2)
    laserObj->AddGameComponent(CT_BoxCollider, coll);

    RigidBody *rigid = new RigidBody(laserObj, coll);
    //rigid->SetStatic();
    rigid->isGhost = true;
    rigid->useGravity = false;
    rigid->SetActive(false);
    laserObj->AddGameComponent(CT_Body, rigid);

    TileMapDetection *tilemap = new TileMapDetection(laserObj, GlobalFactory->GetTileMapData(), GlobalFactory->GetGameObjectList());
    laserObj->AddGameComponent(CT_TileMapDetector, tilemap);

    //Create beam component, add to the pool and indicate that it's available
    LaserBeam *beamComp = new LaserBeam(laserObj, this, *laserSpeed);
    laserObj->AddGameComponent(CT_LASERBEAM, beamComp);
    pool_[i].first = FREE;
    pool_[i].second = beamComp;

  }
  int i = 5;
}

bool PlayerController::Initialize()
{
  GlobalSystem->RegisterListener(MessageTypes::MESSAGE_INPUT, controller);
  GlobalSystem->RegisterListener(MessageTypes::MESSAGE_KEYBOARD, keyboard);
  playerTileCollision = reinterpret_cast<TileMapCollision*>(parent->GetComponent(CT_TileMapCollider));
  playerBody = reinterpret_cast<RigidBody *>(parent->GetComponent(CT_Body));
  playerSpriteRend = reinterpret_cast<SpriteRenderer *>(parent->GetComponent(CT_SpriteRenderer));
  keyboard->Initialize();
  controller->Initialize();

  InitializeCollisionCallback();
  //playerBody->AddTriggerCallback();
  return true;
}

PlayerController::~PlayerController()
{
  GlobalSystem->UnregisterListener(MessageTypes::MESSAGE_INPUT, keyboard);
  GlobalSystem->UnregisterListener(MessageTypes::MESSAGE_KEYBOARD, controller);
}

void PlayerController::Update(float dt)
{
  lastFrameDT = dt; //used in jump code, which is triggered by the messaging system
  laserTimer += dt; //Tick up how long it's been since we last fired a laser

  //When player dies, stop for 2 seconds and then reload
  if (!playerIsAlive)
  {
    deathTimer += dt;
    if (deathTimer > TIME_TO_DIE)
    {
      gGameStateCurr = GS_LOAD;
    }
    return;
  }

  jumpTimer += dt;
  WalkTimer += dt;

  //Cap player's downwards and upwards velocities
  if (playerBody->velocity.y < -(*maxDownwardsVelocity))
    playerBody->velocity.y = -(*maxDownwardsVelocity);
  if (playerBody->velocity.y > *maxUpwardsVelocity)
    playerBody->velocity.y = *maxUpwardsVelocity;

  
  //Keep the timer at 0 while we're grounded
  //Regen fuel while we're grounded
  if (myJumpState == JS_Grounded)
  {
    if (playerTileCollision->BottomIsColliding())
      jumpTimer = 0;

    fuelRegenTimer += dt;
    if (fuelRegenTimer > *fuelRegenDelay)
    {
      fuel = *maxFuel;
    }

    return;
  }

  /* Written by Steven Gallwas prevents player from falling off of platforms*/
  if (myJumpState == JS_PLATFORM)
  {
    fuelRegenTimer += dt;
    if (fuelRegenTimer > *fuelRegenDelay)
    {
      fuel = *maxFuel;
    }

    if (Platform != NULL)
    {

      float PlayerCheckX = abs(Owner->GetTransform()->GetPosition().x - Platform->GetTransform()->GetPosition().x);
      float PlayerCheckY = abs(Owner->GetTransform()->GetPosition().y - Platform->GetTransform()->GetPosition().y);
       

      if(PlayerCheckX < 1.0 && PlayerCheckY < 2.0)
      {
        // calculate how far the platform has moved
        float XDiff = Platform->GetTransform()->GetPosition().x - PreviousPlatformPosition.x;
        float YDiff = Platform->GetTransform()->GetPosition().y - PreviousPlatformPosition.y;


        playerBody->position.x += XDiff;
        Owner->GetTransform()->GetPosition().x += XDiff;

        float PlatformYVel = ((RigidBody*)Platform->GetComponent(CT_Body))->velocity.y;
        float PlatformXVel = ((RigidBody*)Platform->GetComponent(CT_Body))->velocity.x;

        playerBody->velocity.y = PlatformYVel;
         
        if(YDiff <= 0)
        {
          playerBody->position.y += YDiff;
          Owner->GetTransform()->GetPosition().y += YDiff;
        }
          // move the player with the platform
        PreviousPlatformPosition = Platform->GetTransform()->GetPosition();
        
      }
        
      else
      {
        Platform = NULL;

          //myJumpState = JS_Grounded;
      }

    }
  }// end of Steven Code 

  //We're jumping:
  
  //Reset us when we hit the ground
  if (playerTileCollision->BottomIsColliding() && jumpTimer > BIT_MORE_THAN_ONE_FRAME)
  {
    jumpCount = 0;
    jumpTimer = 0;
    myJumpState = JS_Grounded;
    jumpButtonReleased = false;
    return;
  }
  //Keep our variable-height jump going up to the max height
  if (myJumpState == JS_VariableJumping)
  {
    playerBody->velocity.y = *playerJumpVelocity * (*variableJumpPower);

    //At max height, stop our variable jump.
    if (jumpTimer > *variableJumpTime)
    {
      jumpTimer = 0;
      myJumpState = JS_Jumping;
    }
      
  }

  //AlignToNearestPixel();
  
}

void PlayerController::Release()
{

}

//Only ever called by the controllers:
void PlayerController::PressJump()
{
  if (!playerIsAlive)
    return;

  //Possible options:
  //A: If we're not jumping, and we're on solid ground or left solid ground within a few frames,
  //   apply jump velocity and change our state
  if ((myJumpState == JS_Grounded || myJumpState == JS_PLATFORM) && (playerTileCollision->BottomIsColliding() || jumpTimer < *timeToJumpAfterLeftGround))
  {
    if (*variableJumpHeightEnabled)
      myJumpState = JS_VariableJumping;
    else
      myJumpState = JS_Jumping;

    if (*variableJumpHeightEnabled)
      playerBody->velocity.y = *playerJumpVelocity * (*variableJumpPower);
    else
      playerBody->velocity.y = *playerJumpVelocity;

    jumpTimer = 0.0f;
    fuelRegenTimer = 0.0f;
    ++jumpCount;
    return;
  }

  //B: If we're jumping, and not holding down the jump button, and jetpacks are enabled, apply jetpack velocity
  if (myJumpState == JS_Jumping && *jetpackEnabled && fuel > 0 && jumpButtonReleased)
  {
    fuel -= 1;
    playerBody->velocity.y = *thrust;
    //playerBody->velocity.y = *thrust;
    return;
  }

  //C: If we're falling, jetpack!
  if (myJumpState == JS_Grounded && playerBody->velocity.y < 0.0f && *jetpackEnabled && fuel > 0)
  {
    fuel -= 1;
    playerBody->velocity.y = *thrust;
    //playerBody->velocity.y = *thrust; @alternate between these two options, see which feels better
    return;
  }
}

//Need this to be passed other controller variables- namely, movement direction (to boost up, down, left, or right)
void PlayerController::HoldJump()
{
  //I originally had the Jetpack code here, but changed it to be based on individual button presses.
  //Move options B and C back here and change to multiplying thrust by dt if you want normal jetpacks
}

void PlayerController::ReleaseJump()
{
  //A: If we have variable jump height, switch us to the upward state
  if (*variableJumpHeightEnabled && myJumpState == JS_VariableJumping)
  {
    myJumpState = JS_Jumping;
    playerBody->velocity.y = *playerJumpVelocity;
    jumpButtonReleased = true;
  }
}

void PlayerController::OnCollision(GameObject otherObject)
{
    if ((otherObject->GetComponent(CT_MOVPLATFORM)) && jumpTimer > BIT_MORE_THAN_ONE_FRAME)
    {
        myJumpState = JS_PLATFORM;
        jumpTimer = 0;
        Platform = otherObject;
        PreviousPlatformPosition = otherObject->GetTransform()->GetPosition();
        jumpButtonReleased = false;
        fuelRegenTimer = 0.0f;
    }

    if ((otherObject->GetComponent(CT_SPAWN_BLOCK) || otherObject->GetComponent(CT_DESTROY_ON_ACTIVATE))  && jumpTimer > BIT_MORE_THAN_ONE_FRAME)
    {
        myJumpState = JS_Grounded;
        jumpTimer = 0;
        jumpButtonReleased = false;
        fuelRegenTimer = 0.0f;
    }
    
    //If we hit a "kill the player" object, reload at last checkpoint
    //(after 2 seconds- see the beginning of Update)
    if (otherObject->GetComponent(CT_KILLPLAYER))
    {
      KillPlayer();
    }

}


  int PlayerController::getJumpState(void)
  {
    return myJumpState;
  }

  void PlayerController::AlignToNearestPixel()
  {
    //playerBody->position.x = MathF::Round(playerBody->position.x * PIXELS_PER_UNIT) / PIXELS_PER_UNIT;
   // playerBody->position.y = MathF::Round(playerBody->position.y * PIXELS_PER_UNIT) / PIXELS_PER_UNIT;
    
    //Owner->GetTransform()->GetPosition().x = playerBody->position.x;
   // Owner->GetTransform()->GetPosition().y = playerBody->position.y;

  }

  float PlayerController::getJumpTimer(void)
  {
      return jumpTimer;
  }


  void PlayerController::KillPlayer()
  {
    playerIsAlive = false;
    parent->SetInVisible();
  }

  //Called by one of our laser beams once it collides with something
  //CURRENTLY ASSUMES THAT THE LASER BEAM RESETS ITSELF IN ITS ONCOLLISION
  void PlayerController::ReturnLaser(LaserBeam *objtoreturn)
  {
    //Set availability to true at its spot in the list.
    for (unsigned i = 0; i < MAX_LASER_COUNT; ++i)
    {
      if (pool_[i].second == objtoreturn)
      {
        pool_[i].first = FREE;
        return;
      }
    }
  }

  //Called in FireLaser.
  bool PlayerController::GetLaser(LaserBeam **objtotake)
  {
    //Iterate through the pool. Give the first available instance of the object.
    for (unsigned i = 0; i < MAX_LASER_COUNT; ++i)
    {
      if (pool_[i].first == FREE)
      {
        *objtotake = pool_[i].second;
        pool_[i].first = TAKEN;
        return true;
      }
    }

    return false;
  }

  //Fires a laser in the aimed direction (theta is in radians)
  void PlayerController::ControllerFireLaser(float &theta)
  {
    if (laserTimer < *laserFiringSpeed)
      return;
    laserTimer = 0.0f; //Reset the timer
    
    //Get the laser. (If too many lasers are in the air, do nothing.)
    LaserBeam *toLaunch;
    if (!GetLaser(&toLaunch))
     return;

    Vector3D launchPosition = parent->GetTransform()->GetPosition();
    launchPosition.y += 0.8f; //Magic number, make the laser spawn in their head

    Vector2D vel(MathF::ApproxCos(theta) * *laserSpeed, MathF::ApproxSin(theta) * *laserSpeed);
    
    toLaunch->Launch(launchPosition, theta, vel);
  }

  void PlayerController::KBFireLaser(bool isFacingRight)
  {
    //If it's been more than .2 (or however many we set) seconds, fire a laser.
    if (laserTimer < *laserFiringSpeed)
      return;

    laserTimer = 0.0f; //Reset the timer

    //Get the laser. (If too many lasers are in the air, do nothing.)
    LaserBeam *toLaunch;
    if (!GetLaser(&toLaunch))
      return;

    //Fire the laser. (From this point on, the beam will take care of everything,
    //including resetting itself.)
    if (isFacingRight)
      toLaunch->Launch(parent->GetTransform()->GetPosition(), 0.0f, Vector2D(*laserSpeed,0.f));
    else
      toLaunch->Launch(parent->GetTransform()->GetPosition(), PI, Vector2D(-(*laserSpeed),0.f)); //0 is default rotation, PI is rotated 180 deg (one radian)

  }
