#ifndef __TFREQUENCY_SCAN__H
#define __TFREQUENCY_SCAN__H

_CLASSDEF(TFREQUENCY_SCANChld)

class TFREQUENCY_SCANChld : public AWindow
{
  int id;
  int Child_Number;
  int scanning_direction;
  AEdit *Min_dB;
  AEdit *Max_dB;
  AEdit *Listen_Time;
  AEdit *Hold_Time;
  TCheckBox *Infinity;
  TALL_CHANNELSChld *PTAll_Channels;
  struct time last_change;
  struct time Hold_Start;
  int this_channel;
  long start;
  long stop;
  Start_Control *PStart;
  Stop_Control *PStop;
  Step_Size_Control *PStep;
  TFrequency_Window *Start_Window;
  TFrequency_Window *Stop_Window;
  TFrequency_Window *Step_Window;

public:
  TFREQUENCY_SCANChld::TFREQUENCY_SCANChld(AWindow *AParent,int ID, LPSTR AName,int child_number);
  ~TFREQUENCY_SCANChld();
  void GetWindowClass(WNDCLASS _FAR & AWndClass);
  void TFREQUENCY_SCANChld::EvActivate(uint,bool,HWND);
  LPSTR GetClassName();
   void SetupWindow();

  int Where_Y();
  void ReturnFocus();
//  void Paint(TDC& DC, BOOL,TRect &);
  void EvTimer(UINT);
  void EvMove(TPoint &point);
  void EvChar(UINT key, UINT repeatCount,UINT flags);
  void Change_Max_SS();
  void Change_Min_SS();
  void Change_Start_Frequency();
  void Change_Stop_Frequency();
  void Change_Step_Size();
  void Change_Listen_Time();
  void Change_Infinity();
  void Change_Hold_Time();
  void Scan_Backward();
  void Prev_Frequency();
  void Stop_Scanning();
  void Next_Frequency();
  void Scan_Forward();
  void S_Meter_Report(double value);
  long TFREQUENCY_SCANChld::Get_Frequency();
  void TFREQUENCY_SCANChld::Set_Frequency(long value);
  DECLARE_RESPONSE_TABLE(TFREQUENCY_SCANChld);
};

struct Frequency_Scan_Struct

{
  double max_ss;
  double min_ss;
  double listen_time;
  int infinity;
  double hold_time;
  long start_frequency;
  long stop_frequency;
  long step;
};

#endif // __TFREQUENCY_SCAN_H

