#include "Precompiled.h"
#include "HeadlessChickenAI.h"

HeadlessChickenAI::HeadlessChickenAI(int type, GameObject parent) : IComponent(Component_Type::CT_HEADLESS_CHICKEN, parent)
{
  AItype = type;
  ChickenBody = NULL;
  MoveSpeed = 8;
  MoveModifier = 1;
  MoveRange = 8;
  MoveDistance = 0;
}

bool HeadlessChickenAI::Initialize()
{
  ChickenBody = reinterpret_cast<RigidBody*>(Owner->GetComponent(CT_Body));
  return true;
}

void HeadlessChickenAI::Update(float dt)
{
  switch (AItype)
  {
    case AI_TYPE::AI_RUN_LEFT:
    Owner->GetTransform()->GetPosition().x += this->MoveSpeed * dt;
    ChickenBody->position.x += this->MoveSpeed * dt;
    break;

    case AI_TYPE::AI_RUN_RIGHT:
    Owner->GetTransform()->GetPosition().x -= this->MoveSpeed * dt;
    ChickenBody->position.x += this->MoveSpeed * dt;
    break;

    case AI_TYPE::AI_PACE:
    Owner->GetTransform()->GetPosition().x += (MoveModifier * MoveSpeed * dt);
    ChickenBody->position.x += (MoveModifier * MoveSpeed * dt);

    MoveDistance += (MoveSpeed * dt);

    if (MoveDistance > this->MoveRange)
    {
      MoveDistance *= -1;
      MoveModifier *= -1;
    }
    break;
  }
}

void HeadlessChickenAI::Release()
{

}

