#include "Precompiled.h"
#include "ActionTest.h"

//static ObjectAllocator ActionAllocator{ sizeof(Action), OAConfig() };



ActionTest::ActionTest(IEntity * Parent) : IComponent(Component_Type::CT_ACTIONTEST, Parent)
{
}

bool ActionTest::Initialize()
{
	
	val = Owner->GetTransform()->GetPosition();
	val2 = Owner->GetTransform()->GetScale();
	
	Vector3D endval = Vector3D(val.x - 6, val.y + 5, val.z);
	Vector3D endval2 = Vector3D(val.x - 4, val.y - 3 , val.z);
	Vector3D endval3 = Vector3D(val.x + 2, val.y + 2, val.z);
	//action = reinterpret_cast<Action*>(ActionAllocator.Allocate());
	//new (action) Action(&val, endval, 2, Ease::QuarticInOut);

	ActionSequence* seq = Action::Sequence(&Owner->Actions);
	Action::Property(seq, &val, endval, 1, Ease::QuadInOut);
	ActionGroup* grp = Action::Group(seq);
	Action::Property(grp, &val, endval2, 2, Ease::CubicInOut);
	Action::Property(grp, &val2, Vector3D(2, 1, 1), 2, Ease::CubicInOut);

	Action::Property(seq, &val, endval3, 1, Ease::QntInOut);
	
	//Action::Call<void(ActionTest::*)(void)>(seq, &ActionTest::TestCall);

	return true;
}

void ActionTest::TestCall(void)
{

}

void ActionTest::Update(float dt)
{
	//Calling a member function pointer on an allocated object.
	//((action->*(action->Update))(dt));
	Owner->Actions->Update(dt);
	Owner->GetTransform()->SetPosition(val);
	Owner->GetTransform()->SetScale(val2);
}

void ActionTest::Release()
{
	//action->~Action();
	//ActionAllocator.Free(action);
}

// C2064c.cpp
//struct C {
//	void func1(){}
//	void func2(){}
//};
//
//typedef void (C::*pFunc)();
//
//int bleh() {
//	C* c;
//	pFunc funcArray = &C::func1;
//	(c->*funcArray)();
//
//	return 9;
//}