#ifndef __TBANDSCAN__H
#define __TBANDSCAN__H

_CLASSDEF(TBANDSCANChld)

class TBANDSCANChld : public AWindow
{
  int id;
  int Child_Number;
  int scanning_direction;
  AEdit *Min_dB;
  AEdit *Max_dB;
  AEdit *Band;
  AEdit *Channel;
  AEdit *Listen_Time;
  AEdit *Hold_Time;
  TCheckBox *Infinity;
  struct time last_change;
  struct time Hold_Start;
  int this_channel;

public:
  TALL_CHANNELSChld *PTAll_Channels;

  TBANDSCANChld::TBANDSCANChld(AWindow *AParent,int ID, LPSTR AName,int child_number);
  ~TBANDSCANChld();
  void GetWindowClass(WNDCLASS _FAR & AWndClass);
  void TBANDSCANChld::EvActivate(uint,bool,HWND);
  LPSTR GetClassName();
   void SetupWindow();

  int Where_Y();
  void ReturnFocus();
//  void Paint(TDC& DC, BOOL,TRect &);
  void EvTimer(UINT);
  void EvMove(TPoint &point);
  void EvChar(UINT key, UINT repeatCount,UINT flags);
  void Open_Channels();
  void Change_Max_SS();
  void Change_Min_SS();
  void Change_Band();
  void Change_Channel();
  void Change_Listen_Time();
  void Change_Infinity();
  void Change_Hold_Time();
  void Scan_Backward();
  void Prev_Channel();
  void Stop_Scanning();
  void Next_Channel();
  void Scan_Forward();
  void S_Meter_Report(double value);
  DECLARE_RESPONSE_TABLE(TBANDSCANChld);
};

struct Band_Scan_Struct

{
  double max_ss;
  double min_ss;
  double listen_time;
  int infinity;
  double hold_time;
  int band;
  int channel;
};

#endif // __TBANDSCAN_H

