#ifndef __TALL_CHANNELS__H
#define __TALL_CHANNELS__H

_CLASSDEF(TALL_CHANNELSChld)

class TALL_CHANNELSChld : public AWindow
{
  int id;
  int Child_Number;
  AEdit *Min_dB;
  AEdit *Max_dB;
  AEdit *Band;
  AEdit *Channel;
  AEdit *Listen_Time;
  AEdit *Hold_Time;
  AButton *Channel_Buttons[81];
  TCheckBox *Infinity;
  TCheckBox *Selection[MAX_QUICK_STATIONS];

public:
  TALL_CHANNELSChld::TALL_CHANNELSChld(AWindow *AParent,int ID, LPSTR AName,int child_number);
  ~TALL_CHANNELSChld();
  void GetWindowClass(WNDCLASS _FAR & AWndClass);
  void TALL_CHANNELSChld::EvActivate(uint,bool,HWND);
  LPSTR GetClassName();
  void SetupWindow();

  int Where_Y();
  void ReturnFocus();
//  void Paint(TDC& DC, BOOL,TRect &);
  void EvTimer(UINT);
  void EvMove(TPoint &point);
  void EvChar(UINT key, UINT repeatCount,UINT flags);
  void Get_Channel(WPARAM);
  void Set_Channel(WPARAM);
  void Get_Selected();
  void Update_Channel_Names();
  DECLARE_RESPONSE_TABLE(TALL_CHANNELSChld);
};
#endif // __TALL_CHANNELS_H

