#include "awindow.h"
#include "stdio.h"


extern int starting_places[][2];
extern int delete_class[];

AWindow::AWindow(TWindow *AParent, LPSTR AName,int child_number)
		:TFrameWindow(AParent, AName)

{
  Child_Number = child_number;
  delete_class[child_number] = 0;
}

AWindow::~AWindow()

{
  if (Child_Number!=-1)
  {
    starting_places[Child_Number][0] = Attr.X;
    starting_places[Child_Number][1] = Attr.Y;
    delete_class[Child_Number] = 1;
  }
}

void AWindow::EvMove(TPoint &point)

{
  TWindow::EvMove(point);
  if (Child_Number!=-1)
  {
    starting_places[Child_Number][0] = Attr.X;
    starting_places[Child_Number][1] = Attr.Y;
  }
}

void AWindow::EvActivate(UINT active,BOOL b,HWND c)

{
  if (active)
  {
    BringWindowToTop();
    this->Show(SW_SHOWNORMAL);
    if (Child_Number != -1)
      MoveWindowToTopOfList(Child_Number);
  }
  TWindow::EvActivate(active,b,c);
//  TWindow::WMActivate(Msg);
}

void AWindow::EvChar(uint key, uint repeatCount, uint flags)

{
//  MessageBox("A key has been pressed",NULL,MB_OK);
//  if (Keyboard(key,repeatCount,flags))
//    TFrameWindow::EvChar(key,repeatCount,flags);
}

void AWindow::Move(int x,int y,int w,int h)

{
  if (IsIconic())
	 OpenIcon(this->HWindow);
  MoveWindow(x,y,w,h,1);
//  BringWindowToTop();
}

AWindow::Where_X()

{
  return Attr.X;
}

AWindow::Where_Y()

{
  return (Attr.Y+41);
}

void AWindow::Redraw()

{
  InvalidateRgn(NULL,0);
  UpdateWindow();
}

DEFINE_RESPONSE_TABLE1(AWindow,TFrameWindow)
  EV_WM_ACTIVATE,
  EV_WM_CHAR,
  EV_WM_MOVE,
END_RESPONSE_TABLE;

ABitmapButton::ABitmapButton(TWindow *AParent, int id,LPSTR name,int x,int y,
                             int w,int h,TBitmap *Unpressed,TBitmap *Pressed):AWindow(AParent,name,-1)

{
  button_pressed = 0;
  pressed = Pressed;
  unpressed = Unpressed;
  ID = id;
  Attr.Style = WS_CHILD | WS_VISIBLE;
  Attr.X = x;
  Attr.Y = y;
  Attr.W = w;
  Attr.H = h;
}

void ABitmapButton::Paint(TDC& DC, BOOL,TRect &)

{
  TMemoryDC *MDC;

  MDC = new TMemoryDC(DC);
  if (button_pressed)
    MDC->SelectObject(*pressed);
  else
    MDC->SelectObject(*unpressed);
  DC.BitBlt(0,0,20,24,*MDC,0,0,SRCCOPY);
  MDC->RestoreObjects();
  delete (MDC);
}

void ABitmapButton::EvLButtonDown(UINT,TPoint&)

{
  button_pressed = 1;
  Redraw();
}

void ABitmapButton::EvLButtonUp(UINT,TPoint&)

{
  button_pressed = 0;
  Redraw();
  ::PostMessage(Parent->HWindow,WM_COMMAND,ID,NULL);
}

DEFINE_RESPONSE_TABLE1(ABitmapButton,TWindow)
  EV_WM_LBUTTONUP,
  EV_WM_LBUTTONDOWN,
END_RESPONSE_TABLE;

AEdit::AEdit(TWindow *AParent, int id,LPSTR name,int x,int y,int w,int h)
		:TEdit(AParent, id,name,x,y,w,h,80)

{
  ID = id;
}

AEdit::AEdit(TWindow *AParent, int id)
		:TEdit(AParent, id, 80)

{
  ID = id;
}

int AEdit::Get_Value()

{
  char string[80];

  GetText(string,79);
  return atoi(string);
}

long AEdit::Get_Value_Long()

{
  char string[80];

  GetText(string,79);
  return atol(string);
}

double AEdit::Get_Value_Double()

{
  char string[80];

  GetText(string,79);
  return atof(string);
}

void AEdit::Set_Value(int value)

{
  char string[80];

  sprintf(string,"%d",value);
  SetText(string);
}

void AEdit::Set_Value(long value)

{
  char string[80];

  sprintf(string,"%ld",value);
  SetText(string);
}


//#include "thisCls.h"

AButton::AButton(TWindow *AParent, int id, LPSTR AName,int x,int y,
                             int w,int h,BOOL IsDefault):
                             TButton(AParent,-1,AName,x,y,w,h,IsDefault)

{
  ID = id;
}

AButton::~AButton()

{
}

char far* AButton::GetClassName()

{
  return "BUTTON";
}

void AButton::EvRButtonUp(UINT modKeys,TPoint& point)

{
  ::PostMessage(Parent->HWindow,WM_COMMAND,ID+100,NULL);
  TButton::EvRButtonUp(modKeys,point);
  SetFocus();
}

void AButton::EvLButtonUp(UINT modKeys,TPoint& point)

{
  ::PostMessage(Parent->HWindow,WM_COMMAND,ID,NULL);
  TButton::EvLButtonUp(modKeys,point);
}

DEFINE_RESPONSE_TABLE1(AButton,TButton)
  EV_WM_RBUTTONUP,
  EV_WM_LBUTTONUP,
END_RESPONSE_TABLE;

WhiteStatic::WhiteStatic(TWindow* parent, int ID, UINT length):
                              TStatic(parent,ID,length)

{
}

WhiteStatic::WhiteStatic(TWindow* parent, int ID, const char far* title,
                         int x,int y,int w,int h, UINT length):
                         TStatic(parent,ID,title,x,y,w,h,length)

{
}

LPSTR WhiteStatic::GetClassName()

{
  return("WhiteStatic");
}

void WhiteStatic::GetWindowClass(WNDCLASS _FAR & AWndClass)

{
  TWindow::GetWindowClass(AWndClass);
  hbr = CreateSolidBrush(RGB(255,255,255));
  AWndClass.hbrBackground = hbr;
}

void WhiteStatic::Paint(TDC& DC, BOOL,TRect &)

{
  int count;
  char string[80];

  TStatic::GetText(string,79);
  DC.TextOut(0,0,string,strlen(string));
  DC.RestoreObjects();
}

void print_int(int i)

{
  char string[80];

  sprintf(string,"%d",i);
  MessageBox(NULL,string,"Print Int",MB_OK);
}


