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
#include "ScrollComponent.h"


ScrollComponent::ScrollComponent(IEntity * Parent, float speed, Vector3D direction) : IComponent(Component_Type::CT_SCROLL, Parent)
{
  this->ScrollSpeed = speed;
  this->ScrollDirection = direction;
  Scrolling = true;
  TimeLimit = 0.0;
  this->Delay = 0.0;
}

ScrollComponent::ScrollComponent(IEntity * Parent, float speed, Vector3D direction, float timelimit) : IComponent(Component_Type::CT_SCROLL, Parent)
{
    this->ScrollSpeed = speed;
    this->ScrollDirection = direction;
    Scrolling = true;
    TimeLimit = timelimit;
}

bool ScrollComponent::Initialize()
{
	return true;
}

void ScrollComponent::Update(float dt)
{
    if (Delay > 0)
    {
        Delay -= dt;
        StopScrolling();

        if (Delay <= 0)
        {
            StartScrolling();
            Delay = 0.0f;
        }
        else
        {
            return;
        }
    }

    if (TimeLimit > 0)
    {
        TimeLimit -= dt;

        if (TimeLimit <= 0)
        {
            StopScrolling();
            TimeLimit = 0.0f;
        }
    }

    if (Scrolling == true)
    {
        this->Owner->GetTransform()->GetWorldPosition() += (this->ScrollDirection * (this->ScrollSpeed * dt));
    }
}

void ScrollComponent::Release()
{

}

void ScrollComponent::SetDirection(Vector3D newDir)
{
    ScrollDirection = newDir;
}

void ScrollComponent::StopScrolling()
{
    Scrolling = false;
}

void ScrollComponent::StartScrolling()
{
    Scrolling = true;
}

void ScrollComponent::SetTimeLimit(float Time)
{
    TimeLimit = Time;
}

void ScrollComponent::AddDelay(float newDelay)
{
    Delay = newDelay;
}