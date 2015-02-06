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
#include "PauseMenu.h"
#include "FactoryAccess.h"
#include "MenuController.h"
#include "GameStateList.h"
#include "Engine.h"

void createPauseMenu()
{

  // create the menu controller, and the pause buttons.  Set the menu to inactive
    GameObject Selector = GlobalFactory->CreateGameObject("PauseController", "Claw-Open.png", Vector3D(0, 0, 0), d3dColors::White, true);
  MenuController * MController = new MenuController(Selector);
  Selector->AddGameComponent(CT_MENUCONTROLLER, MController);
  //MController->Initialize();


  // create all of the buttons for the pause menu change the y value to sort the buttons

  // go to level 2
  GameObject Button1 = GlobalFactory->CreateGameObject("PauseController", "Menu_Level2.png", Vector3D(0, -2, 0), d3dColors::White, true);
  Button1->GetTransform()->GetScale().x = 2;
  Button1->GetTransform()->GetScale().y = 2;
  MenuButtons * MButton1 = new MenuButtons(Button1, "Level_2");
  Button1->AddGameComponent(CT_MENUBUTTON, MButton1);
  //MButton1->Initialize();

  // quit the game 
  GameObject Button2 = GlobalFactory->CreateGameObject("PauseController", "Menu_Quit.png", Vector3D(0, -8, 0), d3dColors::White, true);
  Button2->GetTransform()->GetScale().x = 2;
  Button2->GetTransform()->GetScale().y = 2;
  MenuButtons * MButton2 = new MenuButtons(Button2, "QUIT");
  Button2->AddGameComponent(CT_MENUBUTTON, MButton2);
 // MButton2->Initialize();



  // resume the game 
  GameObject Button3 = GlobalFactory->CreateGameObject("PauseController", "Menu_Resume.png", Vector3D(0, 2, 0), d3dColors::White, true);
  Button3->GetTransform()->GetScale().x = 2;
  Button3->GetTransform()->GetScale().y = 2;
  MenuButtons * MButton3 = new MenuButtons(Button3, "UNPAUSE");
  Button3->AddGameComponent(CT_MENUBUTTON, MButton3);
  //MButton3->Initialize();


  //how to play
  
  GameObject Button4 = GlobalFactory->CreateGameObject("PauseController", "Menu_HowTo.png", Vector3D(0, -4, 0), d3dColors::White ,true);
  Button4->GetTransform()->GetScale().x = 2;
  Button4->GetTransform()->GetScale().y = 2;
  MenuButtons * MButton4 = new MenuButtons(Button4, "HOWTOPLAY");
  Button4->AddGameComponent(CT_MENUBUTTON, MButton4);
  //MButton4->Initialize();
  
  
  GameObject Button5 = GlobalFactory->CreateGameObject("PauseController", "Menu_Main.png", Vector3D(0, -6, 0), d3dColors::White, true);
  Button5->GetTransform()->GetScale().x = 2;
  Button5->GetTransform()->GetScale().y = 2;
  MenuButtons * MButton5 = new MenuButtons(Button5, "Level_Menu");
  Button5->AddGameComponent(CT_MENUBUTTON, MButton5);
  //MButton5->Initialize();
  

  GameObject Button6 = GlobalFactory->CreateGameObject("PauseController", "Menu_Level1.png", Vector3D(0, 0, 0), d3dColors::White, true);
  Button6->GetTransform()->GetScale().x = 2;
  Button6->GetTransform()->GetScale().y = 2;
  MenuButtons * MButton6 = new MenuButtons(Button6, "Level_1");
  Button6->AddGameComponent(CT_MENUBUTTON, MButton6);


  // set the controller to inactive at the start.
    MController->DeactivateMenu();


}
