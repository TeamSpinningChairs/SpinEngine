/****************************************************************************/
/*!
\author Steven Gallwas And Henry Morgan
\par    email: s.gallwas\@digipen.edu
\par    Course: GAM 200
\date
\brief


Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#include "Precompiled.h"
#include "Wall.h"
#include "WallHand.h"
#include "FactoryAccess.h"
#include "IEntity.h"
#include "SpriteRenderer.h"
#include "Engine.h"
#include "TileMapDetection.h"
#include <random>
#include "DynamicHand.h"
#include "WallListener.h"
#include "DestroyOnWall.h"
#include <random>

/*************************************************************************/
/*!
\brief

*/
/*************************************************************************/
Target::Target()
{
	this->Targeted = false;
	this->TargetObject = NULL;
}

/*************************************************************************/
/*!
\brief

*/
/*************************************************************************/
WallComp::WallComp(IEntity * Parent) : IComponent(Component_Type::CT_WALL, Parent), parent(Parent)
{
  Wall_Listener = new WallListener(this);
	
  ActiveTentacleCount = 0;
	
  LaunchTimer = 0;

  WallImage = NULL;

  WallDelay = 0.0f;

  this->WallActive = false;

  Player = NULL;

  ParticleCreateTimer = 0.0;

  //We create a temp variable and load "-WALL SETTINGS-" so that it's saved back as a label.
  //(we save in the same order we load)
  float *temp = NULL;
  
  if (GlobalSettings->GetFloatValue("___ WALL SETTINGS ___", temp, false) &&
    GlobalSettings->GetFloatValue("Wall Movement Speed", moveSpeed, true))
  {
    prevMoveSpeed = *moveSpeed;
  }
  //else
  //{
  //  moveSpeed = 1.0f;
  //  prevMoveSpeed = moveSpeed;
  //}

  //Set pointers to NULL
  ZeroMemory(WallParticleEmitters, sizeof(WallParticleEmitters));
}

WallComp::~WallComp()
{
  //Remove the wall listener.
  GlobalSystem->UnregisterListener(MESSAGE_WALL, Wall_Listener);

  // delete all allocated memory
  delete Wall_Listener;

  /*UNCOMMENT IF YOU'VE REENABLED PARTICLE EMITTERS
  for (int i = 0; i < PARTICLE_EM_COUNT; i++)
  {
    WallParticleEmitters[i]->~ParticleEmitter();
    MemoryManager::Free_Component(CT_ParticleEmitter, WallParticleEmitters[i]);
    ENGINE->m_Graphics->RemoveParticleEmitter(WallParticleEmitters[i]);
  }
  ZeroMemory(WallParticleEmitters, sizeof(WallParticleEmitters));*/
}

/*************************************************************************/
/*!
\brief

*/
/*************************************************************************/
bool WallComp::Initialize()
{

  //Create all of the shifting roiling particle things
  particleSwarm.Initialize(&parent->GetTransform()->GetPosition());



	// fill in the array of hands that target the tile map
  /*
	for (int i = 0; i < HANDCOUNT; i++)
	{
		GameObject hand = GlobalFactory->CreateGameObject("hand", "Claw.png", this->Owner->GetTransform()->GetPosition());

		WallHand * TempHand = new WallHand(hand, this->Owner, i, activeTargets);
		
		TileMapDetection * TileCheck = new TileMapDetection(hand, GlobalFactory->GetTileMapData(), GlobalFactory->GetGameObjectList());

		this->Hands[i] = TempHand;

		hand->AddGameComponent(CT_HAND, TempHand);

		hand->AddGameComponent(CT_TileMapDetector, TileCheck);

		//Have their starting position be somewhere along the y-axis near the wall's center. Anywhere from -3 to +3
		hand->GetTransform()->GetPosition().y += (static_cast<float>((std::rand() % 30)) / 4.0f) * (std::rand() % 2 == 0 ? -1 : 1);
		hand->GetTransform()->GetPosition().y += 3.5f;
	}
  */

	// fill in the array for dynamic hands, that will target the player, or platforms, or in a random direction;
	for (int i = 0; i < DYNAMIC_HANDCOUNT + RESERVED_HANDS; i++)
	{
		//GameObject hand = GlobalFactory->CreateGameObject("hand", "Claw.png", this->Owner->GetTransform()->GetPosition());

		GameObject hand = GlobalFactory->CreateGameObject("hand", "Claw-Open.png", this->Owner->GetTransform()->GetPosition());

		SpriteRend Handrend = reinterpret_cast<SpriteRend>(hand->GetComponent(CT_SpriteRenderer));
    //Handrend->GetSpriteColor() = d3dColors::Red;

		Handrend->AddSprite("Claw-Closed.png", 0, 0, 1, 1 / 30.0f, "Closed", false);

		//hand->GetComponent(CT_SpriteRenderer)

		DynamicHand * DHand = new DynamicHand(hand, this->Owner);
    DHand->Initialize();

		TileMapDetection * TileCheck = new TileMapDetection(hand, GlobalFactory->GetTileMapData(), GlobalFactory->GetGameObjectList());
		
		hand->AddGameComponent(CT_HAND, DHand);
		
		this->Dynamic_Hands[i] = DHand;

		hand->AddGameComponent(CT_TileMapDetector, TileCheck);
	}

  for (int i = 0; i < TargetCount; ++i)
	{
    Targets[i].Targeted = false;
    Targets[i].TargetObject = NULL;
	}

  // Register the wall listener
  GlobalSystem->RegisterListener(MESSAGE_WALL, Wall_Listener);

  // create the wall image behind the wall
  
  
  WallImage = GlobalFactory->CreateGameObject("WallImage", "SCBody.png", Owner->GetTransform()->GetPosition(), d3dColors::White, true);
  WallImage2 = GlobalFactory->CreateGameObject("WallImage", "SCPrison.png", Owner->GetTransform()->GetPosition(), d3dColors::White, true);

  WallImage->GetTransform()->GetScale().x = 8.5;//16;// 15;
  WallImage->GetTransform()->GetScale().y = 7.5;//400;// 15;

  WallImage2->GetTransform()->GetScale().x = 8.5;//16;// 15;
  WallImage2->GetTransform()->GetScale().y = 7.5;//400;// 15;

  //WallImage->GetTransform()->GetPosition().x -= 15.0f;
  //WallImage->SetInVisible();

  Owner->SetInVisible();

	//this->WallActive = true;
	this->Position.x = this->Owner->GetTransform()->GetPosition().x;
	this->Position.y = this->Owner->GetTransform()->GetPosition().y;

  Player = ENGINE->m_Factory->FindObjectByName("MainPlayer");

  return true; //I disabled particle emitters for the shifting-wall-thing
  //
  // NEW PART OF THE INIT PROCESS, SETTING THE PARTICLE EMITTERS
  //

  //CREATE THE PARTICLE EMITTERS
  ParticleEmitter* _mem;
  //Constant difference variables
  float x_diff = 16.0f;
  float y_diff = 2.0f;
  float x_dif_mult = 1.0f;

  //Allocate the emitters with the Memory Manager
  for (int i = 0; i < PARTICLE_EM_COUNT; i++)
  {
    _mem = reinterpret_cast<ParticleEmitter*>(
      MemoryManager::Allocate_Component(CT_ParticleEmitter));
    WallParticleEmitters[i] = new (_mem) ParticleEmitter(WallImage, 
      emitter_types::ET_SQUARE, blend_types::BT_ADD, 0, /*"Assets\\Textures\\Claw-Open.png"*/"", d3dColors::Black, d3dColors::Black);

    WallParticleEmitters[i]->blend_colors = false;
    WallParticleEmitters[i]->angle_1 = 75;
    WallParticleEmitters[i]->angle_2 = 285;

    WallParticleEmitters[i]->speed_1 = -2.0f;
    WallParticleEmitters[i]->speed_2 = -4.0f;

    WallParticleEmitters[i]->acceleration_1 = -15;
    WallParticleEmitters[i]->acceleration_2 = -25;
    
    WallParticleEmitters[i]->scale = 0.3f;
    WallParticleEmitters[i]->end_scale = 0.6f;
    WallParticleEmitters[i]->particle_lifetime = 1.0f;


    //Set the x difference of the emitters
    WallParticleEmitters[i]->x_diff_from_owner = x_dif_mult * x_diff;
    if( (i + 1) % 2 == 0)
    {
      //if iteration number is even then increase the multiplication
      //factor for the x difference
      x_dif_mult += 1.0f;
    }

    //Set the y difference of the emitters
    WallParticleEmitters[i]->y_diff_from_owner = y_diff;
    if (i + 1 % 2)
    {
      //if odd number then set it down
      y_diff *= -1.0f;
    }
  }
  //CREATE THE PARTICLE EMITTERS

  //PARTICLE EMITTER INITIALIZATION
  for (size_t i = 0; i < PARTICLE_EM_COUNT; i++)
  {
    if (!WallParticleEmitters[i]->Initialize())
    {
      return false;
    }
    ENGINE->m_Graphics->AddParticleEmitter(WallParticleEmitters[i]);
  }
  // END PARTICLE EMITTER INITIALIZATION

  WallParticleEmitters[0]->particle_count = 0;

	return true;
}

/*************************************************************************/
/*!
\brief

*/
/*************************************************************************/
void WallComp::Update(float dt)
{
    if (WallActive == false)
    {
        return;
    }

    /*UNCOMMENT IF/WHEN YOU REENABLE THE PARTICLE EMITTERS
    if (WallDelay > 0)
    {
        ParticleCreateTimer += dt;

        WallDelay -= dt;
        
        if (WallDelay <= 0)
        {
            WallDelay = 0.0f;
            WallParticleEmitters[0]->particle_count = 500;
        }

        return;
    }*/

	LaunchTimer += dt;

  /*
	if (*moveSpeed != prevMoveSpeed)
	{
		for (int i = 0; i < HANDCOUNT; ++i)
		{
			Hands[i]->AdjustMoveSpeed(*moveSpeed);
		}
	}
	
	// update all launched tentacles 
  
	for (int i = 0; i < HANDCOUNT; ++i)
	{
		Hands[i]->Update(dt);
	}
  */

	for (int i = 0; i < DYNAMIC_HANDCOUNT + RESERVED_HANDS; ++i)
	{
		Dynamic_Hands[i]->Update(dt);
	}

	// move the wall forward 

  //calculate the distance between the player and the wall

  if(this->Player != NULL)
  {
    float distance = this->Player->GetTransform()->GetPosition().x - this->Owner->GetTransform()->GetPosition().x; 

    float SpeedScalar = distance * .070;

    this->Owner->GetTransform()->GetPosition().x += (*moveSpeed + SpeedScalar) * dt;
    WallImage->GetTransform()->GetPosition() = this->Owner->GetTransform()->GetPosition() - Vector3D(1.5,0,0);
    WallImage2->GetTransform()->GetPosition() = this->Owner->GetTransform()->GetPosition() - Vector3D(1.5, 0, 0);
    //Keep both wall images in front of the world and claw/segment sprites, but just behind the actual UI sprites
    WallImage->GetTransform()->GetPosition().z = -8.f;
    WallImage2->GetTransform()->GetPosition().z = -8.f;

    //@follow player on y-axis
    this->Owner->GetTransform()->GetPosition().y = Player->GetTransform()->GetPosition().y;
  }
  else
  {
    this->Owner->GetTransform()->GetPosition().x += *moveSpeed * dt;
    WallImage->GetTransform()->GetPosition().x += *moveSpeed *dt;

    this->Player = ENGINE->m_Factory->FindObjectByName("MainPlayer");
  }


	// launch tentacles at dynamic targets outside of the tilemap i.e. Player
	this->LaunchTentacle();
	
}

/*************************************************************************/
/*!
\brief

*/
/*************************************************************************/
void WallComp::Release()
{
  ////Remove the wall listener.
  //GlobalSystem->UnregisterListener(MESSAGE_WALL, Wall_Listener);

  //// delete all allocated memory
  //delete Wall_Listener;

}

/*************************************************************************/
/*!
\brief

*/
/*************************************************************************/
GameObject WallComp::CreateHand(void)
{
	//create a base gameobject and give it all of the components it needs to be a hand
	// return the game object with all of the components
	return NULL;
}

/*************************************************************************/
/*!
\brief
function to choose a target from the vector of targets
*/
/*************************************************************************/
GameObject WallComp::ChooseTarget(void)
{
	// return the first unselected target 
	for (int i = 0; i < TargetCount; i++)
	{
		if (this->Targets[i].TargetObject == NULL)
		{
			continue;
		}

		else if (this->Targets[i].Targeted == false)
		{
			this->Targets[i].Targeted = true;
			return this->Targets[i].TargetObject;
		}
	}
	return NULL;
}

/*************************************************************************/
/*!
\brief
	launches a Tentacle at a target
*/
/*************************************************************************/
void WallComp::LaunchTentacle(void)
{
	//chose a target
	GameObject target = NULL;
	
	int TargetNum;

	for (int i = 0; i < TargetCount; i++)
	{
		if (this->Targets[i].TargetObject == NULL)
		{
			continue;
		}

		else if (this->Targets[i].Targeted == false)
		{
			this->Targets[i].Targeted = true;
			target = this->Targets[i].TargetObject;
			TargetNum = i;
		}
	}

	if (target != NULL)
	{
		// find an inactive tentacle
		for (int i = 0; i < DYNAMIC_HANDCOUNT; i++)
		{
			if (this->Dynamic_Hands[i]->GetHandActive() == false)
			{
				this->Dynamic_Hands[i]->ActivateHand();
				this->Dynamic_Hands[i]->TargetNumber = TargetNum;
				this->Dynamic_Hands[i]->Re_Initialize(target);
				return;
			}
		}
	}

	// if there is no target, then we should launch an inactive tentacle in a random direction
	else
	{
		if (LaunchTimer > LAUNCH_DELAY)
		{
			for (int i = 0; i < DYNAMIC_HANDCOUNT; i++)
			{
				if (this->Dynamic_Hands[i]->GetHandActive() == false)
				{
					Vector3D NewDir = ChooseRandomDirection();

					Dynamic_Hands[i]->ActivateHand();
					Dynamic_Hands[i]->TargetNumber = -1;
					Dynamic_Hands[i]->Re_Initialize(NewDir);
					LaunchTimer = 0.0f;
					return;
				}
			}
		}
	}

}

/*************************************************************************/
/*!
\brief

*/
/*************************************************************************/
Vector3D WallComp::ChooseRandomDirection(void)
{
	Vector3D RandDir;

	int test1 = (rand() % 20 + 6);
	int test2 = (rand() % 20 + 6);

	float LO = 5;
	float HI = 12;
  
  float YHI = 40;
  float YLO = -40;

	float test3 = LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
	float test4 = YLO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (YHI - YLO)));

	RandDir.x = (test3 + Owner->GetTransform()->GetPosition().x);
	RandDir.y = (test4 + Owner->GetTransform()->GetPosition().y);
	RandDir.z = 0;

	return RandDir;
}


/*************************************************************************/
/*!
\brief

*/
/*************************************************************************/
void WallComp::GrabImmediately(GameObject target)
{
    // find an inactive tentacle
    
    for (int i = DYNAMIC_HANDCOUNT; i < DYNAMIC_HANDCOUNT + RESERVED_HANDS; i++)
    {
        if (this->Dynamic_Hands[i]->GetHandActive() == false)
        {
            this->Dynamic_Hands[i]->ActivateHand();
            this->Dynamic_Hands[i]->TargetNumber = -1;
            this->Dynamic_Hands[i]->Re_Initialize(target);
            this->Dynamic_Hands[i]->ChaseRange = 35;
            return;
        }
    }
    
    // find an inactive tentacle, and launch it at 
}

/*************************************************************************/
/*!
\brief

*/
/*************************************************************************/
void WallComp::WallStartup()
{
    // change the wall to red and make ominious sounds
}

float WallComp::GetMoveSpeed()
{
  return *moveSpeed;
}
