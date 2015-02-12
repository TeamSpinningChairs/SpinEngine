/****************************************************************************/
/*!
\author Henry Morgan
\par    email: henry.morgan@digipen.edu
\par    Course: GAM 200
\brief

Paralaxes a background

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#include "Precompiled.h"
#include "ParallaxBG.h"
#include "IComponent.h"
#include "JSONLoader.h"
#include "FactoryAccess.h"
#include "SpriteRenderer.h"
#include "GraphicsManager.h"
#include "Transform.h"
#include "Settings.h"

ParallaxBG::ParallaxBG(GameObject parent) : parent_(parent), pos_(*parent->GetTransform()),
IComponent(CT_ParallaxBG, parent), prevXPos_(parent->GetTransform()->GetWorldPosition().x)
{
  float *temp = NULL;
  GlobalSettings->GetFloatValue("___ BACKGROUND SETTINGS ___", temp, false);
  GlobalSettings->GetFloatValue("Foreground Speed", foregroundSpeed_, true);
  GlobalSettings->GetFloatValue("Midground Speed", midgroundSpeed_, true);
  GlobalSettings->GetFloatValue("Background Speed", backgroundSpeed_, true);

  JSONLoader loader;
  //Open and load the master file
  loader.LoadArchive("Assets\\master.json");
  DynamicElement *root = &loader.Root();

  DynamicElement *backgrounds;
  root->GetObjectMember(&backgrounds, "backgrounds");

  //Get all three backgrounds
  DynamicElement *bg;
  backgrounds->GetObjectMember(&bg, "fore");
  bg->GetObjectMember(&bg, "path");
  std::string path;
  bg->GetStringVal(&path);
  HRESULT result = D3DXCreateTextureFromFile(GlobalFactory->GetDevice(), path.c_str(), &texture_fore);
  if (FAILED(result))
  {
    MessageBox(NULL, ("FAILED TO LOAD TEXTURE: " + path).c_str(), NULL, NULL);
  }

  backgrounds->GetObjectMember(&bg, "middle");
  bg->GetObjectMember(&bg, "path");
  bg->GetStringVal(&path);
  result = D3DXCreateTextureFromFile(GlobalFactory->GetDevice(), path.c_str(), &texture_mid);
  if (FAILED(result))
  {
    MessageBox(NULL, ("FAILED TO LOAD TEXTURE: " + path).c_str(), NULL, NULL);
  }

  backgrounds->GetObjectMember(&bg, "back");
  bg->GetObjectMember(&bg, "path");
  bg->GetStringVal(&path);
  result = D3DXCreateTextureFromFile(GlobalFactory->GetDevice(), path.c_str(), &texture_back);
  if (FAILED(result))
  {
    MessageBox(NULL, ("FAILED TO LOAD TEXTURE: " + path).c_str(), NULL, NULL);
  }

  GameObject mem_ = MemoryManager::Allocate_GameObj();
  fore1 = new (mem_) IEntity(0U, pos_.GetPosition(), Vector3D(), Vector3D(32.0f, 40.0f, 1.0f));

  mem_ = MemoryManager::Allocate_GameObj();
  fore2 = new (mem_) IEntity(0U, pos_.GetPosition(), Vector3D(), Vector3D(32.0f, 40.0f, 1.0f));
  fore2->GetTransform()->GetWorldPosition().x += 64.0f;

  mem_ = MemoryManager::Allocate_GameObj();
  mid1 = new (mem_) IEntity(0U, pos_.GetPosition(), Vector3D(), Vector3D(32.0f, 40.0f, 1.0f));
  
  mem_ = MemoryManager::Allocate_GameObj();
  mid2 = new (mem_) IEntity(0U, pos_.GetPosition(), Vector3D(), Vector3D(32.0f, 40.0f, 1.0f));
  mid2->GetTransform()->GetWorldPosition().x += 64.0f;

  mem_ = MemoryManager::Allocate_GameObj();
  back1 = new (mem_) IEntity(0U, pos_.GetPosition(), Vector3D(), Vector3D(32.0f, 40.0f, 1.0f));
  
  mem_ = MemoryManager::Allocate_GameObj();
  back2 = new (mem_) IEntity(0U, pos_.GetPosition(), Vector3D(), Vector3D(32.0f, 40.0f, 1.0f));
  back2->GetTransform()->GetWorldPosition().x += 64.0f;

  //Create all six sprite renderers (two copies of each)
  sp_back1 = new SpriteRenderer(back1, GlobalFactory->GetDevice(), d3dColors::White, "idle");
  GlobalFactory->GetGraphicsManager()->AddBGSpriteRend(sp_back1);
  sp_back1->AddSprite(new Sprite(texture_back));
  sp_back2 = new SpriteRenderer(back2, GlobalFactory->GetDevice(), d3dColors::White, "idle");
  GlobalFactory->GetGraphicsManager()->AddBGSpriteRend(sp_back2);
  sp_back2->AddSprite(new Sprite(texture_back));
  
  sp_mid1 = new SpriteRenderer(mid1, GlobalFactory->GetDevice(), d3dColors::White, "idle");
  GlobalFactory->GetGraphicsManager()->AddBGSpriteRend(sp_mid1);
  sp_mid1->AddSprite(new Sprite(texture_mid));
  sp_mid2 = new SpriteRenderer(mid2, GlobalFactory->GetDevice(), d3dColors::White, "idle");
  GlobalFactory->GetGraphicsManager()->AddBGSpriteRend(sp_mid2);
  sp_mid2->AddSprite(new Sprite(texture_mid));
  
  sp_fore1 = new SpriteRenderer(fore1, GlobalFactory->GetDevice(), d3dColors::White, "idle");
  GlobalFactory->GetGraphicsManager()->AddBGSpriteRend(sp_fore1);
  sp_fore1->AddSprite(new Sprite(texture_fore));
  sp_fore2 = new SpriteRenderer(fore2, GlobalFactory->GetDevice(), d3dColors::White, "idle");
  GlobalFactory->GetGraphicsManager()->AddBGSpriteRend(sp_fore2);
  sp_fore2->AddSprite(new Sprite(texture_fore));

  
}

ParallaxBG::~ParallaxBG()
{
  texture_back->Release();
  texture_mid->Release();
  texture_fore->Release();

  //I didn't want to create a BGSpriteRenderer class that had a different destructor
  //but was otherwise the same, so instead we manually remove each BG sprite renderer from
  //the graphics manager. They'll try to remove themselves from the regular sprite renderer vector
  //in their destructors, but nothing will happen since they're not in it.
  GlobalFactory->GetGraphicsManager()->RemoveBGSpriteRend(sp_fore1);
  GlobalFactory->GetGraphicsManager()->RemoveBGSpriteRend(sp_fore2);
  GlobalFactory->GetGraphicsManager()->RemoveBGSpriteRend(sp_mid1);
  GlobalFactory->GetGraphicsManager()->RemoveBGSpriteRend(sp_mid2);
  GlobalFactory->GetGraphicsManager()->RemoveBGSpriteRend(sp_back1);
  GlobalFactory->GetGraphicsManager()->RemoveBGSpriteRend(sp_back2);

  SPIN_ENGINE_SAFE_DELETE(sp_fore1);
  SPIN_ENGINE_SAFE_DELETE(sp_fore2);
  SPIN_ENGINE_SAFE_DELETE(sp_mid1);
  SPIN_ENGINE_SAFE_DELETE(sp_mid2);
  SPIN_ENGINE_SAFE_DELETE(sp_back1);
  SPIN_ENGINE_SAFE_DELETE(sp_back2);

  fore1->~IEntity();
  MemoryManager::Free_GameObject(fore1);
  fore1 = NULL;
  //SPIN_ENGINE_SAFE_DELETE(fore1);
  
  fore2->~IEntity();
  MemoryManager::Free_GameObject(fore2);
  fore2 = NULL;
  //SPIN_ENGINE_SAFE_DELETE(fore2);
  
  mid1->~IEntity();
  MemoryManager::Free_GameObject(mid1);
  mid1 = NULL;
  //SPIN_ENGINE_SAFE_DELETE(mid1);

  mid2->~IEntity();
  MemoryManager::Free_GameObject(mid2);
  mid2 = NULL;
  //SPIN_ENGINE_SAFE_DELETE(mid2);

  back1->~IEntity();
  MemoryManager::Free_GameObject(back1);
  back1 = NULL;
  //SPIN_ENGINE_SAFE_DELETE(back1);

  back2->~IEntity();
  MemoryManager::Free_GameObject(back2);
  back2 = NULL;
  //SPIN_ENGINE_SAFE_DELETE(back2);
}

bool ParallaxBG::Initialize()
{
  //fore1->GetTransform()->GetWorldPosition().x = fore1->GetTransform()->GetWorldPosition().x + (horizDiff * (*foregroundSpeed_));
  fore1->GetTransform()->GetWorldPosition().y = pos_.GetPosition().y + 16.5f; //magic number for our prototype resolution, make it account for resolution later
  //fore2->GetTransform()->GetWorldPosition().x = fore2->GetTransform()->GetWorldPosition().x + (horizDiff * (*foregroundSpeed_));
  fore2->GetTransform()->GetWorldPosition().y = pos_.GetPosition().y + 16.5f;

 // mid1->GetTransform()->GetWorldPosition().x = mid1->GetTransform()->GetWorldPosition().x + (horizDiff * (*midgroundSpeed_));
  mid1->GetTransform()->GetWorldPosition().y = pos_.GetPosition().y + 16.5f;
 // mid2->GetTransform()->GetWorldPosition().x = mid2->GetTransform()->GetWorldPosition().x + (horizDiff * (*midgroundSpeed_));
  mid2->GetTransform()->GetWorldPosition().y = pos_.GetPosition().y + 16.5f;

 // back1->GetTransform()->GetWorldPosition().x = back1->GetTransform()->GetWorldPosition().x + (horizDiff * (*backgroundSpeed_));
  back1->GetTransform()->GetWorldPosition().y = pos_.GetPosition().y + 12.5f;
 // back2->GetTransform()->GetWorldPosition().x = back2->GetTransform()->GetWorldPosition().x + (horizDiff * (*backgroundSpeed_));
  back2->GetTransform()->GetWorldPosition().y = pos_.GetPosition().y + 12.5f;
  return true;
}

void ParallaxBG::Update(float dt)
{
  float horizDiff = prevXPos_ - pos_.GetPosition().x;
  
  //If the camera is one (current, temp) background width ahead of the bottom-left
  //of the background sprite, move the background sprite one background ahead
  CheckForWrap(fore1);
  CheckForWrap(fore2);
  CheckForWrap(mid1);
  CheckForWrap(mid2);
  CheckForWrap(back1);
  CheckForWrap(back2);

  //Update positions

  //Here: Instead of separate velocities, multiply the position of each by the one before. I.E. mid goes half the distance of fore, and back goes 1/8 the distance of mid.
  //Calculate distance between last and current position for the foreground, work with that.
  //float prevLayerStartPos = fore1->GetTransform()->GetWorldPosition().x; //1 is the layer prior to 2, and 2 is the layer prior to 3

  //fore1->GetTransform()->GetWorldPosition().x = fore1->GetTransform()->GetWorldPosition().x + (horizDiff * (*foregroundSpeed_));
  //fore1->GetTransform()->GetWorldPosition().y = pos_.GetPosition().y + 16.5f; //magic number for our prototype resolution, make it account for resolution later
  //fore2->GetTransform()->GetWorldPosition().x = fore2->GetTransform()->GetWorldPosition().x + (horizDiff * (*foregroundSpeed_));
 // fore2->GetTransform()->GetWorldPosition().y = pos_.GetPosition().y + 16.5f;

  //horizDiff = fore1->GetTransform()->GetWorldPosition().x - prevLayerStartPos;
  //float prevLayerStartPos = mid1->GetTransform()->GetWorldPosition().x;

  //We subtract the value of (horizDiff * midgroundSpeed) because our "parallaxing" on the front layer is just holding it still.
  //horizDiff is a negative number, so instead of making the background scroll more slowly towards us, we actually
  //push it away slightly.
  //This won't work if you want the camera tied to the player(s)
  mid1->GetTransform()->GetWorldPosition().x = mid1->GetTransform()->GetWorldPosition().x - (horizDiff * (*midgroundSpeed_)); 
  //mid1->GetTransform()->GetWorldPosition().y = pos_.GetPosition().y + 16.5f;
  mid2->GetTransform()->GetWorldPosition().x = mid2->GetTransform()->GetWorldPosition().x - (horizDiff * (*midgroundSpeed_));
  //mid2->GetTransform()->GetWorldPosition().y = pos_.GetPosition().y + 16.5f;

  //horizDiff = mid1->GetTransform()->GetWorldPosition().x - prevLayerStartPos;
  
  back1->GetTransform()->GetWorldPosition().x = back1->GetTransform()->GetWorldPosition().x - (horizDiff * (*backgroundSpeed_));
 // back1->GetTransform()->GetWorldPosition().y = pos_.GetPosition().y + 16.5f;
  back2->GetTransform()->GetWorldPosition().x = back2->GetTransform()->GetWorldPosition().x - (horizDiff * (*backgroundSpeed_));
  //back2->GetTransform()->GetWorldPosition().y = pos_.GetPosition().y + 16.5f;
  

  prevXPos_ = pos_.GetPosition().x;
}

void ParallaxBG::Release()
{

}

void ParallaxBG::CheckForWrap(GameObject bg)
{
  //Coordinates of camera are at bottom-left of the screen.
  if (pos_.GetPosition().x - bg->GetTransform()->GetWorldPosition().x > 66.0f) //Background is out of sight by 2 tiles
    bg->GetTransform()->GetWorldPosition().x += 128.0f;
  else if (pos_.GetPosition().x - bg->GetTransform()->GetWorldPosition().x < -66.0f)
      bg->GetTransform()->GetWorldPosition().x -= 128.0f;
}