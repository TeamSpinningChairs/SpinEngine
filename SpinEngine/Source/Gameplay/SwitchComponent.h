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

class SwitchComponent : public IComponent
{
    // the main part of the this class.  this will be used to trigger elevators, and doors ect
    bool SwitchActive;

    public:

    SwitchComponent(GameObject parent);

    SwitchComponent(GameObject parent, bool StartingState);

    void ActivateSwitch();

    void DeactivateSwitch();

    bool Initialize();
	void Serialize (DynamicElement* props, Serializer::DataNode* data) {}
    void Update(float dt);

    void Release();

    void ToggleActive(void);

    bool GetSwitchStatus(void);
};

