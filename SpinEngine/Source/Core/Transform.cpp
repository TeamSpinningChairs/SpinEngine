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
	ZilchBindFieldGetSetAs(localPosition, "Translation");
	ZilchBindFieldGetSetAs(localRotation, "Rotation");
	ZilchBindFieldGetSetAs(localScale, "Scale");
	ZilchBindFieldGetSetAs(position, "WorldTranslation");
	ZilchBindFieldGetSetAs(rotation, "WorldRotation");
	ZilchBindFieldGetSetAs(scale, "WorldScale");

}

Transform::Transform(IEntity* Owner, Vector3D pos, Vector3D rot, Vector3D scal) :
                   IComponent(Component_Type::CT_TransformComponent, Owner, "Transform"), localPosition(pos),
                   localRotation(rot), localScale(scal)
{
	//position = pos;
	//rotation = rot;
	//scale = scal;
	Owner->Transform = this;
}

Transform::~Transform()
{
}

bool Transform::Initialize()
{
  UpdateTransformations();
	
  return true;
}

//Use if you need the transformations to be updated IMMEDIATELY and not at the start of the next frame.
void Transform::UpdateTransformations()
{
	if (Owner->Parent != nullptr)
	{
		Transform* parentPos = Owner->Parent->Transform;
		UpdatePosition(parentPos);
		UpdateRotation(parentPos);
		UpdateScale(parentPos);
	}
	else
	{
		position = localPosition;
		rotation = localRotation;
		scale = localScale;
	}
}

void Transform::Update(float dt)
{

}

void Transform::Release()
{
}

Vector3D& Transform::GetPosition()
{
	return localPosition;
}

Vector3D& Transform::GetRotation()
{
  return localRotation;
}

Vector3D& Transform::GetScale()
{
	return localScale;
}

Vector3D& Transform::GetWorldPosition()
{
	return position;
}

Vector3D& Transform::GetWorldRotation()
{
	return rotation;
}

Vector3D& Transform::GetWorldScale()
{
	return scale;
}

void Transform::SetPosition(const Vector2D vec)
{
	localPosition.x = vec.x;
	localPosition.y = vec.y;
}

void Transform::SetPosition(const Vector3D vec)
{
	localPosition = vec;
}

void Transform::SetPositionX(float x)
{
	localPosition.x = x;
}

void Transform::SetPositionY(float y)
{
	localPosition.y = y;
}

void Transform::SetPositionZ(float z)
{
	localPosition.z = z;
}

void Transform::SetPosition(float x, float y, float z)
{
	localPosition.x = x;
	localPosition.y = y;
	localPosition.z = z;
}

void Transform::SetRotation(Vector3D rot)
{
	localRotation = rot;
}

void Transform::SetRotationX(float x)
{
	localRotation.x = x;
}

void Transform::SetRotationY(float y)
{
	localRotation.y = y;
}

void Transform::SetRotationZ(float z)
{
	localRotation.z = z;
}

void Transform::SetRotation(float x, float y, float z)
{
	localRotation.x = x;
	localRotation.y = y;
	localRotation.z = z;
}

void Transform::SetRotation(const Vector2D rot)
{
	localRotation.x = rot.x;
	localRotation.y = rot.y;
}

void Transform::SetScale(Vector3D scal)
{
  localScale = scal;
}

void Transform::SetScaleX(float x)
{
	localScale.x = x;
}

void Transform::SetScaleY(float y)
{
	localScale.y = y;
}

void Transform::SetScaleZ(float z)
{
	localScale.z = z;
}

void Transform::SetScale(float x, float y, float z)
{
	localScale.x = x;
	localScale.y = y;
	localScale.z = z;
}

void Transform::SetScale(const Vector2D scal)
{
	localScale.x = scal.x;
	localScale.y = scal.y;
}

void Transform::UpdatePosition(Transform* trans)
{
	if (Owner->InheritPosition && Owner->Parent != nullptr)
	{
		Vector3D positionAdd = trans->GetWorldPosition();
		float parentAngle = trans->GetWorldRotation().z;
		float cos0 = cos(parentAngle * 0.0174532925199); //Pi/2
		float sin0 = sin(parentAngle * 0.0174532925199); //Pi/2
		Vector3D newPos;
		//glm::mat2 matrix;
		//matrix.x = { cosX, -sinX };
		//matrix.y = {-sinX, cosX};


		if (Owner->Pivot)
		{
			//DOES NOT DEAL WITH Z POSITION
			newPos.x = (localPosition.x * cos0) - (localPosition.y * sin0);
			newPos.y = (localPosition.y * cos0) + (localPosition.x * sin0);
		}
		else
		{
			newPos.x = localPosition.x;
			newPos.y = localPosition.y;
		}
		position = newPos + positionAdd;

	}
	else
	{
		position = localPosition;
	}
}
void Transform::UpdateScale(Transform* trans)
{
	if (Owner->InheritScale  && Owner->Parent != nullptr)
	{
		Vector3D parentScale = trans->GetWorldScale();
		scale.x = localScale.x * parentScale.x;
		scale.y = localScale.y * parentScale.y;
		scale.z = localScale.z * parentScale.z;
	}
	else
	{
		scale = localScale;
	}

}
void Transform::UpdateRotation(Transform* trans)
{
	if (Owner->InheritRotation  && Owner->Parent != nullptr)
	{
		rotation = localRotation + trans->GetWorldRotation();
	}
	else
	{
		rotation = localRotation;
	}
}