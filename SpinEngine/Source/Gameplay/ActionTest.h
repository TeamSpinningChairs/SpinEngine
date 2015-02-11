#include "Precompiled.h"
#include "Action.h"

class ActionTest : public IComponent
{

public:
	ActionTest(IEntity * Parent);

	//ChaseComponent(IEntity * Parent, float speed);

	bool Initialize();
	void Serialize (DynamicElement* props, Serializer::DataNode* data) {}
	void Update(float dt);

	void TestCall(void);

	void Release();
	
	

private:

	Action* action;
	Vector3D val;
	Vector3D val2;
	

};