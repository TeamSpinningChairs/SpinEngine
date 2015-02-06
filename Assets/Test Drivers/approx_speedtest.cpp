//Henry's test driver for the Approx functions in MathF
#include "mathlib.h"
#include <iostream> //cout
#include <Windows.h>
//#include <random>

using std::cout;
using std::endl;
#define TEST_COUNT 100000
#define PI 3.1415926535f
#define SQRT_TEST_RANGE 100 //we test by getting square roots from 0 to SQRT_TEST_RANGE

//What I tested out the maximum differences as being.
//(the margin of error is a bit lower than these values in each case)
//(sin and cos: values within PI, atan: values between 0 and 1f,
//sqrt: values between 0 and 100f)
#define SIN_THRESHOLD .08f
#define COS_THRESHOLD .023f
#define ATAN_THRESHOLD .05f
#define SQRT_THRESHOLD .344f

int main()
{
  
  
  //Fill an array with random numbers.
  float *inputs = new float[TEST_COUNT];
  for (int i = 0; i < TEST_COUNT; ++i)
    inputs[i] = (static_cast<float>(std::rand() % 180) * PI) / 180.0f;

  float *atanInputs = new float[TEST_COUNT];
  for (int i = 0; i < TEST_COUNT; ++i)
    atanInputs[i] = static_cast<float>(std::rand() % 100) / 100.0f;

  float *sqrtInputs = new float[TEST_COUNT];
  for (int i = 0; i < TEST_COUNT; ++i)
    sqrtInputs[i] = static_cast<float>(std::rand() % SQRT_TEST_RANGE);

  //SIN
  ////////////////
  LARGE_INTEGER startTime;
  QueryPerformanceCounter(&startTime);
  for (int i = 0; i < TEST_COUNT; ++i)
    sinf(inputs[i]);
  LARGE_INTEGER now;
  QueryPerformanceCounter(&now);
  _int64 microseconds = now.QuadPart - startTime.QuadPart;
  cout << microseconds << " microseconds for std::sinf." << endl;

  QueryPerformanceCounter(&startTime);
  for (int i = 0; i < TEST_COUNT; ++i)
    MathF::ApproxSin(inputs[i]);
  QueryPerformanceCounter(&now);
  microseconds = now.QuadPart - startTime.QuadPart;
  cout << microseconds << " microseconds for ApproxSin." << endl << endl;

  for (int i = 0; i < TEST_COUNT; ++i)
  {
    float a = sinf(inputs[i]);
    float b = MathF::ApproxSin(inputs[i]);
    if (abs(a - b) > SIN_THRESHOLD)
      cout << "ApproxSin had too distant an approx.\n";
  }

  //COS
  ////////////////
  QueryPerformanceCounter(&startTime);
  for (int i = 0; i < TEST_COUNT; ++i)
    cosf(inputs[i]);
  QueryPerformanceCounter(&now);
  microseconds = now.QuadPart - startTime.QuadPart;
  cout << microseconds << " microseconds for std::cosf." << endl;

  QueryPerformanceCounter(&startTime);
  for (int i = 0; i < TEST_COUNT; ++i)
    MathF::ApproxCos(inputs[i]);
  QueryPerformanceCounter(&now);
  microseconds = now.QuadPart - startTime.QuadPart;
  cout << microseconds << " microseconds for ApproxCos." << endl << endl;

  for (int i = 0; i < TEST_COUNT; ++i)
  {
    float a = cosf(inputs[i]);
    float b = MathF::ApproxCos(inputs[i]);
    if (abs(a - b) > COS_THRESHOLD)
      cout << "ApproxCos had too distant an approx.\n";
  }

  //ATAN
  ////////////////
  QueryPerformanceCounter(&startTime);
  for (int i = 0; i < TEST_COUNT; ++i)
    atanf(atanInputs[i]);
  QueryPerformanceCounter(&now);
  microseconds = now.QuadPart - startTime.QuadPart;
  cout << microseconds << " microseconds for std::atanf." << endl;

  QueryPerformanceCounter(&startTime);
  for (int i = 0; i < TEST_COUNT; ++i)
    MathF::ApproxATan(atanInputs[i]);
  QueryPerformanceCounter(&now);
  microseconds = now.QuadPart - startTime.QuadPart;
  cout << microseconds << " microseconds for ApproxATan." << endl << endl;

  for (int i = 0; i < TEST_COUNT; ++i)
  {
    float a = atanf(atanInputs[i]);
    float b = MathF::ApproxATan(atanInputs[i]);
    if (abs(a - b) > ATAN_THRESHOLD)
      cout << "ApproxATan had too distant an approx.\n";
  }

  /*for (int i = 0; i < TEST_COUNT; ++i)
  {
    float a = sinf(inputs[i]);
  }*/

  //SQRT
  ////////////////
  QueryPerformanceCounter(&startTime);
  for (int i = 0; i < TEST_COUNT; ++i)
    sqrtf(sqrtInputs[i]);
  QueryPerformanceCounter(&now);
  microseconds = now.QuadPart - startTime.QuadPart;
  cout << microseconds << " microseconds for std::sqrtf." << endl;

  QueryPerformanceCounter(&startTime);
  for (int i = 0; i < TEST_COUNT; ++i)
    MathF::ApproxSqrt(sqrtInputs[i]);
  QueryPerformanceCounter(&now);
  microseconds = now.QuadPart - startTime.QuadPart;
  cout << microseconds << " microseconds for ApproxSqrt." << endl << endl;

  //We're testing 0.5 for values between 0 and 100.
  //(error gets larger for larger numbers, obv)
  for (int i = 0; i < TEST_COUNT; ++i)
  {
    float a = sqrtf(sqrtInputs[i]);
    float b = MathF::ApproxSqrt(sqrtInputs[i]);
    if (abs(a - b) > SQRT_THRESHOLD)
      cout << "ApproxSqrt had too distant an approx.\n";
  }

  delete[] inputs;
  delete[] sqrtInputs;
  delete[] atanInputs;

  return 0;
}