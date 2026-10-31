#ifndef __ENTER_FREQUENCYDlg__H
#define __ENTER_FREQUENCYDlg__H

_CLASSDEF(TENTER_FREQUENCYDlg)

class TENTER_FREQUENCYDlg : public TDialog

{
public:
  Frequency_Control *Control;
  TEdit *Edit117;
  TButton *MHz_Button;

  TENTER_FREQUENCYDlg(TWindow *AParent, LPSTR AName,Frequency_Control *control);
  ~TENTER_FREQUENCYDlg();
  virtual void RTENTER_FREQUENCY102();
  virtual void RTENTER_FREQUENCY103();
  virtual void RTENTER_FREQUENCY104();
  virtual void RTENTER_FREQUENCY105();
  virtual void RTENTER_FREQUENCY106();
  virtual void RTENTER_FREQUENCY107();
  virtual void RTENTER_FREQUENCY108();
  virtual void RTENTER_FREQUENCY109();
  virtual void RTENTER_FREQUENCY110();
  virtual void RTENTER_FREQUENCY111();
  virtual void RTENTER_FREQUENCY112();
  void Hz();
  void KHz();
  void MHz();
  void Close();
  void Check_Edit();
  DECLARE_RESPONSE_TABLE(TENTER_FREQUENCYDlg);
};
#endif // __ENTER_FREQUENCYDlg_H

