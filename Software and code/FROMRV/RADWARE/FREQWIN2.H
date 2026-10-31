#ifndef __FREQWIN2__H
#define __FREQWIN2__H

_CLASSDEF(TFreq_Window2);

class TFreq_Window2: public AWindow

{
  public:
  HBRUSH hbr;
  int locked_mouse;
  int Color;
  int last_banded;
  int Size_Number;
  Frequency_Control *Control;
  long adjustment;
  int ID,resizable,center_x,center_y,max_x,max_y,digit;
  int box_height,box_length,char_length,char_height,old_mouse_x,old_mouse_y;
  char last_frequency[15];
  TButton *button,*equal_button;
  TDC *DC;
  TFont *font;

  TFreq_Window2(TWindow *AParent,int ID, LPSTR AName,Frequency_Control *,
                    int child_number,int size_number,int Resizable,int color);
  ~TFreq_Window2();
  virtual LPSTR GetClassName();
  virtual void GetWindowClass(WNDCLASS _FAR & );
  void EvSize(UINT modKeys,TSize &Size);
  void UpdateAll(void);
  void Set_Value(long freq);
  void Set_Equal();
  void Paint(TDC& DC, BOOL,TRect &);
  void Redraw_Frequency();
  void EvLButtonUp(UINT,TPoint&);
  void EvLButtonDown(UINT,TPoint&);
  void EvRButtonDown(UINT,TPoint&);
  void EvMouseMove(UINT,TPoint&);
  void EvActivate(UINT,BOOL,HWND);
  void CallEnterDlg();
  int Width();
  int Height();
  void Check_For_Repaint();
  void EvKeyDown(UINT key, UINT repeatCount,UINT flags);
  DECLARE_RESPONSE_TABLE(TFreq_Window2);
};

#endif // __FREQWIN2_H

