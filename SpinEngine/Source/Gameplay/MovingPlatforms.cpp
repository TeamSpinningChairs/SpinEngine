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
#include "MovingPlatforms.h"
#include "IEntity.h"
#include "DebugMessages.h"

// default constructor makes a pacing vertical platform 
MovingPlatform::MovingPlatform(GameObject parent) : IComponent(Component_Type::CT_MOVPLATFORM, parent), CollisionDelegate(parent)
{

}

// create a moving platform of any type
MovingPlatform::MovingPlatform(GameObject parent, int type, float distance, float movespeed) : IComponent(Component_Type::CT_MOVPLATFORM, parent), CollisionDelegate(parent)
{
    PlatformType = type;
    PaceDistance = distance;
    MoveSpeed = movespeed;
    PlatformActive = true;
    PlatformBody = NULL;
    StartingPoint.x = 0;
    StartingPoint.y = 0;
    MoveDelay = 0.0f; // 1.0f;
    MovingLeft = false;
    MovingRight = true;
    MovingUp = true;
    MovingDown = false;
    StartAtTop = false;
    PreviousVel = 0.0f;
    Switch = NULL;
    PaceDownThenUp = false;
    PreviousPosition = Owner->GetTransform()->GetWorldPosition();
}

// initializes stuff
bool MovingPlatform::Initialize()
{
	PlatformBody = reinterpret_cast<RigidBody *>(this->Owner->GetComponent(CT_Body));

  Switch = reinterpret_cast<SwitchComponent *>(this->Owner->GetComponent(CT_SWITCH));

	// if there is no rigid body, something bad should happen here or else people will be confused
	if (this->PlatformBody == NULL)
	{
		SendDebugMessage("Platform Missing rigid body Component");
		this->PlatformActive = false;
		return false;
	}

  if (this->Switch == NULL)
  {
      SendDebugMessage("Platform Missing Switch Component");
      this->PlatformActive = false;
      return false;
  }
  

	// set the starting point of the object so we can remember how far we've moved
	this->StartingPoint.x = this->Owner->GetTransform()->GetWorldPosition().x;
	this->StartingPoint.y = this->Owner->GetTransform()->GetWorldPosition().y;
	
  //Now whenever we collide with something, OnCollision will be called
  InitializeCollisionCallback();
	return true;
}

// moves the platform based on its type
void MovingPlatform::Update(float dt)
{
    // nullify forces on the platform
    PlatformBody->acceleration.x = 0.0;
    PlatformBody->acceleration.y = 0.0;
    PlatformBody->velocity.x = 0.0f;
    PlatformBody->velocity.y = 0.0f;

    // if the platform is not active do not move
    if (Switch->GetSwitchStatus() == false)
    {
        this->PlatformBody->velocity.x = 0;
        this->PlatformBody->velocity.y = 0;
        this->PlatformBody->SetStatic();

        //SnapToCellX(PlatformBody->GetOwner()->GetTransform()->GetWorldPosition().x, PlatformBody->position.x);
        //SnapToCellY(PlatformBody->GetOwner()->GetTransform()->GetWorldPosition().y, PlatformBody->position.y);
        return;
    }
 
	// if there is a move delay don't move until it ends
	if (this->MoveDelay > 0)
	{
    this->PlatformBody->SetStatic();
		this->MoveDelay -= dt;
		this->PlatformBody->velocity.x = 0;
		this->PlatformBody->velocity.y = 0;

		if (this->MoveDelay <= 0)
		{
      this->PlatformBody->isStatic = false;
			this->MoveDelay = 0;
		}

		return;
	}

  this->PlatformBody->isStatic = false;

	// logic for moving the platform;
	switch (this->PlatformType)
	{
		case PLATFORM_DOWN:
        this->PlatformBody->velocity.x = 0.0f;
        this->PlatformBody->velocity.y = -this->MoveSpeed;
			
        break;

		case PLATFORM_UP:
        this->PlatformBody->velocity.x = 0.0f;
        this->PlatformBody->velocity.y = this->MoveSpeed;

        break;

		case PLATFORM_LEFT:
        this->PlatformBody->velocity.y = 0.0f;
        this->PlatformBody->velocity.x = -this->MoveSpeed;

        break;

		case PLATFORM_RIGHT:
        this->PlatformBody->velocity.y = 0.0f;
        this->PlatformBody->velocity.x = this->MoveSpeed;

        break;

		case PLATFORM_PACE_HORIZONTAL:
		{
        this->PlatformBody->velocity.y = 0.0f;
        this->PlatformBody->velocity.x = this->MoveSpeed;

			float distance = this->Owner->GetTransform()->GetWorldPosition().x - this->StartingPoint.x;

			if (distance > this->PaceDistance && this->MovingRight == true)
			{
				this->PlatformBody->velocity.x = 0;
				this->MovingLeft = true;
				this->MovingRight = false;
				this->MoveSpeed *= -1;
				this->MoveDelay = .25f;
			}

			else if (distance <= 0 && this->MovingLeft == true)
			{

				this->PlatformBody->velocity.x = 0;
				this->MovingLeft = false;
				this->MovingRight = true;
				this->MoveSpeed *= -1;
				this->MoveDelay = .25f;
			}

				break;
		}
		
		case PLATFORM_PACE_VERTICAL:
		{
        if (this->StartAtTop)
        {
            this->MovingDown = true;
            this->MovingUp = false;
            this->MoveSpeed *= -1;
            this->StartAtTop = false;
            this->PaceDownThenUp = true;
        }

        this->PlatformBody->velocity.x = 0.0f;
        this->PlatformBody->velocity.y = this->MoveSpeed;

        float distance = this->Owner->GetTransform()->GetWorldPosition().y - this->StartingPoint.y;

        if(this->PaceDownThenUp == false)
        {
          if (distance >= this->PaceDistance && this->MovingUp == true)
          {
              this->PlatformBody->velocity.y = 0;
              this->MovingDown = true;
              this->MovingUp = false;
              this->MoveSpeed *= -1;
              //this->MoveDelay = 0.45f;
              Switch->DeactivateSwitch();
          }

          else if (distance <= 0 && this->MovingDown == true)
          {
              this->PlatformBody->velocity.y = 0;
              this->MovingDown = false;
              this->MovingUp = true;
              this->MoveSpeed *= -1;
              //this->MoveDelay = 0.45f;
              Switch->DeactivateSwitch();
          }
        }

        else
        {
           if (distance <= -this->PaceDistance && this->MovingDown == true)
          {
              this->PlatformBody->velocity.y = 0;
              this->MovingUp = true;
              this->MovingDown = false;
              this->MoveSpeed *= -1;
              //this->MoveDelay = 0.45f;
              Switch->DeactivateSwitch();
          }

          else if (distance >= 0 && this->MovingUp == true)
          {
              this->PlatformBody->velocity.y = 0;
              this->MovingDown = true;
              this->MovingUp = false;
              this->MoveSpeed *= -1;
              //this->MoveDelay = 0.45f;
              Switch->DeactivateSwitch();
          }
        }




        break;
		}
	}
}

// frees all allocated memory which there shouldn't be any
void MovingPlatform::Release()
{

}

// incase we want a moving platform that changes to a different one at runtime
void MovingPlatform::ChangeType(int type)
{
	this->PlatformType = type;
}

// allows us to change the speed of a platform at runtime
void MovingPlatform::SetSpeeed(float speed)
{
	this->MoveSpeed = speed;
}

// allows us to change how far a platform moves at runtime
void MovingPlatform::SetPaceDistance(float Distance)
{
	this->PaceDistance = Distance;
}

void MovingPlatform::SnapToCellX(float &transfcoord, float &rigidcoord)
{
  transfcoord = static_cast<int>(transfcoord);
  rigidcoord = transfcoord;
}

void MovingPlatform::SnapToCellY(float &transfcoord, float &rigidcoord)
{
  transfcoord = static_cast<int>(transfcoord) + 1.0f;
  rigidcoord = transfcoord;
}

void MovingPlatform::OnCollision(GameObject collObj)
{

}

void MovingPlatform::SetPlatformType(int TYPE)
{
  PlatformType = TYPE;
}

MovingPlatform::~MovingPlatform()
{

}