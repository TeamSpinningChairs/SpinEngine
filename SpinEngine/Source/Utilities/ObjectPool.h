/****************************************************************************/
/*!
\author Henry Morgan
\par    email: henry.morgan@digipen.edu
\par    Course: GAM 200
\brief

Templated class to provide a reusable pool of objects. Currently unused,
but would eventually work well for bullets or lasers or something.

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#pragma once
#include "Precompiled.h"
//Henry
//Templated class for having a contiguous pool of objects in memory.
//Currently has no functionality for expanding the pool- you're expected
//to check the bool returned from Get before doing anything with the
//pooled object (since if you're using all of the pool, Get will
//return false and not actually provide you with anything).

//(Probably not going back to this until I add lasers.)

template<typename T>
class ObjectPool
{
public:
  ObjectPool(unsigned poolsize);
  ~ObjectPool();

  //Given a pointer, provides an object. Returns false if pool was empty.
  bool Get(T **objtotake);
  //Returns an object to the free pool (after reinitializing it).
  void Give(T *objtoreturn);

private:
  const unsigned poolSize_;
  std::pair<bool, T> *pool_;


  ObjectPool();
  ObjectPool(const ObjectPool& rhs);
};