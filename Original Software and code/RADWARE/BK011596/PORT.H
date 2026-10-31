#ifndef __TPORT__H
#define __TPORT__H

_CLASSDEF(TPORTChld)

class TPORTChld : public AWindow

{
  TStatic *Display;
  char Text[300];
  int cr;

public:
  char Last_Char;
  TPORTChld(TWindow *AParent,int id, LPSTR AName,int child_number,int w,int h);
  ~TPORTChld();
  void EvActivate(uint active,bool b,HWND c);
  void SetupWindow();
  void Add_Int_String(int *string);
//  DECLARE_RESPONSE_TABLE(TPORTChld);
};
#endif // __TPORT__H
