/****************************************************************************/
/*!
\author Henry Morgan
\par    email: henry.morgan@digipen.edu
\par    Course: GAM 200
\brief

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#include "Precompiled.h"
#include "KillPlayer.h"

KillPlayer::KillPlayer(GameObject parent) : IComponent(CT_KILLPLAYER,parent)
{

}

bool KillPlayer::Initialize()
{
  return true;
}

void KillPlayer::Update(float dt)
{

}

void KillPlayer::Release()
{

}