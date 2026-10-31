#ifndef __ABUTTON__H
#define __ABUTTON__H

_CLASSDEF(AButton)

class AButton : public TButton

{
  int ID;
public:
  AButton(TWindow *AParent, int id, LPSTR AName,int x,int y,
        int w,int h,BOOL IsDefault);
  ~AButton();
  void EvRButtonUp(UINT modKeys,TPoint& point);
  DECLARE_RESPONSE_TABLE(AButton);
};
#endif // __ABUTTON__H

