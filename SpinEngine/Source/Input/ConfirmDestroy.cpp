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
#include "ConfirmDestroy.h"
#include "FactoryAccess.h"
#include "MenuController.h"
#include "HowToPlayComponent.h"

ConfirmDestroy::ConfirmDestroy(GameObject Parent, MenuController* main) : IComponent(Component_Type::CT_CONFIRMDESTROY, Parent)
{
    TopMenu = main;
    CurrentChoice = 0;
    Header = NULL;
    Initialized = false;
    MenuReturn = false;
}

ConfirmDestroy::~ConfirmDestroy()
{
    GlobalFactory->RemoveGameObject(Header->GetEntityId());
}

bool ConfirmDestroy::Initialize()
{
    if (Initialized == false)
    {
        // create the yes and no buttons 0 = no, 1 = yes
        this->Buttons.push_back(GlobalFactory->CreateGameObject("NoButton", "Menu_No.png", Vector3D(0, 0, 0), d3dColors::White, true));
        this->Buttons.push_back(GlobalFactory->CreateGameObject("YesButton", "Menu_Yes.png", Vector3D(0, -2, 0), d3dColors::White, true));
        Header = GlobalFactory->CreateGameObject("ConfirmPrompt", "Menu_Prompt.png", Vector3D(0, 2, 0), d3dColors::White, true);

        Header->GetTransform()->GetWorldScale() *= 2;
        Buttons[0]->GetTransform()->GetWorldScale() *= 2;
        Buttons[1]->GetTransform()->GetWorldScale() *= 2;

        HowToPlayComponent * MenuUpdate1 = new HowToPlayComponent(Buttons[0]);
        Buttons[0]->AddGameComponent(CT_HOWTOPLAY, MenuUpdate1);

        HowToPlayComponent * MenuUpdate2 = new HowToPlayComponent(Buttons[1]);
        Buttons[1]->AddGameComponent(CT_HOWTOPLAY, MenuUpdate2);

        HowToPlayComponent * MenuUpdate3 = new HowToPlayComponent(Header);
        Header->AddGameComponent(CT_HOWTOPLAY, MenuUpdate3);
        

        if (Buttons.size() != 0)
        {
            this->Owner->GetTransform()->GetWorldPosition() = (Buttons[CurrentChoice]->GetTransform()->GetWorldPosition() - Vector3D(3, 0, 0));
        }

        HideConfirmation();
        Initialized = true;
        return true;
    }
    return false;
}

void ConfirmDestroy::Update(float dt)
{
    // don't use update
}

void ConfirmDestroy::Release()
{
    // remove gameobjects
}

void ConfirmDestroy::MoveSelector()
{
    if (CurrentChoice == 0)
    {
        CurrentChoice = 1;
    }
    else
    {
        CurrentChoice = 0;
    }

    this->Owner->GetTransform()->GetWorldPosition() = (Buttons[CurrentChoice]->GetTransform()->GetWorldPosition() - Vector3D(3, 0, 0));
}

void ConfirmDestroy::UpdatePosition()
{
    Vector3D newPos = ENGINE->m_Graphics->GetMainCamera()->GetPosition();

    Header->GetTransform()->GetWorldPosition().x = newPos.x;
    Header->GetTransform()->GetWorldPosition().y = newPos.y + 2;

    for (unsigned i = 0; i < Buttons.size(); i++)
    {
        Buttons[i]->GetTransform()->GetWorldPosition().x = newPos.x;
        Buttons[i]->GetTransform()->GetWorldPosition().y = newPos.y - (2 * i);
    }

    this->Owner->GetTransform()->GetWorldPosition() = (Buttons[CurrentChoice]->GetTransform()->GetWorldPosition() - Vector3D(3, 0, 0));
}

void ConfirmDestroy::HideConfirmation()
{
    CurrentChoice = 0;
    this->Owner->SetInVisible();
    Header->SetInVisible();

    for (GameObject button : Buttons)
    {
        button->SetInVisible();
    }
}

void ConfirmDestroy::ShowConfirmation()
{
    this->Owner->SetVisible();
    Header->SetVisible();

    for (GameObject button : Buttons)
    {
        button->SetVisible();
    }
}

void ConfirmDestroy::ExecuteOrder()
{
    // quit game
    if (CurrentChoice == 1)
    {
        if (MenuReturn == true)
        {
            gCurrentLevel = "Level_Menu";
            gGameStateCurr = GS_LOAD;
            TopMenu->QuitActive = false;
            HideConfirmation();
            MenuReturn = false;
            gGameStatePaused = false;
        }
        else
        {
            gGameStateCurr = GS_QUIT;
        }
    }

    // hide the confirmation menu and reacivate the main menu.
    else
    {
        TopMenu->ShowMenu();
        HideConfirmation();
        TopMenu->QuitActive = false;
        MenuReturn = false;
    }
}