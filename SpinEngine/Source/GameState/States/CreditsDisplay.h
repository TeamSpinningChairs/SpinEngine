/****************************************************************************/
/*!
\author Steven Galwas
\par    email: henry.morgan@digipen.edu
\par    Course: GAM 200
\brief

Displays the credits for our game. Single component for what is technically
a "level." Revised by Henry.

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/

#pragma once
#include "IComponent.h"
#include "IEntity.h"
class SpriteRenderer;

class CreditsDisplay : public IComponent
{
public:
  CreditsDisplay(GameObject Parent);
  ~CreditsDisplay();

  bool Initialize();

  void Serialize (DynamicElement* props, Serializer::DataNode* data) {}

  void Update(float dt);

  void Release();

  float Timer;

private:
  IEntity *parent;



  GameObject CreditsScreenObj;
  GameObject CameraObject;
  SpriteRenderer* CreditsRend;
};