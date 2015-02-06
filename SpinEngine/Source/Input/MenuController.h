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
#include "NewMessageSystem.h"
#include "Engine.h"
#include "ConfirmDestroy.h"

class MenuController;
class String;

enum MenuMessageTypes {Add_Button, ActivatePause, DeactivatePause, UPDATE_POSITION};

/****************************************************************************/
/*!
    menu keyboard listeners
*/
/****************************************************************************/
//takes keyboard messages register under that
class MenuKeyboardListener : public MessageListener
{
public:
    MenuKeyboardListener(MenuController *);

    // recieves input from the keyboard to update the menu
    void OnMessageRecieved(Message * SentMessage);

    MenuController * ParentObj;
};

/****************************************************************************/
/*!
menu gamepad listener
*/
/****************************************************************************/
// takes gamepad messages regiser under that
class MenuGamepadListener : public MessageListener
{
  public:
    MenuGamepadListener(MenuController *);

    // recieves input from the gamepad to update the menu
    void OnMessageRecieved(Message * SentMessage);

    MenuController * ParentObj;
};

/****************************************************************************/
/*!
Menu Listener class 
*/
/****************************************************************************/
//takes menu messages register under that
class MenuListener : public MessageListener
{
  public:

    MenuListener(MenuController *);

    void OnMessageRecieved(Message * SentMessage);

    MenuController * ParentObj;
};

/****************************************************************************/
/*!
Menu Message Class
*/
/****************************************************************************/

// basic menu message required to add listeners to the 
class MenuMessage : public Message
{
public:

  MenuMessage(GameObject);

    GameObject MenuButton;

    int Type;

    Vector3D newPosition;
};

/****************************************************************************/
/*!
Menu Button Class
*/
/****************************************************************************/

// menu buttons will add themselves using menu messages
class MenuButtons : public IComponent
{
public:
  MenuButtons(GameObject owner, std::string lvlname);

  bool Initialize();
  void Serialize (DynamicElement* props, Serializer::DataNode* data) {}
    // add self to the menucontroller, then don't update anymore
  void Update(float dt);

    // remove all listeners
  void Release();

  void LoadLevel();

  bool Added;
  std::string Level;
  
};

/****************************************************************************/
/*!
menu Controller

This is the main worker of the menu will 
update position based on the current selection
and launch commands 
*/
/****************************************************************************/

// this is the main workhorse of the menu will update position based on the current selection
class MenuController : public IComponent
{

public:
  MenuController(GameObject);

  ~MenuController();

  void AddButton(GameObject);

  void ActivateButton();

  void MoveUp(void);

  void MoveDown(void);

  void ActivateMenu(void);

  void DeactivateMenu(void);

  void ShowMenu(void);

  void HideMenu(void);

  void HideExit(void);

  // register all listeners
  bool Initialize();

  // 
  void Serialize (DynamicElement* props, Serializer::DataNode* data) {}

  void Update(float dt);

  // remove all listeners
  void Release();

  void SortButtons();

  void SortButtonsHorizontal();

  void UpdatePosition(Vector3D);

  bool MenuActive;

  bool HowToPlayActive;

  bool QuitActive;

  GameObject HowtoPlayScreen;

  ConfirmDestroy * Confirmation;

private:

    bool Initialized;

    // the current selection in the array 
    int currentMenuSelection;

    // make an array of the buttons
    // we may want to sort the menu by y position
    std::vector<GameObject> Buttons;

    // we'll need a to make a menu, keyboard, controller listener
    MenuKeyboardListener * Menu_Keyboard;
    MenuGamepadListener * Menu_Gamepad;
    MenuListener * Menu_Listen;
    GameObject QuitConfirm;

};