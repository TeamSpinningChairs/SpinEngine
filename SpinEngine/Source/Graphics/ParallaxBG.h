#pragma once
//Component containing three gameobjects (foreground, midground, background)
//that parallax. Attached to the camera.
//Henry

class SpriteRenderer;
class Sprite;
struct IDirect3DTexture9;
class IComponent;

class ParallaxBG : public IComponent
{
public:
  ParallaxBG(GameObject parent);
  ~ParallaxBG();

  bool Initialize() override;
  void Serialize (DynamicElement* props, Serializer::DataNode* data) {}
  void Update(float dt) override;
  void Release() override;

private:
  ParallaxBG();
  ParallaxBG(const ParallaxBG &rhs);

  void CheckForWrap(GameObject bg);

  GameObject parent_;
  Transform &pos_;
  float prevXPos_;

  float *foregroundSpeed_;
  float *midgroundSpeed_;
  float *backgroundSpeed_;

  SpriteRenderer *sp_fore1;
  SpriteRenderer *sp_fore2;
  SpriteRenderer *sp_mid1;
  SpriteRenderer *sp_mid2;
  SpriteRenderer *sp_back1;
  SpriteRenderer *sp_back2;

  IDirect3DTexture9 *texture_fore;
  IDirect3DTexture9 *texture_mid;
  IDirect3DTexture9 *texture_back;

  //"Parent" objects. They only exist to move the sprite renderers.
  GameObject fore1;
  GameObject fore2;
  GameObject mid1;
  GameObject mid2;
  GameObject back1;
  GameObject back2;
  
};
