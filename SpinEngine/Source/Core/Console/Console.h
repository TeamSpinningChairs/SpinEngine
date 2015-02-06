/******************************************************************************/
/*!
\file   Console.h
\author Micah Rust
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\briefContains all of the code to create a console:
input/output, Colors.
*/
/******************************************************************************/


#pragma once
#include "Precompiled.h"

/*! check whether the colors of the text and background are the game.
This ensures that they are never the same color*/
#define PROTECTCOLORS true


namespace SpinningConsole
{
  /*!numbered 0-15: The console colors can be put into:
  ->SetColor: Set text and background color
  ->SetTextColor: Set the color of the text
  ->SetBackgroundcolor: Set the color of the background*/
  enum ConsoleColors {
    black = 0,                                                                         //! 0
    darkblue = FOREGROUND_BLUE,                                                        //! 1
    darkgreen = FOREGROUND_GREEN,                                                      //! 2  
    darkcyan = FOREGROUND_GREEN | FOREGROUND_BLUE,                                     //! 3
    darkred = FOREGROUND_RED,                                                          //! 4
    darkmagenta = FOREGROUND_RED | FOREGROUND_BLUE,                                    //! 5
    darkyellow = FOREGROUND_RED | FOREGROUND_GREEN,                                    //! 6
    darkgray = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,                    //! 7
    gray = FOREGROUND_INTENSITY,                                                       //! 8
    blue = FOREGROUND_INTENSITY | FOREGROUND_BLUE,                                     //! 9
    green = FOREGROUND_INTENSITY | FOREGROUND_GREEN,                                   //! 10
    cyan = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,                  //! 11
    red = FOREGROUND_INTENSITY | FOREGROUND_RED,                                       //! 12
    magenta = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,                 //! 13
    yellow = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,                 //! 14
    white = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,//! 15
  };

  /*!Create console and setup colors
  
  @TODO Incorporate a way to talk to event system through a hot key '`'*/
   void Create_Spinning_Console(const char* name);
  /*!Free up the console after program is done*/
   void Free_Spinning_Console(void);

  /*!Set the text and background color*/
   void SetColor(const ConsoleColors textcolor, const ConsoleColors backgroundcolor);
  /*!set the text color of the console*/
   void SetTextColor(const ConsoleColors textcolor);
  /*!Set the background color of the console*/
   void SetBackgroundColor(const ConsoleColors backgroundcolor);
  /*!Get the colors currently in use by the console*/
   void GetConsoleColors(ConsoleColors &textcolor, ConsoleColors &backgroundcolor);

  /*!
  _textcolor           = current color of the printed text
  _backgroundcolor     = current color of the background around the printed test.*/
  extern ConsoleColors _textcolor, _backgroundcolor;

  /*!
  _deftextcolo        = The original color of the text.
  _defbackgroundcolor = The original color of the background around the printed text*/
  extern ConsoleColors _deftextcolor, _defbackgroundcolor;

  //! handle to the console
  extern HANDLE _hstdconsole;

  /*!Overloaded output opperator so that the console colors can be changed via:
  -> std::cout << red << "Hello"
  creating a red "Hello" */
  //template<class _element, class _traits>
  std::ostream& operator<<(std::ostream& os, ConsoleColors color);

  /*!Overloaded input opperator so that the console colors can be changed via:
  -> char[30] a;
  -> std::cin >> blue >> a
  Letting us type in blue */
  std::istream& operator>>(std::istream& is, ConsoleColors color);

};
