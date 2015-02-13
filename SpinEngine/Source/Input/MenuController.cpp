/****************************************************************************/
/*!
\file   MenuController
\author Steven Gallwas
\par    email: s.gallwas\@digipen.edu
\par    Course: GAM 200
\date   9/16/14
\brief
This file contains the implementation for the base messaging system.

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#include "Precompiled.h"
#include "MenuController.h"
#include "KeyBoard.h"
#include "InputHandler.h"
#include "FactoryAccess.h"
#include "HowToPlayComponent.h"

/****************************************************************************/
/*!
menu keyboard listeners
*/
/****************************************************************************/

MenuKeyboardListener::MenuKeyboardListener(MenuController* parent)
{
    this->ParentObj = parent;
}


void MenuKeyboardListener::OnMessageRecieved(Message * SentMessage)
{
    if (!SentMessage)
    {
        MessageBox(NULL, "PlayerKeyboardListener got a null message? What?", NULL, NULL);
        return;
    }

    if(this->ParentObj->MenuActive == false)
    {
      return;
    }

    KeyMessage * GivenInput = reinterpret_cast<KeyMessage *>(SentMessage);
    
    if (this->ParentObj->HowToPlayActive == true)
    {
        if (GivenInput->KeyStatus == KEY_TRIGGERED)
        {
            this->ParentObj->HowToPlayActive = false;
            this->ParentObj->HowtoPlayScreen->SetInVisible();
            this->ParentObj->ShowMenu();
            return;
        }
    }

    else if (ParentObj->QuitActive == true)
    {
        switch (GivenInput->KeyNumber)
        {
        case VK_UP:
            if (GivenInput->KeyStatus == KEY_TRIGGERED)
            {
                this->ParentObj->Confirmation->MoveSelector();
            }
            // move selector up
            break;

        case VK_DOWN:
            if (GivenInput->KeyStatus == KEY_TRIGGERED)
            {
                this->ParentObj->Confirmation->MoveSelector();
            }
            // move selector down
            break;

        case 'W':
            if (GivenInput->KeyStatus == KEY_TRIGGERED)
            {
                this->ParentObj->Confirmation->MoveSelector();
            }
            // move selector up
            break;

        case 'S':
            if (GivenInput->KeyStatus == KEY_TRIGGERED)
            {
                this->ParentObj->Confirmation->MoveSelector();
            }
            // move selector down
            break;

        case VK_SPACE:
            if (GivenInput->KeyStatus == KEY_TRIGGERED)
            {
                this->ParentObj->Confirmation->ExecuteOrder();
            }
            break;

        case VK_RETURN:
            if (GivenInput->KeyStatus == KEY_TRIGGERED)
            {
                this->ParentObj->Confirmation->ExecuteOrder();
            }

            break;
        }
    
        return;
    }

    switch (GivenInput->KeyNumber)
    {
    case VK_UP:
        if (GivenInput->KeyStatus == KEY_TRIGGERED)
        {
          this->ParentObj->MoveUp();
        }
        // move selector up
        break;

    case VK_DOWN:
        if (GivenInput->KeyStatus == KEY_TRIGGERED)
        {
          this->ParentObj->MoveDown();
        }
        // move selector down
        break;

    case 'W':
        if (GivenInput->KeyStatus == KEY_TRIGGERED)
        {
            this->ParentObj->MoveUp();
        }
        // move selector up
        break;

    case 'S':
        if (GivenInput->KeyStatus == KEY_TRIGGERED)
        {
            this->ParentObj->MoveDown();
        }
        // move selector down
        break;

    case VK_SPACE:
        if (GivenInput->KeyStatus == KEY_TRIGGERED)
        {
          this->ParentObj->ActivateButton();
        }
        break;
    case VK_RETURN:
        if (GivenInput->KeyStatus == KEY_TRIGGERED)
        {
          this->ParentObj->ActivateButton();
        }

        break;
    }
}

/****************************************************************************/
/*!
menu gamepad listener
*/
/****************************************************************************/

MenuGamepadListener::MenuGamepadListener(MenuController* parent)
{
    this->ParentObj = parent;
}

    // recieves input from the gamepad to update the menu
void MenuGamepadListener::OnMessageRecieved(Message * SentMessage)
{
  if (!SentMessage)
  {
    MessageBox(NULL, "PlayerControllerListener got a null message? What?", NULL, NULL);
    return;
  }

    if(this->ParentObj->MenuActive == false)
    {
      return;
    }

    InputMessage *GivenInput = reinterpret_cast<InputMessage *>(SentMessage);

    if (this->ParentObj->HowToPlayActive == true)
    {
      if (GivenInput->KeyPressed == true)
        {
            this->ParentObj->HowToPlayActive = false;
            this->ParentObj->HowtoPlayScreen->SetInVisible();
            this->ParentObj->ShowMenu();
            return;
        }
    }

    if (ParentObj->QuitActive == true)
    {
      if (GivenInput->LeftStickState == STICK_STATE::STICK_TRIGGERED)
      {
        this->ParentObj->Confirmation->ExecuteOrder();
      }

        switch(GivenInput->TriggeredKey)
        {
      
      case BUTTONS_X:
        if(GivenInput->KeyPressed)
        {
          this->ParentObj->Confirmation->ExecuteOrder();
        }

		    break;

      case BUTTONS_A:
        if(GivenInput->KeyPressed)
        {
          this->ParentObj->Confirmation->ExecuteOrder();
        }

		    break;

      case BUTTONS_START:
        if(GivenInput->KeyPressed)
        {
          this->ParentObj->Confirmation->ExecuteOrder();
        }
        break;
    
      case DPAD_UP:
        if(GivenInput->KeyPressed)
        {
          this->ParentObj->Confirmation->MoveSelector();
        }
        break;
    
      case DPAD_DOWN:
        if(GivenInput->KeyPressed)
        {
          this->ParentObj->Confirmation->MoveSelector();
        }
        break;
      }
    
      return;
    }

    if (GivenInput->LeftStickState == STICK_TRIGGERED)
    {
      if (GivenInput->leftStick.first > 0)
      {
        this->ParentObj->MoveUp();
      }

      else
      {
        this->ParentObj->MoveDown();
      }
    }


  switch(GivenInput->TriggeredKey)
  {
      
  case BUTTONS_X:
    if(GivenInput->KeyPressed)
    {
        this->ParentObj->ActivateButton();
    }

		break;

  case BUTTONS_A:
    if(GivenInput->KeyPressed)
    {
        this->ParentObj->ActivateButton();
    }

    break;

  case BUTTONS_START:
    if (GivenInput->KeyPressed)
    {
      //this->ParentObj->ActivateButton();
    }
    break;
    
  case DPAD_UP:
    if(GivenInput->KeyPressed)
    {
        this->ParentObj->MoveUp();
    }
    break;
    
  case DPAD_DOWN:
    if(GivenInput->KeyPressed)
    {
      this->ParentObj->MoveDown();
    }
    break;
  }
}

/****************************************************************************/
/*!
Menu Listener class
*/
/****************************************************************************/

//takes menu messages register under that
MenuListener::MenuListener(MenuController * parent)
{
  this->ParentObj = parent;
}

void MenuListener::OnMessageRecieved(Message * SentMessage)
{
    if (!SentMessage)
  {
    MessageBox(NULL, "PlayerControllerListener got a null message? What?", NULL, NULL);
    return;
  }

    MenuMessage *GivenInput = reinterpret_cast<MenuMessage *>(SentMessage);

    if(GivenInput->Type == Add_Button)
    {
      this->ParentObj->AddButton(GivenInput->MenuButton);

      MenuButtons * Button = reinterpret_cast<MenuButtons *>(GivenInput->MenuButton->GetComponent(CT_MENUBUTTON));

      Button->Added = true;
    }
  
    else if(GivenInput->Type == UPDATE_POSITION)
    {
      this->ParentObj->UpdatePosition(GivenInput->newPosition);
    }
    else if(GivenInput->Type == ActivatePause)
    {
      this->ParentObj->MenuActive = true;
      this->ParentObj->ActivateMenu();
    }

    else if(GivenInput->Type == DeactivatePause)
    {
      this->ParentObj->MenuActive = false;
      this->ParentObj->DeactivateMenu(); 
      this->ParentObj->HideExit();
    }
}

// basic menu message required to add listeners to the 
/****************************************************************************/
/*!
Menu Message Class
*/
/****************************************************************************/

MenuMessage::MenuMessage(GameObject Button)
{
  this->MenuButton = Button;
  this->MessageType = MESSAGE_MENU;
  this->Type = Add_Button;
  this->newPosition = Vector3D(0,0,0);
}

/****************************************************************************/
/*!
Menu Button Class
*/
/****************************************************************************/
// menu buttons will add themselves using menu messages

MenuButtons::MenuButtons(GameObject Parent, std::string lvlname)  : IComponent(Component_Type::CT_MENUBUTTON, Parent)
{
  Level = lvlname;
  this->Added = false;
}

bool MenuButtons::Initialize()
{
  return true;

  //MenuMessage msg(this->Owner);

  //GlobalSystem->MyDispatchMessage(&msg);
}

// add self to the menucontroller, then don't update anymore
void MenuButtons::Update(float dt)
{
  // add self to the menucontroller if not already added
  if(this->Added == false)
  {
    MenuMessage msg(this->Owner);

    GlobalSystem->MyDispatchMessage(&msg);
  }
}

// remove all listeners
void MenuButtons::Release()
{

}

void MenuButtons::LoadLevel()
{
  gCurrentLevel = Level;
  gGameStateCurr = GS_LOAD;
}

/****************************************************************************/
/*!
menu Controller

This is the main worker of the menu will
update position based on the current selection
and launch commands
*/
/****************************************************************************/
MenuController::MenuController(GameObject Parent) : IComponent(Component_Type::CT_MENUCONTROLLER, Parent)
{
  MenuActive = true;
  currentMenuSelection = 0;
  Menu_Keyboard = new MenuKeyboardListener(this);
  Menu_Gamepad = new MenuGamepadListener(this);
  Menu_Listen = new MenuListener(this);
  QuitActive = false;
  Confirmation = NULL;

  QuitConfirm = NULL;
  HowtoPlayScreen = NULL;
  HowToPlayActive = false;
  Initialized = false;
}

// register all listeners
bool MenuController::Initialize()
{    
    if (Initialized == false)
    {
        GlobalSystem->RegisterListener(MESSAGE_KEYBOARD, Menu_Keyboard);
        GlobalSystem->RegisterListener(MESSAGE_INPUT, Menu_Gamepad);
        GlobalSystem->RegisterListener(MESSAGE_MENU, Menu_Listen);

        HowtoPlayScreen = GlobalFactory->CreateGameObject("HowTo", "HowToPlayScreen.png", ENGINE->m_Graphics->GetMainCamera()->GetPosition(), d3dColors::White, true);
        HowtoPlayScreen->GetTransform()->GetScale().x *= 8;
        HowtoPlayScreen->GetTransform()->GetScale().y *= 8;
        HowtoPlayScreen->GetTransform()->GetPosition().z = 1;
        HowtoPlayScreen->SetInVisible();

        HowToPlayComponent * test = new HowToPlayComponent(HowtoPlayScreen);
        HowtoPlayScreen->AddGameComponent(CT_HOWTOPLAY, test);

        QuitConfirm = GlobalFactory->CreateGameObject("QuitConfirmation", "Claw-Open.png", this->Owner->GetTransform()->GetPosition(), d3dColors::White, true);
        Confirmation = new ConfirmDestroy(QuitConfirm, this);
        QuitConfirm->AddGameComponent(CT_CONFIRMDESTROY, Confirmation);
        Confirmation->Initialize();
        Initialized = true;
    }
    return true;
}

void MenuController::ActivateButton()
{
  MenuButtons * Button = reinterpret_cast<MenuButtons *>(Buttons[currentMenuSelection]->GetComponent(CT_MENUBUTTON));

  // special case for unpausing the game
  if(Button->Level == "UNPAUSE")
  {
    MenuMessage msg_pause_deactivate(NULL);
    msg_pause_deactivate.Type = DeactivatePause;

    GlobalSystem->MyDispatchMessage(&msg_pause_deactivate);
    gGameStatePaused = false;
    ENGINE->m_Sound->ResumeAll();
  }

  // special case for how to play
  else if(Button->Level == "HOWTOPLAY")
  {
    HowtoPlayScreen->GetTransform()->GetPosition() = ENGINE->m_Graphics->GetMainCamera()->GetPosition();
    HowtoPlayScreen->GetTransform()->GetPosition().z = 3;
    HowtoPlayScreen->SetVisible();
    HideMenu();
    HowToPlayActive = true;
  }

  
  else if (Button->Level == "Level_Menu")
  {
      Confirmation->UpdatePosition();
      Confirmation->ShowConfirmation();
      Confirmation->MenuReturn = true;
      this->HideMenu();
      QuitActive = true;
  }
  /**/

  else if (Button->Level == "QUIT")
  {
      Confirmation->UpdatePosition();
      Confirmation->ShowConfirmation();
      this->HideMenu();
      QuitActive = true;
  }

  else
  {
    gGameStatePaused = false;
    DeactivateMenu();
    Button->LoadLevel();
  }
}

void MenuController::AddButton(GameObject newbutton)
{
    this->Buttons.push_back(newbutton);
    this->SortButtons();
}

void MenuController:: MoveUp(void)
{
  if(currentMenuSelection == 0)
  {
    currentMenuSelection = Buttons.size() - 1;
  }
  else
  {
    --currentMenuSelection;
  }

    if(Buttons.size() != 0)
  {
    this->Owner->GetTransform()->GetPosition() = Buttons[currentMenuSelection]->GetTransform()->GetPosition();
    this->Owner->GetTransform()->GetPosition().x -= 3;
  }

}

void MenuController::MoveDown(void)
{
  if(currentMenuSelection == Buttons.size() - 1)
  {
    currentMenuSelection = 0;
  }
  else
  {
    ++currentMenuSelection;
  }

  
  if(Buttons.size() != 0)
  {
    this->Owner->GetTransform()->GetPosition() = Buttons[currentMenuSelection]->GetTransform()->GetPosition();
    this->Owner->GetTransform()->GetPosition().x -= 3;
  }

}

MenuController::~MenuController()
{
  GlobalSystem->UnregisterListener(MESSAGE_KEYBOARD, Menu_Keyboard);
  GlobalSystem->UnregisterListener(MESSAGE_INPUT, Menu_Gamepad);
  GlobalSystem->UnregisterListener(MESSAGE_MENU, Menu_Listen);

  SPIN_ENGINE_SAFE_DELETE(Menu_Keyboard);
  SPIN_ENGINE_SAFE_DELETE(Menu_Gamepad);
  SPIN_ENGINE_SAFE_DELETE(Menu_Listen);
}


// 
void MenuController::Update(float dt)
{

}

// remove all listeners
void MenuController::Release()
{
    
}

void MenuController::SortButtons()
{
    // variable to track the position of the smallest element
    unsigned CurrentSmallest;
    
    for(unsigned i = 0; i < Buttons.size(); i++) 
    {
      CurrentSmallest = i;
    
      // loop through the array and compare each value from the starting
      for(unsigned j = (i + 1); j < Buttons.size(); j++)
      {
        if(Buttons[j]->GetTransform()->GetPosition().y > Buttons[CurrentSmallest]->GetTransform()->GetPosition().y)
        {
          CurrentSmallest = j;
        }
      }
      
      //Check that the first element is not already the smallest
      if(i == CurrentSmallest)
        continue;
      
      else
      {
        // set up a temporary variable to store the current value
        GameObject temp = Buttons[i];
        
        // swap the two elements
        Buttons[i] = Buttons[CurrentSmallest];
        
        // set the element at smallest equal to the temp value
        Buttons[CurrentSmallest] = temp;
      }
    }

    currentMenuSelection = 0; //Buttons.size() - 1;

  if(Buttons.size() != 0)
  {
    this->Owner->GetTransform()->GetPosition() = Buttons[currentMenuSelection]->GetTransform()->GetPosition();
    this->Owner->GetTransform()->GetPosition().x -= 3;
  }
}

void MenuController::SortButtonsHorizontal()
{
    // variable to track the position of the smallest element
    unsigned CurrentSmallest;

    for (unsigned i = 0; i < Buttons.size(); i++)
    {
        CurrentSmallest = i;

        // loop through the array and compare each value from the starting
        for (unsigned j = (i + 1); j < Buttons.size(); j++)
        {
            if (Buttons[j]->GetTransform()->GetPosition().x > Buttons[CurrentSmallest]->GetTransform()->GetPosition().x)
            {
                CurrentSmallest = j;
            }
        }

        //Check that the first element is not already the smallest
        if (i == CurrentSmallest)
            continue;

        else
        {
            // set up a temporary variable to store the current value
            GameObject temp = Buttons[i];

            // swap the two elements
            Buttons[i] = Buttons[CurrentSmallest];

            // set the element at smallest equal to the temp value
            Buttons[CurrentSmallest] = temp;
        }
    }

    currentMenuSelection = 0; //Buttons.size() - 1;

    if (Buttons.size() != 0)
    {
        this->Owner->GetTransform()->GetPosition() = Buttons[currentMenuSelection]->GetTransform()->GetPosition();
        this->Owner->GetTransform()->GetPosition().y += 3;
    }
}


void MenuController::ActivateMenu()
{
      // make all buttons invisible if the menu is not active
  if(QuitActive == true)
  {
    return;
  }

      Owner->SetVisible();

      for (unsigned i = 0; i < Buttons.size(); i++)
      {
          Buttons[i]->SetVisible();
      }

      MenuActive = true;
}


void MenuController::DeactivateMenu()
{
      // make all buttons invisible if the menu is not active

      Owner->SetInVisible();

      this->currentMenuSelection = 0;

      for (unsigned i = 0; i < Buttons.size(); i++)
      {
          Buttons[i]->SetInVisible();
      }

      MenuActive = false;
}

void MenuController::ShowMenu(void)
{
    Owner->SetVisible();

    for (unsigned i = 0; i < Buttons.size(); i++)
    {
        Buttons[i]->SetVisible();
    }
}

void MenuController::HideMenu(void)
{
    Owner->SetInVisible();

    for (unsigned i = 0; i < Buttons.size(); i++)
    {
        Buttons[i]->SetInVisible();
    }
}


void MenuController::UpdatePosition(Vector3D newPos)
{
      // array is sorted by smallest to largest
      for (unsigned i = 0; i < Buttons.size(); i++)
      {
        Buttons[i]->GetTransform()->GetPosition().x = newPos.x;
        Buttons[i]->GetTransform()->GetPosition().y = newPos.y - (2 * i);
      }

      //SortButtons();

    if(Buttons.size() != 0)
    {
      this->Owner->GetTransform()->GetPosition() = Buttons[currentMenuSelection]->GetTransform()->GetPosition();
      this->Owner->GetTransform()->GetPosition().x -= 3;
    }
  
}

void MenuController::HideExit()
{
    Confirmation->HideConfirmation();
    QuitActive = false;
}