#ifndef __TSENT_MORSE__H
#define __TSENT_MORSE__H

_CLASSDEF(TSENT_MORSEChld)

class TSENT_MORSEChld : public AWindow

{
  TStatic *sent_data;
  int Child_Number;

public:
  TSENT_MORSEChld::TSENT_MORSEChld(TWindow *AParent, LPSTR AName,int child_number,int w,int h);
  ~TSENT_MORSEChld();
  void EvActivate(uint active,bool b,HWND c);
//  BOOL CanClose();
  void Add_Char(char c);
};
#endif // __TSENT_MORSE__H
