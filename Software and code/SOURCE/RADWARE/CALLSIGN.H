#ifndef __CALLSIGN__H
#define __CALLSIGN__H

_CLASSDEF(TCallSign);

class TCallSign: public AWindow

{
  public:
  int ID,center_x,center_y,max_x,max_y;
  int box_height,box_length,char_length,char_height;
  TFont *font;
  int Size_Number;
  char CallSign[20];

  TCallSign(TWindow *AParent,int ID, LPSTR AName,int child_number,int size_number,
             char *callsign,char *icon_name);
  ~TCallSign();
  virtual LPSTR GetClassName();
  void EvSize(UINT modKeys,TSize &Size);
  void Paint(TDC& DC, BOOL,TRect &);
  void EvLButtonDown(UINT,TPoint &);
  void Update_CallSign(char *string);
  int Where_X();
  int Where_Y();
  int Width();
  int Height();
  void EvChar(UINT key, UINT repeatCount,UINT flags);
  DECLARE_RESPONSE_TABLE(TCallSign);
};

#endif // __CALLSIGN_H

