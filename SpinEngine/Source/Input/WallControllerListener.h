/****************************************************************************/
/*!
\author Steven Gallwas
\par    email: s.gallwas\@digipen.edu
\par    Course: GAM 200
\brief


Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#pragma once
#include "Core\NewMessageSystem.h"
#include "SpriteRenderer.h"

class WallController;

class WallControllerListener : public MessageListener
{
    public:
        WallControllerListener(IEntity *playerObj, int controllerNum);
        void OnMessageRecieved(Message * SentMessage);
        void Initialize();
  
    private:
        IEntity *const player;
        RigidBody *PlayerBody;
        SpriteRenderer *PlayerSprite;
        Transform *PlayerTransform;
        int controllerNum;
        WallController * WallControl;
        int *ControlType;
};