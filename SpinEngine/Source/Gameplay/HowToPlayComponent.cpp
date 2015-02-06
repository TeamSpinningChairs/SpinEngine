/****************************************************************************/
/*!
\author Steven Gallwas
\par    email: s.gallwas\@digipen.edu
\par    Course: GAM 200
\brief
this componente exists only to find the how to play screen later

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#include "Precompiled.h"
#include "HowToPlayComponent.h"


HowToPlayComponent::HowToPlayComponent(GameObject parent) : IComponent(Component_Type::CT_HOWTOPLAY, parent)
{

}

bool HowToPlayComponent::Initialize()
{
    return true;
}

void HowToPlayComponent::Update(float dt)
{

}

void HowToPlayComponent::Release()
{

}