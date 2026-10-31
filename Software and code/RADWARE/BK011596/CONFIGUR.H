#ifndef __TCONFIGURE__H
#define __TCONFIGURE__H

_CLASSDEF(TCONFIGUREDlg)

class TCONFIGUREDlg : public TDialog

{
  TCheckBox *Leading_Zeros;
  TCheckBox *Frequency_Wrap;
  TCheckBox *Stations_Reset_Bands;
  TCheckBox *PTT_Joystick;
  TCheckBox *Save_On_Exit;
  TCheckBox *Transmit_Filters;
  TRadioButton *Sound_220,*Sound_240;
  TRadioButton *No_Key,*One_Key,*Two_Keys;
  TScrollBar *Scroll[8];
  TFreq_Window2 *PTI_ADJUST;
  TFreq_Window2 *PTTX_I_ADJUST;
  AEdit *Address;
  TEdit *callsign;
  AEdit *I_Adjust;
  AEdit *TX_I_Adjust;
  int ignore_address;
  
public:
  TCONFIGUREDlg::TCONFIGUREDlg(TWindow *AParent, LPSTR AName);
  ~TCONFIGUREDlg();

  virtual void SetupWindow();
  virtual void OK();
  void TCONFIGUREDlg::EvHScroll(UINT,UINT,HWND);
  void DIP_SWITCH();
  void ADDRESS();
  void I_ADJUST_WINDOW();
  void TX_I_ADJUST_WINDOW();
  void UPDATE_I_ADJUST();
  void UPDATE_TX_I_ADJUST();
  DECLARE_RESPONSE_TABLE(TCONFIGUREDlg);
};
#endif // __TCONTROL_H

