#include "Precompiled.h"
#include "FlashAnimator.h"

FlashAnimator::FlashAnimator(GameObject parent, GameObject anchor, std::string animationname) : IComponent(CT_CoolRoilingParticle, parent),
anchorPos(&anchor->GetTransform()->GetWorldPosition()), myPos(&parent->GetTransform()->GetWorldPosition())
{
  //Get our animation info from ObjectData (also have ObjectData load and parse the animation data into an easily readable form)
}

bool FlashAnimator::Initialize()
{
  return true;
}
void FlashAnimator::Release()
{
}

void FlashAnimator::Update(float dt)
{ 
  //Move us to our anchor's position (I.E. to the player's position) and offset us by our default amount.
  *myPos = *anchorPos + offset;

  //Interpolate the previous and next frames' positions/rotations/scales by the current time
  
  //Rotate and scale accordingly, then move us to the correct position.
}

//Order of events:
//Export flash data to XML, convert XML to JSON
//Factory startup, ObjectData loads & parses all animation files in master.json
//(you will specify the file *and* individual animation names to look for in master.json)
//On construction, ask global factory accessor for animation data relating to our animation name
//Get that data, use it as needed.
//This should be just as complex as required to support both further flash animations and reuse
//of any individual animation's data (since there'll be up to four players)