/****************************************************************************/
/*!
\author Steven Gallwas
\par    email: s.gallwas\@digipen.edu
\par    Course: GAM 200
\date  
\brief


Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#pragma once
#include "IComponent.h"
#include "IEntity.h"

class DestroyOnWall : public IComponent
{

public:
	DestroyOnWall(GameObject Parent, GameObject Wall);

	bool Initialize();

	void Serialize (DynamicElement* props, Serializer::DataNode* data) {}

	void Update(float dt);

	void Release();

private:
	GameObject DeathWall;

};