#include "thisID.h"
#include "thisCls.h"
#include <stdio.h>
#include <math.h>

//REGEN_VARIABLES
extern int starting_places[][2];
extern int delete_class[];

extern TBitmap *knobs[];
extern TCONTROLChld *PTAGC,*PTFilter;
extern int knob_speed;

WORD control_timer_message;
TPoint control_timer_point;
double pi = 3.14159;

//REGEN_VARIABLES

TCONTROLChld::TCONTROLChld(AWindow *AParent,int ID,LPSTR AName,int child_number,
               Radio_Control *control,char labels[][20],int label_quantity,
               char icon[],int Wrap)
		     :AWindow(AParent,AName,child_number)
{
  int x,y;

  Child_Number = child_number;
  x = starting_places[Child_Number][0];
  y = starting_places[Child_Number][1];
  wrap = Wrap;
  strcpy(icon_name,icon);
  strcpy(name,AName);
  Attr.Style = WS_POPUP | WS_VISIBLE | WS_CAPTION |WS_MINIMIZEBOX|WS_SYSMENU;
  Attr.X = x;
  if (y<0)
    Attr.Y = -y;
  else
    Attr.Y = y;
  Attr.W = 140;
  Attr.H = 101;
  mute = 0;
  id = ID;
  switch (label_quantity)
  {
    case -64 : label_quantity = 64;
               buttons[0] = new TButton(this,200,labels[0],0,0,50,18,FALSE);
               break;
    case 2   : buttons[0] = new TButton(this,101,labels[0],10,0,46,18,FALSE);
               buttons[1] = new TButton(this,102,labels[1],81,0,46,18,FALSE);
               break;
    case 3   : buttons[0] = new TButton(this,101,labels[0],6,15,36,18,FALSE);
               buttons[1] = new TButton(this,102,labels[1],51,0,36,18,FALSE);
               buttons[2] = new TButton(this,103,labels[2],96,15,36,18,FALSE);
               break;
    case 4   : buttons[0] = new TButton(this,101,labels[0],1,27,36,18,FALSE);
               buttons[1] = new TButton(this,102,labels[1],20,0,36,18,FALSE);
               buttons[2] = new TButton(this,103,labels[2],81,0,36,18,FALSE);
               buttons[3] = new TButton(this,104,labels[3],101,27,36,18,FALSE);
               break;
    case 5   : buttons[0] = new TButton(this,101,labels[0],0,45,34,18,FALSE);
               if (strlen(labels[1])>3)
 			        buttons[1] = new TButton(this,102,labels[1],0,15,42,18,FALSE);
 			      else
                 buttons[1] = new TButton(this,102,labels[1],8,15,34,18,FALSE);
 			      buttons[2] = new TButton(this,103,labels[2],52,0,34,16,FALSE);
               buttons[3] = new TButton(this,104,labels[3],96,15,34,18,FALSE);
               buttons[4] = new TButton(this,105,labels[4],104,45,34,18,FALSE);
               break;
    case 6   : buttons[0] = new TButton(this,101,labels[0],2,30,32,16,FALSE);
               buttons[1] = new TButton(this,102,labels[1],10,15,32,16,FALSE);
			      buttons[2] = new TButton(this,103,labels[2],26,0,32,16,FALSE);
				   buttons[3] = new TButton(this,104,labels[3],80,0,32,16,FALSE);
				   buttons[4] = new TButton(this,105,labels[4],96,15,32,16,FALSE);
				   buttons[5] = new TButton(this,106,labels[5],104,30,32,16,FALSE);
				   break;
    case 8   : buttons[0] = new TButton(this,101,labels[0],2,45,32,16,FALSE);
               buttons[1] = new TButton(this,102,labels[1],2,30,32,16,FALSE);
               buttons[2] = new TButton(this,103,labels[2],10,15,32,16,FALSE);
               buttons[3] = new TButton(this,104,labels[3],26,0,32,16,FALSE);
               buttons[4] = new TButton(this,105,labels[4],80,0,32,16,FALSE);
               buttons[5] = new TButton(this,106,labels[5],96,15,32,16,FALSE);
               buttons[6] = new TButton(this,107,labels[6],104,30,32,16,FALSE);
               buttons[7] = new TButton(this,108,labels[7],104,45,32,16,FALSE);
               break;
    case 9   : buttons[0] = new TButton(this,101,labels[0],2,45,32,16,FALSE);
               buttons[1] = new TButton(this,102,labels[1],2,30,32,16,FALSE);
               buttons[2] = new TButton(this,103,labels[2],10,15,32,16,FALSE);
               buttons[3] = new TButton(this,104,labels[3],26,0,30,16,FALSE);
               buttons[4] = new TButton(this,105,labels[4],56,0,24,16,FALSE);
               buttons[5] = new TButton(this,106,labels[5],80,0,32,16,FALSE);
               buttons[6] = new TButton(this,107,labels[6],96,15,32,16,FALSE);
               buttons[7] = new TButton(this,108,labels[7],104,30,32,16,FALSE);
               buttons[8] = new TButton(this,109,labels[8],104,45,32,16,FALSE);
               break;
  }
  Control = control;
  max_labels = label_quantity;
}

TCONTROLChld::~TCONTROLChld()

{
  Attr.Y *= -1;
  if (Attr.Y>=0)
    Attr.Y = -1;
  Attr.H = Attr.W = -1;
  starting_places[Child_Number][0] = Attr.X;
  starting_places[Child_Number][1] = Attr.Y;
  delete_class[Child_Number] = 1;
}
/*
void TCONTROLChld::SetupWindow()

{
//  SetParent(NULL);
}
*/
void TCONTROLChld::Relabel_Buttons(char labels[][20])

{
  int count;

  if (max_labels<13)
    for(count=0;count<max_labels;count++)
      buttons[count]->SetCaption(labels[count]);
}

void TCONTROLChld::EvActivate(uint active,bool b,HWND c)

{
  if (active != WA_INACTIVE)
    MoveWindowToTopOfList(Child_Number);
  AWindow::EvActivate(active,b,c);
//  PostMessage(WM_COMMAND,201,NULL);
}

void TCONTROLChld::ReturnFocus()

{
  ::SetFocus(Parent->HWindow);
}

LPSTR TCONTROLChld::GetClassName()

{
  return(name);
}

void TCONTROLChld::GetWindowClass(WNDCLASS _FAR & AWndClass)

{
  AWindow::GetWindowClass(AWndClass);
  hbr = CreateSolidBrush(RGB(255,255,255));
  AWndClass.hbrBackground = hbr;
}

void TCONTROLChld::Button1()

{
  Set_Value(0);
}

void TCONTROLChld::Button2()

{
  Set_Value(1);
}

void TCONTROLChld::Button3()

{
  Set_Value(2);
}

void TCONTROLChld::Button4()

{
  Set_Value(3);
}

void TCONTROLChld::Button5()

{
  Set_Value(4);
}

void TCONTROLChld::Button6()

{
  if (Control->Get_Max()>=5)
  {
	 Set_Value(5);
  }
}

void TCONTROLChld::Button7()

{
  Set_Value(6);
}

void TCONTROLChld::Button8()

{
  Set_Value(7);
}

void TCONTROLChld::Button9()

{
  Set_Value(8);
}

void TCONTROLChld::Button10()

{
  Set_Value(9);
}

void TCONTROLChld::ButtonMute()

{
  if (mute)
  {
	 Control->Restore();
	 mute = 0;
  }
  else
  {
    Control->Mute();
    mute = 1;
  }
}

TCONTROLChld::Where_Y()

{
  return(Attr.Y);
}

void TCONTROLChld::EvMove(TPoint &point)

{
  AWindow::EvMove(point);
  starting_places[Child_Number][0] = Attr.X;
  starting_places[Child_Number][1] = Attr.Y;
}

void TCONTROLChld::EvChar(UINT key, UINT repeatCount,UINT flags)

{
  if (key == 13)
  {
    ClipCursor(NULL);
    ShowCursor(0);
    Attr.H = -1;
    while (ShowCursor(1)<0)
      {}
  }
}

void TCONTROLChld::Check_For_Repaint()

{
  if (last_value != Control->Get_Value())
  {
    InvalidateRgn(NULL,0);
    UpdateWindow();
  }
}

void TCONTROLChld::Paint(TDC& DC, BOOL,TRect &)

{
  TMemoryDC *MDC;

  MDC = new TMemoryDC(DC);
  switch (max_labels)
  {
	 case 2  : MDC->SelectObject(*knobs[Control->Get_Value()*4+6]);
				  break;
    case 3  : MDC->SelectObject(*knobs[Control->Get_Value()*4+4]);
              break;
	 case 5  : MDC->SelectObject(*knobs[Control->Get_Value()*4]);
				  break;
	 case 6  : if (Control->Get_Value()>2)
					 MDC->SelectObject(*knobs[Control->Get_Value()*2+4]);
				  else
					 MDC->SelectObject(*knobs[Control->Get_Value()*2+2]);
				  break;
	 case 8  : if (Control->Get_Value()>3)
					 MDC->SelectObject(*knobs[Control->Get_Value()*2+2]);
              else
                MDC->SelectObject(*knobs[Control->Get_Value()*2]);
              break;
    case 9  : MDC->SelectObject(*knobs[Control->Get_Value()*2]);
              break;
    case 16 : MDC->SelectObject(*knobs[Control->Get_Value()]);
				  break;
    case 37 : MDC->SelectObject(*knobs[Control->Get_Value()*17/37]);
              break;
	 case 64 : MDC->SelectObject(*knobs[Control->Get_Value()*17/64]);
              break;
    case 80 : MDC->SelectObject(*knobs[Control->Get_Value()*17/80]);
              break;
    default : MDC->SelectObject(*knobs[Control->Get_Value()*4+2]);
  }
  DC.BitBlt(37,20,64,64,*MDC,0,0,SRCCOPY);
  MDC->RestoreObjects();
  last_value = Control->Get_Value();
  delete (MDC);
}

void TCONTROLChld::EvLButtonDown(UINT,TPoint& point)

{
  int x, y, AGC, control,old_agc,old_filter;
  HRGN region;
  long step;
  double tangent;
  int settings;

  x = point.x;
  y = point.y;
  if (control_timer_message == 0 && max_labels > 2)
  {
	 SetTimer(1,knob_speed,NULL);
	 control_timer_message = WM_LBUTTONDOWN;
	 control_timer_point = point;
  }
  if (sqrt(pow(66-x,2)+pow(46-y,2))<=21)
  {
	 control = Control->Get_Value();
	 if (--control<0)
		if (wrap)
        control = Control->Get_Max();
      else
        control = Control->Get_Min();
	 Set_Value(control);
	 region = CreateRectRgn(37,20,107,84);
	 InvalidateRgn(region,0);
	 DeleteObject(region);
	 UpdateWindow();
  }
  else
  {
	 if (y<=46 && max_labels>10)
	 {
		if (x!=66)
		  tangent = atan((y-46.0)/(x-66));
		else
		  tangent = pi/2;
		if (tangent<0)
		  tangent+=pi;
		settings = tangent*max_labels/pi;
		if (x>66 && settings == 0)
		  settings = max_labels-1;
		Set_Value(settings);
		region = CreateRectRgn(37,15,107,78);
		InvalidateRgn(region,0);
		DeleteObject(region);
		UpdateWindow();
	 }
  }
}

void TCONTROLChld::EvRButtonDown(UINT,TPoint& point)

{
  int x, y,control;
  HRGN region;
  long step;

  x = point.x;
  y = point.y;
  if (control_timer_message == 0 && max_labels > 2)
  {
	 SetTimer(1,knob_speed,NULL);
	 control_timer_message = WM_RBUTTONDOWN;
	 control_timer_point = point;
  }
  if (sqrt(pow(66-x,2)+pow(46-y,2))<=21)
  {
	 control = Control->Get_Value();
    if (++control>Control->Get_Max())
      if (wrap)
        control = Control->Get_Min();
      else
        control = Control->Get_Max();
	 Set_Value(control);
	 region = CreateRectRgn(37,15,107,78);
    InvalidateRgn(region,0);
    DeleteObject(region);
    UpdateWindow();
  }
}

void TCONTROLChld::EvTimer(UINT)

{
  if (control_timer_message == WM_LBUTTONDOWN)
	 EvLButtonDown(NULL,control_timer_point);
  if (control_timer_message == WM_RBUTTONDOWN)
	 EvRButtonDown(NULL,control_timer_point);
}

void TCONTROLChld::EvLButtonUp(UINT,TPoint&)

{
  control_timer_message = 0;
  KillTimer(1);
}

void TCONTROLChld::EvRButtonUp(UINT,TPoint&)

{
  control_timer_message = 0;
  KillTimer(1);
}

int TCONTROLChld::Set_Value(int value)

{
  int answer;
  int old_agc,old_filter;

  old_agc = get_agc();
  old_filter = get_filter();
  answer = Control->Set_Value(value);
  if (get_agc() != old_agc)
  {
    ::InvalidateRgn(PTAGC->HWindow,NULL,0);
    ::UpdateWindow(PTAGC->HWindow);
  }
  if (get_filter() != old_filter)
  {
    ::InvalidateRgn(PTFilter->HWindow,NULL,0);
    ::UpdateWindow(PTFilter->HWindow);
  }
  ::PostMessage(Parent->HWindow,WM_COMMAND,id,0);
  InvalidateRgn(NULL,0);
  UpdateWindow();
  return answer;
}

DEFINE_RESPONSE_TABLE1(TCONTROLChld,TFrameWindow)
  EV_COMMAND(101,Button1),
  EV_COMMAND(102,Button2),
  EV_COMMAND(103,Button3),
  EV_COMMAND(104,Button4),
  EV_COMMAND(105,Button5),
  EV_COMMAND(106,Button6),
  EV_COMMAND(107,Button7),
  EV_COMMAND(108,Button8),
  EV_COMMAND(109,Button9),
  EV_COMMAND(110,Button10),
  EV_COMMAND(200,ButtonMute),
  EV_COMMAND(201,ReturnFocus),
  EV_WM_ACTIVATE,
  EV_WM_LBUTTONDOWN,
  EV_WM_LBUTTONUP,
  EV_WM_RBUTTONDOWN,
  EV_WM_RBUTTONUP,
  EV_WM_TIMER,
  EV_WM_MOVE,
END_RESPONSE_TABLE;

