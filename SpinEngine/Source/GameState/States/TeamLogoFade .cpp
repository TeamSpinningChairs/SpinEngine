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
#include "Precompiled.h"
#include "TeamLogoFade .h"
#include "SpriteRenderer.h"
#include "FactoryAccess.h"
#include "GraphicsManager.h"
#include "Engine.h"

TeamLogoFade::TeamLogoFade(GameObject Parent) : IComponent(Component_Type::CT_TEAMLOGOFADE, Parent),
current(0.0f), pre_intro(0.5f), fade_in(1.0f), show_time(1.0f),
fade_out(1.0f), pre_intro_done(false), faded_in(false), showed(false),
faded_out(false), splashscreenObj(NULL), splashRend(NULL)
{
  splashscreenObj = new IEntity(0, Vector3D(), Vector3D(), Vector3D(9.0f, 4.5f, 1.0f));
  splashRend = new SpriteRenderer(splashscreenObj, GlobalFactory->GetDevice(), d3dColors::Black);
  splashRend->AddSprite("Assets\\Textures\\SpinningChairsLogoWhite.png", 0, 0, 1, 1.0f, "TeamLogo", true);
  //splashRend->AddSprite("Assets\\Textures\\SpinningChairsLogoWhite.png", 0, 0, 1, 1.0f, "TeamLogo", false);
  splashscreenObj->AddGameComponent(CT_SpriteRenderer, splashRend);
  GlobalFactory->GetGraphicsManager()->AddSpriteRend(splashRend);
  GlobalFactory->AddGameObject(splashscreenObj);
  splashscreenObj->GetTransform()->GetWorldScale().y *= 2.0f;

  current = 0.0f;
  pre_intro = 0.5f;
  fade_in = 1.0f;
  show_time = 1.3f;
  fade_out = 1.0f;

}
TeamLogoFade ::~TeamLogoFade ()
{
  //The Factory will unload everything for us.
}

bool TeamLogoFade ::Initialize()
{
  return true;
}

void TeamLogoFade ::Update(float dt)
{
  if (ENGINE->m_Input->IsAnyKeyTriggered())
  {
    gGameStateCurr = GS_LOAD;
    gCurrentLevel = "Level_Menu";
    return;
  }

  current += dt;
  if (!pre_intro_done)
  {
    if (current >= pre_intro)
    {
      current = 0.0f;
      pre_intro_done = true;
      return;
    }

    return;
  }

  if (!faded_in)
  {
    //splashRend->GetSpriteColor() += ( (current / fade_in) * d3dColors::White);
    d3dColors::Change_Alpha_Value(splashRend->GetSpriteColor(), current / fade_in);

    if (current >= fade_in)
    {
      //splashRend->GetSpriteColor() += static_cast<D3DCOLOR>(
      //  ((current / fade_out) * static_cast<float>(d3dColors::White)));
      d3dColors::Change_Alpha_Value(splashRend->GetSpriteColor(), 1.0f);
      current = 0.0f;
      faded_in = true;
      return;
    }

    return;
  }

  if (!showed)
  {
    if (current >= show_time)
    {
      //splashRend->ChangeState("TeamLogo");
      current = 0.0f;
      showed = true;
      return;
    }

    return;
  }

  if (!faded_out)
  {
    //splashRend->GetSpriteColor() -= static_cast<D3DCOLOR>(
    //  ((current / fade_out) * static_cast<float>(d3dColors::Black)));

    d3dColors::Change_Alpha_Value(splashRend->GetSpriteColor(), (fade_out - current) / fade_out);

    if (current >= fade_out)
    {
      splashRend->GetSpriteColor() = d3dColors::Black;
      current = 0.0f;
      faded_in = true;

      gGameStateCurr = GS_LOAD;
      gCurrentLevel = "Level_Menu";
      return;
    }
    return;
  }
}

void TeamLogoFade ::Release()
{

}