	  /********************************************************************
		*                                                                  *
		*   Source File: Frequency_Window.cpp                                          *
		*   Description: Dialog implementation for PRORADIO                *
		*   Date:        Tue Sep 15 12:37:33 1992                          *
		*                                                                  *
		********************************************************************/

#include "thisCls.h"
#include <math.h>;
#include <stdio.h>;

extern int starting_places[][2];
extern int delete_class[];

TPTTChld::TPTTChld(TWindow *AParent,int id, LPSTR AName,int child_number,
						 Radio_Control *control,char *)
                   : AWindow(AParent, AName, child_number)

{
  int x,y;

  x = starting_places[child_number][0];
  y = starting_places[child_number][1];
  if (y<0)
    y = -y;
  Attr.X = x;
  Attr.Y = y;
  Attr.W = 140;
  Attr.H = 101;
  Attr.Style = WS_POPUP | WS_VISIBLE | WS_CAPTION |WS_MINIMIZEBOX|WS_SYSMENU;
  Control = control;
  PTT_Button = new TButton(this,101,"Transmit",0,0,138,81);
  ID = id;
}

TPTTChld::~TPTTChld()

{
  Attr.H *= -1;
}

LPSTR TPTTChld::GetClassName()

{
  return("PTT");
}

TPTTChld::Where_X()

{
  return Attr.X;
}

TPTTChld::Where_Y()

{
  return(Attr.Y);
}

void TPTTChld::TOGGLE_BUTTON()

{
  if (Control->Get_Value()==0)
  {
    Control->Set_Value(1);
    PTT_Button->SetCaption("Recieve");
  }
  else
  {
    Control->Set_Value(0);
    PTT_Button->SetCaption("Transmit");
  }
}

void TPTTChld::EvChar(UINT key, UINT,UINT)

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

DEFINE_RESPONSE_TABLE1(TPTTChld,TWindow)
  EV_COMMAND(101,TOGGLE_BUTTON),
  EV_WM_CHAR,
END_RESPONSE_TABLE;
