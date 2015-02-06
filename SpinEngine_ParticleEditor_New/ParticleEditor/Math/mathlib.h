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
#pragma once
#include <math.h>
#include <algorithm>

const float PI = 3.14159265358979323846264338327950F;

class Vector2D
{
public:
  float x;
  float y;
private:

public:
  Vector2D() : x(0), y(0) {}

  Vector2D(const float x, const float y) : x(x), y(y) {}

  void Set(float newX, float newY)
  {
    x = newX;
    y = newY;
  }

  void Clear()
  {
    x = 0.0f;
    y = 0.0f;
  }

  float Magnitude()
  {
    return sqrtf(x*x + y*y);
  }

  Vector2D Normalize()
  {
    Vector2D result;

    // Find the magnitude
    float mag = this->Magnitude();

    result.x = x / mag;
    result.y = y / mag;

    return result;
  }

  void Invert()
  {
    x = -x;
    y = -y;
  }

  float SqrMagnitude()
  {
    return(x*x + y*y);
  }

  // Add
  void operator+=(const Vector2D &vec)
  {
    x += vec.x;
    y += vec.y;
  }

  Vector2D operator+(const Vector2D& vec) const
  {
    return Vector2D(x + vec.x, y + vec.y);
  }


  // Subtract
  void operator-=(const Vector2D &vec)
  {
    x -= vec.x;
    y -= vec.y;
  }

  Vector2D operator-(const Vector2D& vec) const
  {
    return Vector2D(x - vec.x, y - vec.y);
  }

  Vector2D operator-(void)
  {
    return (Vector2D(-1 * x, -1 * y));
  }
  
  friend Vector2D operator*(const Vector2D& vec, const float scale)
  {
    return Vector2D(vec.x*scale, vec.y*scale);
  }

  friend Vector2D operator*(const float scale, const Vector2D& vec)
  {
    return Vector2D(vec.x*scale, vec.y*scale);
  }
 
  void operator*=(const float scale)
  {
    x *= scale;
    y *= scale;
  }

  void operator/=(const float scale)
  {
    if (scale == 0.0f)
    {
      throw "Division by zero";
    }

    x /= scale;
    y /= scale;
  }

  friend Vector2D operator/(const Vector2D& vec, const float scale)
  {
    if (scale == 0.0f)
    {
      throw "Division by zero";
    }
      return Vector2D(vec.x/scale, vec.y/scale);
  }
  friend Vector2D operator/(const float scale, const Vector2D& vec)
  {
    if (scale == 0.0f)
      return Vector2D(0.0f, 0.0f);
    else
      return Vector2D(vec.x/scale, vec.y/scale);
  }

  // Returns true if the vectors are equal.
  bool operator==(const Vector2D& rhs)
  {
    return (x == rhs.x && y == rhs.y);
  }

  // Returns true if vectors are not equal
  bool operator!=(const Vector2D& rhs)
  {
    return !(*this == rhs);
  }

  // Adds the vector to this, scaled by the given amount
  void AddScale(const Vector2D& vec, float scale)
  {
    x += vec.x * scale;
    y += vec.y * scale;
  }

  void SubScale(const Vector2D& vec, float scale)
  {
    x -= vec.x * scale;
    y -= vec.y * scale;
  }
 
  
  /* STATIC FUNCTIONS */

  // Distance
  static float Distance(Vector2D &pVec0, Vector2D &pVec1)
  {
    float xd = pVec1.x - pVec0.x;
    float yd = pVec1.y - pVec0.y;

    return sqrtf(xd*xd + yd*yd);
  }

  // Dot Product
  static float DotProduct(Vector2D &pVec0, Vector2D &pVec1)
  {
    return pVec0.x*pVec1.x + pVec0.y*pVec1.y;
  }

  static Vector2D CrossProduct(float a, const Vector2D &pVec)
  {
    return Vector2D(-a*pVec.y, a*pVec.x);
  }

  static Vector2D CrossProduct(const Vector2D &pVec, float a)
  {
    return Vector2D(a*pVec.y, -a*pVec.x);
  }

  static float CrossProduct(const Vector2D &pVec0, const Vector2D &pVec1)
  {
    return(pVec0.x * pVec1.y - pVec0.y * pVec1.x);
  }

  // Max
  // Returns a vector that is made from the largest components of two vectors.
  static Vector2D Max(Vector2D &pVec0, Vector2D &pVec1)
  {
    Vector2D max;
    if (pVec0.x > pVec1.x)
    {
      max.x = pVec0.x;
    }
    else
    {
      max.x = pVec1.x;
    }

    if (pVec0.y > pVec1.y)
    {
      max.y = pVec0.y;
    }
    else
    {
      max.y = pVec1.y;
    }

    return max;
  }

  // Min
  static Vector2D Min(Vector2D &pVec0, Vector2D &pVec1)
  {
    Vector2D min;
    if (pVec0.x < pVec1.x)
    {
      min.x = pVec0.x;
    }
    else
    {
      min.x = pVec1.x;
    }

    if (pVec0.y < pVec1.y)
    {
      min.y = pVec0.y;
    }
    else
    {
      min.y = pVec1.y;
    }

    return min;
  }

};
typedef Vector2D Vector2;

class Vector3D
{
public:
  float x;
  float y;
  float z;
private:
  
public:
  Vector3D() : x(0), y(0), z(0) {} // constructor to zero out

  Vector3D(const float x, const float y, const float z)
    : x(x), y(y), z(z) {}

  void Set(float newX, float newY, float newZ)
  {
    x = newX;
    y = newY;
    z = newZ;
  }

  // Zero all the components of the vector
  void Clear()
  {
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
  }

  float Magnitude()
  {
    return sqrtf(x*x + y*y + z*z);
  }

  Vector3D Normalize()
  {
    Vector3D result;

    // Find the magnitude
    float mag = this->Magnitude();

    result.x = x / mag;
    result.y = y / mag;
    result.z = z / mag;

    return result;
  }

  void Invert()
  {
    x = -x;
    y = -y;
    z = -z;
  }

  float SqrMagnitude()
  {
    return(x*x + y*y + z*z);
  }

  // Add
  void operator+=(const Vector3D &vec)
  {
    x += vec.x;
    y += vec.y;
    z += vec.z;
  }

  Vector3D operator+(const Vector3D& vec) const
  {
    return Vector3D(x + vec.x, y + vec.y, z + vec.z);
  }

  // Subtract
  void operator-=(const Vector3D &vec)
  {
    x -= vec.x;
    y -= vec.y;
    z -= vec.z;
  }

  Vector3D operator-(const Vector3D& vec) const
  {
    return Vector3D(x - vec.x, y - vec.y, z - vec.z);
  }

  Vector3D operator-(void)
  {
    return (Vector3D(-1 * x, -1 * y, -1 * z));
  }
  
  void operator*=(const float scale)
  {
    x *= scale;
    y *= scale;
    z *= scale;
  }
  
  friend Vector3D operator*(const Vector3D& vec, const float scale)
  {
    return Vector3D(vec.x*scale, vec.y*scale, vec.z*scale);
  }

  friend Vector3D operator*(const float scale, const Vector3D& vec)
  {
    return Vector3D(vec.x*scale, vec.y*scale, vec.z*scale);
  }
  
  void operator/=(const float scale)
  {
    if (scale == 0.0f)
    {
      throw "Division by zero";
    }
    x /= scale;
    y /= scale;
    z /= scale;
  }

  friend Vector3D operator/(const Vector3D& vec, const float scale)
  {
    if (scale == 0.0f)
    {
      throw "Division by zero";
    }
    return Vector3D(vec.x/scale, vec.y/scale, vec.z/scale);
  }

  friend Vector3D operator/(const float scale, const Vector3D& vec)
  {
    if (scale == 0.0f)
      return Vector3D(0.0f, 0.0f, 0.0f);
    else
      return Vector3D(vec.x/scale, vec.y/scale, vec.z/scale);
  }

  // Returns true if the vectors are equal.
  bool operator==(const Vector3D& rhs)
  {
    return (x == rhs.x && y == rhs.y && z == rhs.z);
  }

  // Returns true if vectors are not equal
  bool operator!=(const Vector3D& rhs)
  {
    return !(*this == rhs);
  }

  // Adds the vector to this, scaled by the given amount
  void addScale(const Vector3D& vec, float scale)
  {
    x += vec.x * scale;
    y += vec.y * scale;
    z += vec.z * scale;
  }

  void subScale(const Vector3D& vec, float scale)
  {
    x -= vec.x * scale;
    y -= vec.y * scale;
    z -= vec.z * scale;
  }
 
  
  /* STATIC FUNCTIONS */

  // Distance
  static float Distance(Vector3D &pVec0, Vector3D &pVec1)
  {
    float xd = pVec1.x - pVec0.x;
    float yd = pVec1.y - pVec0.y;
    float zd = pVec1.z - pVec0.z;

    return sqrt(xd*xd + yd*yd + zd*zd);
  }

  // Dot Product
  static float DotProduct(Vector3D &pVec0, Vector3D &pVec1)
  {
    return pVec0.x*pVec1.x + pVec0.y*pVec1.y + pVec0.z*pVec1.z;
  }
  // Cross Product
  static Vector3D CrossProduct(Vector3D &pVec0, Vector3D &pVec1)
  {
    return Vector3D(pVec0.y*pVec1.z-pVec0.z*pVec1.y, pVec0.z*pVec1.x-pVec0.x*pVec1.z, 
                    pVec0.x*pVec1.y-pVec0.y*pVec1.x);
  }

  // Projects a vector onto another vector.
  static Vector3D Projection(Vector3D &pVec0, Vector3D &pVec1)
  {
    // Dot product between pVec0 and pVec1
    float dot =DotProduct(pVec0, pVec1);

    // Calculate bottom
    float bot = pVec1.x*pVec1.x + pVec1.y*pVec1.y + pVec1.z*pVec1.z;
    // Find the scalar projection
    float scalarProj = dot / bot;

    // Vector scalar multiplication 
    return Vector3D(scalarProj*pVec1.x, scalarProj*pVec1.y, scalarProj*pVec1.z);
  }

  // Reflects a vector off the plane defined by a normal.
  static Vector3D Reflection(Vector3D &inDir, Vector3D &inNorm)
  {
   float scale = DotProduct(inDir, inNorm);
   // s = (v dot n)n
   Vector3D s(scale*inNorm.x, scale*inNorm.y, scale*inNorm.z);
  
   // 2*s
   s.x *= 2.0f;
   s.y *= 2.0f;
   s.z *= 2.0f;

  // r = 2 * (v dot n)n - v
  // r = s - v
   return Vector3D(s.x - inDir.x, s.y - inDir.y, s.z - inDir.z);
  }

  static Vector3D Max(Vector3D &pVec0, Vector3D &pVec1)
  {
    Vector3D max;
    if (pVec0.x > pVec1.x)
    {
      max.x = pVec0.x;
    }
    else
    {
      max.x = pVec1.x;
    }

    if (pVec0.y > pVec1.y)
    {
      max.y = pVec0.y;
    }
    else
    {
      max.y = pVec1.y;
    }

    if (pVec0.z > pVec1.z)
    {
      max.z = pVec0.z;
    }
    else
    {
      max.z = pVec1.z;
    }

    return max;
  }

  // Min
  static Vector3D Min(Vector3D &pVec0, Vector3D &pVec1)
  {
    Vector3D min;
    if (pVec0.x < pVec1.x)
    {
      min.x = pVec0.x;
    }
    else
    {
      min.x = pVec1.x;
    }

    if (pVec0.y < pVec1.y)
    {
      min.y = pVec0.y;
    }
    else
    {
      min.y = pVec1.y;
    }

    if (pVec0.z < pVec1.z)
    {
      min.z = pVec0.z;
    }
    else
    {
      min.z = pVec1.z;
    }

    return min;
  }
};
typedef Vector3D Vector3;

class Vector4D
{
public:
  float x;
  float y;
  float z;
  float w;

private:

public:
  Vector4D() : x(0), y(0), z(0), w(0) {}

  Vector4D(const float x, const float y, const float z, const float w)
    : x(x), y(y), z(z), w(w) {}

  void Set(float newX, float newY, float newZ, float newW)
  {
    x = newX;
    y = newY;
    z = newZ;
    w = newW;
  }

   // Zero all the components of the vector
  void Clear()
  {
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
    w = 0.0f;
  }

  float Magnitude()
  {
    return sqrtf(x*x + y*y + z*z + w*w);
  }

  Vector4D Normalize()
  {
    Vector4D result;

    // Find the magnitude
    float mag = this->Magnitude();

    result.x = x / mag;
    result.y = y / mag;
    result.z = z / mag;
    result.w = w / mag;

    return result;
  }

  void Invert()
  {
    x = -x;
    y = -y;
    z = -z;
    w = -w;
  }

  float SqrMagnitude()
  {
    return(x*x + y*y + z*z + w*w);
  }

  // Add
  void operator+=(const Vector4D &vec)
  {
    x += vec.x;
    y += vec.y;
    z += vec.z;
    w += vec.w;
  }

  Vector4D operator+(const Vector4D& vec) const
  {
    return Vector4D(x + vec.x, y + vec.y, z + vec.z, w + vec.w);
  }

  // Subtract
  void operator-=(const Vector4D &vec)
  {
    x -= vec.x;
    y -= vec.y;
    z -= vec.z;
    w -= vec.w;
  }

  Vector4D operator-(const Vector4D& vec) const
  {
    return Vector4D(x - vec.x, y - vec.y, z - vec.z, w - vec.w);
  }

  Vector4D operator-(void)
  {
    return (Vector4D(-1 * x, -1 * y, -1 * z, -1 * w));
  }
  void operator*=(const float scale)
  {
    x *= scale;
    y *= scale;
    z *= scale;
    w *= scale;
  }
  
  friend Vector4D operator*(const Vector4D vec, const float scale)
  {
    return Vector4D(vec.x*scale, vec.y*scale, vec.z*scale, vec.w*scale);
  }

  friend Vector4D operator*(const float scale, const Vector4D vec)
  {
    return Vector4D(vec.x*scale, vec.y*scale, vec.z*scale, vec.w*scale);
  }
  
  void operator/=(const float scale)
  {
    if (scale == 0.0f)
    {
      throw "Division by zero";
    }
    x /= scale;
    y /= scale;
    z /= scale;
    w /= scale;
  }

  friend Vector4D operator/(const Vector4D vec, const float scale)
  {
    if (scale == 0.0f)
    {
      throw "Division by zero";
    }
    return Vector4D(vec.x/scale, vec.y/scale, vec.z/scale, vec.w/scale);
  }

  friend Vector4D operator/(const float scale, const Vector4D vec)
  {
    return Vector4D(vec.x/scale, vec.y/scale, vec.z/scale, vec.w/scale);
  }

  // Returns true if the vectors are equal.
  bool operator==(const Vector4D& rhs)
  {
    return (x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w);
  }

  // Returns true if vectors are not equal
  bool operator!=(const Vector4D& rhs)
  {
    return !(*this == rhs);
  }

  // Adds the vector to this, scaled by the given amount
  void AddScale(const Vector4D& vec, float scale)
  {
    x += vec.x * scale;
    y += vec.y * scale;
    z += vec.z * scale;
    w += vec.w * scale;
  }

  void AubScale(const Vector4D& vec, float scale)
  {
    x -= vec.x * scale;
    y -= vec.y * scale;
    z -= vec.z * scale;
    w -= vec.w * scale;
  }

  /* STATIC FUNCTIONS */

  // Distance
  static float Distance(Vector4D &pVec0, Vector4D &pVec1)
  {
    float xd = pVec1.x - pVec0.x;
    float yd = pVec1.y - pVec0.y;
    float zd = pVec1.z - pVec0.z;
    float wd = pVec1.w - pVec0.w;

    return sqrtf(xd*xd + yd*yd + zd*zd + wd*wd);
  }

  // Dot Product
  static float DotProduct(Vector4D &pVec0, Vector4D &pVec1)
  {
    return pVec0.x*pVec1.x + pVec0.y*pVec1.y + pVec0.z*pVec1.z + pVec0.w*pVec1.w;
  }

  // Projects a vector onto another vector.
  static Vector4D Projection(Vector4D &pVec0, Vector4D &pVec1)
  {
    // Dot product between pVec0 and pVec1
    float dot = DotProduct(pVec0, pVec1);

    // Calculate bottom
    float bot = pVec1.x*pVec1.x + pVec1.y*pVec1.y + pVec1.z*pVec1.z + pVec1.w*pVec1.w;
    // Find the scalar projection
    float scalarProj = dot / bot;

    // Vector scalar multiplication 
    return Vector4D(scalarProj*pVec1.x, scalarProj*pVec1.y, scalarProj*pVec1.z, scalarProj*pVec1.w);
  }

    static Vector4D Max(Vector4D &pVec0, Vector4D &pVec1)
  {
    Vector4D max;
    if (pVec0.x > pVec1.x)
    {
      max.x = pVec0.x;
    }
    else
    {
      max.x = pVec1.x;
    }

    if (pVec0.y > pVec1.y)
    {
      max.y = pVec0.y;
    }
    else
    {
      max.y = pVec1.y;
    }

    if (pVec0.z > pVec1.z)
    {
      max.z = pVec0.z;
    }
    else
    {
      max.z = pVec1.z;
    }

    if (pVec0.w > pVec1.z)
    {
      max.w = pVec0.w;
    }
    else
    {
      max.w = pVec1.w;
    }

    return max;
  }

  // Min
  static Vector4D Min(Vector4D &pVec0, Vector4D &pVec1)
  {
    Vector4D min;
    if (pVec0.x < pVec1.x)
    {
      min.x = pVec0.x;
    }
    else
    {
      min.x = pVec1.x;
    }

    if (pVec0.y < pVec1.y)
    {
      min.y = pVec0.y;
    }
    else
    {
      min.y = pVec1.y;
    }

    if (pVec0.z < pVec1.z)
    {
      min.z = pVec0.z;
    }
    else
    {
      min.z = pVec1.z;
    }

    if (pVec0.w < pVec1.z)
    {
      min.w = pVec0.w;
    }
    else
    {
      min.w = pVec1.w;
    }

    return min;
  }
};
typedef Vector4D Vector4;




class Matrix3x3
{
public:
  float matrix[9];

  // Identity
  void identity()
  {
    matrix[0] = 1.0f;
    matrix[1] = 0.0f;
    matrix[2] = 0.0f;
    matrix[3] = 0.0f;
    matrix[4] = 1.0f;
    matrix[5] = 0.0f;
    matrix[6] = 0.0f;
    matrix[7] = 0.0f;
    matrix[8] = 1.0f;
  }

  // Tranpose
  void setTranspose(const Matrix3x3 &m)
  {
    matrix[0] = m.matrix[0];
    matrix[1] = m.matrix[3];
    matrix[2] = m.matrix[6];
    matrix[3] = m.matrix[1];
    matrix[4] = m.matrix[4];
    matrix[5] = m.matrix[7];
    matrix[6] = m.matrix[2];
    matrix[7] = m.matrix[5];
    matrix[8] = m.matrix[8];
  }
  // Return a new matrix containing the transpose
  Matrix3x3 Tranpose() const
  {
    Matrix3x3 resultMatrix;
    resultMatrix.setTranspose(*this);

    return resultMatrix;

  }
  
  // Multiply 2 Matrices
  Matrix3x3 operator*(const Matrix3x3 &pMat) const
  {
    Matrix3x3 result;

    for (int i = 0; i < 3; ++i)
    {
      for (int j = 0; j < 3; ++j)
      {
        float sum = 0.0;
        for (int k = 0; k < 3; ++k)
        {
          sum += matrix[i * 3 + k] * pMat.matrix[k * 3 + j];
        }
        result.matrix[i * 3 + j] = sum;
      }
    }

    return result;
  }

  void operator*=(const Matrix3x3 &pMat)
  {
    Matrix3x3 result;

    for (int i = 0; i < 3; ++i)
    {
      for (int j = 0; j < 3; ++j)
      {
        float sum = 0.0;
        for (int k = 0; k < 3; ++k)
        {
          sum += matrix[i * 3 + k] * pMat.matrix[k * 3 + j];
        }
        result.matrix[i * 3 + j] = sum;
      }
    }

    // Store values in result back to original matrix
    for (int i = 0; i < 9; ++i)
    {
      matrix[i] = result.matrix[i];
    }
  }


  // Inverse
  void setInverse(const Matrix3x3 &pMat)
  {
    float t1 = pMat.matrix[0] * pMat.matrix[4];
    float t2 = pMat.matrix[0] * pMat.matrix[5];
    float t3 = pMat.matrix[1] * pMat.matrix[3];
    float t4 = pMat.matrix[2] * pMat.matrix[3];
    float t5 = pMat.matrix[1] * pMat.matrix[6];
    float t6 = pMat.matrix[2] * pMat.matrix[6];

    // Determinant
    float det = (t1 * pMat.matrix[8] - t2 * pMat.matrix[7] - t3 * pMat.matrix[8] +
                t4 * pMat.matrix[7] + t5 * pMat.matrix[5] - t6 * pMat.matrix[4]);

    // Check if determinant is a non-zero
    if (det == 0.0f)
      return;

    float inv = 1.0f/det;

    matrix[0] = (pMat.matrix[4] * pMat.matrix[8] - pMat.matrix[5] * pMat.matrix[7]) * inv;
    matrix[1] = -(pMat.matrix[1] * pMat.matrix[8] - pMat.matrix[2] * pMat.matrix[7]) * inv;
    matrix[2] = (pMat.matrix[1] * pMat.matrix[5] - pMat.matrix[2] * pMat.matrix[4]) * inv;
    matrix[3] = -(pMat.matrix[3] * pMat.matrix[8] - pMat.matrix[5] * pMat.matrix[6]) * inv;
    matrix[4] = (pMat.matrix[0] * pMat.matrix[8] - t6) * inv;
    matrix[5] = -(t2-t4) * inv;
    matrix[6] = (pMat.matrix[3] * pMat.matrix[7] - pMat.matrix[4] * pMat.matrix[6]) * inv;
    matrix[7] = -(pMat.matrix[0] * pMat.matrix[7] - t5) * inv;
    matrix[8] = (t1 - t3) * inv;
    
  }

  // Returns a new matrix with the inverse of the matrix
  Matrix3x3 Inverse() const
  {
    Matrix3x3 result;
    result.setInverse(*this);
    return result;
  }

  // Invert the matrix
  void invert()
  {
    setInverse(*this);
  }


  // Multiply a matrix with a vector
  Vector3D operator*(const Vector3D &pVec) const
  {
    return Vector3D(
      pVec.x * matrix[0] + pVec.y * matrix[1] + pVec.z * matrix[2],
      pVec.x * matrix[3] + pVec.y * matrix[4] + pVec.z * matrix[5],
      pVec.x * matrix[6] + pVec.y * matrix[7] + pVec.z * matrix[8]);
  }

  /* STATIC FUNCTIONS */

  // Translate
  static Matrix3x3 Matrix3x3Translate(Matrix3x3 pMat, float x, float y)
  {
    pMat.matrix[2] += x;
    pMat.matrix[5] += y;

    return pMat;
  }

  // Scaling
  static Matrix3x3 Matrix3x3Scale(Matrix3x3 pMat, float x, float y)
  {
    pMat.matrix[0] *= x;
    pMat.matrix[4] *= y;

    return pMat;
  }

  // Rotate Degree
  static Matrix3x3 Matrix3x3RotRad(Matrix3x3 pMat, float angle)
  {

    pMat.matrix[0] = cosf(angle * PI / 180.0f);
    pMat.matrix[1] = -sinf(angle * PI / 180.0f);
    pMat.matrix[3] = sinf(angle * PI / 180.0f);
    pMat.matrix[4] = cosf(angle * PI / 180.0f);

    return pMat;
  }
  // Rotate Radian
  static Matrix3x3 Matrix3x3RotDeg(Matrix3x3 pMat, float angle)
  {
    pMat.matrix[0] = cosf(angle);
    pMat.matrix[1] = -sinf(angle);
    pMat.matrix[3] = sinf(angle);
    pMat.matrix[4] = cosf(angle);

    return pMat;
  }
};
typedef Matrix3x3 Matrix3;

class Matrix4x4
{
public:
  float matrix[16];
};
typedef Matrix4x4 Matrix4;

//const Vector2D GRAVITY(0.0f, -10.0f);

//MathFunctions
class MathF
{
public:

  static float Lerp(float from, float to, float time);
  static float InverseLerp(float from, float to, float current);
  static float MoveTowards(float current, float target, float maxDelta);

  template <typename T>
  static T Max(T a, T b);
  template <typename T>
  static T Min(T a, T b);

  template <typename T>
  static T Clamp(T value, T min, T max);
  static float MathF::Clamp01(float value);

  static bool FloatsAreEqual(float a, float b);
  static const float eps;

  static float RadToDegree(float radians);
  static float DegreeToRad(float degrees);

  static float ApproxSin(float radAngle);
  static float ApproxCos(float radAngle);
  static float ApproxATan(Vector3 input);
  static float ApproxATan(float val);

  static float ApproxSqrt(float a);

  static float Round(float a);
  

  static Vector3 RotateAroundPoint(Vector3 origin, float radAngle, Vector3 start);
  static Vector3 ApproxRotateAroundPoint(Vector3 origin, float radAngle, Vector3 start);

private:
  //0! through 12!
  static const float nots[13];
  //Predivided (1 / nots[i]) so we can avoid dividing at runtime in the Approx functions.
  static const double predivnots[13];
  //1/1 through 1/11
  static const float predivs[11];
};