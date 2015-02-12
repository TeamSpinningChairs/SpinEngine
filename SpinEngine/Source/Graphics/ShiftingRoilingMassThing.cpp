#include "Precompiled.h"
#include "ShiftingRoilingMassThing.h"
#include "ShiftingRoilingParticle.h"
#include "FactoryAccess.h" //To add individual heavy particles
#include "MemoryManager.h"


#define PARTICLE_COUNT 250 //make this tweakable in settings.json later
#define DISABLED true

//250 made a neat amount with WIDTH 4 and HEIGHT 30

void ShiftingRoilingMassThing::Initialize(Vector3D *wallTransform)
{
  if (DISABLED)
    return;

  wallPos = wallTransform;
  //MemoryManager::Add_Component_Allocator(CT_CoolRoilingParticle, sizeof(ShiftingRoilingParticle));
  //This wasn't working- when the engine tried to run the Initialize function, it'd get junk memory and not an IComponent

  for (int i = 0; i < PARTICLE_COUNT; ++i)
  {
    //Create the particle, initialize it, and add it to the vector
    //NOTE: I made the wall sprites "UI" sprites so that they'd be on top of the red "wall" rectangle, because I can't find where that's created and need to ask steven
    //so I can make it be created BEFORE the sprites for this. 

    GameObject particleObject = GlobalFactory->CreateGameObject("RoilingParticle", "RoilingParticle.png", *wallTransform,4294967295UL,true);
    ShiftingRoilingParticle *particle = new ShiftingRoilingParticle; //reinterpret_cast<ShiftingRoilingParticle*>(MemoryManager::Allocate_Component(CT_CoolRoilingParticle));
    particle->Initialize(&particleObject->GetTransform()->GetWorldPosition(), wallTransform);
    particleObject->AddGameComponent(CT_CoolRoilingParticle, reinterpret_cast<Component>(particle));    
    coolRoilingParticles.push_back(particle);
  }

  //Create the thread and register it with the factory
  //(and give the factory a pointer to it)
}

void ShiftingRoilingMassThing::ThreadUpdate()
{
  return;
  //if(gated) wait

  //Time to update!
  for (auto it : coolRoilingParticles)
  {
    //if(gated), means we took longer than we should, just stop.
    //else
    it->Update();
  }

  //Done updating, set ourselves to gated
}