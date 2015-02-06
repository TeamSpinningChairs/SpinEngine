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

#include "IComponent.h"
#include "IEntity.h"
#include "CollisionDelegate.h"
#include "SwitchComponent.h"
#include "SoundInstance.h"

enum DoorCloseDirection{DOORCLOSE_LEFT, DOORCLOSE_RIGHT};

class Door2Trigger : public IComponent, public CollisionDelegate
{
public:
    Door2Trigger(GameObject);

    bool Initialize();

	void Serialize (DynamicElement* props, Serializer::DataNode* data) {}

    void Update(float dt);

    void Release();

    bool doorTriggered;



private:


    void OnCollision(GameObject) override;
};

class DoorType2Comp : public IComponent
{
public:
    
    DoorType2Comp(GameObject, int CloseDirection);

    // creates the door trigger
    bool Initialize();

	void Serialize (DynamicElement* props, Serializer::DataNode* data) {}

    void Update(float dt);

    void Release();

    float  movedistace;

    bool Slammed;

private:
    int DoorCloseDirection;

    Door2Trigger * DoorTrigger;
    SoundInstance instance;

};

