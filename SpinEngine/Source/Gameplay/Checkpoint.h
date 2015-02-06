/****************************************************************************/
/*!
\author Henry Morgan
\par    email: henry.morgan@digipen.edu
\par    Course: GAM 200
\brief

Saves player progress on collision with player.

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#pragma once

#include "IComponent.h"
#include "IEntity.h"

class Checkpoint : public IComponent, public CollisionDelegate
{
public:
  Checkpoint(GameObject parent, std::string name);

  bool Initialize();

  void Serialize (DynamicElement* props, Serializer::DataNode* data) {}

  void Update(float dt);

  void Release();

private:
  bool triggered_;
  std::string myCheckpointName_;

  void OnCollision(GameObject) override;
};