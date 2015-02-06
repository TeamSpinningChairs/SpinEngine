/****************************************************************************/
/*!
\author Esteban Maldonado
\par    email: esteban.maldonado@digipen.edu
\par    Course: GAM 200
\brief

Contains constant values for specific colors

Copyright: All content @ 2014 DigiPen (USA) Corporation, all rights reserved.

*/
/****************************************************************************/
#include"Precompiled.h"
#include "Colors.h"
#include "DebugMessages.h"

const D3DCOLOR d3dColors::Change_Alpha_Value(D3DCOLOR& color, float alpha)
{
  if (alpha >= 0.0f && alpha <= 1.0f)
  {
    //D3DCOLOR new_color = (color & 0x00FFFFFF) | (static_cast<int>(255.0f * alpha) << 24);
    int a = (static_cast<byte>(255.0f * alpha));
    //byte r = (new_color & 0x00FF0000);
    //byte g = (new_color & 0x0000FF00);
    //byte b = (new_color & 0x000000FF);
    //
    //char a_text[40];
    //itoa((new_color & 0xFF000000), a_text, 2);
    //SendDebugMessage(a_text);

    color = D3DCOLOR_ARGB(a,a,a,a);


    return color;
  }
  return color;
}
