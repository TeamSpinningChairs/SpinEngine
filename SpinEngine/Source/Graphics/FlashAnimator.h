//A component that will get animation data and adjust its parent's position/scale/rotation accordingly
//Currently planning to have this component "anchor" its parent to another object. Might remove
//that if/when adding parenting functionality to the Transform class becomes necessary.
#pragma once
#include "Precompiled.h"


class FlashAnimator : public IComponent
{
public:

  FlashAnimator(GameObject parent, GameObject anchor, std::string animationname);

  //These are required, but unused
  bool Initialize() override;
  void Update(float dt) override;
  void Release() override;

private:
  Vector3D *const anchorPos; //Pointer to our anchor object's position (the "parent" of our parent)
  Vector3D *const myPos; //Pointer to our position
  Vector3D offset; //The offset between the anchor's position and our position
};