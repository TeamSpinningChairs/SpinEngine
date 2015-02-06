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

#include "SwitchComponent.h"
#include "IComponent.h"
#include "IEntity.h"
#include "TileMapDetection.h"
#define CREATE_DELAY 0.25f

enum SpawnDIR
{
    SPAWN_UP,
    SPAWN_DOWN,
    SPAWN_LEFT,
    SPAWN_RIGHT,
    SPAWN_TOTAL
};


struct SpawnInstruction
{
    SpawnInstruction(int dir, int numblocks);

    int Direction;

    int NumBlocks;
};

// I might want this for later
class BlockSpawner : public IComponent
{

public:

bool Initialize();

void Serialize (DynamicElement* props, Serializer::DataNode* data) {}

void Update(float dt);

void Release();

void RestartInstruction();

void PauseInstruction();

void AddInstruction(SpawnInstruction NewInstruction);

void ClearAllInstructions();

void RepeatAllInstructions();

private:

// pointer to the last block that was made
GameObject CurrentBlock;

// array of instruction structs, so we can tell the block to do things
std::vector<SpawnInstruction> Instructions;

SwitchComponent * Blockswitch;

int CurrentInstruction;

int BlocksMade;

float CreateTimer;

};


// component for the level 2 platforms
class ExtendBlock : public IComponent
{
public:

ExtendBlock(GameObject);

bool Initialize();

void Serialize (DynamicElement* props, Serializer::DataNode* data) {}

void Update(float dt);

void Release();

void DeactivateDirection(int direction);

void ActivateDirection(int direction);

void SetCreateTimer(float timer);

private:

float CreateTimer;

TileMapDetection * TileDetection;

bool CreateDirections[SPAWN_TOTAL];

Vector3D CurrentBlockPosition[SPAWN_TOTAL];

//std::vector<bool> CreateDirections;

//std::vector<Vector3D> CurrentBlockPosition;

void CreateBlock(int Direction);

SwitchComponent * BlockSwitch;



};