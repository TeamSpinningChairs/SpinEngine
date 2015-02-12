#include "Precompiled.h"
#include "LaserBeam.h"
#include "PlayerController.h"
#include "CollisionDelegate.h"
#include "IEntity.h"
#include "IComponent.h"
#include "TileMapDetection.h"
#include "FactoryAccess.h" //to remove certain tiles when we hit them

#define MAX_FLIGHT_TIME 6.0f
#define DESTRUCTIBLE_TILE_ID 2 //the second tile in the Tiled spritesheet (and in Master.json), 0 being empty space and 1 being desert

LaserBeam::LaserBeam(IEntity *parent, PlayerController *player, float speed) : IComponent(CT_LASERBEAM, parent), CollisionDelegate(parent),
myPlayer(player), myParent(parent), flightTimer(0), flying(false), flySpeed(speed)
{

}

bool LaserBeam::Initialize()
{
  InitializeCollisionCallback();
  hidePosition = myParent->GetTransform()->GetPosition();

  tileMapDetection = reinterpret_cast<TileMapDetection*>(myParent->GetComponent(CT_TileMapDetector));
  rigid = reinterpret_cast<RigidBody*>(myParent->GetComponent(CT_Body));
  return true;
}
void LaserBeam::Update(float dt)
{
  if (!flying)
    return;

  flightTimer += dt;
  if (flightTimer > MAX_FLIGHT_TIME)
    HideAndReturn();

  GameObject currTile;
  //Check the tilemap for any collisions (the physics system will do the rest)
  //adding 1/2 the width and height of the laser (so that we're checking the center)
  int currID = tileMapDetection->GetGameObject(Vector2D((rigid->position.x), rigid->position.y + 1.0f), currTile);
 // int currID = tileMapDetection->GetCurrentGameObject(currTile);
  if (currID == 0)
    return;

  //Regardless of ID,, hide
  HideAndReturn();
  //If ID == destroyable, destroy, then hide
  if (currID == DESTRUCTIBLE_TILE_ID)
  {
    tileMapDetection->RemoveCurrentTileFromMap(true);
   // GlobalFactory->RemoveTileObject(currTile->GetEntityId());
    int i = 5;
    i++;
  }

}
void LaserBeam::Release()
{

}

//Set us active, initialize any variables correctlyGetTransform()->GetPosition()
//Theta is our intended rotation in radians
void LaserBeam::Launch(Vector3D pos, float theta, Vector2D velocity)
{
  flying = true;
 rigid->SetActive(true);

  //Set our position and rotation in preparation for launch
  myParent->GetTransform()->GetPosition() = pos;
  rigid->position.x = pos.x;
  rigid->position.y = pos.y;

  myParent->GetTransform()->GetRotation().z = theta;

  rigid->setVelocity(velocity);
}

//This doesn't involve collisions with the tilemap (which is outside of the physics system)
void LaserBeam::OnCollision(GameObject collObj)
{  
  //Ignore collisions with other lasers
  if (collObj->GetComponent(CT_LASERBEAM) != nullptr)
    return;
  //Ignore collisions with players
  if (collObj->GetComponent(CT_Player_Controller) != nullptr)
    return;

  //All effects of the collision should be within the affected object's OnCollision. (I.E. destroyable blocks)

  HideAndReturn();
}

//Reset any variables we have, go back to the hidden position, and return ourselves to the array of accessible lasers
void LaserBeam::HideAndReturn()
{
  myParent->GetTransform()->GetPosition() = hidePosition;
  rigid->position = Vector2D(hidePosition.x,hidePosition.y);
  flying = false;
  flightTimer = 0;
  rigid->SetActive(false);
  myPlayer->ReturnLaser(this);
}
