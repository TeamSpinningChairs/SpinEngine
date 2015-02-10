/****************************************************************************/
/*!
\author Judy Cheng
\par    email: j.cheng\@digipen.edu (Optional)
\par    Course: GAM 200
\date   xx/xx/xx (Optional)
\brief

Header for the primitive shapes


Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#pragma once
#include <cstdlib>
#include "rigidbody.h"
#include "Precompiled.h"
#include "manifold.h"
#include "DataNode.h"

class Primitive:  public IComponent
{
public:
	ZilchDeclareDerivedType(Primitive, IComponent);
  enum ShapeID
  {
    pCircle,
    pAABB,
    pCount
  };

  Transform *pTrans;// position
  float radius;     // For circle
  Vector2D halfSize; // halfsize
  ShapeID Id;       // The Shape ID
  RigidBody *body;  // The rigidbody the collider is attached to
  PhysicsMaterial material;
  bool active;      // True for search
  

  bool Initialize() override;
  void Serialize (DynamicElement* props, Serializer::DataNode* data) {}
  void Update(float dt) override;
  void Release() override;

  //Primitive();
  //Primitive(IEntity* Owner);
  Primitive(GameObject parent, ShapeID pID, Component_Type type);

  virtual ~Primitive() = 0;
  virtual Primitive *Clone(void) const = 0;
  virtual void SetOrientation(float radians) const = 0;
  virtual ShapeID GetID(void) const = 0;
};

class Circle : public Primitive
{
public:
	ZilchDeclareDerivedType(Circle, Primitive);
  Circle();
  Circle(GameObject Owner);
  ~Circle();

  bool Initialize() override;
  void Serialize (DynamicElement* props, Serializer::DataNode* data) {}
  void Update(float dt) override;
  void Release() override;

  Primitive *Clone(void) const
  {
    return new Circle();
  }

  void SetOrientation(float radians) const {}

  ShapeID GetID(void) const
  {
    return pCircle;
  }

};

class AABB : public Primitive
{
public:
	ZilchDeclareDerivedType(AABB, Primitive);
  AABB();
  AABB(GameObject Owner);
  ~AABB();

  bool Initialize() override;
  void Serialize(DynamicElement* props, Serializer::DataNode* data);
  void Update(float dt) override;
  void Release() override;

  Primitive *Clone(void) const
  {
    return new AABB();
  }

  void SetOrientation(float radians) const {}

  ShapeID GetID(void) const
  {
    return pAABB;
  }

  void SetHalfSize(float width, float height)
  {
    halfSize.x = width;
    halfSize.y = height;

    vertexCount = 4;
    AABB_vertices[0].Set(-width, -height);
    AABB_vertices[1].Set(width, -height);
    AABB_vertices[2].Set(width, height);
    AABB_vertices[3].Set(-width, height);
  }

  int vertexCount;
  Vector2D AABB_vertices[4];
};