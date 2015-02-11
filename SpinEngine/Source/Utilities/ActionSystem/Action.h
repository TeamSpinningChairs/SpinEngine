/****************************************************************************/
/*!
\author Joshua Biggs
\par    email: Joshua.B@Digipen.edu
\par    Course: GAM 255
\brief

Contains An action system that allows the user to interpolate between two values.

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#pragma once
#include"Precompiled.h"
#include "SampleCurve.h"
#include "ZilchCompiledLib.h"
class ActionSequence;
class ActionProperty;
class ActionGroup;

class Action
{	

	typedef void (Action::*UpdateFunc)(float dt);
	
	public:
		ZilchDeclareBaseType(Action, Zilch::TypeCopyMode::ReferenceType);
		//DO NOT CALL THIS DEFAULT CONSTRUCTOR.
		Action();
		Action(float time);
		template<typename T>
		Action(T function); //Void Function Pointer
		Action(float* startVal, float endVal, float time, Ease::Eases ease);
		Action(int* startVal, int endVal, float time, Ease::Eases ease);
		Action(Vector2D* startVal, Vector2D endVal, float time, Ease::Eases ease);
		Action(Vector3D* startVal, Vector3D endVal, float time, Ease::Eases ease);
		Action(Vector4D* startVal, Vector4D endVal, float time, Ease::Eases ease);
		~Action();
		
		//These functions reset the values in the action so that it can be used multiple times.
		void SetValues(float time);
		template<typename T>
		void SetValues(T function); //Void Function Pointer
		void SetValues(float startVal, float* currentVal, float endVal, float time, Ease::Eases ease);
		void SetValues(int startVal, int* currentVal, int endVal, float time, Ease::Eases ease);
		void SetValues(Vector2D* startVal, Vector2D* currentVal, Vector2D* endVal, float time, Ease::Eases ease);
		void SetValues(Vector3D* startVal, Vector3D* currentVal, Vector3D* endVal, float time, Ease::Eases ease);
		void SetValues(Vector4D* startVal, Vector4D* currentVal, Vector4D* endVal, float time, Ease::Eases ease);

		void SetValues(ActionGroup* grp);

		//These functions control the interpolation of values.
		void UpdateFloat(float dt);
		void UpdateInt(float dt);
		void UpdateVec2(float dt);
		void UpdateVec3(float dt);
		void UpdateVec4(float dt);
		void UpdateDelay(float dt);

		void UpdateGroup(float dt);

		//Generates a new action sequence at the given memory location. If a sequence already exists,
		//then the exisisting sequence is cleared.
		static ActionSequence* Sequence(ActionSequence** seq);
		static ActionSequence* Sequence(IEntity* seq);
		static ActionSequence* Sequence(Zilch::Any seq);

		static void Property(ActionSequence* seq, float* startVal, float endVal, float time, int ease);
		static void Property(ActionSequence* seq, int* startVal, int endVal, float time, int ease);
		static void Property(ActionSequence* seq, Vector2D* startVal, Vector2D endVal, float time, int ease);
		static void Property(ActionSequence* seq, Vector3D* startVal, Vector3D endVal, float time, int ease);
		static void Property(ActionSequence* seq, Vector4D* startVal, Vector4D endVal, float time, int ease);

		static void Property(ActionGroup* grp, float* startVal, float endVal, float time, int ease);
		static void Property(ActionGroup* grp, int* startVal, int endVal, float time, int ease);
		static void Property(ActionGroup* grp, Vector2D* startVal, Vector2D endVal, float time, int ease);
		static void Property(ActionGroup* grp, Vector3D* startVal, Vector3D endVal, float time, int ease);
		static void Property(ActionGroup* grp, Vector4D* startVal, Vector4D endVal, float time, int ease);

		//ZILCH VERSIONS
		/*static void Property(ActionSequence* seq, Zilch::Property startVal, float endVal, float time, int ease);
		static void Property(ActionSequence* seq, Zilch::Property startVal, int endVal, float time, int ease);
		static void Property(ActionSequence* seq, Zilch::Property startVal, Vector2D endVal, float time, int ease);
		static void Property(ActionSequence* seq, Zilch::Property startVal, Vector3D endVal, float time, int ease);
		static void Property(ActionSequence* seq, Zilch::Property startVal, Vector4D endVal, float time, int ease);*/

		static void Delay(ActionSequence* seq, float delay);
		template<typename T>
		static void Call(ActionSequence* seq, T function);
		static void Call(ActionSequence* seq, Zilch::Any);

		static void Delay(ActionGroup* grp, float delay);
		template<typename T>
		static void Call(ActionGroup* grp, T function);

		static ActionGroup* Group(ActionSequence* seq);

		void Pause();
		void Resume();
		bool IsActive();
		bool IsCompleted();
		bool IsStarted();

		UpdateFunc Update = nullptr;
	private:
		void* CurrentVal;

		bool Active = true;
		bool Completed = false;
		bool Started = false;

		Vector4D StartVal = Vector4D(0, 0, 0, 0);
		Vector4D EndVal = Vector4D(0, 0, 0, 0);

		ActionGroup* CurrentGroup = nullptr;

		float Time = 0;
		float TimePassed = 0;
		SampleCurve* Curve = nullptr;

		//Possible Optimizations:
			//Don't store the values in the SetValues function.
		
		
};

class ActionGroup
{
	public:
		ZilchDeclareBaseType(ActionGroup, Zilch::TypeCopyMode::ReferenceType);
		void AddProperty(ActionProperty* action);
		
		void Update(float dt);

		void SetValues();

		void Clear();
		void Pause();
		void Resume();
		bool IsPaused();

		bool Active = true;
		bool Completed = false;
	private:
		int CompletedCount = 0;
		std::vector<ActionProperty*> ActionProps = std::vector<ActionProperty*>(0);
		std::vector<Action*> Actions = std::vector<Action*>(0);
};

//A temporary property of values to be passed into the action class.
class ActionProperty
{
	typedef void (ActionProperty::*SetValuesFunc)(Action* action);
	public:
		ZilchDeclareBaseType(ActionProperty, Zilch::TypeCopyMode::ReferenceType);
		ActionProperty(ActionGroup* grp);
		ActionProperty(float time);
		template<typename T>
		ActionProperty(T function); //Void Function Pointer
		ActionProperty(float* startVal, float endVal, float time, Ease::Eases ease);
		ActionProperty(int* startVal, int endVal, float time, Ease::Eases ease);
		ActionProperty(Vector2D* startVal, Vector2D* endVal, float time, Ease::Eases ease);
		ActionProperty(Vector3D* startVal, Vector3D* endVal, float time, Ease::Eases ease);
		ActionProperty(Vector4D* startVal, Vector4D* endVal, float time, Ease::Eases ease);

		
		~ActionProperty();
		
		SetValuesFunc SetValues;
		
		ActionGroup* Group = nullptr;
	private:
		void* StartValue;
		Vector4D EndValue = Vector4D(0,0,0,0);
		float Time = 0;
		Ease::Eases EaseType = Ease::Eases::Linear;

		void ValueSetterFloat(Action* action);
		void ValueSetterInt(Action* action);
		void ValueSetterVec2(Action* action);
		void ValueSetterVec3(Action* action);
		void ValueSetterVec4(Action* action);
		
		void ValueSetterDelay(Action* action);
		void ValueSetterCall(Action* action); //Void Function Pointer
		void ValueSetterGroup(Action* action);

		
		
};
