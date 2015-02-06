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
#include "Precompiled.h"
#include "SwitchComponent.h"


SwitchComponent::SwitchComponent(GameObject Parent, bool StartingState) : IComponent(Component_Type::CT_SWITCH, Parent)
{
    SwitchActive = StartingState;
}

SwitchComponent::SwitchComponent(GameObject Parent) : IComponent(Component_Type::CT_SWITCH, Parent)
{
    SwitchActive = false;
}

bool SwitchComponent::Initialize()
{
    return true;
}

void SwitchComponent::Update(float dt)
{
    return;
}

void SwitchComponent::Release()
{
    return;
}

void SwitchComponent::ToggleActive(void)
{
    SwitchActive = !SwitchActive;
}

bool SwitchComponent::GetSwitchStatus(void)
{
    return SwitchActive;
}

void SwitchComponent::ActivateSwitch(void)
{
    SwitchActive = true;
}

void SwitchComponent::DeactivateSwitch(void)
{
    SwitchActive = false;
}