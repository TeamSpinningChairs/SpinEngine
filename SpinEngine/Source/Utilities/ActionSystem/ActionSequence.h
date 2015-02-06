#pragma once
#include "Precompiled.h"
#include "Action.h"

class ActionSequence
{
	public:
		ZilchDeclareBaseType(ActionSequence, Zilch::TypeCopyMode::ReferenceType);
		ActionSequence(void);
		~ActionSequence(void);
		void AddProperty(ActionProperty* actionProp);
		void Update(float dt);
		
		bool Active = false;
		
		void Pause();
		void Resume();
		bool IsPaused();

		void Clear();

private:
		Action* MainAction;

		
		

		int ActionIndex = 0;
		int LastAction = 0;

		std::unordered_map<int, ActionProperty*> QeuedActions;
};
