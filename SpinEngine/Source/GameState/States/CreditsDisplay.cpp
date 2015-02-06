/****************************************************************************/
/*!
\author Steven Galwas
\par    email: henry.morgan@digipen.edu
\par    Course: GAM 200
\brief

Displays the credits for our game. Single component for what is technically
a "level." Revised by Henry.

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#include "Precompiled.h"
#include "CreditsDisplay.h"
#include "SpriteRenderer.h"
#include "FactoryAccess.h"
#include "GraphicsManager.h"
#include "Engine.h"

CreditsDisplay::CreditsDisplay(GameObject Parent) : IComponent(Component_Type::CT_CREDITSDISPLAY, Parent)
{
  Timer = 0.0f;
  CreditsScreenObj = GlobalFactory->CreateGameObject("credits", "Menu_Credits.png", Vector3D(0, 0, 0));
  CreditsScreenObj->GetTransform()->GetScale().x *= 18;
  CreditsScreenObj->GetTransform()->GetScale().y *= 30;

  CameraObject = GlobalFactory->CreateGameObject("credits", "Menu_Credits.png", Vector3D(0, 16, 0));
  CameraObject->SetInVisible();
  ScrollComponent *CreditScroll = new ScrollComponent(CameraObject, 2, Vector3D(0, -1, 0), 15.0f);
  CreditScroll->AddDelay(3.0f);

  //Currently, we have the camera declared *here* instead of in Factory like normal.
  //This is so we can add the ScrollComponent. Later, if you feel like it (but probably
  //not because this is a single-use case), give the main camera in Factory
  //to this constructor directly, and find a way to guarantee that that camera is
  //actually the camera in the level file, and not the default one in GraphicsManager.

  //(as a result of the camera being here, there is not one in the level file)

  Camera *cam = new Camera(CameraObject);
  ENGINE->m_Graphics->SetMainCamera(cam);
  if (ENGINE->m_Graphics->m_bFullScreen)
  {
    cam->Set_Width_Height(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
  }
  else
  {
    cam->Set_Width_Height(ENGINE->m_Graphics->GetWindow()->GetWindowWidth(),
      ENGINE->m_Graphics->GetWindow()->GetWindowHeight());
  }

  cam->SetBackColor(d3dColors::Black);
  CameraObject->AddGameComponent(CT_CameraComponent, cam);
  CameraObject->AddGameComponent(CT_SCROLL, CreditScroll);
  ENGINE->m_Graphics->SetMainCamera(cam);
}
CreditsDisplay::~CreditsDisplay()
{
  //The Factory will unload everything for us.
  //GameStateManager will set main_camera back to the default one in graphics manager when we unload
}

bool CreditsDisplay::Initialize()
{
  return true;
}

void CreditsDisplay::Update(float dt)
{
    Timer += dt;

    if (Timer > 13.0f)
    {
        if (ENGINE->m_Input->IsKeyPressed(VK_SPACE))
        {
            gGameStateCurr = GS_LOAD;
            gCurrentLevel = "Level_Menu";
        }

        if (Timer > 19.0f)
        {
            gGameStateCurr = GS_LOAD;
            gCurrentLevel = "Level_Menu";
        }
    }
}

void CreditsDisplay::Release()
{

}