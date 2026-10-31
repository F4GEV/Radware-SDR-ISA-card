#ifndef __TABOUT__H
#define __TABOUT__H

_CLASSDEF(TAboutDlg)

class TAboutDlg : public TDialog

{

public:
  TAboutDlg::TAboutDlg(TWindow *AParent, LPSTR AName);
  ~TAboutDlg();

  void Ordering();
  DECLARE_RESPONSE_TABLE(TAboutDlg);
};
#endif // __TABOUT_H

