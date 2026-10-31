#ifndef __VOICEDlg__H
#define __VOICEDlg__H

_CLASSDEF(TVOICEDlg)

class TVOICEDlg : public TDialog

{
  int Which;

public:
  TEdit *Label_Edit;
  TEdit *Filename_Edit;
  TRadioButton *Mic_Record;
  TRadioButton *Air_Record;
  TRadioButton *Speaker_Play;
  TRadioButton *Air_Play;

  TVOICEDlg(TWindow *AParent, int which);
  ~TVOICEDlg();
  virtual void Respond_Mic_Record();
  virtual void Respond_Air_Record();
  virtual void Respond_Speaker_Play();
  virtual void Respond_Air_Play();
  virtual void Respond_Record();
  virtual void OK();
  void SetupWindow();
  DECLARE_RESPONSE_TABLE(TVOICEDlg);
};
#endif // __VOICEDlg_H

