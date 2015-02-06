#pragma once

//Lasers, The Class
class PlayerController;
class CollisionDelegate;
class IComponent;
class IEntity;
class TileMapDetection;

class LaserBeam : public IComponent, public CollisionDelegate
{
public:
  LaserBeam(IEntity *parent, PlayerController *player, float speed);

  bool Initialize();
  void Serialize (DynamicElement* props, Serializer::DataNode* data) {}
  void Update(float dt);
  void Release();

  void Launch(Vector3D pos, float theta, Vector2D velocity);
  void OnCollision(GameObject) override;

private:
  PlayerController *myPlayer;
  TileMapDetection *tileMapDetection;
  RigidBody *rigid;
  IEntity *myParent;
  Vector3D hidePosition;

  float flightTimer;
  bool flying;
  float flySpeed;

  void HideAndReturn();
};