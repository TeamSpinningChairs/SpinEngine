/****************************************************************************/
/*!
\author Judy Cheng
\par    email: j.cheng\@digipen.edu (Optional)
\par    Course: GAM 200
\date   xx/xx/xx (Optional)
\brief

Contains the primitive shapes


Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#include "Precompiled.h"
#include "primitives.h"

ZilchDefineType(Primitive, SpinningZilch)
{
	type->HandleManager = ZilchManagerId(Zilch::PointerManager);
	ZilchBindFieldGetSetAs(active, "Active");
	ZilchBindFieldGetSetAs(radius, "Radius")
}

ZilchDefineType(Circle, SpinningZilch)
{
	type->HandleManager = ZilchManagerId(Zilch::PointerManager);
}

ZilchDefineType(AABB, SpinningZilch)
{
	type->HandleManager = ZilchManagerId(Zilch::PointerManager);
}

////// PRIMITIVE //////
Primitive::Primitive(GameObject parent, ShapeID pID, Component_Type type)
  :IComponent(type, parent), Id(pID)
{
  radius = 0.0f;
  active = true; //false;
}

Primitive::~Primitive()
{
    for (auto iter = physics->colliders.begin(); iter != physics->colliders.end(); ++iter)
    {
        if (*iter == this)
        {
            physics->colliders.erase(iter);
            return;
        }
    }
}

bool Primitive::Initialize()
{
  physics->colliders.push_back(this);
  return true;
}

void Primitive::Update(float dt) {}
void Primitive::Release(){}


////// CIRCLE //////
Circle::Circle()
  : Primitive(nullptr, pCircle, Component_Type::CT_CircleCollider)
{

}

Circle::Circle(GameObject Owner)
  :Primitive(Owner, pCircle, Component_Type::CT_CircleCollider)
{
}

Circle::~Circle(){}

bool Circle::Initialize()
{
  pTrans = Owner->GetTransform();
  physics->colliders.push_back(this);
  return true;
}

void Circle::Update(float dt){}

void Circle::Release(){}

////// AABB //////
AABB::AABB()
  :Primitive(Owner, pAABB, Component_Type::CT_BoxCollider)
{
}

AABB::AABB(GameObject Owner)
  :Primitive(Owner, pAABB, Component_Type::CT_BoxCollider)
{

}
AABB::~AABB()
{
}

bool AABB::Initialize()
{
  pTrans = Owner->GetTransform();
  physics->colliders.push_back(this);
  return true;
}
void AABB::Serialize(DynamicElement* props, Serializer::DataNode* data)
{

}
void AABB::Update(float dt){}

void AABB::Release(){}

