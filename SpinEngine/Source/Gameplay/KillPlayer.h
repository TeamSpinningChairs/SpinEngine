/****************************************************************************/
/*!
\author Henry Morgan
\par    email: henry.morgan@digipen.edu
\par    Course: GAM 200
\brief

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#pragma once

#include "IComponent.h"
#include "IEntity.h"

class KillPlayer : public IComponent
{
public:

  KillPlayer(GameObject parent);

  bool Initialize();

  void Serialize (DynamicElement* props, Serializer::DataNode* data) {}

  void Update(float dt);
  void Release();
};