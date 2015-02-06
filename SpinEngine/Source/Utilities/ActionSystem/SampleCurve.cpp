#include "Precompiled.h"
#include "SampleCurve.h"


ZilchDefineType(SampleCurve, SpinningZilch)
{

}

SampleCurve::SampleCurve(Ease::Eases ease)
{
	switch (ease)
	{
		//case Ease::CircIn:
		//{
		//	//Not yet implemented.
		//	
		//}break;
		//case Ease::CircInOut:
		//{
		//	//Not yet implemented.
		//	;
		//}break;
		//case Ease::CircOut:
		//{

		//}break;
		case Ease::CubicIn:
		{
			Sample = &MathF::CubicIn;
		}break;
		case Ease::CubicInOut:
		{
			Sample = &MathF::CubicInOut;
		}break;
		case Ease::CubicOut:
		{
			Sample = &MathF::CubicOut;
		}break;
		/*case Ease::ExpoIn:
		{
			
		}break;
		case Ease::ExpoInOut:
		{

		}break;
		case Ease::ExpoOut:
		{

		}break;*/
		case Ease::Linear:
		{
			Sample = &MathF::Linear;
		}break;
		case Ease::QntIn:
		{
			Sample = &MathF::QuinticIn;
		}break;
		case Ease::QntInOut:
		{
			Sample = &MathF::QuinticInOut;
		}break;
		case Ease::QntOut:
		{
			Sample = &MathF::QuinticOut;
		}break;
		case Ease::QuadIn:
		{
			Sample = &MathF::QuadIn;
		}break;
		case Ease::QuadInOut:
		{
			Sample = &MathF::QuadInOut;
		}break;
		case Ease::QuadOut:
		{
			Sample = &MathF::QuadOut;
		}break;
		case Ease::QuarticIn:
		{
			Sample = &MathF::QuarticIn;
		}break;
		case Ease::QuarticInOut:
		{
			Sample = &MathF::QuarticInOut;
		}break;
		case Ease::QuarticOut:
		{
			Sample = &MathF::QuarticOut;
		}break;
		default:
		{
			//No ease specified. Using linear.
			std::cout << "This ease is not yet implemented. Using Linear as default." << std::endl;
			Sample = &MathF::Linear;
		}
	};
}