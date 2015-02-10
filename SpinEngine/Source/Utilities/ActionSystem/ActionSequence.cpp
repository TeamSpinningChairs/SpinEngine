#include "Precompiled.h"
#include "ActionSequence.h"

ZilchDefineType(ActionSequence, SpinningZilch)
{
	type->HandleManager = ZilchManagerId(Zilch::PointerManager);
	ZilchBindConstructor();
	ZilchBindDestructor();
}

ActionSequence::ActionSequence(void)
{
	MainAction = new Action();
}

ActionSequence::~ActionSequence(void)
{
	Clear();
	delete MainAction;
}

void ActionSequence::AddProperty(ActionProperty* actionProp)
{
	if (!Active)
	{
		(actionProp->*(actionProp->SetValues))(MainAction);
		Active = true;
		return;
	}
	QeuedActions[ActionIndex] = actionProp;
	++ActionIndex;
}

void ActionSequence::Update(float dt)
{
	if (!Active)
	{
		return;
	}
	if (MainAction->IsCompleted())
	{
		if (LastAction == ActionIndex)
		{
			Active = false;
			return;
		}
		(QeuedActions[LastAction]->*(QeuedActions[LastAction]->SetValues))(MainAction);
		delete QeuedActions[LastAction];
		QeuedActions[LastAction] = nullptr;
		QeuedActions.erase(LastAction);
		++LastAction;
	}

	(MainAction->*(MainAction->Update))(dt);
}

void ActionSequence::Pause()
{
	Active = false;
}

void ActionSequence::Resume()
{
	Active = true;
}

bool ActionSequence::IsPaused()
{
	return Active;
}

void ActionSequence::Clear()
{
	for (unsigned i = 0; i < QeuedActions.size() - 1; ++i)
	{
		if (QeuedActions[i] != nullptr)
		{
			delete QeuedActions[i];
			QeuedActions[i] = nullptr;
			QeuedActions.erase(i);
		}
	}
	LastAction = 0;
	ActionIndex = 0;

	
}

