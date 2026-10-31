#ifndef __TDIAGNOSTIC__H
#define __TDIAGNOSTIC__H

_CLASSDEF(TDIAGNOSTICDlg)

class TDIAGNOSTICDlg : public TDialog

{
  TStatic *Static_1848[22];

public:
  TDIAGNOSTICDlg(TWindow *AParent, LPSTR AName);
  ~TDIAGNOSTICDlg();

  void Request_Update();
  void Recieved_1848();
  DECLARE_RESPONSE_TABLE(TDIAGNOSTICDlg);
};
#endif

