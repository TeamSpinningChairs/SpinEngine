/****************************************************************************/
/*!
\author Esteban Maldonado
\par    email: esteban.maldonado@digipen.edu
\par    Course: GAM 200
\brief

Class interface for the camera component

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#pragma once 

//#include <DirectXMath.h>
#include "Precompiled.h" //<d3d9.h>, <d3dx9.h>, <d3d9types.h>
#include "Core\Transform.h"

//using DirectX::XMVECTOR;
//using DirectX::XMMATRIX;

class Camera : public IComponent
{
public:
  Camera(IEntity* Owner);
  ~Camera()  override;

  //Inherited Methods
  bool Initialize() override;
  void Serialize (DynamicElement* props, Serializer::DataNode* data) {}
  void Update(float dt) override;
  void Release() override;

  //Setters
	void SetPosition(float x, float y, float z);
  void SetPosition(Vector3D pos);

	void SetRotation(float x, float y, float z);
  void SetRotation(Vector3D rot);

  void SetZnear(float z);
  void SetZfar(float z);
  void Set_Width_Height(float w, float h);

  void SetBackColor(D3DCOLOR color);

  void SetToFollowWall();

  //Getters
  void GetViewMatrix(D3DXMATRIX& viewMatrix);
  void GetProjMatrix(D3DXMATRIX& projMatrix);

  float GetZnear();
  float GetZfar();

  float GetAspectRatio();
  float GetWidth();
  float GetHeight();

  D3DCOLOR GetBackColor();

  Vector3D& GetPosition();
  Vector3D& GetRotation();

  Camera& operator=(const Camera& cam);

  void AddPlayerPosition(Transform *);

private:
  void AlignToNearestPixel();
  void WallCameraMovement();
  bool followingWall;
  Transform *playerPositions[4];

	D3DXMATRIX m_viewMatrix;
  D3DXMATRIX m_projMatrix;
	Vector3D position;
	Vector3D rotation;

  float z_near;
  float z_far;

  float *zoomFactor;
  float *pixelsPerUnit;
  void SnapToNearestPixel();

  float view_angle;
  float width;
  float height;
  D3DCOLOR background_color;
  
};