#ifndef __AWINDOW__H
#define __AWINDOW__H

#include <owl\owlall.h>
void MoveWindowToTopOfList(int which);

_CLASSDEF(AWindow)
class AWindow : public TFrameWindow
{
public:
  int Child_Number;

  AWindow(TWindow *AParent, LPSTR AName,int child_number);
  AWindow::~AWindow();
  void EvChar(uint key, uint repeatCount, uint flags);
  void Move(int x,int y,int w,int h);
  void EvActivate(UINT active,BOOL,HWND);
  void EvMove(TPoint &point);
  void Redraw();
  int Where_X();
  int Where_Y();
  DECLARE_RESPONSE_TABLE(AWindow);
};

int Keyboard(uint key, uint repeatCount, uint flags);

_CLASSDEF(ABitmapButton)

class ABitmapButton : AWindow

{
  int ID;
  TBitmap *pressed,*unpressed;
  int button_pressed;

  public:
  ABitmapButton(TWindow *AParent,int id,LPSTR Name,int x,int y,int w,int h,
                TBitmap *Unpressed,TBitmap *Pressed);
  void Paint(TDC& DC, BOOL,TRect &);
  void EvLButtonDown(UINT,TPoint&);
  void EvLButtonUp(UINT,TPoint&);
  DECLARE_RESPONSE_TABLE(ABitmapButton);
};

_CLASSDEF(AEdit)
class AEdit : public TEdit

{
  int ID;
public:
  AEdit(TWindow *AParent, int id);
  AEdit(TWindow *AParent,int id,LPSTR name,int x,int y,int w,int h);
  void Set_Value(int);
  void Set_Value(long);
  int Get_Value();
  long Get_Value_Long();
  double Get_Value_Double();

};

_CLASSDEF(AButton)

class AButton : public TButton

{
  int ID;
public:
  AButton(TWindow *AParent, int id, LPSTR AName,int x,int y,
        int w,int h,BOOL IsDefault);
  ~AButton();
  char far* GetClassName();
  void EvRButtonUp(UINT modKeys,TPoint& point);
  void EvLButtonUp(UINT modKeys,TPoint& point);
  DECLARE_RESPONSE_TABLE(AButton);
};

_CLASSDEF(WhiteStatic)
class WhiteStatic : public TStatic

{
  HBRUSH hbr;
  
public:
  WhiteStatic(TWindow *AParent, int id,UINT length);
  WhiteStatic(TWindow* parent, int ID, const char far* title,
                         int x,int y,int w,int h, UINT length);
  LPSTR GetClassName();
  void GetWindowClass(WNDCLASS _FAR & AWndClass);
  void Paint(TDC& DC, BOOL,TRect &);
};

void print_int(int);

#endif // __AWINDOW_H

