/****************************************************************************/
/*!
\author Judy Cheng
\par    email: j.cheng\@digipen.edu (Optional)
\par    Course: GAM 200
\date   xx/xx/xx (Optional)
\brief

THe math library that contains functions for vectors, matrices, and basic
math calculations.

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#include "Precompiled.h"
#include "mathlib.h"

const float MathF::nots[13] = { 0, 1.f, 2.f, 6.f, 24.f, 120.f, 720.f, 5040.f, 40320.f, 362880.f, 3628800.f, 39916800.f, 479001600.f };
const double MathF::predivnots[13] = { 0, 1.0, 0.5, 0.16666666666666666666666666666667,
0.04166666666666666666666666666667, 0.00833333333333333333333333333333, 0.00138888888888888888888888888889,
.0001984126984126984126984126984127, .000024801587301587301587301587301587,
.0000027557319223985890652557319223986, .00000027557319223985890652557319223986,
.000000025052108385441718775052108385442, .0000000020876756987868098979210090321201 };
const float MathF::predivs[11] = { 0, 1.f, 1.f / 2.f, 1.f / 3.f, 1.f / 4.f, 1.f / 5.f, 1.f / 6.f, 1.f / 7.f, 1.f / 8.f, 1.f / 9.f, 1.f / 10.f };

ZilchDefineType(Vector2D, SpinningZilch)
{
	type->HandleManager = ZilchManagerId(Zilch::PointerManager);
	ZilchBindConstructor();
	ZilchBindVirtualConstructor(float, float);
	ZilchBindVirtualConstructor(Zilch::Real2);
	ZilchBindDestructor();
	ZilchBindFieldGetSetAs(x, "X");
	ZilchBindFieldGetSetAs(y, "Y");
}

ZilchDefineType(Vector3D, SpinningZilch)
{
	type->HandleManager = ZilchManagerId(Zilch::PointerManager);
	ZilchBindConstructor();
	ZilchBindVirtualConstructor(float, float, float);
	ZilchBindVirtualConstructor(Zilch::Real3);
	ZilchBindDestructor();
	ZilchBindFieldGetSetAs(x, "X");
	ZilchBindFieldGetSetAs(y, "Y");
	ZilchBindFieldGetSetAs(y, "Z");
}

ZilchDefineType(Vector4D, SpinningZilch)
{
	type->HandleManager = ZilchManagerId(Zilch::PointerManager);
	ZilchBindConstructor();
	ZilchBindVirtualConstructor(float, float, float, float);
	ZilchBindVirtualConstructor(Zilch::Real4);
	ZilchBindDestructor();
	ZilchBindFieldGetSetAs(x, "X");
	ZilchBindFieldGetSetAs(y, "Y");
	ZilchBindFieldGetSetAs(y, "Z");
	ZilchBindFieldGetSetAs(y, "W");

	ZilchBindFieldGetSetAs(x, "R");
	ZilchBindFieldGetSetAs(y, "G");
	ZilchBindFieldGetSetAs(y, "B");
	ZilchBindFieldGetSetAs(y, "A");
}


//Interpolates between min and max by time. time is clamped between 0 and 1.
//Set time to 0.5 to get the average of min and max.
float MathF::Lerp(float from, float to, float time)
{
  if (time > 1.f)
    time = 1.f;
  if (time < 0)
    time = 0;
  return (to - from) * time;
}
//Returns the interpolation value required to get a value when interpolating between A and B.
float MathF::InverseLerp(float from, float to, float value)
{
  if (FloatsAreEqual(from, to))
    return 0;
  if (value > to)
    value = to;
  if (value < from)
    value = from;

  return (value - from) / (to - from); //pretty sure there's a more optimized way to get it
}
//Like Lerp, but with a guaranteed max change. Negative delta values will move away from target.
//I.E. position.x = MoveTowards(startPos,endPos,currentDistanceToEndPos * dt)
//will move towards target, slowing down the closer it gets.
//Can be used for primitive tweening
float MathF::MoveTowards(float current, float target, float maxDelta)
{
  current += maxDelta;
  if (current > target)
    current = target;
  return current;
}

template <typename T>
T MathF::Max(T a, T b)
{
  if (a < b)
    return b;
  return a;
}
template <typename T>
T MathF::Min(T a, T b)
{
  if (a < b)
    return a;
  return b;
}

template <typename T>
T MathF::Clamp(T value, T min, T max)
{
  if (value < min)
    value = min;
  else if (!(value < max)) //we're only ever gonna use this with default types, do I really need to only use guaranteed operators?
    value = max;
  return value;
}

float MathF::Clamp01(float value)
{
  if (value < 0)
    value = 0;
  else if (value > 1.f) //we're only ever gonna use this with default types, do I really need to only use guaranteed operators?
    value = 1.f;
  return value;
}

//100 * FLT_EPSILON is approx. 1e-5f
const float MathF::eps = 100 * FLT_EPSILON;

//Still not *completely* guaranteed.
bool MathF::FloatsAreEqual(float a, float b)
{
  //return Abs(a - b) < 1e-5f; Simpler, but not guaranteed.

  //More guaranteed. (We're casting a wider net, but it's still VERY small)
  return abs(a - b) < eps * (1 + Max(abs(a), abs(b)));
}

float MathF::RadToDegree(float radians)
{
  return (radians * 180.f) / PI;
}
float MathF::DegreeToRad(float degrees)
{
  return (degrees * PI) / 180.f;
}




//Uses Taylor series' to get within .0074 for values -3.14 to 3.14
//Within .005 for -3.03 to 3.03
//Inputs are clamped from -3.14 to 3.14
float MathF::ApproxSin(float radAngle)
{
  //if (abs(radAngle) > PI)
    //radAngle = Clamp(radAngle, -PI, PI);

 // bool useFullSeries = false;
  //if (abs(radAngle) > 3.03f)
   // useFullSeries = true;

  //Get the values to multiply by
  //float x2 = radAngle * radAngle;
 // float x3 = x2 * radAngle;
 // float x5 = x3 * x2;
 // float x7 = x5 * x2;
  //float x9 = x7 * x2;
  //float x11;
  //if(useFullSeries)
  //  x11 = x9 * x2;

  return radAngle - (radAngle * radAngle * radAngle * predivnots[3]) + (radAngle * radAngle * radAngle * radAngle * radAngle * predivnots[5])
    - (radAngle * radAngle * radAngle * radAngle * radAngle * radAngle * radAngle * predivnots[7]);// + (x9 * predivnots[9]);// -(x11 * predivnots[11]);
  //result -= x3 * predivnots[3];
  //result += x5 * predivnots[5];
  //result -= x7 * predivnots[7];
  //result += x9 * predivnots[9];
  ////if (useFullSeries)
  //  result -= x11 * predivnots[11];

}
//Within .005 for all -3.14 to 3.14
//Within .005 from -2.666 to 2.666
float MathF::ApproxCos(float radAngle)
{
  //if (abs(radAngle) > PI)
   // radAngle = Clamp(radAngle, -PI, PI);

 // bool useFullSeries = false;
  //if (abs(radAngle) > 2.66f)
  //  useFullSeries = true;

  //Get the values to multiply by
  //float x2 = radAngle * radAngle;
  //float x4 = x2 * x2;
 // float x6 = x4 * x2;
 // float x8 = x6 * x2;
  //float x10 = x8 * x2;
 // float x12;
 // if (useFullSeries)
 //   x12 = x10 * x2;
  //This is seriously the fastest way?
  //I guess I thought it'd take more time to multiply large floats.
  return 1 - radAngle * radAngle * predivnots[2]
    + radAngle * radAngle * radAngle * radAngle * predivnots[4]
    - radAngle * radAngle * radAngle * radAngle * radAngle * radAngle * predivnots[6]
    + radAngle * radAngle * radAngle * radAngle * radAngle * radAngle * radAngle * radAngle * predivnots[8];
  //result -= x10 * predivnots[10];
  //if (useFullSeries)
   // result += x12 * predivnots[12];

  //return result;
}

//Accurate to within .005 for values below .768
//Accurate to within .01 for values between .768 and 1.0
//For all other values, does not work, so it just calls atan
float MathF::ApproxATan(float val)
{
 // if (abs(val) > 1.0f)
   // return atan(val);
  //~12150
  //Get the values to multiply by
  /*float x2 = val * val;
  float x3 = x2 * val;
  float x5 = x3 * x2;
  float x7 = x5 * x2;
  float x9 = x7 * x2;
  float x11 = x9 * x2;*/

  return val - val * val * val * predivs[3]
    + val * val * val * val * val * predivs[5]
    - val * val * val * val * val * val * val * predivs[7]
    + val * val * val * val * val * val * val * val * val * predivs[9]
    - val * val * val * val * val * val * val * val * val * val * val * predivs[11];
  /*result += x5 * predivs[5];
  result -= x7 * predivs[7];
  result += x9 * predivs[9];
  result -= x11 * predivs[11];*/

  //return result;
}
//Normalizes and outputs atan of the angle of a vector
//(z-axis is ignored)
float MathF::ApproxATan(Vector3D input)
{
  //Normalize if necessary
  //nope, won't make a difference
  /*float length;
  if (input.x > 1.0f || input.y > 1.0f)
  {
    length = (input.x * input.x) + (input.y * input.y);
    length = ApproxSqrt(length);
    input.x = input.x / length;
    input.y = input.y / length;
  }*/

  return ApproxATan(input.y / input.x);
}

//Assumes that float is in the IEEE 754 single precision floating point format
//and that int is 32 bits.
//Gets close. Within .3~ for sqrt(25.f), .08 for sqrt(pi)
//About 2/3rds the time, not much of a benefit unless you do it a bunch
float MathF::ApproxSqrt(float a)
{
  int valInt= *reinterpret_cast<int*>(&a); /* Same bits, but as an int */

  valInt -= 1 << 23; /* Subtract 2^m. */
  valInt >>= 1; /* Divide by 2. */
  valInt += 1 << 29; /* Add ((b + 1) / 2) * 2^m. */

  return *reinterpret_cast<float *>(&valInt); /* Interpret again as float */
}

Vector3D MathF::RotateAroundPoint(Vector3D origin, float radAngle, Vector3D start)
{
  //cos a -sin a    x
  //sin a cos a     y
  start -= origin;
  float x = start.x, y = start.y;
  start.x = cos(radAngle) * x - sin(radAngle) * y;
  start.y = sin(radAngle) * x + cos(radAngle) * y;
  start += origin;
  return start;
}

Vector3D MathF::ApproxRotateAroundPoint(Vector3D origin, float radAngle, Vector3D start)
{
  //cos a -sin a    x
  //sin a cos a     y
  start -= origin;
  float x = start.x, y = start.y;
  start.x = ApproxCos(radAngle) * x - ApproxSin(radAngle) * y;
  start.y = ApproxSin(radAngle) * x + ApproxCos(radAngle) * y;
  start += origin;
  return start;
}

float MathF::Round(float a)
{
  if (a - static_cast<float>(static_cast<int>(a)) > 0.5f)
    return static_cast<float>(static_cast<int>(a) + 1);

  return static_cast<float>(static_cast<int>(a));
}

//Josh's Easing functions
//Linear
float MathF::Linear(float currentTime, float startValue, float endValue, float duration)
{
	float change = endValue - startValue;

	return change * currentTime / duration + startValue;
}

//Quadratic
float MathF::QuadIn(float currentTime, float startValue, float endValue, float duration)
{
	float change = endValue - startValue;

	currentTime /= duration;

	return change * currentTime * currentTime + startValue;
}

float MathF::QuadOut(float currentTime, float startValue, float endValue, float duration)
{
	float change = endValue - startValue;

	currentTime /= duration;

	return -change * currentTime * (currentTime - 2) + startValue;
}

float MathF::QuadInOut(float currentTime, float startValue, float endValue, float duration)
{
	float change = endValue - startValue;

	currentTime /= duration / 2;
	if (currentTime < 1)
	{
		return change / 2 * currentTime * currentTime + startValue;
	}

	currentTime -= 1;
	return -change / 2 * (currentTime * (currentTime - 2) - 1) + startValue;
}

//Cubic
float MathF::CubicIn(float currentTime, float startValue, float endValue, float duration)
{
	float change = endValue - startValue;

	currentTime /= duration;
	return change * currentTime * currentTime * currentTime + startValue;
}

float MathF::CubicOut(float currentTime, float startValue, float endValue, float duration)
{
	float change = endValue - startValue;

	currentTime /= duration;
	currentTime -= 1;
	return change * (currentTime * currentTime * currentTime + 1) + startValue;
}

float MathF::CubicInOut(float currentTime, float startValue, float endValue, float duration)
{
	float change = endValue - startValue;

	currentTime /= duration / 2;
	if (currentTime < 1)
	{
		return change / 2 * currentTime * currentTime * currentTime + startValue;
	}

	currentTime -= 2;
	return change / 2 * (currentTime * currentTime * currentTime + 2) + startValue;
}

//Quartic
float MathF::QuarticIn(float currentTime, float startValue, float endValue, float duration)
{
	float change = endValue - startValue;

	currentTime /= duration;
	return change * currentTime * currentTime * currentTime * currentTime + startValue;
}

float MathF::QuarticOut(float currentTime, float startValue, float endValue, float duration)
{
	float change = endValue - startValue;

	currentTime /= duration;
	currentTime -= 1;
	return -change * (currentTime * currentTime * currentTime * currentTime - 1) + startValue;
}

float MathF::QuarticInOut(float currentTime, float startValue, float endValue, float duration)
{
	float change = endValue - startValue;

	currentTime /= duration / 2;
	if (currentTime < 1)
	{
		return change / 2 * currentTime * currentTime * currentTime * currentTime + startValue;
	}

	currentTime -= 2;
	return -change / 2 * (currentTime * currentTime * currentTime * currentTime - 2) + startValue;
}

//Quintic
float MathF::QuinticIn(float currentTime, float startValue, float endValue, float duration)
{
	float change = endValue - startValue;

	currentTime /= duration;
	return change * currentTime * currentTime * currentTime * currentTime * currentTime + startValue;
}

float MathF::QuinticOut(float currentTime, float startValue, float endValue, float duration)
{
	float change = endValue - startValue;

	currentTime /= duration;
	currentTime -= 1;
	return change * (currentTime * currentTime * currentTime * currentTime * currentTime + 1) + startValue;
}

float MathF::QuinticInOut(float currentTime, float startValue, float endValue, float duration)
{
	float change = endValue - startValue;

	currentTime /= duration / 2;
	if (currentTime < 1)
	{
		return change / 2 * currentTime * currentTime * currentTime * currentTime * currentTime + startValue;
	}

	currentTime -= 2;
	return change / 2 * (currentTime * currentTime * currentTime * currentTime * currentTime + 2) + startValue;
}


