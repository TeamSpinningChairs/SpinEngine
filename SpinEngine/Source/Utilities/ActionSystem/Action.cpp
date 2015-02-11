#include "Precompiled.h"
#include "Action.h"
#include "ActionSequence.h"


ZilchDefineType(Action, SpinningZilch)
{
	type->HandleManager = ZilchManagerId(Zilch::PointerManager);
	
	ZilchBindStaticMethodOverload(Sequence, ActionSequence*, IEntity*);
	ZilchBindStaticMethodOverload(Sequence, ActionSequence*, Zilch::Any);

	ZilchBindStaticMethodOverload(Group, ActionGroup*, ActionSequence*);

	ZilchBindStaticMethodOverload(Property, void, ActionSequence* seq, Vector2D* startVal, Vector2D endVal, float time, int ease);
	ZilchBindStaticMethodOverload(Property, void, ActionSequence* seq, Vector3D* startVal, Vector3D endVal, float time, int ease);
	ZilchBindStaticMethodOverload(Property, void, ActionSequence* seq, Vector4D* startVal, Vector4D endVal, float time, int ease);
	ZilchBindStaticMethodOverload(Delay, void, ActionSequence* seq, float time);
	ZilchBindStaticMethodOverload(Call, void, ActionSequence* seq, Zilch::Any);

	ZilchBindStaticMethodOverload(Property, void, ActionGroup* seq, Vector2D* startVal, Vector2D endVal, float time, int ease);
	ZilchBindStaticMethodOverload(Property, void, ActionGroup* seq, Vector3D* startVal, Vector3D endVal, float time, int ease);
	ZilchBindStaticMethodOverload(Property, void, ActionGroup* seq, Vector4D* startVal, Vector4D endVal, float time, int ease);
	//ZilchBindStaticMethodOverload(Call, void, ActionGroup* seq, Zilch::Any);
	ZilchBindStaticMethodOverload(Delay, void, ActionGroup* seq, float time);
}

ZilchDefineType(ActionGroup, SpinningZilch)
{
	type->HandleManager = ZilchManagerId(Zilch::PointerManager);
}

ZilchDefineType(ActionProperty, SpinningZilch)
{
	type->HandleManager = ZilchManagerId(Zilch::PointerManager);
}

//###############################################################################################//
//Action
//###############################################################################################//
Action::Action()
{
	Time = 0;
	TimePassed = 0;
}
//Calls
template<typename T>
Action::Action(T function)
{
	SetValues(function);
}
//Floats
Action::Action(float* startVal, float endVal, float time, Ease::Eases ease)
{
	SetValues(*startVal, startVal, endVal, time, ease);
	
}
//Ints
Action::Action(int* startVal, int endVal, float time, Ease::Eases ease)
{
	SetValues(*startVal, startVal, endVal, time, ease);
	
}
//Vec2
Action::Action(Vector2D* startVal, Vector2D endVal, float time, Ease::Eases ease)
{
	SetValues(startVal, startVal, &endVal, time, ease);
	
}

//Vec3
Action::Action(Vector3D* startVal, Vector3D endVal, float time, Ease::Eases ease)
{
	SetValues(startVal, startVal, &endVal, time, ease);
	
}

Action::Action(Vector4D* startVal, Vector4D endVal, float time, Ease::Eases ease)
{
	SetValues(startVal, startVal, &endVal, time, ease);

}

Action::Action(float time)
{
	SetValues(time);
}

void Action::SetValues(float time)
{
	Update = &Action::UpdateDelay;
	Time = time;
	TimePassed = 0;
	StartVal = Vector4D(0, 0, 0, 0);
	CurrentVal = nullptr;
	EndVal = Vector4D(0, 0, 0, 0);
	Active = true;
	Started = true;
	Completed = false;
}
template<typename T>
void Action::SetValues(T function)
{
	Update = &Action::UpdateDelay;
	Time = 0;
	TimePassed = 0;
	StartVal = Vector4D(0, 0, 0, 0);
	CurrentVal = nullptr;
	EndVal = Vector4D(0, 0, 0, 0);
	Active = true;
	Started = true;
	Completed = true;
	//No need for a "UpdateCall" function.
	//function();
}

void Action::SetValues(float startVal, float* currentVal, float endVal, float time, Ease::Eases ease)
{
	Update = &Action::UpdateFloat;
	StartVal.x = startVal;
	CurrentVal = currentVal;
	EndVal.x = endVal;
	Time = time;
	TimePassed = 0;
	Active = true;
	Started = true;
	Completed = false;
	if (Curve != nullptr)
	{
		delete Curve;
	}
	Curve = new SampleCurve(ease);
}

void Action::SetValues(int startVal, int* currentVal, int endVal, float time, Ease::Eases ease)
{
	Update = &Action::UpdateInt;
	StartVal.x = startVal;
	CurrentVal = currentVal;
	EndVal.x = endVal;
	Time = time;
	TimePassed = 0;
	Active = true;
	Started = true;
	Completed = false;
	if (Curve != nullptr)
	{
		delete Curve;
	}
	Curve = new SampleCurve(ease);
}

void Action::SetValues(Vector2D* startVal, Vector2D* currentVal, Vector2D* endVal, float time, Ease::Eases ease)
{
	Update = &Action::UpdateVec2;
	StartVal.x = startVal->x;
	StartVal.y = startVal->y;
	CurrentVal = currentVal;
	EndVal.x = endVal->x;
	EndVal.y = endVal->y;
	Time = time;
	TimePassed = 0;
	Active = true;
	Started = true;
	Completed = false;
	if (Curve != nullptr)
	{
		delete Curve;
	}
	Curve = new SampleCurve(ease);
}

void Action::SetValues(Vector3D* startVal, Vector3D* currentVal, Vector3D* endVal, float time, Ease::Eases ease)
{
	Update = &Action::UpdateVec3;
	StartVal.x = startVal->x;
	StartVal.y = startVal->y;
	StartVal.z = startVal->z;
	CurrentVal = currentVal;
	EndVal.x = endVal->x;
	EndVal.y = endVal->y;
	EndVal.z = endVal->z;
	Time = time;
	TimePassed = 0;
	Active = true;
	Started = true;
	Completed = false;
	if (Curve != nullptr)
	{
		delete Curve;
	}
	Curve = new SampleCurve(ease);
}

void Action::SetValues(Vector4D* startVal, Vector4D* currentVal, Vector4D* endVal, float time, Ease::Eases ease)
{
	Update = &Action::UpdateVec4;
	StartVal = *startVal;
	CurrentVal = currentVal;
	EndVal = *endVal;
	Time = time;
	TimePassed = 0;
	Active = true;
	Started = true;
	Completed = false;
	if (Curve != nullptr)
	{
		delete Curve;
	}
	Curve = new SampleCurve(ease);
}

void Action::SetValues(ActionGroup* grp)
{
	Update = &Action::UpdateGroup;
	CurrentGroup = grp;
	grp->SetValues();
	StartVal = Vector4D(0, 0, 0, 0);
	CurrentVal = nullptr;
	EndVal = Vector4D(0, 0, 0, 0);
	Time = 0;
	TimePassed = 0;
	Active = true;
	Started = true;
	Completed = false;
	if (Curve != nullptr)
	{
		delete Curve;
	}
	Curve = nullptr;
}

void Action::UpdateFloat(float dt)
{
	if (!Active)
	{
		return;
	}
	float* valPtr = reinterpret_cast<float*>(CurrentVal);
	float startVal = StartVal.x;
	float endVal = EndVal.x;

	float newVal = Curve->Sample(TimePassed, startVal, endVal, Time);

	TimePassed += dt;

	if (newVal != endVal)
	{
		*valPtr = newVal;
	}

	if (TimePassed >= Time)
	{
		Completed = true;
		Active = false;
		Started = false;
		TimePassed = Time;
		*valPtr = endVal;
	}

}

void Action::UpdateInt(float dt)
{
	if (!Active)
	{
		return;
	}
	int* valPtr = reinterpret_cast<int*>(CurrentVal);
	int startVal = StartVal.x;
	int endVal = EndVal.x;

	int newVal = Curve->Sample(TimePassed, startVal, endVal, Time);

	TimePassed += dt;

	if (newVal != endVal)
	{
		*valPtr = newVal;
	}

	if (TimePassed >= Time)
	{
		Completed = true;
		Active = false;
		Started = false;
		TimePassed = Time;
		*valPtr = endVal;
	}

}

void Action::UpdateVec2(float dt)
{
	if (!Active)
	{
		return;
	}
	Vector2D* valPtr = reinterpret_cast<Vector2D*>(CurrentVal);
	Vector2D startVal = Vector2D(StartVal.x, StartVal.y);
	Vector2D endVal = Vector2D(EndVal.x, EndVal.y);

	float newValX = Curve->Sample(TimePassed, startVal.x, endVal.x, Time);
	float newValY = Curve->Sample(TimePassed, startVal.y, endVal.y, Time);
	Vector2D newVal(newValX, newValY);

	TimePassed += dt;

	if (newVal.x != endVal.x)
	{
		*valPtr = newVal;
	}

	if (TimePassed >= Time)
	{
		Completed = true;
		Active = false;
		Started = false;
		TimePassed = Time;
		*valPtr = endVal;
	}

}

void Action::UpdateVec3(float dt)
{
	if (!Active)
	{
		return;
	}
	Vector3D* valPtr = reinterpret_cast<Vector3D*>(CurrentVal);
	Vector3D startVal = Vector3D(StartVal.x, StartVal.y, StartVal.z);
	Vector3D endVal = Vector3D(EndVal.x, EndVal.y, EndVal.z);

	float newValX = Curve->Sample(TimePassed, startVal.x, endVal.x, Time);
	float newValY = Curve->Sample(TimePassed, startVal.y, endVal.y, Time);
	float newValZ = Curve->Sample(TimePassed, startVal.z, endVal.z, Time);
	Vector3D newVal(newValX, newValY, newValZ);

	TimePassed += dt;

	if (newVal.x != endVal.x)
	{
		*valPtr = newVal;
	}

	if (TimePassed >= Time)
	{
		Completed = true;
		Active = false;
		Started = false;
		TimePassed = Time;
		*valPtr = endVal;
	}

}

void Action::UpdateVec4(float dt)
{
	if (!Active)
	{
		return;
	}
	Vector4D* valPtr = reinterpret_cast<Vector4D*>(CurrentVal);
	Vector4D startVal = StartVal;
	Vector4D endVal = EndVal;

	float newValX = Curve->Sample(TimePassed, startVal.x, endVal.x, Time);
	float newValY = Curve->Sample(TimePassed, startVal.y, endVal.y, Time);
	float newValZ = Curve->Sample(TimePassed, startVal.z, endVal.z, Time);
	float newValW = Curve->Sample(TimePassed, startVal.w, endVal.w, Time);
	Vector4D newVal(newValX, newValY, newValZ, newValW);

	TimePassed += dt;

	if (newVal.x != endVal.x)
	{
		*valPtr = newVal;
	}

	if (TimePassed >= Time)
	{
		Completed = true;
		Active = false;
		Started = false;
		TimePassed = Time;
		*valPtr = endVal;
	}
}

void Action::UpdateGroup(float dt)
{
	if (!Active)
	{
		return;
	}
	if (!CurrentGroup->Completed)
	{
		CurrentGroup->Update(dt);
	}
	else
	{
		Completed = true;
		Active = false;
		Started = false;
	}
}

void Action::UpdateDelay(float dt)
{
	if (!Active)
	{
		return;
	}

	TimePassed += dt;

	if (TimePassed >= Time)
	{
		TimePassed = Time;
		Completed = true;
		Active = false;
		Started = false;
	}
}


void Action::Pause()
{
	Active = false;
}

void Action::Resume()
{
	Active = true;
}

bool Action::IsActive()
{
	return Active;
}

bool Action::IsCompleted()
{
	return Completed;
}

bool Action::IsStarted()
{
	return Started;
}



ActionSequence* Action::Sequence(ActionSequence** seq)
{
	if (*seq == nullptr)
	{
		*seq = new ActionSequence();
	}
	else
	{
		(*seq)->Clear();
	}
	return *seq;
}


ActionSequence* Action::Sequence(Zilch::Any seq)
{
	Zilch::Handle* handle = (Zilch::Handle*) (seq.GetData());
	Zilch::PropertyDelegateTemplate* property = (Zilch::PropertyDelegateTemplate*) handle->Dereference();
	ExecutableState* state = ZILCH->GetDependencies();

	Zilch::Call call(property->Get, state);
	call.Invoke(ZILCH->Report);

	Zilch::Handle* sequence = (Zilch::Handle*) call.GetReturnUnchecked();
	ActionSequence* act = (ActionSequence*)sequence->Dereference();

	if (sequence->Dereference() == nullptr)
	{
		Zilch::LibraryRef* library = &(ZILCH->lib);
		Zilch::BoundType* ZilchClass = ZilchTypeId(ActionSequence);
		ErrorIf(ZilchClass == nullptr, "Failed to find a Zilch type named ", "ActionSequence");
		Zilch::Handle newSequence = state->AllocateDefaultConstructedHeapObject(ZilchClass, ZILCH->Report, Zilch::HeapFlags::NonReferenceCounted);

		Zilch::Call call2(property->Set, state);
		call2.SetHandle(0, newSequence);
		call2.Invoke(ZILCH->Report);
	}
	else
	{
		act->Clear();
	}

	if (sequence->Dereference() != nullptr)
	{
		ActionSequence* act = (ActionSequence*) sequence;
	}

	return act;
}

ActionSequence* Action::Sequence(IEntity* seq)
{



	if (seq->Actions == nullptr)
	{
		seq->Actions = new ActionSequence();
	}
	else
	{
		seq->Actions->Clear();
	}
	return seq->Actions;

	//typecast GetData() as ZilchHandle and then typescast handle.Dereference as ZilchPropertyDelegateTempelate.

	//##########################ZILCH TEST##############################
	//Zilch::BoundType* type = (BoundType*)seq.StoredType;
	//Zilch::PropertyDelegateTemplate* type2 = (PropertyDelegateTemplate*)seq.GetData();
	//
	//int i = 0;
	//int len = type->AllFunctions.size();
	//for (i = 0; i < len; ++i)
	//{
	//	std::cout << type->AllFunctions[i]->Name.c_str() << std::endl;
	//	std::cout << type->AllFunctions[i]->Type->ToString().c_str() << std::endl;
	//}
	//printf("\n");
	//int j = 0;
	//int len2 = type->AllProperties.size();
	//for (j = 0; j < len2; ++j)
	//{
	//	std::cout << type->AllProperties[j]->Name.c_str() << std::endl;
	//	std::cout << type->AllProperties[j]->PropertyType->ToString().c_str() << std::endl;
	//}

	//Zilch::Array<Zilch::Type*> args;
	//Zilch::Function* asd = type->FindFunction("[GetGet]", args, ZilchTypeId(void), Zilch::FindMemberOptions::None);

	////std::cout << type->BaseType->ToString().c_str() << std::endl;


	//type->HandleManager = ZilchManagerId(Zilch::PointerManager);

	//Zilch::Property* proper = (Zilch::Property*) type->AllProperties[0];
	//
	//
	//printf("\n");
	//int k = 0;
	//int len3 = 0;
	//for (k = 0; k < len3; ++k)
	//{
	//	std::cout << type->Attributes[k].Name.c_str() << std::endl;
	//}
	////std::cout << proper->Get->BoundFunction()->ToString() <<

	//Zilch::ExecutableState* state = ZILCH->GetDependencies();
	////seq.StoredType->
	//Zilch::Handle handle = Zilch::Handle(state, type, seq.GetData());
	//type2->Get.ThisHandle = handle;
	////type2->Get.BoundFunction->Name. = Zilch::String("Get");
	//type2->Get.BoundFunction->Type = (Zilch::DelegateType*) type->AllProperties[1]->PropertyType;

	//type2->Set.ThisHandle.Type = handle.Type;
	//type2->Set.ThisHandle.Offset = handle.Offset;
	//type2->Set.ThisHandle.Flags = handle.Flags;
	//type2->Set.ThisHandle.Manager = handle.Manager;
	////type2->Set.BoundFunction->Name = Zilch::String("Set");
	//type2->Set.BoundFunction->Type = (Zilch::DelegateType*) type->AllProperties[0]->PropertyType;
	//Zilch::HeapManager::
	//Zilch::Call call1(type2->Get.BoundFunction, state);
	//call1.Invoke(ZILCH->Report);
	


	//handle.Type->
	/*Zilch::HandleManagers& managers = HandleManagers::GetInstance();
	handle.Manager = managers.GetSharedManager(ZilchManagerId(PointerManager));
	handle.Type = (Zilch::Type) seq.StoredType;*/
	
	

	
	
	/*Zilch::LibraryRef* library = &(ZILCH->lib);
	Zilch::BoundType* ZilchClass = ZilchTypeId(ActionSequence);
	ErrorIf(ZilchClass == nullptr, "Failed to find a Zilch type named ", "ActionSequence");
	Zilch::Handle ActiveScript = state->AllocateDefaultConstructedHeapObject(ZilchClass, ZILCH->Report, Zilch::HeapFlags::NonReferenceCounted);
	
	
	args.push_back(ZilchTypeId(ActionSequence));*/
	
	/*Zilch::Variable var = Zilch::Variable();
	var.Name = "sequence";
	var.ResultType = ZilchTypeId(ActionSequence);
	ActionSequence* newSeq = new ActionSequence();*/
	//36
	//Zilch::Field* field1 = seq.StoredType->GetInstanceField("Set");
	//field1->Get->.append(&var);
	//std::string str = 
	//Zilch::Mana
	//field1->Get->Type->Parameters.push_back(ZilchTypeId(ActionSequence));
	//field1->Get->Type->Return;
	//Zilch::Handle hndl = Zilch::Handle(state, field1->Get->Owner, seq.GetData());
	//field1->Get->Owner->HandleManager = ZilchManagerId(Zilch::HandleManager);
	//Call call1(field1->Get, state);
	//call1.DisableThisChecks();
	//SIZEOF DOES NOT MATCH
	//NOT HANDLE TYPE
	//call1.SetHandle(Zilch::Call::This, handle);
	//call1.
	//call1.SetHandle(0, ActiveScript);
	//call1.Invoke(ZILCH->Report);
	//byte* data = call1.GetReturnUnchecked();
	//Zilch::BoundType* bnd = (Zilch::BoundType*)field1->Get->Type->Return;
	//data->
	//Zilch::Handle funcHand = Zilch::Handle(state, , data);
	//field1->Set->BoundFunction(call1, ZILCH->Report);
	//std::cout << field1->Get->Type->ToString().c_str() << std::endl;
	//
	//Zilch::Field* field = seq.StoredType->GetInstanceField("Get");
	//type2->Get.BoundFunction->Type = (Zilch::DelegateType*) field->Get->Type->Return;
	////type2->Get.ThisHandle.;
	//Call call(type2->Get.BoundFunction, state);
	//call.DisableThisChecks();
	////call.SetHandle(Zilch::Call::This, handle);
	//
	////call.Invoke(ZILCH->Report);
	//std::cout << field->Get->Type->ToString().c_str() << std::endl;
	//ActionSequence* deltype = (ActionSequence*) field->Get->Type->Return;

	//Zilch::Function* ad = seq.StoredType->FindFunction("Get", deltype);
	//Zilch::Function* funer = type->FindFunction("Get", deltype, Zilch::FindMemberOptions::None)
	/*Zilch::Delegate* jk;
	Zilch::Function* fun;
	fun->Attributes = deltype->Attributes;
	fun->Type->Return = deltype->Return;
	fun->Type->Attributes = deltype->Attributes*/;
	//jk->BoundFunction = deltype->
	//byte* sequence = call.GetChecked(field->Get->Type->Return->GetAllocatedSize(), field->Get->Type->, );

	//sequence->Clear();

	//PropertyDelegateTemplate* propertyDelegate;
	//propertyDelegate->Get.BoundFunction = seq.StoredType->
	//Zilch::TypeBinding::DynamicCast<MemberAccessNode*>(ActionSequence*)
	//Zilch::Array<Zilch::Type*> args;
	//args.push_back(ZilchTypeId(float));
	//Zilch::Field* prop = seq.StoredType->GetInstanceField("Get");
	//ActionSequence* i = nullptr;
	//Zilch::InstantiatedTemplate i;
	//i->
	//seq.
	//std::string i = field->PropertyType->ToString().c_str();
	//std::cout << i << std::endl;
	//Zilch::Delegate e;
	
	//Zilch::Property* Get = (Zilch::Property*) seq.;
	//ErrorIf(Get == nullptr, "Failed to find field 'Get' on Zilch type ", seq.StoredType);
	//Zilch::PropertyDelegateTemplate i;
	//Zilch::Function* func = Get->;
	//Get->Get(ZilchTypeId());
	
		// Invoke the Create function, which assigns this object an owner.
	/*Zilch::Call call(func, ZILCH->GetDependencies());
	call.SetHandle(Zilch::Call::This, Get->);
	call.Invoke(ZILCH->Report);*/
	//PropertyDelegateOperatorNode i;
	return nullptr;
}

Action::~Action()
{
	delete Curve;
	Curve = nullptr;
	CurrentVal = nullptr;
}

void Action::Delay(ActionSequence* seq, float delay)
{
	seq->AddProperty(new ActionProperty(delay));
}

void Action::Delay(ActionGroup* grp, float delay)
{
	grp->AddProperty(new ActionProperty(delay));
}

template<typename T>
void Action::Call(ActionSequence* seq, T function)
{
	seq->AddProperty(new ActionProperty(func));
}

template<typename T>
void Action::Call(ActionGroup* grp, T function)
{
	grp->AddProperty(new ActionProperty(func));
}

void Action::Call(ActionSequence* seq, Zilch::Any func)
{
	//DelegateType* type = (DelegateType*)func.GetData();
	//Zilch::Handle handle = Zilch::Handle(ZILCH->GetDependencies(), (BoundType*)func.StoredType, func.GetData());
	
	
	//Zilch::Call call(hand, ZILCH->GetDependencies());


	return;
}

void Action::Property(ActionSequence* seq, float* startVal, float endVal, float time, int ease)
{
	seq->AddProperty(new ActionProperty(startVal, endVal, time, Ease::Eases(ease)));
}
void Action::Property(ActionSequence* seq, int* startVal, int endVal, float time, int ease)
{
	seq->AddProperty(new ActionProperty(startVal, endVal, time, Ease::Eases(ease)));
}
void Action::Property(ActionSequence* seq, Vector2D* startVal, Vector2D endVal, float time, int ease)
{
	seq->AddProperty(new ActionProperty(startVal, &endVal, time, Ease::Eases(ease)));
}
void Action::Property(ActionSequence* seq, Vector3D* startVal, Vector3D endVal, float time, int ease)
{
	seq->AddProperty(new ActionProperty(startVal, &endVal, time, Ease::Eases(ease)));
}
void Action::Property(ActionSequence* seq, Vector4D* startVal, Vector4D endVal, float time, int ease)
{
	seq->AddProperty(new ActionProperty(startVal, &endVal, time, Ease::Eases(ease)));
}

void Action::Property(ActionGroup* grp, float* startVal, float endVal, float time, int ease)
{
	grp->AddProperty(new ActionProperty(startVal, endVal, time, Ease::Eases(ease)));
}
void Action::Property(ActionGroup* grp, int* startVal, int endVal, float time, int ease)
{
	grp->AddProperty(new ActionProperty(startVal, endVal, time, Ease::Eases(ease)));
}
void Action::Property(ActionGroup* grp, Vector2D* startVal, Vector2D endVal, float time, int ease)
{
	grp->AddProperty(new ActionProperty(startVal, &endVal, time, Ease::Eases(ease)));
}
void Action::Property(ActionGroup* grp, Vector3D* startVal, Vector3D endVal, float time, int ease)
{
	grp->AddProperty(new ActionProperty(startVal, &endVal, time, Ease::Eases(ease)));
}
void Action::Property(ActionGroup* grp, Vector4D* startVal, Vector4D endVal, float time, int ease)
{
	grp->AddProperty(new ActionProperty(startVal, &endVal, time, Ease::Eases(ease)));
}

ActionGroup* Action::Group(ActionSequence* seq)
{
	seq->AddProperty(new ActionProperty(0.0f));
	ActionGroup* grp = new ActionGroup();
	ActionProperty* prop = new ActionProperty(grp);
	seq->AddProperty(prop);

	return grp;
}

//###############################################################################################//
//ActionGroup
//###############################################################################################//

void ActionGroup::AddProperty(ActionProperty* action)
{
	ActionProps.push_back(action);
}

void ActionGroup::SetValues()
{
	unsigned size = ActionProps.size();
	Actions.resize(size);

	for (unsigned i = 0; i < size; ++i)
	{
		Actions[i] = new Action();
		(ActionProps[i]->*(ActionProps[i]->SetValues))(Actions[i]);
		delete ActionProps[i];
		ActionProps[i] = nullptr;
	}

	ActionProps.resize(0);
	ActionProps.shrink_to_fit();
}

void ActionGroup::Update(float dt)
{
	if (!Active)
	{
		return;
	}
	int size = Actions.size();
	for (int i = 0; i < size; ++i)
	{
		if (Actions[i] == nullptr)
		{
			continue;
		}
		(Actions[i]->*(Actions[i]->Update))(dt);
		if (Actions[i]->IsCompleted())
		{
			++CompletedCount;
			delete Actions[i];
			Actions[i] = nullptr;
		}
	}

	if (CompletedCount == size)
	{
		Completed = true;
		Active = false;
	}
}

void ActionGroup::Pause()
{
	Active = false;
}

void ActionGroup::Resume()
{
	Active = true;
}

bool ActionGroup::IsPaused()
{
	return Active;
}

void ActionGroup::Clear()
{
	for (auto i : Actions)
	{
		delete i;
		i = nullptr;
	}

	unsigned size = ActionProps.size();
	for (unsigned i = 0; i < size; ++i)
	{
		delete ActionProps[i];
		ActionProps[i] = nullptr;
	}

	ActionProps.resize(0);
	ActionProps.shrink_to_fit();
	Actions.resize(0);
	Actions.shrink_to_fit();
}


//###############################################################################################//
//ActionProperty
//###############################################################################################//
ActionProperty::ActionProperty(float time)
{
	StartValue = nullptr;
	EndValue = Vector4D(0, 0, 0, 0);
	Time = time;
	SetValues = &ActionProperty::ValueSetterDelay;
}

template<typename T>
ActionProperty::ActionProperty(T function)
{
	StartValue = function;
	EndValue = Vector4D(0, 0, 0, 0);
	
	SetValues = &ActionProperty::ValueSetterCall;
}

ActionProperty::ActionProperty(float* startVal, float endVal, float time, Ease::Eases ease)
{
	StartValue = startVal;
	EndValue.x = endVal;
	Time = time;
	EaseType = ease;
	SetValues = &ActionProperty::ValueSetterFloat;
}
ActionProperty::ActionProperty(int* startVal, int endVal, float time, Ease::Eases ease)
{
	StartValue = startVal;
	EndValue.x = endVal;
	Time = time;
	EaseType = ease;
	SetValues = &ActionProperty::ValueSetterInt;
}
ActionProperty::ActionProperty(Vector2D* startVal, Vector2D* endVal, float time, Ease::Eases ease)
{
	StartValue = startVal;
	EndValue.x = endVal->x;
	EndValue.y = endVal->y;
	Time = time;
	EaseType = ease;
	SetValues = &ActionProperty::ValueSetterVec2;
}
ActionProperty::ActionProperty(Vector3D* startVal, Vector3D* endVal, float time, Ease::Eases ease)
{
	StartValue = startVal;
	EndValue.x = endVal->x;
	EndValue.y = endVal->y;
	EndValue.z = endVal->z;
	Time = time;
	EaseType = ease;
	SetValues = &ActionProperty::ValueSetterVec3;
}
ActionProperty::ActionProperty(Vector4D* startVal, Vector4D* endVal, float time, Ease::Eases ease)
{
	StartValue = startVal;
	EndValue = *endVal;
	Time = time;
	EaseType = ease;
	SetValues = &ActionProperty::ValueSetterVec4;
}

ActionProperty::ActionProperty(ActionGroup* grp)
{
	Group = grp;
	SetValues = &ActionProperty::ValueSetterGroup;
}

ActionProperty::~ActionProperty()
{
	if (Group != nullptr && Group->Active != true)
	{
		Group->Clear();
	}
}

void ActionProperty::ValueSetterDelay(Action* action)
{
	action->SetValues(Time);
}
void ActionProperty::ValueSetterCall(Action* action)
{
	action->SetValues(StartValue);
}
void ActionProperty::ValueSetterFloat(Action* action)
{
	float* startVal = reinterpret_cast<float*>(StartValue);
	action->SetValues(*startVal, startVal, EndValue.x, Time, EaseType);
}
void ActionProperty::ValueSetterInt(Action* action)
{
	int* startVal = reinterpret_cast<int*>(StartValue);
	action->SetValues(*startVal, startVal, EndValue.x, Time, EaseType);
}
void ActionProperty::ValueSetterVec2(Action* action)
{
	Vector2D* startVal = reinterpret_cast<Vector2D*>(StartValue);
	action->SetValues(startVal, startVal, &Vector2D(EndValue.x, EndValue.y), Time, EaseType);
}
void ActionProperty::ValueSetterVec3(Action* action)
{
	Vector3D* startVal = reinterpret_cast<Vector3D*>(StartValue);
	action->SetValues(startVal, startVal, &Vector3D(EndValue.x, EndValue.y, EndValue.z), Time, EaseType);
}
void ActionProperty::ValueSetterVec4(Action* action)
{
	Vector4D* startVal = reinterpret_cast<Vector4D*>(StartValue);
	action->SetValues(startVal, startVal, &EndValue, Time, EaseType);
}

void ActionProperty::ValueSetterGroup(Action* action)
{
	action->SetValues(Group);
}
