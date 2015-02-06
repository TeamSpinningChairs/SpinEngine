#pragma once

#include "IComponent.h"
#include "rigidbody.h"


enum AI_TYPE
{
  AI_RUN_LEFT,
  AI_RUN_RIGHT,
  AI_PACE,
};

class HeadlessChickenAI : public IComponent
{
public:
  HeadlessChickenAI(int type, GameObject parent);

  bool Initialize();

  void Update(float dt);

  void Release();

private:

  RigidBody * ChickenBody;
  
  int AItype;

  float MoveSpeed;

  float MoveDistance;

  float MoveRange;

  float MoveModifier;
};

