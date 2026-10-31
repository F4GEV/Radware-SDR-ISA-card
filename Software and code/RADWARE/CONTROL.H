#ifndef __TCONTROL__H
#define __TCONTROL__H

_CLASSDEF(TCONTROLChld)

class TCONTROLChld : public AWindow
{
  int mute;
  PRadio_Control Control;
  TButton *buttons[10];
  int max_labels;
  char name[80];
  char icon_name[20];
  long last_value;
  HBRUSH hbr;
  int id;
  int wrap;
  int Child_Number;

public:
  TCONTROLChld::TCONTROLChld(AWindow *AParent,int ID, LPSTR AName,int child_number,
        Radio_Control *control,char labels[][20],int label_quantity,char icon[],
        int Wrap);
  ~TCONTROLChld();
  void GetWindowClass(WNDCLASS _FAR & AWndClass);
  void TCONTROLChld::EvActivate(uint,bool,HWND);
  LPSTR GetClassName();
//   void SetupWindow();

  int Where_Y();
  void Relabel_Buttons(char labels[][20]);
  void ReturnFocus();
  void Paint(TDC& DC, BOOL,TRect &);
  void EvTimer(UINT);
  void EvMove(TPoint &point);
  void EvLButtonDown(UINT,TPoint& point);
  void EvLButtonUp(UINT,TPoint& point);
  void EvRButtonDown(UINT,TPoint& point);
  void EvRButtonUp(UINT,TPoint& point);
  void Button1();
  void Button2();
  void Button3();
  void Button4();
  void Button5();
  void Button6();
  void Button7();
  void Button8();
  void Button9();
  void Button10();
  void ButtonMute();
  int Set_Value(int value);
  void Check_For_Repaint();
  void EvChar(UINT key, UINT repeatCount,UINT flags);
  DECLARE_RESPONSE_TABLE(TCONTROLChld);
};
#endif // __TCONTROL_H

