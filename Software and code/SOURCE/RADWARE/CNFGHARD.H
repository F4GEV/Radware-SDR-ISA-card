#ifndef __TCONFIGURE_HARDWARE__H
#define __TCONFIGURE_HARDWARE__H

_CLASSDEF(TCONFIGURE_HARDWAREDlg)

class TCONFIGURE_HARDWAREDlg : public TDialog

{
  TRadioButton *DSP_100,*DSP_1D;
  TCheckBox *PTT_Joystick;
  TRadioButton *No_Key,*One_Key,*Two_Keys;
  TRadioButton *Default_DSP_Card,*WinWave_Card;
  TScrollBar *Scroll[8];
  TFreq_Window2 *PTI_ADJUST;
  TFreq_Window2 *PTTX_I_ADJUST;
  AEdit *Address;
  TEdit *callsign;
  AEdit *I_Adjust;
  AEdit *TX_I_Adjust;
  int ignore_address;
  
public:
  TCONFIGURE_HARDWAREDlg::TCONFIGURE_HARDWAREDlg(TWindow *AParent, LPSTR AName);
  ~TCONFIGURE_HARDWAREDlg();

  virtual void SetupWindow();
  virtual void OK();
  void TCONFIGURE_HARDWAREDlg::EvHScroll(UINT,UINT,HWND);
  void DIP_SWITCH();
  void ADDRESS();
/*  void I_ADJUST_WINDOW();
  void TX_I_ADJUST_WINDOW();
  void UPDATE_I_ADJUST();
  void UPDATE_TX_I_ADJUST();
*/  DECLARE_RESPONSE_TABLE(TCONFIGURE_HARDWAREDlg);
};
#endif

