/****************************************************************************/
/*!
\author Esteban Maldonado
\par    email: esteban.maldonado@digipen.edu
\par    Course: GAM 200
\brief

Class interface for the transform component, has all of the necessary members
for moving, rotating and scaling the objects on the world space.

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#include "Precompiled.h"
#include"Transform.h"
#include"IEntity.h"

ZilchDefineType(Transform, SpinningZilch)
{
	type->HandleManager = ZilchManagerId(Zilch::PointerManager);
	ZilchBindFieldGetSetAs(position, "Translation");
	ZilchBindFieldGetSetAs(rotation, "Rotaion");
	ZilchBindFieldGetSetAs(scale, "Scale");

}

Transform::Transform(IEntity* Owner, Vector3D pos, Vector3D rot, Vector3D scal) :
                   IComponent(Component_Type::CT_TransformComponent, Owner, "Transform"), position(pos),
                   rotation(rot), scale(scal)
{
	Owner->Transform = this;
}

Transform::~Transform()
{
}

bool Transform::Initialize()
{
  return true;
}

void Transform::Update(float dt)
{
}

void Transform::Release()
{
}

Vector3D& Transform::GetPosition()
{
  return position;
}

Vector3D& Transform::GetRotation()
{
  return rotation;
}

Vector3D& Transform::GetScale()
{
  return scale;
}

void Transform::SetPosition(const Vector2D pos)
{
  position.x = pos.x;
  position.y = pos.y;
}

void Transform::SetPosition(const Vector3D vec)
{
  position = vec;
}

void Transform::SetPositionX(float x)
{
  position.x = x;
}

void Transform::SetPositionY(float y)
{
  position.y = y;
}

void Transform::SetPositionZ(float z)
{
  position.z = z;
}

void Transform::SetPosition(float x, float y, float z)
{
  position.x = x;
  position.y = y;
  position.z = z;
}

void Transform::SetRotation(Vector3D rot)
{
  rotation = rot;
}

void Transform::SetRotationX(float x)
{
  rotation.x = x;
}

void Transform::SetRotationY(float y)
{
  rotation.y = y;
}

void Transform::SetRotationZ(float z)
{
  rotation.z = z;
}

void Transform::SetRotation(float x, float y, float z)
{
  rotation.x = x;
  rotation.y = y;
  rotation.z = z;
}

void Transform::SetRotation(const Vector2D rot)
{
  rotation.x = rot.x;
  rotation.y = rot.y;
}

void Transform::SetScale(Vector3D scal)
{
  scale = scal;
}

void Transform::SetScaleX(float x)
{
  scale.x = x;
}

void Transform::SetScaleY(float y)
{
  scale.y = y;
}

void Transform::SetScaleZ(float z)
{
  scale.z = z;
}

void Transform::SetScale(float x, float y, float z)
{
  scale.x = x;
  scale.y = y;
  scale.z = z;
}

void Transform::SetScale(const Vector2D scal)
{
  scale.x = scal.x;
  scale.y = scal.y;
}