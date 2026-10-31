//REGEN_FILEHEADING
//REGEN_FILEHEADING

     /********************************************************************
      *                                                                  *
      *   Source File: Frequency_Window.cpp                                          *
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
#include <owl\radiobut.h>
#include <owl\button.h>
#include <math.h>;
//REGEN_VARIABLES
//REGEN_VARIABLES

extern int leading_zeros;

_CLASSDEF(TFreqWindButton)

class TFreqWindButton : public TButton
{
  TFrequency_Window *Mommy;

public:
  TFreqWindButton(TFrequency_Window *parent, int id, const char far* title,
                  int x,int y,int w,int h,bool isDefault = 0,
                  TModule* module = 0);
  void EvKeyDown(UINT key, UINT repeatCount,UINT flags);
  ~TFreqWindButton();
  DECLARE_RESPONSE_TABLE(TFreqWindButton);
};

// Define TFrequency_Window, a TWindow constructor
TFrequency_Window::TFrequency_Window(TWindow *AParent,int id, LPSTR AName,PFrequency_Control control,
		int x,int y):TFrameWindow(AParent, AName)
{
  char string[21];

  Control = control;
  Attr.Style = WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE;
  Attr.X = x;
  Attr.Y = y;
  Attr.W = 160;
  Attr.H = 30;
  adjustment = 0;
  locked_mouse = 0;
  ID = id;
  new TFreqWindButton(this,111,Control->Get_Name(),5,5,60,20,FALSE);
}

TFrequency_Window::~TFrequency_Window()

{
}

LPSTR TFrequency_Window::GetClassName()

{
  return("FREQWIND");
}

void TFrequency_Window::GetWindowClass(WNDCLASS _FAR & AWndClass)

{
//  HBRUSH hbr;

  hbr = CreateSolidBrush(RGB(0,0,255));
  TWindow::GetWindowClass(AWndClass);
  AWndClass.hbrBackground = hbr;
//  DeleteObject(hbr);
}

void TFrequency_Window::CallEnterDlg()

{
  char name[30];

  if (strcmp(Control->Get_Name(),"Span")==0)
    strcpy(name,"Enter Frequency Span");
  else
  {
    strcpy(name,"Enter ");
    strcat(name,Control->Get_Name());
    strcat(name," Frequency");
  }
  GetModule()->ExecDialog(new TENTER_FREQUENCYDlg(this,name,Control));
  this->UpdateAll();
}

void TFrequency_Window::Paint(TDC &DC, BOOL,TRect &)

{
  char value[10];
  char one_number[2]=" ";
  int count,zeros;
  HRGN rgn;

  sprintf(value,"%09.6f",Control->Get_Value()/1000000.0);
  DC.SelectStockObject(BLACK_PEN);
  DC.MoveTo(70,3);
  DC.LineTo(155,3);
  DC.LineTo(155,25);
  DC.LineTo(70,25);
  DC.LineTo(70,3);
  rgn = CreateRectRgn(71,4,154,24);
  DC.SelectStockObject(WHITE_BRUSH);
  DC.PaintRgn(rgn);
  DeleteObject(rgn);
  zeros = leading_zeros;
  for(count=0;count<9;count++)
  {
    if (zeros)
      if (value[count] == '0' || value[count] == '.')
      {
        rgn = CreateRectRgn(95+(4)*count,6,95+(4)*(count+1),16);
        DC.PaintRgn(rgn);
        value[count] = ' ';
        DeleteObject(rgn);
      }
      else
        zeros = 0;
    one_number[0] = value[count];
    DC.TextOut(75+8*count,7,one_number,1);
  }
  DC.RestoreObjects();
}

void TFrequency_Window::EvLButtonUp(UINT,TPoint&)

{
  RECT edges;

  if (adjustment>0)
  {
    ClipCursor(NULL);
    while(ShowCursor(1)<0);
    ::GetWindowRect(HWindow,&edges);
    SetCursorPos(edges.left+old_mouse_x,edges.top+old_mouse_y);
    adjustment = 0;
    locked_mouse = 0;
  }
}

void TFrequency_Window::EvLButtonDown(UINT,TPoint& Point)

{
  RECT edges;
  int x;

  old_mouse_x = Point.x;
  old_mouse_y = Point.y;
  x = Point.x;
  if (x>75 && x<147)
  {
    ::GetWindowRect(HWindow,&edges);
    edges.left+=2;
    edges.top+=2;
    edges.right-=2;
    edges.bottom-=2;
    ClipCursor(&edges);
    ShowCursor(0);
    SetCursorPos(edges.left+this->Attr.W/2,edges.top+this->Attr.H/2);
    adjustment = 8-(x-75)/8;
    if (adjustment>6)
      adjustment--;
    adjustment = pow(10,adjustment);
  }
}

void TFrequency_Window::EvRButtonDown(UINT n,TPoint& Point)

{
  if (locked_mouse)
    EvLButtonUp(n,Point);
  else
  {
    locked_mouse = 1;
    EvLButtonDown(n,Point);
  }
}

void TFrequency_Window::EvMouseMove(UINT flags,TPoint& Point)

{
  RECT edges;
  int x,adjust = 1;
  int center = this->Attr.W/2;
  int amount;

  if ((flags & MK_LBUTTON) || locked_mouse)
  {
    x = Point.x;
//    y = Msg.LParam / 65536;
    if (x < center-5)
    {
      amount = (center-5-x)/12;
      if (amount<1)
        amount = 1;
      Control->Set_Value(Control->Get_Value()-adjustment*amount);
    }
    else
      if (x > center+5)
      {
        amount = (x-center-5)/12;
        if (amount<1)
          amount = 1;
        Control->Set_Value(Control->Get_Value()+adjustment*amount);
      }
      else
        adjust = 0;
    if(adjust)
    {
      ::GetWindowRect(HWindow,&edges);
      SetCursorPos(edges.left+Attr.W/2,edges.top+this->Attr.H/2);
      this->UpdateAll();
    }
  }
}

void TFrequency_Window::EvKeyDown(UINT key, UINT repeatCount,UINT flags)

{
  if (key == 'm' || key == 'M')
  {
    ClipCursor(NULL);
    ShowCursor(0);
    Attr.H = -1;
    while (ShowCursor(1)<0)
      {}
  }
  switch (key)
  {
    case 13       : CallEnterDlg();
                    break;
    case VK_LEFT  : if (adjustment*10<Control->Get_Max())
                    {
                      adjustment *= 10;
/*                      digit--;
                      if (digit == 1 || digit == 5 || digit == 9)
                        digit--;
*/                      InvalidateRgn(NULL,0);
                      UpdateWindow();
                    }
                    break;
    case VK_RIGHT : if (adjustment>1)
                    {
                      adjustment /= 10;
/*                      digit++;
                      if (digit == 1 || digit == 5 || digit == 9)
                        digit++;
*/                      InvalidateRgn(NULL,0);
                      UpdateWindow();
                    }
                    break;
    case VK_UP    : if (Control->Set_Value(Control->Get_Value()+adjustment))
                    {
                      UpdateAll();
                    }
                    break;
    case VK_DOWN  : if (Control->Set_Value(Control->Get_Value()-adjustment))
                    {
                      UpdateAll();
                    }
                    break;
  }
}

void TFrequency_Window::UpdateAll(void)

{
  HRGN Region;

  Region = CreateRectRgn(75,4,148,26);
  InvalidateRgn(Region,0);
  UpdateWindow();
  DeleteObject(Region);
  ::PostMessage(Parent->HWindow,WM_COMMAND,ID,0);
}

DEFINE_RESPONSE_TABLE1(TFrequency_Window,TWindow)
  EV_COMMAND(111,CallEnterDlg),
  EV_WM_LBUTTONDOWN,
  EV_WM_RBUTTONDOWN,
  EV_WM_LBUTTONUP,
  EV_WM_MOUSEMOVE,
  EV_WM_KEYDOWN,
END_RESPONSE_TABLE;

TFreqWindButton::TFreqWindButton(TFrequency_Window *parent, int id, const char far* title,
                         int x,int y,int w,int h,bool isDefault,
                         TModule* module):
                         TButton(parent,id,title,x,y,w,h,isDefault,module)

{
 Mommy = parent;
}

TFreqWindButton::~TFreqWindButton()

{
}

void TFreqWindButton::EvKeyDown(UINT key, UINT repeatCount,UINT flags)

{
  Mommy->EvKeyDown(key,repeatCount,flags);
  TButton::EvKeyDown(key,repeatCount,flags);
}

DEFINE_RESPONSE_TABLE1(TFreqWindButton,TButton)
  EV_WM_KEYDOWN,
END_RESPONSE_TABLE;

