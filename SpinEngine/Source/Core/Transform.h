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
#pragma once
#include "Precompiled.h" //Math/Vec3.h

class Transform : public IComponent
{
public:

	ZilchDeclareDerivedType(Transform, IComponent);
  friend class RigidBody;
  Transform(IEntity* Owner = NULL, Vector3D pos = Vector3D(), Vector3D rot = Vector3D(), 
    Vector3D scal = Vector3D() );
  ~Transform() override;

  //Inherited Methods
  bool Initialize() override;
  void Update(float dt) override;
  void Serialize (DynamicElement* props, Serializer::DataNode* data) {}
  void Release() override;
  

  //Getters
  Vector3D& GetPosition();
  Vector3D& GetRotation();
  Vector3D& GetScale();

  //Setters
  void SetPosition(const Vector3D pos);
  void SetPosition(const Vector2D pos);
  void SetPosition(float x, float y, float z);
  void SetPositionX(float x);
  void SetPositionY(float y);
  void SetPositionZ(float z);

  void SetRotation(Vector3D rot);
  void SetRotation(const Vector2D rot);
  void SetRotationX(float x);
  void SetRotationY(float y);
  void SetRotationZ(float z);
  void SetRotation(float x, float y, float z);

  void SetScale(Vector3D scal);
  void SetScale(const Vector2D scal);
  void SetScaleX(float x);
  void SetScaleY(float y);
  void SetScaleZ(float z);
  void SetScale(float x, float y, float z);


private:
  Vector3D position;
  Vector3D scale;
  Vector3D rotation;
};