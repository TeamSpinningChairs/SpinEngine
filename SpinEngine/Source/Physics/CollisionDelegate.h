/****************************************************************************/
/*!
\author Henry Morgan
\par    email: henry.morgan@digipen.edu
\par    Course: GAM 200
\brief

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#pragma once

//#include "IEntity.h" <-- We don't need it for just pointers

//Have a class inherit CollisionDelegate to allow one of its functions to override Callback.
//When Initialize is called (it must be called by the inheritor, preferably in Initialize),
//It adds its information to the list of delegates in its object's Rigidbody.
//When the object collides with another, all collision delegates will be called.

//Forward declaration
class IEntity; //<-- This will suffice

class CollisionDelegate
{
public:
  CollisionDelegate(IEntity *parent);
  void InitializeCollisionCallback();
  //The callback function
  virtual void OnCollision(IEntity *object) = 0;


private:
  IEntity *const parent_;
  //These should never happen:
  CollisionDelegate();
  CollisionDelegate(CollisionDelegate &rhs);
};