	  /********************************************************************
		*                                                                  *
		*   Source File: Frequency_Window.cpp                                          *
		*   Description: Dialog implementation for PRORADIO                *
		*   Date:        Tue Sep 15 12:37:33 1992                          *
		*                                                                  *
		********************************************************************/

#include "thisCls.h"
#include <math.h>;
#include <owl\inputdia.h>
#include <stdio.h>;

extern int starting_places[][2];
extern int starting_sizes[][2];
extern int delete_class[];

TCallSign::TCallSign(TWindow *AParent,int id, LPSTR AName,int child_number,
							int size_number,char *callsign,char *icon_name):
							AWindow(AParent, AName,child_number)

{
  char string[21];
  int x,y,w,h;

  x = starting_places[child_number][0];
  y = starting_places[child_number][1];
  w = starting_sizes[size_number][0];
  h = starting_sizes[size_number][1];
  Size_Number = size_number;
  Get_CallSign(CallSign);
  if (y<0)
    y = -y;
  starting_places[child_number][1] = y;
  Attr.X = abs(x);
  Attr.Y = abs(y);
  box_height = 22;
  box_length = 125;
  center_y = y/2;
  h = abs(h);
  w = abs(w);
  if (h < 20)
	 h = 80;
  if (w<20)
	 w = 200;
  Attr.Style = WS_MINIMIZEBOX | WS_POPUP | WS_CLIPSIBLINGS | WS_VISIBLE |
					WS_CAPTION | WS_THICKFRAME | WS_SYSMENU;
  Attr.W = w;
  Attr.H = h;
  ID = id;
  char_height = (h-6)*9/16;
  font = new TFont("Times New Roman",18,6);
}

TCallSign::~TCallSign()

{
  Attr.W += 8;
  Attr.H += 27;
  starting_sizes[Size_Number][0] = Attr.W;
  starting_sizes[Size_Number][1] = Attr.H;
  Attr.Y *= -1;
}

LPSTR TCallSign::GetClassName()

{
  return("CALLSIGN");
}

void TCallSign::EvSize(UINT modKeys,TSize &Size)

{
  char string[80];

  Get_CallSign(CallSign);
  max_y = Size.cy;
  center_y = (Size.cy-20)/2+10;
  box_height = Size.cy-6;
  box_length = Size.cx-10;
  Attr.H = Size.cy;
  Attr.W = Size.cx;
  char_length = (box_length-10)/(2.2*strlen(CallSign));
  font->~TFont();
  delete font;
//  font = new TFont("Times New Roman",(Size.cy-30*7/16),char_length);
  char_height = (Size.cy-6)*13/16;
  font = new TFont("Arial",(Size.cy-30*7/16),char_length);
  char_length=char_length*2.2+2;
  InvalidateRgn(NULL,TRUE);
  UpdateWindow();
  starting_sizes[Size_Number][0] = Attr.W+8;
  starting_sizes[Size_Number][1] = Attr.H+27;
}

void TCallSign::Paint(TDC& DC, BOOL,TRect &)

{
  int count;
  char string[2] = " ";

  DC.SelectObject(*font);
  Get_CallSign(CallSign);
  for(count=0;count<strlen(CallSign);count++)
  {
	 string[0] = CallSign[count];
	 DC.TextOut(5+(char_length+1)*count,6,string,1);
  }
  DC.RestoreObjects();
}

TCallSign::Where_X()

{
  return Attr.X+4;
}

TCallSign::Where_Y()

{
  return(Attr.Y+41);
}

TCallSign::Width()

{
  return(Attr.W+8);
}

TCallSign::Height()

{
  return(Attr.H+27);
}

void TCallSign::EvChar(UINT key, UINT repeatCount,UINT flags)

{
  if (key == 'm' || key == 'M')
  {
	 ClipCursor(NULL);
	 ShowCursor(0);
	 Attr.H = -1;
	 while (ShowCursor(1)<0)
		{}
  }
}

void TCallSign::EvLButtonDown(UINT, TPoint &)

{
  char string[80];

  Get_CallSign(string);
  if (TInputDialog(this,"Callsign","Please enter your callsign",
               string,15).Execute()==IDOK)
    Update_CallSign(string);
}

void TCallSign::Update_CallSign(char *string)

{
  Set_CallSign(string);
  InvalidateRgn(NULL,1);
  TSize Size(Attr.W,Attr.H);
  EvSize(NULL,Size);
}

DEFINE_RESPONSE_TABLE1(TCallSign,AWindow)
  EV_WM_LBUTTONDOWN,
  EV_WM_SIZE,
  EV_WM_CHAR,
END_RESPONSE_TABLE;
