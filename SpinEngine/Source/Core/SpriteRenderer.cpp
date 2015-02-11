/****************************************************************************/
/*!
\author Esteban Maldonado
\par    email: esteban.maldonado@digipen.edu
\par    Course: GAM 200
\brief

Class interface for Sprite renderer components, contain all of the necessary
objects for rendering our objects.

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#include "Precompiled.h"
#include "SpriteRenderer.h"
#include "Graphics\Camera.h"
#include "Utilities\MemoryHelper.h"
#include "IEntity.h"
#include "GraphicsManager.h" //to access so we can remove ourself
#include "FactoryAccess.h" //The global point of access for the graphics manager
#include "TileMapData.h"

const float HALF_PI = PI / 2.0f;

ZilchDefineType(SpriteRenderer, SpinningZilch)
{
	type->HandleManager = ZilchManagerId(Zilch::PointerManager);
	ZilchBindMethodAs(ZChangeState, "ChangeState");
	ZilchBindMethodOverload(AddSprite, void, Sprite*);
	ZilchBindMethod(GetPosition);

}

SpriteRenderer::SpriteRenderer(IEntity* Owner, IDirect3DDevice9* device, D3DCOLOR m_color, 
  std::string initStateName) : IComponent(CT_SpriteRenderer, Owner), spriteDrawingDevice(device),
  color(m_color), currentStateName(initStateName), m_spriteStates(SpriteCollection(0)), isStationary(false),
  myPos(const_cast<Vector3D&>((*reinterpret_cast<Transform*>(Owner->GetComponent(CT_TransformComponent))).GetPosition()))
{
  Owner->SpriteRenderer = this;
  reinterpret_cast<Transform*>(Owner->GetComponent(CT_TransformComponent))->GetScale().x *= 1.02f;
  reinterpret_cast<Transform*>(Owner->GetComponent(CT_TransformComponent))->GetScale().y *= 1.04f;
}

SpriteRenderer::~SpriteRenderer()
{
  //Release();
  GlobalFactory->GetGraphicsManager()->RemoveSpriteRend(this);
  GlobalFactory->GetGraphicsManager()->RemoveBGSpriteRend(this);
  GlobalFactory->GetGraphicsManager()->RemoveUISpriteRend(this);
  //Delete all our sprites
  for (SpriteCollIterator it = m_spriteStates.begin(); it != m_spriteStates.end(); it++)
    delete it->second;
}

bool SpriteRenderer::Initialize()
{

  for(SpriteCollIterator it = m_spriteStates.begin(); it != m_spriteStates.end() ; it++)
  {
    if(!it->second->Initialize(spriteDrawingDevice))
    {
      //MessageBox(NULL, "FAILED TO INITIALIZE SPRITES", NULL, NULL);
      return false;
    }
  }

  //If we're a tile, check our surroundings and change our frame and/or rotate as necessary
  //(We automatically change tiles to be correct depending on their surroundings)
  if (isStationary)
  {
    tiles = &GlobalFactory->GetTileMapData();

    //Get our x- and y- position within the tilemap
    //Divide by 2 (since all values are * tile width (which is 2.0f)
    //(we add 0.1f to make sure we're within the bounds of our "tile space."
    // Positions are at the bottom-left and I don't wanna let floating-point math do anything sketchy.)
    int myTileX = static_cast<int>((myPos.x + 0.1f) / 2.0f);
    int myTileY = static_cast<int>((myPos.y + 0.1f) / 2.0f);

    //For each tile, get our neighbors' indeces. Check if we're on an edge, and set any nonexistent neighbor
    //indeces to -1.
    if (myTileX < tiles->width - 1)
      rightNeighborIndex = tiles->numTiles - 1 - (tiles->width - 1) + myTileX + 1 - (tiles->width * myTileY);
    else
      rightNeighborIndex = -1;
    if (myTileX > 0)
      leftNeighborIndex = tiles->numTiles - 1 - (tiles->width - 1) + myTileX - 1 - (tiles->width * myTileY);
    else
      leftNeighborIndex = -1;

    if (myTileY < tiles->height - 1)
      topNeighborIndex = tiles->numTiles - 1 - (tiles->width - 1) + myTileX - (tiles->width * (myTileY + 1));
    else
      topNeighborIndex = -1;
    if (myTileY > 0)
      bottomNeighborIndex = tiles->numTiles - 1 - (tiles->width - 1) + myTileX - (tiles->width * (myTileY - 1));
    else
      bottomNeighborIndex = -1;

    //Store our ID to compare to our neighbors' indeces
    int myIndex = tiles->numTiles - 1 - (tiles->width - 1) + myTileX - (tiles->width * myTileY);
    myTileID = (*(tiles->tiles))[myIndex];
    
    UpdateTileSprite();
  }


  //(We run through this once before we start the update loop, for the benefit of
  //any stationary objects, which won't have their matrices updated again.)
  UpdateMatrices();
  
  return true;
}

void SpriteRenderer::Update(float dt)
{
    if (gGameStatePaused && (this->Owner->GetComponent(CT_MENUCONTROLLER) == NULL && this->Owner->GetComponent(CT_MENUBUTTON) == NULL && this->Owner->GetComponent(CT_HOWTOPLAY) == NULL) && this->Owner->GetComponent(CT_CONFIRMDESTROY) == NULL)
  {
    return;
  }

  if (!isStationary)
  {
    UpdateMatrices();
  }

  m_spriteStates[ currentStateName ]->Update(dt);
}

void SpriteRenderer::Release()
{
  if (m_spriteStates.size() > 0)
  {
    for (SpriteCollIterator it = m_spriteStates.begin(); it != m_spriteStates.end(); it++)
    {
      it->second->Release();
    }
  }
}


void SpriteRenderer::ToggleAsTile()
{
  isStationary = !isStationary;
}

const Vector3D &SpriteRenderer::GetPosition()
{
  return myPos;
}

void SpriteRenderer::Draw(IDirect3DVertexBuffer9** m_ppVBuffer, IDirect3DIndexBuffer9** m_ppIBuffer, bool debug_drawing)
{

  //Vector3D p = reinterpret_cast<Transform*>( 
  //  Owner->GetComponent(CT_TransformComponent))->GetPosition();

  m_spriteStates[currentStateName]->Draw(spriteDrawingDevice, GetWorld(), color, m_ppVBuffer, m_ppIBuffer);
}

D3DMATRIX SpriteRenderer::GetWorld()
{
  return world;
}

SpriteCollection* SpriteRenderer::GetSpriteCollection()
{
  return &m_spriteStates;
}

//Only use this for testing/placeholder spawns in Engine.cpp, as it results in textures being loaded more than once.
void SpriteRenderer::AddSprite(std::string textureName, UINT start_i = 0, UINT end_i = 0, unsigned int total_frames = 1,
  float timePerFrame = 1.0f, std::string state_name = "NewState", bool isDefaultNow)
{
  if ( m_spriteStates.find(state_name) == m_spriteStates.end() )
  {
    //Create a new sprite
    IDirect3DTexture9 *texture;
    HRESULT result = D3DXCreateTextureFromFile(spriteDrawingDevice, textureName.c_str(), &texture);
    if (FAILED(result))
    {
      texture = NULL;
      //MessageBox(NULL, ("FAILED TO LOAD TEXTURE: " + textureName).c_str(), NULL, NULL);
      //return false;
    }

    m_spriteStates[state_name] = new Sprite( texture, textureName, start_i, end_i, total_frames,
      timePerFrame, state_name);
    
    if(isDefaultNow)
    {
      currentStateName = state_name;
    }

    if (!m_spriteStates[state_name]->Initialize(spriteDrawingDevice))
    {
      MessageBox(NULL, ("ERROR: COULD NOT LOAD TEXTURE " + textureName + " SPRITE " + state_name + 
        "NOT INITIALIZED.").c_str(), NULL, NULL);
    }
  }
}

void SpriteRenderer::AddSprite(Sprite *newSprite)
{
  if (newSprite)
  {
    if (m_spriteStates.find(newSprite->GetStateName()) == m_spriteStates.end())
    {
      m_spriteStates[newSprite->GetStateName()] = newSprite;

      //if(isDefaultNow) @Ask Esteban about this part
      //{
      //  currentStateName = state_name;
      //}

      if (!m_spriteStates[newSprite->GetStateName()]->Initialize(spriteDrawingDevice))
      {
        MessageBox(NULL, ("ERROR: COULD NOT LOAD TEXTURE " + newSprite->GetTextureName()
          + " SPRITE " + newSprite->GetStateName() + "NOT INITIALIZED.").c_str(), NULL, NULL);
      }
    }
    else
    {
      MessageBox(NULL, ("ERROR: SPRITE RENDERER WAS GIVEN " + newSprite->GetStateName() + " TWICE.").c_str(), NULL, NULL);
      delete newSprite;
    }
  }
}

void SpriteRenderer::ZChangeState(Zilch::String name)
{
	ChangeState(name.c_str());
}

void SpriteRenderer::ChangeState(std::string new_State_Name)
{
	//If we have such state then make it the current one
	if (m_spriteStates.find(new_State_Name) != m_spriteStates.end())
	{
		currentStateName = new_State_Name;
	}
}

D3DCOLOR& SpriteRenderer::GetSpriteColor()
{
  return color;
}

Sprite* SpriteRenderer::GetCurrentSprite(void)
{
    return m_spriteStates[currentStateName];
}



//Figure out how many neighbors we have, and give ourselves an appropriate random sprite in accordance with our new surroundings
void SpriteRenderer::UpdateTileSprite()
{
  bool leftNeighbor = false, rightNeighbor = false,
    topNeighbor = false, bottomNeighbor = false;
  int sidesCovered = 0;
  //Check our neighboring sprites for tiles with our ID
  if (rightNeighborIndex >= 0 && (*tiles->tiles)[rightNeighborIndex] == myTileID)
  {
    rightNeighbor = true;
    ++sidesCovered;
  }
  if (leftNeighborIndex >= 0 && (*tiles->tiles)[leftNeighborIndex] == myTileID)
  {
    leftNeighbor = true;
    ++sidesCovered;
  }
  if (topNeighborIndex >= 0 && (*tiles->tiles)[topNeighborIndex] == myTileID)
  {
    topNeighbor = true;
    ++sidesCovered;
  }

  if (bottomNeighborIndex >= 0 && (*tiles->tiles)[bottomNeighborIndex] == myTileID)
  {
    bottomNeighbor = true;
    ++sidesCovered;
  }
  //Special case: We count desert tiles as a base neighbor for debris
  else if (bottomNeighborIndex >= 0 && (*tiles->tiles)[bottomNeighborIndex] == 1)
  {
    bottomNeighbor = true;
    ++sidesCovered;
  }


  //Now, depending on which neighbor we have, change our frame and/or rotate accordingly
  if (sidesCovered == 4)
    return;
  if (sidesCovered == 3)
  {
    //Pick one of the one-sided sprites (1 through 8 on the tile sheet)
    m_spriteStates[currentStateName]->SetFrame((rand() % 7) + 1);

    //And then rotate appropriately
    if (!rightNeighbor)
    {
      reinterpret_cast<Transform*>(Owner->GetComponent(CT_TransformComponent))->GetRotation().z = -HALF_PI;
    }
    else if (!leftNeighbor)
    {
      reinterpret_cast<Transform*>(Owner->GetComponent(CT_TransformComponent))->GetRotation().z = HALF_PI;
    }
    else if (!bottomNeighbor)
    {
      reinterpret_cast<Transform*>(Owner->GetComponent(CT_TransformComponent))->GetRotation().z = PI;
    }
    else //no top neighbor
    {
      reinterpret_cast<Transform*>(Owner->GetComponent(CT_TransformComponent))->GetRotation().z = 0.0f;
    }
    UpdateMatrices();
    return;
  }
  if (sidesCovered == 2)
  {
    //Pick one of the two-sided sprites (9-12 on the tile sheet)
    m_spriteStates[currentStateName]->SetFrame((rand() % 4) + 9);

    //And then rotate appropriately
    if (rightNeighbor && leftNeighbor)
    {
      //We only have one case for left/right and top/bottom neighbors right now
      m_spriteStates[currentStateName]->SetFrame(15);
      //Flip on a 50/50 chance=
      if (rand() % 2 == 0)
        reinterpret_cast<Transform*>(Owner->GetComponent(CT_TransformComponent))->GetRotation().z = PI;
      else
        reinterpret_cast<Transform*>(Owner->GetComponent(CT_TransformComponent))->GetRotation().z = 0;
    }
    else if (rightNeighbor && topNeighbor)
    {
      reinterpret_cast<Transform*>(Owner->GetComponent(CT_TransformComponent))->GetRotation().z = PI;
    }
    else if (rightNeighbor && bottomNeighbor)
    {
      reinterpret_cast<Transform*>(Owner->GetComponent(CT_TransformComponent))->GetRotation().z = HALF_PI;
    }
    else if (bottomNeighbor && topNeighbor)
    {
      //We only have one case for left/right and top/bottom neighbors right now
      m_spriteStates[currentStateName]->SetFrame(15);
      //rotate 90 degrees and then flip on a 50/50 chance
      if (rand() % 2 == 0)
        reinterpret_cast<Transform*>(Owner->GetComponent(CT_TransformComponent))->GetRotation().z = PI + HALF_PI;
      else
        reinterpret_cast<Transform*>(Owner->GetComponent(CT_TransformComponent))->GetRotation().z = HALF_PI;
    }
    else if (bottomNeighbor && leftNeighbor)
    {
      reinterpret_cast<Transform*>(Owner->GetComponent(CT_TransformComponent))->GetRotation().z = 0.0f;
    }
    else if (leftNeighbor && topNeighbor)
    {
      reinterpret_cast<Transform*>(Owner->GetComponent(CT_TransformComponent))->GetRotation().z = PI + HALF_PI;
    }

    UpdateMatrices();
    return;
  }



  //At this point, only one side is covered
  //sprite 13 or 14
  m_spriteStates[currentStateName]->SetFrame((rand() % 2) + 13);
  if (rightNeighbor)
  {
    reinterpret_cast<Transform*>(Owner->GetComponent(CT_TransformComponent))->GetRotation().z = HALF_PI;
  }
  else if (leftNeighbor)
  {
    reinterpret_cast<Transform*>(Owner->GetComponent(CT_TransformComponent))->GetRotation().z = PI + HALF_PI;
  }
  else if (topNeighbor)
  {
    reinterpret_cast<Transform*>(Owner->GetComponent(CT_TransformComponent))->GetRotation().z = PI;
  }
  else //bottom neighbor
  {
    reinterpret_cast<Transform*>(Owner->GetComponent(CT_TransformComponent))->GetRotation().z = 0;
  }
  UpdateMatrices();
  return;




}

//Call UpdateTileSprite() on any like-tiled neighbors
//(we call this from TileMapDetection, whenever a tile is removed or destroyed)
void SpriteRenderer::UpdateSurroundingTileSprites()
{
  if (rightNeighborIndex >= 0 && (*tiles->tiles)[rightNeighborIndex] == myTileID)
    reinterpret_cast<SpriteRenderer*>(GlobalFactory->GetGameObjectList()[tiles->factoryIndeces[rightNeighborIndex]]->GetComponent(CT_SpriteRenderer))->UpdateTileSprite();
  if (leftNeighborIndex >= 0 && (*tiles->tiles)[leftNeighborIndex] == myTileID)
    reinterpret_cast<SpriteRenderer*>(GlobalFactory->GetGameObjectList()[tiles->factoryIndeces[leftNeighborIndex]]->GetComponent(CT_SpriteRenderer))->UpdateTileSprite();
  if (topNeighborIndex >= 0 && (*tiles->tiles)[topNeighborIndex] == myTileID)
    reinterpret_cast<SpriteRenderer*>(GlobalFactory->GetGameObjectList()[tiles->factoryIndeces[topNeighborIndex]]->GetComponent(CT_SpriteRenderer))->UpdateTileSprite();
  if (bottomNeighborIndex >= 0 && (*tiles->tiles)[bottomNeighborIndex] == myTileID)
    reinterpret_cast<SpriteRenderer*>(GlobalFactory->GetGameObjectList()[tiles->factoryIndeces[bottomNeighborIndex]]->GetComponent(CT_SpriteRenderer))->UpdateTileSprite();
}

//Update our scale, rotation and translation matrices
//(If we aren't moving, rotating, or scaling up/down, we don't need to do this.
//Hence, for tiles, we only call this after changes, whereas for everything else
//we run it every frame.)
void SpriteRenderer::UpdateMatrices()
{
  D3DXMATRIX Scale, Rx, Ry, Rz, Translation;
  D3DXVECTOR2 scale2D, translate2D;

  Vector3D rot = Owner->GetTransform()->GetRotation();
  D3DXMatrixRotationX(&Rx, Math::DegToRad(rot.x));
  D3DXMatrixRotationY(&Ry, Math::DegToRad(rot.y));
  D3DXMatrixRotationZ(&Rz, Math::DegToRad(rot.z));

  Vector3D scal = Owner->GetTransform()->GetScale();
  D3DXMatrixScaling(&Scale, scal.x, scal.y, scal.z);

  Vector3D& tempTanslate = Owner->GetTransform()->GetPosition();
  D3DXMatrixTranslation(&Translation, tempTanslate.x, tempTanslate.y, tempTanslate.z);

  world = Scale * Rx * Ry * Rz * Translation;
}