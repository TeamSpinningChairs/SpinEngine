/****************************************************************************/
/*!
\author Judy Cheng
\par    email: j.cheng\@digipen.edu (Optional)
\par    Course: GAM 200
\date   xx/xx/xx (Optional)
\brief

The rigidbody class header

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#pragma once
#include "Math\mathlib.h"
#include "IEntity.h"
#include "CollisionDelegate.h"
#include <vector> //for the vector of trigger callbacks
#include <utility> //pair
#include "DataNode.h"

class RigidBody : public IComponent
{
public:
	ZilchDeclareDerivedType(RigidBody, IComponent);
  
  // Will make these variables private later
  friend class Primitive;
  friend class Circle;
  friend class AABB;

  Transform *pTrans;
  Primitive *bodyShape;
  Vector2D position;       // Holds the position of the rigid body
  Vector2D acceleration;
  Vector2D velocity;       // Holds the linear velocity of the rigid body
  Vector2D forceAccum;     // Accumlated force
  float mass;             // Holds the mass of the rigid body
  float invMass;          // Holds the inverted mass
  float rotation;         // Holds the rotation
  float restitution;      // Holds the restitution
  float angularVelocity;
  float orientation;
  float inertia;
  float invInertia;
  /* float friction;
  float stFric;
  float dynFric;*/
 
  bool isStatic;          // True if object is static
  // Set where rigidbody doesn't have a resolution
  bool isGhost;
  bool useGravity;         // Set gravity on or off
  bool isKinematic;        // Something to be added 

  RigidBody(IEntity* Owner, Primitive *shape);
  ~RigidBody();

  bool Initialize() override;
  void Serialize (DynamicElement* props, Serializer::DataNode* data) {}
  void Update(float dt) override;
  void Release() override;

  void set(float mass);
    // Sets the position
  void setPosition(const Vector2D &position);
  void setPosition(const float x, const float y);
  Vector2D getPosition() const;

   // Set the acceleration
  void setAcceleration(const Vector2D &acc);
  void setAcceleration(const float x, const float y);
  Vector2D getAcceleration() const;

  void setVelocity(const Vector2D &velocity);
  void setVelocity(const float x, const float y);
  Vector2D getVelocity() const;
  
  void setMass(const float mass); 
  float getMass() const;
  float getInvMass() const;

  void setRotation(const float rotation);
  float getRotation() const;

  void AddForce(const Vector2D &force);
  void AddForce(Vector2D* force);
  void SetStatic(void);
  void ApplyImpulse(const Vector2D& impulse, const Vector2D& contactVec);
  void ApplyImpulse(Vector2D* impulse, Vector2D* contactVec);
  void SetOrientation(float radians);

  bool IsStatic();
  bool CheckDetectsCollision();
  void MovePosition(Vector2D &pos); // Moves the rigidbody to position.
  void SetDensity(float density);  //Sets the mass based on the attached colliders assuming a constant density.


  //Called in PhysicsManager when two objects collide
  void Trigger(IEntity *collObj);
  void AddTriggerCallback(CollisionDelegate *del);
  //Vector of pairs of collision delegate methods and the objects they're in.
  //(Is all of this necessary? Could you just have one function since it's all the same name?)
  std::vector <CollisionDelegate *> triggerCallbacks;

};