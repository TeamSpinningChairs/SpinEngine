/****************************************************************************/
/*!
\author Esteban Maldonado
\par    email: esteban.maldonado@digipen.edu
\par    Course: GAM 200
\brief

Constain all struct definitions for the different types of vertices used.

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#pragma once

//#include "Precompiled.h" //Colors.h
#include <d3d9.h>
#include "../Includes/d3dx9.h"
#include "../Utilities/Colors.h"

#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define CUSTOMFVF_NORMAL (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 | D3DFVF_NORMAL )

namespace d3dVertex
{
  struct VertexPositionColor
  {
    d3dVertex::VertexPositionColor()
    {
    }
    d3dVertex::VertexPositionColor(float _x, float _y, float _z, D3DCOLOR c) :x(_x), y(_y), z(_z), color(c)
    {
    }
    float x, y, z; //position
    D3DCOLOR color; //color
    static const DWORD FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
  };

  struct VertexPositionTexture
  {
    VertexPositionTexture():x(0),y(0), z(0),u(0), v(0)
    {
    }
    VertexPositionTexture(float _x, float _y, float _z, float _u, float _v) :x(_x), y(_y), z(_z), u(_u), v(_v)
    {
    }
    float x, y, z; //position
    float u, v; //texture coordinates
    static const DWORD FVF = D3DFVF_XYZ | D3DFVF_TEX1;
  };

  struct CUSTOMVERTEX
  {
    CUSTOMVERTEX(FLOAT x, FLOAT y, FLOAT z, DWORD color, FLOAT u, FLOAT v) :x(x), y(y), z(z), color(color), u(u), v(v)
    {
    }
    FLOAT x, y, z;    // from the D3DFVF_XYZRHW flag
    D3DCOLOR color;    // from the D3DFVF_DIFFUSE flag
    FLOAT u, v;   // from the D3DFVF_TEX1 flag
    static const DWORD FVF = CUSTOMFVF;

  };

  struct CUSTOMVERTEX_NORMAL
  {
    CUSTOMVERTEX_NORMAL(FLOAT x, FLOAT y, FLOAT z, DWORD color, FLOAT u, FLOAT v) :x(x), y(y), z(z), color(color), u(u), v(v)
    {
      NORMAL.x = NORMAL.y = 0;
      NORMAL.z = 1.0f;
    }
    FLOAT x, y, z;    // from the D3DFVF_XYZRHW flag
    D3DCOLOR color;    // from the D3DFVF_DIFFUSE flag
    FLOAT u, v;   // from the D3DFVF_TEX1 flag
    D3DVECTOR NORMAL; // Normal Vector for lighting
    static const DWORD FVF = CUSTOMFVF_NORMAL;
  };

  struct line_vertex
  {
    line_vertex(FLOAT x, FLOAT y, FLOAT z, FLOAT rhw, DWORD color) :x(x), y(y), z(z), rhw(rhw), color(color)
    {
    }
    FLOAT x, y, z, rhw;  // The transformed(screen space) position for the vertex.
    D3DCOLOR color;        // The vertex color.
    static const DWORD line_fvf = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;
  };

  //PLEASE DON'T CHANGE THESE ARRAYS! - ESTEBAN
  //60 sided polygon to represent a circle for circle collision
  const int circle_vert_count = 61;

  const d3dVertex::VertexPositionColor circle_debug_draw_verts[circle_vert_count] =
  {
    d3dVertex::VertexPositionColor(1, 0, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(0.994522, 0.104528, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(0.978148, 0.207912, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(0.951057, 0.309017, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(0.913545, 0.406737, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(0.866025, 0.5, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(0.809017, 0.587785, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(0.743145, 0.669131, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(0.669131, 0.743145, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(0.587785, 0.809017, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(0.5, 0.866025, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(0.406737, 0.913545, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(0.309017, 0.951057, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(0.207912, 0.978148, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(0.104528, 0.994522, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(-4.37114e-008, 1, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(-0.104529, 0.994522, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(-0.207912, 0.978148, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(-0.309017, 0.951056, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(-0.406737, 0.913545, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(-0.5, 0.866025, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(-0.587785, 0.809017, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(-0.669131, 0.743145, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(-0.743145, 0.66913, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(-0.809017, 0.587785, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(-0.866025, 0.5, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(-0.913545, 0.406737, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(-0.951057, 0.309017, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(-0.978148, 0.207912, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(-0.994522, 0.104528, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(-1, -8.74228e-008, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(-0.994522, -0.104529, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(-0.978148, -0.207912, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(-0.951057, -0.309017, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(-0.913545, -0.406737, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(-0.866025, -0.5, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(-0.809017, -0.587785, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(-0.743145, -0.669131, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(-0.669131, -0.743145, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(-0.587785, -0.809017, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(-0.5, -0.866025, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(-0.406737, -0.913545, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(-0.309017, -0.951056, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(-0.207911, -0.978148, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(-0.104528, -0.994522, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(1.19249e-008, -1, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(0.104529, -0.994522, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(0.207912, -0.978148, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(0.309017, -0.951056, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(0.406737, -0.913545, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(0.5, -0.866025, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(0.587785, -0.809017, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(0.669131, -0.743145, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(0.743145, -0.669131, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(0.809017, -0.587785, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(0.866026, -0.5, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(0.913546, -0.406736, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(0.951057, -0.309017, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(0.978148, -0.207912, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(0.994522, -0.104528, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(1, 0, 0.0f, d3dColors::Blue)
  };


  const d3dVertex::VertexPositionColor box_debug_draw_verts[5] =
  {
    d3dVertex::VertexPositionColor(-1.0f, 1.0f, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(-1.0f, -1.0f, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(1.0f, -1.0f, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(1.0f, 1.0f, 0.0f, d3dColors::Blue),
    d3dVertex::VertexPositionColor(-1.0f, 1.0f, 0.0f, d3dColors::Blue)
  };

  //PLEASE DON'T CHANGE THESE ARRAYS!
}
