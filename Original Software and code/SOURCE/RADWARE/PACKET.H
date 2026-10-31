#ifndef __TPACKET__H
#define __TPACKET__H

_CLASSDEF(TPACKETChld)

class TPACKETChld : public AWindow

{
  TStatic *Display;
  char Text[300];
  int cr;

public:
  char Last_Char;
  TPACKETChld(TWindow *AParent,int id, LPSTR AName,int child_number,int w,int h);
  ~TPACKETChld();
  void EvActivate(uint active,bool b,HWND c);
  void SetupWindow();
  void Add_Int_String(int *string);
//  DECLARE_RESPONSE_TABLE(TPACKETChld);
};
#endif // __TPACKET__H
