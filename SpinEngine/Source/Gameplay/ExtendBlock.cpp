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

#include "Precompiled.h"
#include "ExtendBlock.h"
#include "FactoryAccess.h"
#include "SpawnedBlock.h"
#include "TileMapDetection.h"
#include "NewMessageSystem.h"
#include "ProximityActivate.h"

/*
Extend block is dependant on the game object having a trigger on collision, rigid body, switch component, 
*/
ExtendBlock::ExtendBlock(GameObject Parent) : IComponent(Component_Type::CT_EXTEND_BLOCK, Parent)
{
    for (int i = 0; i < SPAWN_TOTAL; ++i)
    {
        CreateDirections[i] = false;
        CurrentBlockPosition[i] = this->Owner->GetTransform()->GetPosition();

        //CreateDirections.push_back(false);
        //CurrentBlockPosition.push_back(this->Owner->GetTransform()->GetPosition());
    }

    CreateTimer = 0.25f;
    
    BlockSwitch = NULL;
}


bool ExtendBlock::Initialize()
{
    this->BlockSwitch = reinterpret_cast<SwitchComponent *>(Owner->GetComponent(CT_SWITCH));
    TileDetection = reinterpret_cast<TileMapDetection *>(this->Owner->GetComponent(CT_TileMapDetector));
    return true;
}

void ExtendBlock::Update(float dt)
{
    if (BlockSwitch->GetSwitchStatus() == true)
    {
        CreateTimer += dt;

        for (int i = 0; i < SPAWN_TOTAL; ++i)
        {
            if (CreateDirections[i] == true)
            {
                if (CreateTimer > 0.16)
                {
                    //CreateDirections[i] = false;
                    CreateBlock(i);
                }
            }
        }
        
        if (CreateTimer > 0.16)
        {
            CreateTimer = 0.0f;
        }

    }
}

void ExtendBlock::Release()
{

}

void ExtendBlock::CreateBlock(int Direction)
{
    // create a game object that looks like a standard tile.
    /*
    the game object will need to have a static rigid body, box collider, 
    and a spawned block component which will deactivate the original block 
    when it collides with something other than the player
    */
   
    // update the location of the new block to be one block ahead of the next block
      
      bool blockcheck = false;
      ProximityActivateMessage ActivateMsg(blockcheck);

  switch (Direction)
    {
    case SPAWN_UP:
        CurrentBlockPosition[SPAWN_UP] += Vector3D(0,2,0);
        ActivateMsg.CheckDir = AD_UP;
        break;

    case SPAWN_DOWN:
        CurrentBlockPosition[SPAWN_DOWN] += Vector3D(0, -2, 0);
                ActivateMsg.CheckDir = AD_Down;
        break;

    case SPAWN_LEFT:
        CurrentBlockPosition[SPAWN_LEFT] += Vector3D(-2, 0, 0);
         ActivateMsg.CheckDir = AD_Left;
        break;

    case SPAWN_RIGHT:
        CurrentBlockPosition[SPAWN_RIGHT] += Vector3D(2, 0, 0);
         ActivateMsg.CheckDir = AD_Right;
        break;

    default:
        break;
    }
    
    Vector3D test = CurrentBlockPosition[Direction];


    if(TileDetection->AddTileToMap("Tile_Ground.png", test.x, test.y) == false)
    {
        DeactivateDirection(Direction);
        return;
    }

    ActivateMsg.Position = CurrentBlockPosition[Direction];
    GlobalSystem->MyDispatchMessage(&ActivateMsg);

    
     if(blockcheck == true)
    {
        DeactivateDirection(Direction);
        return;
    }
	   
    // create the base game object.
    //GameObject newblock = GlobalFactory->CreateGameObject("spawnedblock", "DefaultTile", test);

    //GameObject newblock = GlobalFactory->CreateGameObject("hand", "Tile_Ground.png", test);

    // add all required components
    
    /*
    AABB *box = new AABB(newblock);
    //box->active = true;
    box->SetHalfSize(1, 1);
    newblock->AddGameComponent(CT_BoxCollider, box);

    RigidBody *body = new RigidBody(newblock, reinterpret_cast<Primitive*>(box));
    body->SetStatic();
    newblock->AddGameComponent(CT_Body, body);
    

    
    TileMapDetection * TileCheck = new TileMapDetection(newblock, GlobalFactory->GetTileMapData(), GlobalFactory->GetGameObjectList());
    newblock->AddGameComponent(CT_TileMapDetector, TileCheck);

    SpawnedBlock *block = new SpawnedBlock(newblock, this, Direction);
    newblock->AddGameComponent(CT_SPAWN_BLOCK, block);
    
    TileCheck->Initialize();
    //box->Initialize();
    //body->Initialize();
    block->Initialize();
    */
}

void ExtendBlock::DeactivateDirection(int direction)
{
    CreateDirections[direction] = false;
}

void ExtendBlock::ActivateDirection(int direction)
{
    CreateDirections[direction] = true;
}

void ExtendBlock::SetCreateTimer(float newtimer)
{
  this->CreateTimer = newtimer;
}