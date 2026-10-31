_CLASSDEF(TSTATIONSDlg)

class TSTATIONSDlg : public TDialog
{
public:
  TComboBox *LBox101;

  void SetupWindow();
  TSTATIONSDlg(TWindow *AParent, LPSTR AName);
  ~TSTATIONSDlg();
  virtual void RTSTATIONS102();
  virtual void RTSTATIONS103();
  virtual void RTSTATIONS104();
  virtual void RTSTATIONS105();
  DECLARE_RESPONSE_TABLE(TSTATIONSDlg);
};

