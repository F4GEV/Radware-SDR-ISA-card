//REGEN_FILEHEADING
//REGEN_FILEHEADING

     /********************************************************************
      *                                                                  *
      *   Source File: All_Frequencies.cpp                                          *
      *   Description: Dialog implementation for PRORADIO                *
      *   Date:        Tue Sep 15 12:37:33 1992                          *
      *                                                                  *
      ********************************************************************/

#include <owl\framewin.h>
#include <owl\edit.h>
#include <owl\listbox.h>
#include <owl\combobox.h>
#include <owl\scrollba.h>
#include <owl\dialog.h>
#include <bwcc.h>
#include <stdio.h>
#include "thisCls.h"
//#include "SWLIB.h"
#include <owl\radiobut.h>
#include <owl\button.h>
#include <math.h>;

extern int starting_places[][2];
extern short int delete_class[];

// Define TAll_Frequencies, a TWindow constructor
TAllFrequencies::TAllFrequencies(TWindow *AParent, LPSTR AName,PStart_Control Start_Control,
	       PStop_Control Stop_Control,PCenter_Control Center_Control,PSpan_Control Span_Control,
               PStep_Size_Control Step_Size_Control,int child_number):AWindow(AParent, AName,child_number)
{
//   char string[21];
   int x,y;

   Child_Number = child_number;
   x = starting_places[Child_Number][0];
   y = starting_places[Child_Number][1];
   Attr.Style = WS_MINIMIZEBOX | WS_POPUP | WS_CLIPSIBLINGS | WS_VISIBLE |
                WS_CAPTION | WS_SYSMENU;
   SetCaption("Frequencies");
   if (x>=0)
     Attr.X = x;
   else
     Attr.X = -x;
   if (y>=0)
     Attr.Y = y;
   else
     Attr.Y = -y;
   Attr.W = 162;
   Attr.H = 172;
   Start_Window = new TFrequency_Window(this,101,"FREQ",Start_Control,0,0);
   Stop_Window = new TFrequency_Window(this,102,"FREQ",Stop_Control,0,30);
   Center_Window = new TFrequency_Window(this,103,"FREQ",Center_Control,0,60);
   Span_Window = new TFrequency_Window(this,104,"FREQ",Span_Control,0,90);
   Step_Size_Window = new TFrequency_Window(this,105,"FREQ",Step_Size_Control,0,121);
}

// Define TAll_Frequencies destructor
TAllFrequencies::~TAllFrequencies()

{
  Attr.H = -1;
}

/*void TAllFrequencies::Paint(HDC DC, PAINTSTRUCT _FAR &)

{
  static LOGPEN lpBlack = {PS_SOLID,1,1,RGB(0,0,0)};
  int count,pixel;
  HPEN Black,Old_Pen;

  Black = CreatePenIndirect(&lpBlack);
  Old_Pen = SelectObject(DC,Black);
  MoveTo(DC,0,120);
  LineTo(DC,172,120);
  SelectObject(DC,Old_Pen);
  DeleteObject(Black);
  DeleteObject(DC);
}
*/
void TAllFrequencies::Changed_Start()

{
  HRGN Region;

  Region = CreateRectRgn(75,4,148,26);
  ::InvalidateRgn(Stop_Window->HWindow,Region,1);
  ::UpdateWindow(Stop_Window->HWindow);
  ::InvalidateRgn(Center_Window->HWindow,Region,1);
  ::UpdateWindow(Center_Window->HWindow);
  ::InvalidateRgn(Span_Window->HWindow,Region,1);
  ::UpdateWindow(Span_Window->HWindow);
  DeleteObject(Region);
}

void TAllFrequencies::Changed_Stop()

{
  HRGN Region;

  Region = CreateRectRgn(75,4,148,26);
  ::InvalidateRgn(Start_Window->HWindow,Region,1);
  ::UpdateWindow(Start_Window->HWindow);
  ::InvalidateRgn(Center_Window->HWindow,Region,1);
  ::UpdateWindow(Center_Window->HWindow);
  ::InvalidateRgn(Span_Window->HWindow,Region,1);
  ::UpdateWindow(Span_Window->HWindow);
  DeleteObject(Region);
}

void TAllFrequencies::Changed_Center()

{
  HRGN Region;

  Region = CreateRectRgn(75,4,148,26);
  ::InvalidateRgn(Start_Window->HWindow,Region,1);
  ::UpdateWindow(Start_Window->HWindow);
  ::InvalidateRgn(Stop_Window->HWindow,Region,1);
  ::UpdateWindow(Stop_Window->HWindow);
  ::InvalidateRgn(Span_Window->HWindow,Region,1);
  ::UpdateWindow(Span_Window->HWindow);
  DeleteObject(Region);
}

void TAllFrequencies::Changed_Span()

{
  HRGN Region;

  Region = CreateRectRgn(75,4,148,26);
  ::InvalidateRgn(Center_Window->HWindow,Region,1);
  ::UpdateWindow(Center_Window->HWindow);
  ::InvalidateRgn(Start_Window->HWindow,Region,1);
  ::UpdateWindow(Start_Window->HWindow);
  ::InvalidateRgn(Stop_Window->HWindow,Region,1);
  ::UpdateWindow(Stop_Window->HWindow);
  DeleteObject(Region);
}

DEFINE_RESPONSE_TABLE1(TAllFrequencies,TWindow)
  EV_COMMAND(101,Changed_Start),
  EV_COMMAND(102,Changed_Stop),
  EV_COMMAND(103,Changed_Center),
  EV_COMMAND(104,Changed_Span),
END_RESPONSE_TABLE;

