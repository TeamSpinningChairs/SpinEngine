/****************************************************************************/
/*!
\author Esteban Maldonado
\par    email: henry.morgan@digipen.edu
\par    Course: GAM 200
\brief

Displays the team logo for our game. Single component for what is technically
a "level."

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#pragma once
#include "IComponent.h"
#include "IEntity.h"
class SpriteRenderer;

class TeamLogoFade : public IComponent
{
public:

  TeamLogoFade(GameObject Parent);
  ~TeamLogoFade();

  bool Initialize();
  void Serialize (DynamicElement* props, Serializer::DataNode* data) {}
  void Update(float dt);

  void Release();

private:
  IEntity *parent;

  float current;
  float pre_intro;
  float fade_in;
  float show_time;
  float fade_out;

  bool pre_intro_done;
  bool faded_in;
  bool showed;
  bool faded_out;


  IEntity *splashscreenObj;
  SpriteRenderer* splashRend;
};