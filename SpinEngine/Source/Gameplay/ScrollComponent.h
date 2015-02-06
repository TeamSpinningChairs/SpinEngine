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

#include "Core\IComponent.h"

class ScrollComponent : public IComponent
{

public:
	ScrollComponent(IEntity * Parent, float speed, Vector3D direction);
  ScrollComponent(IEntity * Parent, float speed, Vector3D direction, float TimeLimit);
	bool Initialize();
	void Serialize (DynamicElement* props, Serializer::DataNode* data) {}
	void Update(float dt);
  void Release();
  void SetDirection(Vector3D);
  void StopScrolling(void);
  void StartScrolling(void);
  void SetTimeLimit(float);
  void AddDelay(float);

private:
  float Delay;
  float TimeLimit;
  bool Scrolling;
	float ScrollSpeed;
  Vector3D ScrollDirection;
};