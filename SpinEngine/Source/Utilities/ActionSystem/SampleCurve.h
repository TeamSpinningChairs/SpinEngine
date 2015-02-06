/****************************************************************************/
/*!
\author Joshua Biggs
\par    email: Joshua.B@Digipen.edu
\par    Course: GAM 255
\brief

Contains A sample curve used to add eases to an Action/Interpolation.

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/

#pragma once
#include "Precompiled.h"

class SampleCurve
{
	typedef float(*SampleFunc)(float currentTime, float startValue, float endValue, float duration);
	public:
		ZilchDeclareBaseType(SampleCurve, Zilch::TypeCopyMode::ReferenceType);
		SampleCurve(Ease::Eases ease);
		SampleFunc Sample = nullptr;
};
