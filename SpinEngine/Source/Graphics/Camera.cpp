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
#include "Precompiled.h"
#include "Camera.h"
#include "Settings.h"

#define PIXELS_PER_UNIT 64.0f

#define PERSPECTIVE_CAMERA_DISTANCE 35.0f
#define WALL_FOLLOW_OFFSET 16.0f

ZilchDefineType(Camera, SpinningZilch)
{
	type->HandleManager = ZilchManagerId(Zilch::PointerManager);
	ZilchBindMethodOverload(SetPosition, void, Vector3D);
	ZilchBindMethodOverload(SetPosition, void, float, float, float);
	ZilchBindMethodOverload(SetRotation, void, Vector3D);
	ZilchBindMethodOverload(SetRotation, void, float, float, float);
	ZilchBindMethodAs(SetZfar, "SetZFar");
	ZilchBindMethodAs(SetZnear, "SetZNear");
	ZilchBindMethod(GetPosition);
	ZilchBindMethod(AddPlayerPosition);
	ZilchBindMethod(SnapToNearestPixel);
	ZilchBindMethodAs(Set_Width_Height, "SetDimensions");
	ZilchBindMethod(SetZoom);
	ZilchBindMethod(GetZoom);
	ZilchBindFieldGetSet(Owner);

	
}

Camera::Camera(IEntity* Owner) : IComponent( CT_CameraComponent, Owner), 
position(Vector3D()), rotation(Vector3D()), z_near(-10.0f), z_far(5000.0f), view_angle(D3DX_PI / 4.0f),
width(800), height(600), background_color(d3dColors::Black), followingWall(false)
{
  if (Owner)
  {
    position.x = Owner->GetTransform()->GetWorldPosition().x;
    position.y = Owner->GetTransform()->GetWorldPosition().y - 4.0f;
    position.z = Owner->GetTransform()->GetWorldPosition().z - PERSPECTIVE_CAMERA_DISTANCE; //28 is close on the x-axis

    rotation = Owner->GetTransform()->GetWorldRotation();
  }

  for (int i = 0; i < 4; ++i)
  {
    playerPositions[i] = nullptr;
  }

  //Creating junk values; these get deleted and then assigned appropriate tweakable
  //values from settings.json in Initialize()
  //(We don't do it here because Settings is (and must be) created AFTER the graphics
  //manager, and the graphics manager includes Cameras)
  zoomFactor = new float(16.0f);
  pixelsPerUnit = new float(24.0f);
}

Camera::~Camera()
{
}

void Camera::SetPosition(float x, float y, float z)
{
  position.x = x;
  position.y = y;
  position.z = z;
}

void Camera::SetPosition(Vector3D pos)
{
  position = pos;
}

void Camera::SetRotation(float x, float y, float z)
{
	rotation.x = x;
	rotation.y = y;
	rotation.z = z;
}

void Camera::SetRotation(Vector3D rot)
{
  rotation = rot;
}

void Camera::GetViewMatrix(D3DXMATRIX& viewMatrix)
{
  D3DXVECTOR3 up;
  D3DXVECTOR3 lookAt;
  float radians;

  //If the camera has an owner then follow it on X and Y
  if (Owner)
  {
    position.x = Owner->GetTransform()->GetWorldPosition().x;
    //position.y = Owner->GetTransform()->GetWorldPosition().y;
    //AlignToNearestPixel();
    rotation = Owner->GetTransform()->GetWorldRotation();

    if (followingWall)
    {
      //Can't do anything like this until we actually have a strict pixels->world units conversion
     //position.x += width / 2.0f;
      position.x += WALL_FOLLOW_OFFSET;

      //Zoom in when the player's closer
      WallCameraMovement();
    }
  }

  SnapToNearestPixel();

  // Setup the vector that points upwards.
  up.x = 0.0f;//x
  up.y = 1.0f;//y
  up.z = 0.0f;//z

  // Calculate the rotation in radians.
  radians = rotation.y * 0.0174532925f;

  // Setup where the camera is looking.
  lookAt.x = sinf(radians) + position.x;
  lookAt.y = position.y;
  lookAt.z = cosf(radians) + position.z;

  // Create the view matrix from the three vectors.
  D3DXVECTOR3 vm = D3DXVECTOR3(position.x, position.y, position.z);
  D3DXMatrixLookAtLH(&m_viewMatrix, &vm,
    &lookAt, &up);

	viewMatrix = m_viewMatrix;
}

void Camera::GetProjMatrix(D3DXMATRIX& projMatrix)
{
  //D3DXMatrixPerspectiveFovLH(&m_projMatrix, view_angle, width / height, z_near, z_far);
  D3DXMatrixOrthoLH(&m_projMatrix, width, height, z_near, z_far);
  m_projMatrix._11 *= *zoomFactor;
  m_projMatrix._22 *= *zoomFactor;
  //m_projMatrix._33 = 1.0f; //probably unnecessary
  //m_projMatrix._44 = 1.0f; //Also probably unnecessary

  //Position
  //m_projMatrix._41 = (position.x * *zoomFactor);
  //m_projMatrix._42 = (position.y * *zoomFactor);

  projMatrix = m_projMatrix;
}

void Camera::SnapToNearestPixel()
{
  position.x = static_cast<float>(static_cast<int>(position.x * *pixelsPerUnit)) / *pixelsPerUnit;
  position.y = static_cast<float>(static_cast<int>(position.y * *pixelsPerUnit)) / *pixelsPerUnit;
}

bool Camera::Initialize()
{
  float *temp = nullptr;
  delete zoomFactor;
  delete pixelsPerUnit;
  GlobalSettings->GetFloatValue("___ CAMERA SETTINGS ___", temp, false);
  GlobalSettings->GetFloatValue("Camera Zoom Factor", zoomFactor, true);
  GlobalSettings->GetFloatValue("Pixels Per Unit", pixelsPerUnit, true);

  return true;
}

void Camera::Update(float dt)
{
}

Camera& Camera::operator=(const Camera& cam)
{
  position = cam.position;
  rotation = cam.rotation;

  z_near = cam.z_near;
  z_far = cam.z_far;

  width = cam.width;
  height = cam.height;

  return *this;
}

void Camera::SetZoom(float zoom)
{
	*zoomFactor = zoom;
}

float Camera::GetZoom()
{
	return *zoomFactor;
}

void Camera::SetZnear(float z)
{
  z_near = z;
}

void Camera::SetZfar(float z)
{
  z_far = z;
}

float Camera::GetZnear()
{
  return z_near;
}

float Camera::GetZfar()
{
  return z_far;
}

float Camera::GetAspectRatio()
{
  return width / height;
}

float Camera::GetWidth()
{
  return width;
}

float Camera::GetHeight()
{
  return height;
}

Vector3D& Camera::GetRotation()
{
  return rotation;
}

Vector3D& Camera::GetPosition()
{
  return position;
}

D3DCOLOR Camera::GetBackColor()
{
  return background_color;
}

void Camera::SetBackColor(D3DCOLOR color)
{
  background_color = color;
}

void Camera::Set_Width_Height(float w, float h)
{
  width = w;
  height = h;
}

void Camera::Release()
{
}

void Camera::AlignToNearestPixel()
{
  //position.x = MathF::Round(position.x * PIXELS_PER_UNIT) / PIXELS_PER_UNIT;
  //position.y = MathF::Round(position.y * PIXELS_PER_UNIT) / PIXELS_PER_UNIT;
}


void Camera::SetToFollowWall()
{
  followingWall = true;
}


//Adjust position in response to player position
void Camera::WallCameraMovement()
{
  float longestDistanceToPlayer = 0.0f;
  //Find the most distant player.
  for (int i = 0; i < 4; ++i)
  {
    if (playerPositions[i] == nullptr)
      continue;

    if (playerPositions[i]->GetWorldPosition().x - position.x > longestDistanceToPlayer)
      longestDistanceToPlayer = playerPositions[i]->GetWorldPosition().x - position.x;
  }

  //Adjust zoom based on player distance


  //When the player is close, zoom in.

  //Further away, zoom out. How?

}

//Adds another player to the array of tracked players.
//Caps out at 4.
void Camera::AddPlayerPosition(Transform *playerpos)
{
  for (int i = 0; i < 4; ++i)
  {
    if (playerPositions[i] == nullptr)
    {
      playerPositions[i] = playerpos;
      return;
    }
  }
}