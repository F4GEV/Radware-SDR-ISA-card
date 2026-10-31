#ifndef __TPTT__H
#define __TPTT__H

_CLASSDEF(TPTTChld)

class TPTTChld : public AWindow

{
  TButton *PTT_Button;
  int ID;
  Radio_Control *Control;

public:
  TPTTChld::TPTTChld(TWindow *AParent, int id, LPSTR AName,int child_number,
        Radio_Control *control,char *icon);
  ~TPTTChld();
  LPSTR GetClassName();
  int Where_X();
  int Where_Y();
  void EvChar(UINT key, UINT repeatCount,UINT flags);
  void TOGGLE_BUTTON();
  DECLARE_RESPONSE_TABLE(TPTTChld);
};
#endif // __TPTT__H

