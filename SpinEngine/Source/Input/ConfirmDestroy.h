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

class MenuController;

class ConfirmDestroy : public IComponent
{
public:

    ConfirmDestroy(GameObject, MenuController*);

    ~ConfirmDestroy();

    bool Initialize();

	void Serialize (DynamicElement* props, Serializer::DataNode* data) {}

    void Update(float dt);

    void Release();

    void MoveSelector();

    void UpdatePosition();

    void HideConfirmation();

    void ShowConfirmation();

    void ExecuteOrder();

    bool MenuReturn;

private:

    bool Initialized;

    std::vector<GameObject> Buttons;

    GameObject Header;

    int CurrentChoice;

    MenuController * TopMenu;



};