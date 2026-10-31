//REGEN_FILEHEADING
//REGEN_FILEHEADING

#ifndef __SPECTRUMCLS__H
#define __SPECTRUMCLS__H

#define POINTS_IN_FFT 128

#include <owl\applicat.h>
#include <owl\framewin.h>
#include <owl\edit.h>
#include <owl\listbox.h>
#include <owl\combobox.h>
#include <owl\scrollba.h>
#include <owl\dialog.h>
#include <bwcc.h>
#include "SpectrID.h"
#include "Spectrum.h"
#include <owl\radiobut.h>
#include <owl\dc.h>
#include <dsp1lib.h>

_CLASSDEF(Stop_Control);

class Stop_Control : public Frequency_Control

{
  public:
  long *start,*stop;

  Stop_Control(long *start,long *stop);
  virtual char *Get_Name(void);
  virtual char *Get_Error_Message(void);
  virtual long Get_Value(void);
  virtual int Set_Value(long);
};

_CLASSDEF(Start_Control);

class Start_Control : public Frequency_Control

{
  public:
  long *start,*stop;
  Start_Control(long *start,long *stop);
  virtual char *Get_Name(void);
  virtual char *Get_Error_Message(void);
  virtual long Get_Value(void);
  virtual int Set_Value(long);
};

_CLASSDEF(Center_Control);

class Center_Control : public Frequency_Control

{
  public:
  long *start,*stop;
  Center_Control(long *start,long *stop);
  virtual char *Get_Name(void);
  virtual char *Get_Error_Message(void);
  virtual long Get_Value(void);
  virtual int Set_Value(long);
};

_CLASSDEF(Span_Control);

class Span_Control : public Frequency_Control

{
  public:
  long *start,*stop;
  Span_Control(long *start,long *stop);
  virtual char *Get_Name(void);
  virtual char *Get_Error_Message(void);
  virtual long Get_Value(void);
  virtual int Set_Value(long);
  virtual long Get_Min(void);
};

_CLASSDEF(Marker_Control);

class Marker_Control : public Frequency_Control

{
  long marker;

  public:
  long *start,*stop;
  Marker_Control(long *start,long *stop);
  virtual char *Get_Name(void);
  virtual char *Get_Error_Message(void);
  virtual long Get_Value(void);
  virtual int Set_Value(long);
  virtual long Get_Min(void);
  virtual long Get_Max(void);
};

_CLASSDEF(Step_Size_Control);

class Step_Size_Control : public Frequency_Control

{
  long step_size;

  public:
  Step_Size_Control();
  virtual char *Get_Name(void);
  virtual char *Get_Error_Message(void);
  virtual long Get_Value(void);
  virtual int Set_Value(long);
  virtual long Get_Min(void);
  virtual long Get_Max(void);
};

_CLASSDEF(TADDRESSDlg)

class TADDRESSDlg : public TDialog
{
  Radio_Control *Control;
public:
  TRadioButton *Address[9];

  virtual void SetupWindow();
  TADDRESSDlg(TWindow *AParent, LPSTR AName,Radio_Control *control);
  ~TADDRESSDlg();
  virtual void RTADDRESS101();
  virtual void RTADDRESS102();
  virtual void RTADDRESS103();
  virtual void RTADDRESS104();
  virtual void RTADDRESS105();
  virtual void RTADDRESS106();
  virtual void RTADDRESS107();
  virtual void RTADDRESS108();
  virtual void RTADDRESS109();
  virtual void RTADDRESS110();
  virtual void RTADDRESS111();
  DECLARE_RESPONSE_TABLE(TADDRESSDlg);
};

//REGEN_CLASSES
// Declare TFREQDlg, a TDialog descendant
struct Freq_Dlg_struct
{
  char frequency[20];
  WORD MHz;
  WORD KHz;
  WORD Hz;
};

class TFREQDlg : public TDialog
{
public:
   TEdit *Edit101;
   TRadioButton *MHz;
   TRadioButton *KHz;
   TRadioButton *Hz;
   Freq_Dlg_struct Freq_Dlg_Data;

   TFREQDlg(TWindow *AParent, LPSTR AName,char heading[]);
   ~TFREQDlg();
/*   virtual void RTFREQ102(RTMessage Msg) = [ID_FIRST + 102];
   virtual void RTFREQ103(RTMessage Msg) = [ID_FIRST + 103];
   virtual void RTFREQ104(RTMessage Msg) = [ID_FIRST + 104];
   virtual void RTFREQ105(RTMessage Msg) = [ID_FIRST + 105];
   virtual void RTFREQ106(RTMessage Msg) = [ID_FIRST + 106];
*/   //REGEN_FREQ_CLASS
   //REGEN_FREQ_CLASS
};

class TFREQSPANDlg : public TDialog
{
public:
   TEdit *Edit101;
   TRadioButton *MHz;
   TRadioButton *KHz;
   TRadioButton *Hz;
   Freq_Dlg_struct Freq_Dlg_Data;

   TFREQSPANDlg(TWindow *AParent, LPSTR AName,char heading[],long *pointer);
   ~TFREQSPANDlg();
/*   virtual void RTFREQSPAN102(RTMessage Msg) = [ID_FIRST + 102];
   virtual void RTFREQSPAN103(RTMessage Msg) = [ID_FIRST + 103];
   virtual void RTFREQSPAN104(RTMessage Msg) = [ID_FIRST + 104];
   virtual void RTFREQSPAN105(RTMessage Msg) = [ID_FIRST + 105];
   virtual void RTFREQSPAN106(RTMessage Msg) = [ID_FIRST + 106];
*/   //REGEN_FREQ_CLASS
   //REGEN_FREQ_CLASS
};

struct Stats_Dlg_struct

{
  char HIGHFREQ[20];
  char HIGHDBM[20];
  char LOWFREQ[20];
  char LOWDBM[20];
};

class TSTATSDlg : public TFrameWindow
{
public:
   TStatic *HIGHFREQ;
   TStatic *HIGHDBM;
   TStatic *LOWFREQ;
   TStatic *LOWDBM;

   struct Stats_Dlg_struct Stats_Dlg_Data;
   TSTATSDlg(TWindow *AParent, LPSTR AName,int pixel);
   ~TSTATSDlg();
   virtual void SetupWindow();
/*   virtual void RTSTATS105(RTMessage Msg) = [ID_FIRST + 105];
   virtual void RTSTATS106(RTMessage Msg) = [ID_FIRST + 106];
*/   //REGEN_FREQ_CLASS
   //REGEN_FREQ_CLASS
};

_CLASSDEF(TFrequency_Window);

class TFrequency_Window: public TFrameWindow

{
  public:
  HBRUSH hbr;
  PFrequency_Control Control;
  long adjustment;
  int ID;
  int old_mouse_x,old_mouse_y;
  int locked_mouse;

  TFrequency_Window(TWindow *AParent,int ID, LPSTR AName,PFrequency_Control,int x,int y);
  ~TFrequency_Window();
  virtual LPSTR GetClassName();
  virtual void GetWindowClass(WNDCLASS _FAR & );
  void UpdateAll(void);
  void Paint(TDC &DC, BOOL,TRect &);
  void EvLButtonUp(UINT,TPoint&);
  void EvLButtonDown(UINT,TPoint&);
  void EvRButtonDown(UINT,TPoint&);
  void EvMouseMove(UINT,TPoint&);
  void EvKeyDown(UINT key, UINT repeatCount,UINT flags);
  void CallEnterDlg();
  DECLARE_RESPONSE_TABLE(TFrequency_Window);
};

_CLASSDEF(TAllFrequencies);

class TAllFrequencies:public AWindow

{
  int Child_Number;

  public:
  TFrequency_Window *Start_Window,*Stop_Window,*Center_Window,*Span_Window,*Step_Size_Window;

  TAllFrequencies(TWindow *AParent, LPSTR AName,Start_Control *,Stop_Control *,Center_Control *,Span_Control *,
		  Step_Size_Control *,int child_number);
  ~TAllFrequencies();
/*  virtual void Paint(HDC DC, PAINTSTRUCT _FAR & PaintInfo);
*/  virtual void Changed_Start();
  virtual void Changed_Stop();
  virtual void Changed_Center();
  virtual void Changed_Span();
  DECLARE_RESPONSE_TABLE(TAllFrequencies);
};

_CLASSDEF(AMarker)

class AMarker:public AWindow

{
  public:
  long *start,*stop;
  TFrequency_Window *Marker_Window;
  Marker_Control *Marker;
  TWindow *Owner;
  HBRUSH hbr;
  long this_marker,last_marker;
  int ID;
  WhiteStatic *PDBM,*PmVolts;
  TButton *PPeak,*PNext;
  Start_Control *PStart;
  Stop_Control *PStop;
  unsigned int (*spectrum)[][2];
  long (*spectrum_freq)[][2];
  int Child_Number;

  AMarker(TWindow *AParent,int ID, LPSTR AName,int child_number,TWindow *Owner,unsigned int (*spectrum)[MAX_PIXELS][2],
	  long (*spectrum_freq)[MAX_PIXELS][2],Start_Control *PStart,Stop_Control *PStop,long *Start,long *Stop);
  ~AMarker();
  LPSTR GetClassName();
  void GetWindowClass(WNDCLASS _FAR & AWndClass);
  void Changed_Marker();
  void Peak_Button();
  void Next_Button();
  long Get_Value();
  long Get_Last();
  void set_DBM(int dbm);
  DECLARE_RESPONSE_TABLE(AMarker);
};

#include <freqedlg.h>

class TSpectrum : public AWindow

{
  char Name[4];
  long start,stop;
  int fft_points_per_pixel;
  int pixels;
  int ac;
  int need_usb,need_lsb;
  int lsb_points_done,usb_points_done;
  int usb_pixel,lsb_pixel;
  PStart_Control PStart;
  PStop_Control PStop;
  PCenter_Control PCenter;
  PSpan_Control PSpan;
  PStep_Size_Control PStep;                     
  PFrequency_Control PFrequency;
  PTAllFrequencies PAllFreq;
  PAMarker PMarker;
  Int_Control* ac_dc_control;
  Int_Control* scale_control;
  TCONTROLChld *PT_AC_DC;
  TCONTROLChld *PT_Scale;
  int *Data_String;
  int *Data_String2;
  unsigned int sweep[MAX_PIXELS][2];
  long sweep_freq[MAX_PIXELS][2];
  int Scope;
  int ID;
  int wait_lsb;
  int Size_Number;
  double hz_per_point;
  HBRUSH hbr;

public:
  TSpectrum(TWindow *AParent,int id, LPSTR ATitle,int child_number,int size_number,
            Frequency_Control *freq,int *string,int *string2,int scope/*,
            Radio_Control *Ac_Dc_Control, Radio_Control *Scale_Control*/);
  Where_X();
  Where_Y();
  Width();
  Height();
  ~TSpectrum();

  LPSTR GetClassName();
  void GetWindowClass(WNDCLASS _FAR & AWndClass);
  void EvSize(UINT modKeys,TSize &Size);
  void SWEEP();
  void CHANNEL();
  void ADDRESS();
  void START_FREQUENCY();
  void STOP_FREQUENCY();
  void CENTER_FREQUENCY();
  void FREQUENCY_SPAN();
  void STEP_SIZE();
  void SWEEP_SPEED();
  void EvTimer(UINT timer_id);
  void Marker101();
  void Sweep_Data();
  void Button_AC_DC();
  void Button_Frequency();
  void Button_Marker();
  void Button_Scale();
  void Button_Sweep();
  void EvLButtonUp(UINT,TPoint &point);
  void start_sweep();
  void Stop_Sweep();
  int Valid_Frequency(long freq);
  void Paint(TDC &DC,BOOL,TRect &);
  void EvChar(UINT key, UINT repeatCount,UINT flags);
  void USB_Data();
  void LSB_Data();
  DECLARE_RESPONSE_TABLE(TSpectrum);
};

#endif // __SPECTRUMCLS_H

