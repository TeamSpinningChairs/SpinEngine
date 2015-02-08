#include "Precompiled.h"
#include "Action.h"
#include "ActionSequence.h"


ZilchDefineType(Action, SpinningZilch)
{
	type->HandleManager = ZilchManagerId(Zilch::PointerManager);
	ZilchBindStaticMethodOverload(Sequence, ActionSequence*, Zilch::Any);
	//ZilchBindStaticMethodOverload(Property, void, ActionSequence* seq, Zilch::Property startVal, float endVal, float time, int ease);
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
	time = time;
	TimePassed = 0;
	StartVal = Vector4D(0, 0, 0, 0);
	CurrentVal = nullptr;
	EndVal = Vector4D(0, 0, 0, 0);
	Active = true;
	Started = true;
	Completed = false;
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

void Action::Delay(ActionSequence* seq, float delay)
{
	seq->AddProperty(new ActionProperty(delay));
}

void Action::Delay(ActionGroup* grp, float delay)
{
	grp->AddProperty(new ActionProperty(delay));
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
	Zilch::BoundType* type = (BoundType*)seq.StoredType;
	type->HandleManager = ZilchManagerId(Zilch::PointerManager);
	Zilch::ExecutableState* state = ZILCH->GetDependencies();
	//seq.StoredType->
	Zilch::Handle handle = Zilch::Handle(state, type, seq.GetData());

	//Zilch::HandleManagers& managers = HandleManagers::GetInstance();
	//handle.Manager = managers.GetSharedManager(ZilchManagerId(PointerManager));
	//handle.Type = (Zilch::Type) seq.StoredType;
	
	

	
	
	Zilch::LibraryRef* library = &(ZILCH->lib);
	Zilch::BoundType* ZilchClass = ZilchTypeId(ActionSequence);
	ErrorIf(ZilchClass == nullptr, "Failed to find a Zilch type named ", "ActionSequence");
	Zilch::Handle ActiveScript = state->AllocateDefaultConstructedHeapObject(ZilchClass, ZILCH->Report, Zilch::HeapFlags::NonReferenceCounted);
	
	

	//ActionSequence* newSeq = new ActionSequence();
	////36
	//Zilch::Field* field1 = seq.StoredType->GetInstanceField("Set");
	////std::string str = 
	//Call call1(field1->Set, state);
	////SIZEOF DOES NOT MATCH
	////NOT HANDLE TYPE
	//call1.SetHandle(Zilch::Call::This, handle);
	//call1.SetHandle(0, ActiveScript);
	////call1.Invoke(ZILCH->Report);
	//field1->Set->BoundFunction(call1, ZILCH->Report);

	Zilch::Field* field = seq.StoredType->GetInstanceField("Get");
	Call call(field->Get, state);
	call.SetHandle(Zilch::Call::This, handle);
	call.Invoke(ZILCH->Report);
	
	ActionSequence* seqence = (ActionSequence*)call.GetReturnUnchecked();
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
	//i.
	
	
	
	//ActionSequence* sequence = (ActionSequence*) call.GetReturnUnchecked();
	/*if (*seq == nullptr)
	{
		*seq = new ActionSequence();
	}
	else
	{
		(*seq)->Clear();
	}*/
	return nullptr;
}

Action::~Action()
{
	delete Curve;
	Curve = nullptr;
	CurrentVal = nullptr;
}


void Action::Property(ActionSequence* seq, float* startVal, float endVal, float time, Ease::Eases ease)
{
	seq->AddProperty(new ActionProperty(startVal, endVal, time, ease));
}
void Action::Property(ActionSequence* seq, int* startVal, int endVal, float time, Ease::Eases ease)
{
	seq->AddProperty(new ActionProperty(startVal, endVal, time, ease));
}
void Action::Property(ActionSequence* seq, Vector2D* startVal, Vector2D endVal, float time, Ease::Eases ease)
{
	seq->AddProperty(new ActionProperty(startVal, &endVal, time, ease));
}
void Action::Property(ActionSequence* seq, Vector3D* startVal, Vector3D endVal, float time, Ease::Eases ease)
{
	seq->AddProperty(new ActionProperty(startVal, &endVal, time, ease));
}
void Action::Property(ActionSequence* seq, Vector4D* startVal, Vector4D endVal, float time, Ease::Eases ease)
{
	seq->AddProperty(new ActionProperty(startVal, &endVal, time, ease));
}

void Action::Property(ActionGroup* grp, float* startVal, float endVal, float time, Ease::Eases ease)
{
	grp->AddProperty(new ActionProperty(startVal, endVal, time, ease));
}
void Action::Property(ActionGroup* grp, int* startVal, int endVal, float time, Ease::Eases ease)
{
	grp->AddProperty(new ActionProperty(startVal, endVal, time, ease));
}
void Action::Property(ActionGroup* grp, Vector2D* startVal, Vector2D endVal, float time, Ease::Eases ease)
{
	grp->AddProperty(new ActionProperty(startVal, &endVal, time, ease));
}
void Action::Property(ActionGroup* grp, Vector3D* startVal, Vector3D endVal, float time, Ease::Eases ease)
{
	grp->AddProperty(new ActionProperty(startVal, &endVal, time, ease));
}
void Action::Property(ActionGroup* grp, Vector4D* startVal, Vector4D endVal, float time, Ease::Eases ease)
{
	grp->AddProperty(new ActionProperty(startVal, &endVal, time, ease));
}

ActionGroup* Action::Group(ActionSequence* seq)
{
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
