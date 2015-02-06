#include "Precompiled.h"
#include "Action.h"
#include "ActionSequence.h"


ZilchDefineType(Action, SpinningZilch)
{
	type->HandleManager = type->HandleManager = ZilchManagerId(Zilch::PointerManager);
	ZilchBindStaticMethodOverload(Sequence, ActionSequence*, Zilch::Any);
	//ZilchBindStaticMethodOverload(Property, void, ActionSequence* seq, Zilch::Any startVal, float endVal, float time, int ease);
}

ZilchDefineType(ActionGroup, SpinningZilch)
{
	type->HandleManager = type->HandleManager = ZilchManagerId(Zilch::PointerManager);
}

ZilchDefineType(ActionProperty, SpinningZilch)
{
	type->HandleManager = type->HandleManager = ZilchManagerId(Zilch::PointerManager);
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
	std::cout << seq.StoredType->ToString().c_str() << std::endl;
	byte* prop = nullptr;
	//seq.CopyStoredValueTo    
	//Zilch::Property i = static_cast<Zilch::Property>(*prop);
	//ActionSequence* sequence = reinterpret_cast<ActionSequence*> (i->*(i->Get)());
	//std::cout << (prop == nullptr) << std::endl;
	//seq.CopyStoredValueTo(&i)
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

void Action::Property(ActionSequence* seq, Zilch::Any startVal, float endVal, float time, int ease)
{
	std::cout << startVal.StoredType->ToString().c_str() << std::endl;
	//seq->AddProperty(new ActionProperty(startVal, endVal, time, ease));
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
