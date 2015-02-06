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
#include "DestroyOnWallActivate.h"
#include "FactoryAccess.h"

DestroyMessage::DestroyMessage()
{
  this->MessageType = MESSAGE_DESTROY;
}


DestroyListener::DestroyListener(GameObject ParentObj)
{
  owner = ParentObj;
}

void DestroyListener::OnMessageRecieved(Message * SentMessage)
{
  GlobalFactory->RemoveGameObject(owner->GetEntityId());
}


DestroyOnActivate::DestroyOnActivate(GameObject Parent) : IComponent(CT_DESTROY_ON_ACTIVATE, Parent)
{
  DestList = new DestroyListener(this->Owner);
}

bool DestroyOnActivate::Initialize()
{
  GlobalSystem->RegisterListener(MESSAGE_DESTROY, DestList);
  return true;
}

void DestroyOnActivate::Update(float dt)
{
    
}

void DestroyOnActivate::Release()
{
  GlobalSystem->UnregisterListener(MESSAGE_DESTROY, DestList);
}


