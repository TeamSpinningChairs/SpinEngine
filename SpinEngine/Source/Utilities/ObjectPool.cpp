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
#include "Precompiled.h"
#include "ObjectPool.h"

#define FREE true
#define TAKEN false

template<typename T>
ObjectPool<T>::ObjectPool(unsigned poolsize) : poolSize_(poolsize), pool_(new std::pair<bool,T*>[poolsize])
{
}
template<typename T>
ObjectPool<T>::~ObjectPool()
{
  delete[] pool_;
}

//Given a pointer, provides an object. Returns false if pool was empty.
template<typename T>
bool ObjectPool<T>::Get(T **objtotake)
{
  //Iterate through the pool. Give the first available instance of the object.
  for (unsigned i = 0; i < poolSize_; ++i)
  {
    if (pool_[i].first == FREE)
    {
      *objtotake = &(pool_[i].second);
      pool_[i].first = TAKEN;
      return true;
    }
  }

  return false;
}

//Returns an object to the free pool (after reinitializing it).
template<typename T>
void ObjectPool<T>::Give(T *objtoreturn)
{
  //Initialize the object.
  //(How? Create a PoolObject class that poolable things need to inherit from?)
  //For now, just initialize to default values. This will cause errors if there's
  //no default constructor for T, but errors'll lead you here quickly. (hello, future)
  *objtoreturn = T();

  //Set availability to true at its spot in the list.
  for (unsigned i = 0; i < poolSize_; ++i)
  {
    if (&(pool_[i].second) == objtoreturn)
    {
      pool_[i].first = FREE;
      return;
    }
  }

}