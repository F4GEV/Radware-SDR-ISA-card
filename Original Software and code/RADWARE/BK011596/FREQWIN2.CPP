     /********************************************************************
      *                                                                  *
      *   Source File: Frequency_Window.cpp                                          *
      *   Description: Dialog implementation for PRORADIO                *
      *   Date:        Tue Sep 15 12:37:33 1992                          *
      *                                                                  *
      ********************************************************************/

#include "thisCls.h"
//#include "SWLIB.h"
#include <math.h>;
#include <stdio.h>;

#define HIGHLIGHTED_COLOR 200,0,0
extern int banded;
extern int leading_zeros;

extern int starting_places[][2];
extern int starting_sizes[][2];
extern int delete_class[];

extern PFrequency_Control frequency_control;
extern PTX_Frequency_Control tx_frequency_control;

void string_insert(char string[],char chr,int where);

TFreq_Window2::TFreq_Window2(TWindow *AParent,int id, LPSTR AName,
                                       Frequency_Control *control,int child_number,
                                       int size_number,int Resizable,int color):
                     AWindow(AParent, AName,child_number)

{
  char string[21];
  int x,y,w,h;

  Size_Number = size_number;
  x = starting_places[child_number][0];
  y = starting_places[child_number][1];
  w = starting_sizes[size_number][0];
  h = starting_sizes[size_number][1];
  if (y<0)
    y = -y;
  starting_places[child_number][1] = y;
  equal_button = NULL;
  Color = color;
  resizable = Resizable;
  Control = control;
  locked_mouse = 0;
  Attr.X = x;
  Attr.Y = y;
  digit = -1;
  box_height = 22;
  box_length = 125;
  center_y = y/2;
  if (h<0)
    h*=-1;
  if (h == 0)
    h = 80;
  last_banded = banded;
  strcpy(string,Control->Get_Name());
  if (resizable)
  {
    Attr.Style = WS_MINIMIZEBOX | WS_POPUP | WS_CLIPSIBLINGS | WS_VISIBLE |
                 WS_CAPTION | WS_THICKFRAME | WS_SYSMENU;
    if (w<50 || h<=25)
    {
      Attr.W = 200;
      Attr.H = 65;
    }
    else
    {
      Attr.W = w;
      Attr.H = h;
    }
    if (strcmp(string,"Freq. A") == 0 || strcmp(string,"Freq. B") == 0)
    {
      button = new TButton(this,101,string,5,center_y-25,80,25,FALSE);
      if (strcmp("Freq. A",string)==0)
        equal_button = new TButton(this,112,"A=B",5,center_y,80,25,FALSE);
      else
        equal_button = new TButton(this,112,"B=A",5,center_y,80,25,FALSE);
    }
    else
      button = new TButton(this,101,string,5,center_y-12,80,25,FALSE);
  }
  else
  {
    Attr.Style = WS_POPUP | WS_CLIPSIBLINGS | WS_VISIBLE |
                 WS_CAPTION | WS_SYSMENU;
    Attr.W = w;
    Attr.H = h;
    button = new TButton(this,101,string,5,5,60,20,FALSE);
  }
  adjustment = 0;
  ID = id;
  char_height = (h-6)*9/16;
  font = new TFont("Times New Roman",18,6);
}

TFreq_Window2::~TFreq_Window2()

{
  Attr.W += 8;
  Attr.H += 27;
  if (Attr.H<=0)
  {
    Attr.H = 100;
    Attr.W = 300;
  }
/*  starting_places[Child_Number][0] = Attr.X;
  starting_places[Child_Number][1] = Attr.Y;
*/  starting_sizes[Size_Number][0] = Attr.W;
  starting_sizes[Size_Number][1] = Attr.H;
  Attr.Y*=-1;
  font->~TFont();
  delete font;
}

LPSTR TFreq_Window2::GetClassName()

{
  switch (Color)
  {
    case 0 : return("FREQWIN2");
    case 1 : return("FREQWIN21");
    case 2 : return("FREQWIN22");
  }
}

void TFreq_Window2::EvSize(UINT modKeys,TSize &Size)

{
  AWindow::EvSize(modKeys,Size);
  max_y = Size.cy;
  center_y = (Size.cy-20)/2+10;
  box_height = Size.cy-6;
  box_length = Size.cx-100;
  Attr.H = Size.cy;
  Attr.W = Size.cx;
  char_length = (box_length-10)/19;
  font->~TFont();
  delete font;
//  font = new TFont("Times New Roman",(Size.cy-30*7/16),char_length);
  char_height = (Size.cy-6)*13/16;
  font = new TFont("Arial",(Size.cy-30*7/16),char_length);
  char_length = (box_length-10)/13-2;
  if (equal_button)
  {
    ::MoveWindow(button->HWindow,5,center_y-25,80,25,TRUE);
    ::MoveWindow(equal_button->HWindow,5,center_y,80,25,TRUE);
  }
  else
    ::MoveWindow(button->HWindow,5,center_y-12,80,25,TRUE);
  InvalidateRgn(NULL,TRUE);
  UpdateWindow();
  starting_sizes[Size_Number][0] = Attr.W+8;
  starting_sizes[Size_Number][1] = Attr.H+27;
}

void TFreq_Window2::GetWindowClass(WNDCLASS _FAR & AWndClass)

{
  switch (Color)
  {
    case 0 : hbr = CreateSolidBrush(RGB(255,0,0));
             break;
    case 1 : hbr = CreateSolidBrush(RGB(0,0,255));
             break;
    case 2 : hbr = CreateSolidBrush(RGB(0,128,0));
             break;
  }
  TWindow::GetWindowClass(AWndClass);
  AWndClass.hbrBackground = hbr;
}

void TFreq_Window2::CallEnterDlg()

{
  char name[30];

  if (strcmp(Control->Get_Name(),"Span")==0)
    strcpy(name,"Enter Frequency Span");
  else
  {
    strcpy(name,"Enter ");
    strcat(name,Control->Get_Name());
    if (name[strlen(name)-1]=='.')
      strcpy(name+strlen(name)-1,"uency");
    else
      if (name[strlen(name)-1]!='y')
        strcat(name," Frequency");
  }
  new TENTER_FREQUENCYDlg(this,name,Control)->Execute();
  InvalidateRgn(NULL,0);
  UpdateWindow();
  ::PostMessage(Parent->HWindow,WM_COMMAND,ID,0);
}

void TFreq_Window2::Set_Equal()

{
  if (strcmp(Control->Get_Name(),"Freq. A")==0)
    Control->Set_Value(tx_frequency_control->Get_Value());
  else
    Control->Set_Value(frequency_control->Get_Value());
  Redraw();
}

void TFreq_Window2::Paint(TDC& DC, BOOL,TRect &)

{
  char value[14];
  char one_number[2]=" ";
  int count,zeros;
  HRGN rgn;

  if (last_banded != banded)
    strcpy(last_frequency,"XXXXXXXXXXX");
  last_banded = banded;
  sprintf(value,"%010ld",Control->Get_Value());
  string_insert(value,',',1);
  string_insert(value,',',5);
  string_insert(value,',',9);
  DC.MoveTo(90,center_y-box_height/2);
  DC.LineTo(90+box_length,center_y-box_height/2);
  DC.LineTo(90+box_length,center_y+box_height/2);
  DC.LineTo(90,center_y+box_height/2);
  DC.LineTo(90,center_y-box_height/2);
  rgn = CreateRectRgn(91,center_y-box_height/2+1,90+box_length,center_y+box_height/2);
  DC.SelectStockObject(WHITE_BRUSH);
  DC.PaintRgn(rgn);
  DeleteObject(rgn);
  DC.SelectObject(*font);
  zeros = leading_zeros;
  for(count=2;count<13;count++)
  {
    if (zeros)
      if (value[count] == '0' || value[count] == ',')
      {
        rgn = CreateRectRgn(95+(char_length+2)*count,6,95+(char_length+2)*(count+1),
                            char_height+6);
        DC.PaintRgn(rgn);
        value[count] = ' ';
        DeleteObject(rgn);
      }
      else
      {
        DC.SetTextColor(TColor(0,0,0));
        zeros = 0;
      }
    one_number[0] = value[count];
    if (count == digit)
      DC.SetTextColor(TColor(HIGHLIGHTED_COLOR));
    else
      if (banded)
        DC.SetTextColor(TColor(0,0,255));
      else
        DC.SetTextColor(TColor(0,0,0));
    if (one_number[0] == ',')
      DC.TextOut(97+(char_length+2)*count,6,one_number,1);
    else
      DC.TextOut(95+(char_length+2)*count,6,one_number,1);
  }
  DC.RestoreObjects();
  strcpy(last_frequency,value);
}

void TFreq_Window2::EvLButtonUp(UINT,TPoint& point)

{
  RECT edges;

  if (adjustment>0)
  {
//    ClipCursor(NULL);
    ReleaseCapture();
    ShowCursor(0);
	 while(ShowCursor(1)<0);
    adjustment = 0;
    ::GetWindowRect(HWindow,&edges);
    SetCursorPos(edges.left+old_mouse_x+4,edges.top+old_mouse_y+23);
    digit = -1;
    InvalidateRgn(NULL,0);
    UpdateWindow();
    locked_mouse = 0;
  }
}

void TFreq_Window2::Set_Value(long freq)

{
  Control->Set_Value(freq);
  Redraw_Frequency();
  ::PostMessage(Parent->HWindow,WM_COMMAND,ID,0);
}

void TFreq_Window2::Redraw_Frequency()

{
  DC = new TClientDC(*this);
  DC->SelectObject(*font);
  UpdateAll();
  DC->RestoreObjects();
  delete DC;
}

void TFreq_Window2::EvLButtonDown(UINT,TPoint& point)

{
  RECT edges;
  int x;

  old_mouse_x = point.x;
  old_mouse_y = point.y;
  x = point.x;
  if (x>95 && x<95+box_length)
  {
	 ::GetWindowRect(HWindow,&edges);
    edges.left+=5;
	 edges.top+=24;
	 edges.right-=5;
	 edges.bottom-=5;
//    ClipCursor(&edges);
	 SetCapture();
	 SetCursorPos((edges.left+edges.right)/2,edges.top+this->Attr.H/2);
	 adjustment = 12-(x-95)/(char_length+2);
	 digit = 12 - adjustment;
    if (digit == 1 || digit == 5 || digit == 9)
      digit++;
	 if (adjustment>10)
      adjustment--;
    if (adjustment>6)
      adjustment--;
    if (adjustment>2)
      adjustment--;
    adjustment = pow(10,adjustment);
	 if (adjustment!=0)
  		ShowCursor(0);
    InvalidateRgn(NULL,0);
	 UpdateWindow();
    ::PostMessage(Parent->HWindow,WM_COMMAND,ID,0);
  }
}

void TFreq_Window2::EvRButtonDown(UINT n,TPoint& point)

{
  if (locked_mouse)
    EvLButtonUp(n,point);
  else
  {
    locked_mouse = 1;
    EvLButtonDown(n,point);
  }
}

void TFreq_Window2::EvMouseMove(UINT flags,TPoint& Point)

{
  RECT edges;
  int x,adjust = 1;
  int center = this->Attr.W/2;
  int amount;
  int new_x;
  int display;
  int Post = 0;

  DC = new TClientDC(*this);
  DC->SelectObject(*font);
  if ((flags & MK_LBUTTON) || locked_mouse)
  {
    x = Point.x;
    if (x<center-1 || x>center+1)
	 {
      ::GetWindowRect(HWindow,&edges);
		SetCursorPos((edges.left+edges.right)/2,24+edges.top+this->Attr.H/2);
	 }
    if (x < center-1)
    {
      amount = (center-1-x)/2;
      if (amount<1)
        amount = 1;
      display = 9;
      while (amount>0)
      {
        if (Control->Set_Value(Control->Get_Value()-adjustment))
          Post = 1;
        if (--display == 0)
        {
          display = 9;
          UpdateAll();
        }
        amount--;
      }
    }
    else
		if (x > center+1)
      {
        amount = (x-center-1)/2;
        if (amount<1)
          amount = 1;
        display = 9;
        while (amount>0)
        {
          if (Control->Set_Value(Control->Get_Value()+adjustment))
            Post = 1;
          if (--display == 0)
          {
            UpdateAll();
            display = 9;
          }
          amount--;
        }
      }
      else
        adjust = 0;
    UpdateAll();
    if (Post)
      ::PostMessage(Parent->HWindow,WM_COMMAND,ID,0);
  }
  DC->RestoreObjects();
  delete DC;
}

void TFreq_Window2::EvActivate(UINT active,BOOL b,HWND h)

{
  if (active)
  {
    BringWindowToTop();
    this->Show(SW_SHOWNORMAL);
    if (Child_Number != -1)
      MoveWindowToTopOfList(Child_Number);
  }
  ShowCursor(0);
  while (ShowCursor(1)<0);
  TWindow::EvActivate(active,b,h);
}

void TFreq_Window2::Check_For_Repaint()

{
  char value[15];

  sprintf(value,"%010ld",Control->Get_Value());
  string_insert(value,',',1);
  string_insert(value,',',5);
  string_insert(value,',',9);
  if (strcmp(value,last_frequency)!=0 || banded != last_banded)
    Redraw_Frequency();
}

void TFreq_Window2::UpdateAll(void)

{
  HRGN Region;
  int count;
  char value[15];
  char one_number[2]="0";
  int zeros;
  int x;
  int blank_second_comma = 0;
  HRGN rgn;

  if (last_banded != banded)
    strcpy(last_frequency,"XXXXXXXXXXXXX");
  last_banded = banded;
  DC->SelectStockObject(WHITE_BRUSH);
  sprintf(value,"%010ld",Control->Get_Value());
  string_insert(value,',',1);
  string_insert(value,',',5);
  string_insert(value,',',9);
  zeros = leading_zeros;
  if (Control->Get_Value()<1000000l)
    blank_second_comma = 1;
  for(count=0;count<13;count++)
  {
    if ((value[count]!=last_frequency[count] && count>1) ||
        (count == 5 && blank_second_comma))
    {
      if (count == digit)
      {
        DC->SetTextColor(TColor(HIGHLIGHTED_COLOR));
        zeros = 0;
      }
      else
        if (zeros && (value[count] == '0' || value[count] == ','))
        {
          rgn = CreateRectRgn(95+(char_length+2)*count,6,95+(char_length+2)*(count+1),
                              char_height+6);
          DC->PaintRgn(rgn);
          value[count] = ' ';
          DeleteObject(rgn);
        }
        else
        {
          zeros = 0;
          if (banded)
            DC->SetTextColor(TColor(0,0,255));
          else
            DC->SetTextColor(TColor(0,0,0));
        }
      one_number[0] = value[count];
      DC->TextOut(95+(char_length+2)*count,6,one_number,1);
    }
    else
      if (zeros)
        if (value[count]!=' ' && value[count]!='0' && value[count]!=',')
          zeros = 0;
  }
  strcpy(last_frequency,value);
}

TFreq_Window2::Width()

{
  return(Attr.W+8);
}

TFreq_Window2::Height()

{
  return(Attr.H+27);
}

void TFreq_Window2::EvChar(UINT key, UINT repeatCount,UINT flags)

{
  if (key == 'm' || key == 'M')
  {
    ClipCursor(NULL);
    ShowCursor(0);
    Attr.H = -1;
    while (ShowCursor(1)<0)
      {}
  }
  if (key == 13)
    CallEnterDlg();
}

void string_insert(char string[],char chr,int where)

{
  char c1,c2;
  int count;

  count = where;
  c2 = chr;
  do
  {
    c1 = string[count];
    string[count] = c2;
    c2 = c1;
  }
  while (string[count++]!=0);
}

DEFINE_RESPONSE_TABLE1(TFreq_Window2,AWindow)
  EV_COMMAND(101,CallEnterDlg),
  EV_COMMAND(112,Set_Equal),
  EV_WM_SIZE,
  EV_WM_LBUTTONUP,
  EV_WM_LBUTTONDOWN,
  EV_WM_RBUTTONDOWN,
  EV_WM_MOUSEMOVE,
  EV_WM_CHAR,
  EV_WM_ACTIVATE,
END_RESPONSE_TABLE;