_CLASSDEF(TBANDChld)

// Declare TBANDChld, a AWindow descendant
class TBANDChld : public AWindow
{
  AButton *Button[9];
  int Child_Number;

public:
  TBANDChld(AWindow *AParent, LPSTR AName,int child_number);
  ~TBANDChld();

  Where_Y();
  void Reply_Button(WPARAM ID);
  DECLARE_RESPONSE_TABLE(TBANDChld);
  void EvActivate(uint active,bool,HWND);
};
