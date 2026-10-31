#define SSB_SWEEP 1
   /* this forces the spectrum analyzer to only look at the LSB data */
#define msg_ECHO_TOGGLE 501
#define msg_TALK_TOGGLE 504
#define msg_LPF_TOGGLE  507
#define msg_PLOT_SEND   509
#define msg_RECIEVE    1504
#define msg_TRANSMIT   1505

#include <owl\framewin.h>
#include <owl\edit.h>
#include <owl\listbox.h>
#include <owl\combobox.h>
#include <owl\scrollba.h>
#include <owl\dialog.h>
#include <bwcc.h>
#include "thisID.h"
#include "Spectrum.h"
#include "thisCls.h"
#include "stdio.h"
#include <math.h>
#include <owl\inputdia.h>
#include "smartdsp.h"

//REGEN_VARIABLES
#define MIN_SPEED 1
#define UHF 0

extern HPEN Blue,Green,Red;

extern int starting_places[][2];
extern int starting_sizes[][2];
extern int delete_class[];

int send_to_null=0,show_errors;
int grid = 1;
int comm;
long step_size = 6000l;
long current_frequency=-1l;
int have_data,sweep_speed = 1,in_timer = 0;
void set_comm(void);
int freq_speed=150;
extern int spectrum_string[];
char ac_dc_labels[2][20]={"AC","DC"};
char scale_labels[9][20]={"x1","x2","x5","x10","x20","x50","x100","x200","x500"};
int scale_values[9] = {1,2,5,10,20,50,100,200,500};

struct channel_status_struct
{
  int gain,attn,smeter,agc;
};

struct channel_status_struct channel_status={-1,-1,-1,-1};
//REGEN_VARIABLES

TSpectrum::TSpectrum(TWindow *AParent, int id,LPSTR ATitle,int child_number,int size_number,
                     Frequency_Control *freq,int *string,int *string2,
                     int scope/*,Radio_Control *Ac_Dc_Control,
                     Radio_Control *Scale_Control*/) : AWindow(AParent, ATitle,child_number)
{
  int count;
  char title[80];
  int ssb;
  int x,y,w,h;

  Child_Number = child_number;
  x = starting_places[child_number][0];
  y = starting_places[child_number][1];
  w = starting_sizes[size_number][0];
  h = starting_sizes[size_number][1];
  if (y<0)
    y = -y;
  strncpy(Name,ATitle,3);
  Name[3] = 0;
  ID = id;
  ac_dc_control = new Int_Control(0,1);
  scale_control = new Int_Control(0,8);
/*  ac_dc_control = Ac_Dc_Control;
  scale_control = Scale_Control;
*/  Scope = scope;
  Data_String = string;
  Data_String2 = string2;
/*  if (scope > -2 && scope != 1)
    Attr.Style = WS_MINIMIZEBOX | WS_POPUP | WS_CLIPSIBLINGS | WS_VISIBLE |
                 WS_CAPTION | WS_THICKFRAME | WS_SYSMENU;
  else
*/    Attr.Style = WS_MINIMIZEBOX | WS_POPUP | WS_CLIPSIBLINGS | WS_VISIBLE |
                 WS_CAPTION | WS_BORDER | WS_SYSMENU;
  Attr.X = x;
  Attr.Y = y;
  if (h<0)
    h *= -1;
  if (h == 0)
    h = 304;
  ac = 1;
  if (scope == 1)
  {
    Attr.W = 131;
    Attr.H = 314;
  }
  else
    if (scope>-2)
    {
      Attr.W = 131;
      Attr.H = 274;
    }
    else
    {
      Attr.W = 256;
      Attr.H = 296;
    }
  for(count=0;count<MAX_PIXELS;count++)
  {
    sweep[count][0] = 0;
    sweep[count][1] = 1;
    sweep_freq[count][0] = 0;
    sweep_freq[count][1] = 0;
  }
  PFrequency = freq;
  start = PFrequency->Get_Min();
  stop = PFrequency->Get_Max()-4000;
  PStart = new Start_Control(&start,&stop);
  PStop = new Stop_Control(&start,&stop);
  PCenter = new Center_Control(&start,&stop);
  PSpan = new Span_Control(&start,&stop);
  PStep = new Step_Size_Control();
  if (SSB_SWEEP)
	 PStep->Set_Value(4000);
  else
	 PStep->Set_Value(8000);
  show_errors = 0;
  PMarker = NULL;
  PAllFreq = NULL;
  PT_AC_DC = NULL;
  PT_Scale = NULL;
//  AssignMenu("Spectrum");
  if (scope == -2)
  {
    PFrequency->Set_Value(start);
	 PAllFreq = new TAllFrequencies(this,"FREQ",PStart,PStop,PCenter,PSpan,PStep,55);
	 PMarker = new AMarker(this,101,"MARKER",54,this,&sweep,&sweep_freq,PStart,PStop,&start,&stop);
	 new TButton(this,202,"Freqs",4,254,80,20,FALSE);
	 new TButton(this,203,"Marker",88,254,80,20,FALSE);
	 new TButton(this,204,"Sweep",172,254,80,20,FALSE);
	 start_sweep();
  }
  if (scope == 1)
  {
    new TButton(this,207,"AC/DC",0,253,129,20,FALSE);
    new TButton(this,208,"Scale",0,273,129,20,FALSE);
    if (strncmp(Name,"USB",3)==0)
      ssb = 1;
    else
      ssb = 0;
    sprintf(title,"%s AC DC",Name);
    PT_AC_DC = new TCONTROLChld(this,-1,title,50+ssb,ac_dc_control,ac_dc_labels,2,"R232",1);
    sprintf(title,"%s Scale",Name);
    PT_Scale = new TCONTROLChld(this,-1,title,52+ssb,scale_control,scale_labels,9,"R232",1);
  }
}

void TSpectrum::LSB_Data()

{
  int count;
  HRGN region;
  int needed_lsb_pixel;

  if (need_lsb)
  {
    need_lsb = 0;
    for(count=10;count<POINTS_IN_FFT && lsb_pixel < pixels;count++)
    {
      if(Data_String[count]/100+2<sweep[lsb_pixel][0])
        sweep[lsb_pixel][0] = Data_String[count]/100+2;
      if(Data_String[count]/100+2>sweep[lsb_pixel][1])
        sweep[lsb_pixel][1] = Data_String[count]/100+2;
      if (++lsb_points_done >= fft_points_per_pixel)
      {
        ++lsb_pixel;
        if (SSB_SWEEP)
        {
          region = CreateRectRgn(0+lsb_pixel,0,3+lsb_pixel,252);
          InvalidateRgn(region,TRUE);
        }
        UpdateWindow();
        if (SSB_SWEEP)
          DeleteObject(region);
        needed_lsb_pixel = (1.0*PFrequency->Get_Value()-PStart->Get_Value())*
                            (Attr.W-10)/PSpan->Get_Value()+.5;
        while(needed_lsb_pixel>=lsb_pixel)
        {
          sweep[lsb_pixel][0] = sweep[lsb_pixel-1][0];
          sweep[lsb_pixel][1] = sweep[lsb_pixel-1][1];
          if (SSB_SWEEP)
          {
            region = CreateRectRgn(0+lsb_pixel,0,3+lsb_pixel,252);
            InvalidateRgn(region,TRUE);
          }
          UpdateWindow();
          if (SSB_SWEEP)
            DeleteObject(region);
          ++lsb_pixel;
        }
        lsb_points_done = 0;
      }
    }
    if (!need_usb || SSB_SWEEP)
      if (PFrequency->Get_Value()+PStep->Get_Value()>=PStop->Get_Value())
        start_sweep();
      else
      {
        if (Valid_Frequency(PFrequency->Get_Value()+PStep->Get_Value()))
        {
          PFrequency->Set_Value(PFrequency->Get_Value()+PStep->Get_Value());
          need_usb = 1-SSB_SWEEP;
          need_lsb = 1;
          ::PostMessage(Parent->HWindow,WM_COMMAND,ID,0);
          if (SSB_SWEEP)
          {
            if ((lsb_points_done+=fft_points_per_pixel/(POINTS_IN_FFT-5))>=fft_points_per_pixel)
            {
              ++lsb_pixel;
              lsb_points_done = 0;
            }
            lsb_points_done += fft_points_per_pixel%(POINTS_IN_FFT-5);
            if ((usb_points_done+=fft_points_per_pixel/(POINTS_IN_FFT-5))>=fft_points_per_pixel)
            {
              ++usb_pixel;
              usb_points_done = 0;
            }
            usb_points_done += fft_points_per_pixel%(POINTS_IN_FFT-5);
          }
          else
          {
            Stop_Sweep();
            return;
          }
        }
      }
  }
}

void TSpectrum::USB_Data()

{
  int count;

  if (need_usb)
  {
    need_usb = 0;
    for(count=2;count<POINTS_IN_FFT && usb_pixel < pixels;count++)
    {
      if(Data_String2[count]/100+2<sweep[usb_pixel][0])
        sweep[usb_pixel][0] = Data_String[count]/100+2;
      if(Data_String2[count]/100+2>sweep[usb_pixel][1])
        sweep[usb_pixel][1] = Data_String[count]/100+2;
      if (++usb_points_done >= fft_points_per_pixel)
      {
        ++usb_pixel;
		  usb_points_done = 0;
      }
    }
    InvalidateRgn(NULL,0);
    UpdateWindow();
    if (!need_lsb)
      if (PFrequency->Get_Value()+PStep->Get_Value()>=PStop->Get_Value())
        start_sweep();
      else
      {
        if (Valid_Frequency(PFrequency->Get_Value()+PStep->Get_Value()))
        {
          PFrequency->Set_Value(PFrequency->Get_Value()+PStep->Get_Value());
          need_usb = need_lsb = 1;
          ::PostMessage(Parent->HWindow,WM_COMMAND,ID,0);
          if ((lsb_points_done+=fft_points_per_pixel/(POINTS_IN_FFT-5))>=fft_points_per_pixel)
          {
            ++lsb_pixel;
            lsb_points_done = 0;
          }
          lsb_points_done += fft_points_per_pixel%(POINTS_IN_FFT-5);
          if ((usb_points_done+=fft_points_per_pixel/(POINTS_IN_FFT-5))>=fft_points_per_pixel)
          {
            ++usb_pixel;
            usb_points_done = 0;
          }
          usb_points_done += fft_points_per_pixel%(POINTS_IN_FFT-5);
        }
        else
        {
          Stop_Sweep();
          return;
        }
      }
  }
}

// Define TMainWindow destructor
LPSTR TSpectrum::GetClassName()

{
  return("SPECTRUM");
}

void TSpectrum::GetWindowClass(WNDCLASS _FAR & AWndClass)

{
  TWindow::GetWindowClass(AWndClass);
  hbr = CreateSolidBrush(RGB(255,255,255));
  AWndClass.hbrBackground = hbr;
//  AWndClass.hIcon = LoadIcon(_hInstance,"SPECTRUM");
}

TSpectrum::~TSpectrum()

{
  ClipCursor(NULL);
  ShowCursor(0);
  Attr.H *= -1;
  need_lsb = need_usb = 0;
  while (ShowCursor(1)<0)
    {}
  starting_sizes[Size_Number][0] = Attr.W;
  starting_sizes[Size_Number][1] = Attr.H;
  delete_class[Child_Number] = 1;
}

void TSpectrum::EvChar(UINT key, UINT repeatCount,UINT flags)

{
  if (key == 13)
  {
    ClipCursor(NULL);
    ShowCursor(0);
    Attr.H = -1;
    while (ShowCursor(1)<0)
      {}
  }
}

void TSpectrum::Button_AC_DC()

{
  char string[80];
  int ssb;

  if (PT_AC_DC == NULL || PT_AC_DC->Attr.Y < 0)
  {
    if (strncmp(Name,"USB",3)==0)
      ssb = 1;
    else
      ssb = 0;
    sprintf(string,"%s AC DC",Name);
    PT_AC_DC = new TCONTROLChld(this,-1,string,50+ssb,ac_dc_control,ac_dc_labels,2,"R232",1);
    PT_AC_DC->Create();
  }
  else
    PT_AC_DC->Show(SW_SHOWNORMAL);
}

void TSpectrum::Button_Scale()

{
  char string[80];
  int ssb;

  if (PT_Scale == NULL || PT_Scale->Attr.Y < 0)
  {
    sprintf(string,"%s Scale",Name);
    if (strncmp(Name,"USB",3)==0)
      ssb = 1;
    else
      ssb = 0;
    PT_Scale = new TCONTROLChld(this,-1,string,52+ssb,scale_control,scale_labels,9,"R232",1);
    PT_Scale->Create();
  }
  else
    PT_Scale->Show(SW_SHOWNORMAL);
}

void TSpectrum::Button_Frequency()

{
  if (PAllFreq == NULL || delete_class[55])
  {
    PAllFreq = new TAllFrequencies(this,"FREQ",PStart,PStop,PCenter,PSpan,PStep,55);
    PAllFreq->Create();
    delete_class[55] = 0;
  }
  else
    PAllFreq->Show(SW_SHOWNORMAL);
}

void TSpectrum::Button_Marker()

{
  if (PMarker == NULL || delete_class[54])
  {
	 PMarker = new AMarker(this,101,"MARKER",54,this,&sweep,&sweep_freq,PStart,PStop,&start,&stop);
    PMarker->Create();
    delete_class[54] = 0;
  }
  else
    PMarker->Show(SW_SHOWNORMAL);
}

void TSpectrum::Button_Sweep()

{
//  DSPSendMsg(msg_PLOT_TOGGLE,NULL,0);
  start_sweep();
}
  
void TSpectrum::SWEEP()

{
  start_sweep();
/*   int count;               // R232-8

   if (current_frequency == -1)
   {
     in_timer = 1;
     if (SetTimer(1,sweep_speed,NULL)==0)
       MessageBox("Windows can not allocate timer","ERROR",
                  MB_ICONHAND | MB_OK);
     else
     {
       for(count=0;count<MAX_PIXELS;count++)
       {
         sweep[count][0] = 500;
         sweep[count][1] = 0;
       }
       current_frequency = PStart->Get_Value();
       if (Valid_Frequency(PFrequency->Get_Value()+PStep->Get_Value()))
       {
         PFrequency->Set_Value(current_frequency);
         have_data = 0;
         InvalidateRgn(NULL,1);
         UpdateWindow();
       }
       else
       {
         Stop_Sweep();
         return;
       }
     }
     in_timer = 0;
   }
   else
     if (MessageBox("Sweep already running,\n Abort Sweep?",
		"SWEEP",MB_ICONQUESTION|MB_YESNO)==IDYES)
     {
       KillTimer(1);
       current_frequency = -1;
     }
*/
}

void TSpectrum::EvTimer(UINT timer_id)

{
/*  int old_pixel,new_pixel,smeter;
  HRGN region;
  int count;
  HDC DC,hDC;
  static LOGPEN lpBlue = {PS_SOLID,1,1,RGB(0,0,255)};
  HPEN Blue;

  DC = GetDC(HWindow);
  Blue = CreatePenIndirect(&lpBlue);
  SelectObject(DC,Blue);
  if (!in_timer)
  {
    KillTimer(1);
    do
    {
    in_timer = 1;
    if (have_data)
    {
      have_data = 0;
      old_pixel = (current_frequency-PStart->Get_Value())*1.0*MAX_PIXELS/(PStop->Get_Value()-PStart->Get_Value());
      current_frequency+=PStep->Get_Value();
      new_pixel = (current_frequency-PStart->Get_Value())*1.0*MAX_PIXELS/(PStop->Get_Value()-PStart->Get_Value());
      if (current_frequency>PStop->Get_Value())
      {
        MessageBox("DONE","DONE",MB_OK);
	     current_frequency = -1;
      }
      else
      {
        if (PFrequency->Set_Value(current_frequency)==0)
          current_frequency -= PStep->Get_Value();
      }
      if (old_pixel!=new_pixel)
      {
        MoveTo(DC,old_pixel+1,251); //251-sweep[old_pixel][0]);
        LineTo(DC,old_pixel+1,251-sweep[old_pixel][1]);
      }
    }
    else
    {
      have_data = 1;
      new_pixel = (current_frequency-PStart->Get_Value())*1.0*MAX_PIXELS/(PStop->Get_Value()-PStart->Get_Value());
      if (!send_to_null)
        smeter = get_smeter();
      else
        if (current_frequency == PCenter->Get_Value())
	       smeter = 250;
        else
        {
          if ((smeter = 25000*PStep->Get_Value()/(PCenter->Get_Value()-current_frequency))<0)
            smeter *= -1;
          smeter = sqrt(smeter);
          if (smeter>249)
	         smeter = 249;
        }
      if (smeter == 0)
        smeter = 1;
      if (sweep[new_pixel][0] == 0)
        sweep[new_pixel][0] = smeter;
      if (smeter<sweep[new_pixel][0])
      {
        sweep[new_pixel][0] = smeter;
        sweep_freq[new_pixel][0] = frequency;
      }
      if (smeter>sweep[new_pixel][1])
      {
        sweep[new_pixel][1] = smeter;
        sweep_freq[new_pixel][1] = frequency;
      }
    }
    in_timer = 0;
    wait_duration(sweep_speed);
    }
    while(current_frequency!=-1);
    if (current_frequency != -1)
      if (SetTimer(1,sweep_speed,NULL)==0)
        MessageBox("Windows can not allocate timer",
		   "ERROR",MB_ICONHAND | MB_OK);
  }
  else
    printf("Overload");
//  wait_duration(1);
  ReleaseDC(HWindow,DC);
  DeleteObject(Blue);
*/}

void TSpectrum::CHANNEL()               /* r232-8 */

{
//  stop_smeter(HWindow);
/*  TADDRESSDlg(this, "CHANNEL").Execute();
  InvalidateRgn(NULL,0);
  UpdateWindow();
*///  start_smeter(HWindow);
}

void TSpectrum::ADDRESS()               /* r232-8 */

{
   //stop_smeter(HWindow);
   // Execute modal dialog
/*   TADDRESSDlg(this,"ADDRESS").Execute();
   InvalidateRgn(NULL,0);
   UpdateWindow();
*///   start_smeter(HWindow);
}

void TSpectrum::START_FREQUENCY()

{
  TENTER_FREQUENCYDlg(this,"Enter Start Frequency",PStart).Execute();
  if (PAllFreq!=NULL)
  {
    ::InvalidateRgn(PAllFreq->HWindow,NULL,1);
    ::UpdateWindow(PAllFreq->HWindow);
  }
}

void TSpectrum::STOP_FREQUENCY()

{
  TENTER_FREQUENCYDlg(this,"Enter Stop Frequency",PStop).Execute();
  if (PAllFreq!=NULL)
  {
    ::InvalidateRgn(PAllFreq->HWindow,NULL,1);
    ::UpdateWindow(PAllFreq->HWindow);
  }
}

void TSpectrum::CENTER_FREQUENCY()
{
  GetModule()->ExecDialog(new TENTER_FREQUENCYDlg(this,"Enter Center Frequency",PCenter));
  if (PAllFreq!=NULL)
  {
    ::InvalidateRgn(PAllFreq->HWindow,NULL,1);
    ::UpdateWindow(PAllFreq->HWindow);
  }
}

void TSpectrum::FREQUENCY_SPAN()

{
  GetModule()->ExecDialog(new TENTER_FREQUENCYDlg(this,"Enter Frequency Span",PSpan));
  if (PAllFreq!=NULL)
  {
    ::InvalidateRgn(PAllFreq->HWindow,NULL,1);
    ::UpdateWindow(PAllFreq->HWindow);
  }
}

void TSpectrum::STEP_SIZE()

{
  GetModule()->ExecDialog(new TENTER_FREQUENCYDlg(this,"Enter Step Size",PStep));
  if (PAllFreq!=NULL)
  {
    ::InvalidateRgn(PAllFreq->HWindow,NULL,1);
    ::UpdateWindow(PAllFreq->HWindow);
  }
}

void TSpectrum::SWEEP_SPEED()

{
   char string[16];
   LPSTR PString;
   int value;

   PString = string;
   sprintf(string,"%d",sweep_speed);
   if (GetApplication()->ExecDialog(new TInputDialog(this,"SWEEP SPEED",
			"Speed in milliseconds per step",PString,15,NULL)) == IDOK)
   {
     value = atoi(string);
     if (value<MIN_SPEED)
       MessageBox("Speed is to fast.","ERROR",MB_ICONHAND | MB_OK);
     else
       sweep_speed = value;
   }
}

void TSpectrum::Sweep_Data()

{
  int count;
  int max,min,average,y_offset;
  double scale;
  HRGN Region;

  switch (Scope)
  {
    case -1 : for(count=1;count<MAX_PIXELS && count<(POINTS_IN_FFT);count++)
                sweep[(POINTS_IN_FFT)-count][1] = Data_String[count]/100+2;
              break;
    case 0 : for(count=1;count<MAX_PIXELS && count<256;count++)
               sweep[count][1] = Data_String[count]/100+2;
             break;
    case 1 : average = 0;
             max = 0;
             min = 256;
             scale = 500/scale_values[scale_control->Get_Value()];
             for(count=0;count<MAX_PIXELS && count<POINTS_IN_FFT;count++)
             {
               sweep[count][1] = Data_String[count]/scale;
               average += sweep[count][1];
               if (sweep[count][1]<min)
                 min = sweep[count][1];
               if (sweep[count][1]>max)
                 max = sweep[count][1];
               sweep[count][0] = sweep[count-1][1];
             }
             if (ac_dc_control->Get_Value())
               average = 0;
             else
               average = 1.0*average/POINTS_IN_FFT+0.5;
//             y_offset = 250-Attr.H/2 - average;
             y_offset = 125 - average;
             for(count=0;count<POINTS_IN_FFT;count++)
             {
               sweep[count][0] = sweep[count][0] + y_offset;
               sweep[count][1] = sweep[count][1] + y_offset;
             }
             break;
    case 2 : for(count=0;count<MAX_PIXELS && count<64;count++)
             {
               sweep[count][0] = Data_String[count]/4+100;
               sweep[count][1] = Data_String2[count]/4+100;
             }
             break;
  }
  Region = CreateRectRgn(0,0,250,250);
  InvalidateRgn(Region,1);
  UpdateWindow();
  DeleteObject(Region);
}

void TSpectrum::Marker101()

{
  HRGN Region,Region2;
  int pixel;

  if (PMarker!=NULL)
  {
    pixel = (PMarker->Get_Last()-PStart->Get_Value())*1.0*MAX_PIXELS/(PStop->Get_Value()-PStart->Get_Value());
    Region = CreateRectRgn(pixel-1,249-sweep[pixel][1],pixel+4,
  			   254-sweep[pixel][1]);
    InvalidateRgn(Region,1);
    pixel = (PMarker->Get_Value()-PStart->Get_Value())*1.0*MAX_PIXELS/(PStop->Get_Value()-PStart->Get_Value());
   // PMarker->set_DBM(sweep[pixel][1]);
    Region2 = CreateRectRgn(pixel-1,249-sweep[pixel][1],pixel+4,
	  		   254-sweep[pixel][1]);
    InvalidateRgn(Region2,1);
    UpdateWindow();
    DeleteObject(Region);
    DeleteObject(Region2);
  }
}

void TSpectrum::EvLButtonUp(UINT,TPoint &point)

{
  int x;

  x = point.x;
}

void TSpectrum::EvSize(UINT modKeys,TSize &Size)

{
  Attr.H = Size.cy;
  Attr.W = Size.cx;
}

TSpectrum::Where_X()

{
  return Attr.X+4;
}

TSpectrum::Where_Y()

{
  return(Attr.Y);
}

TSpectrum::Width()

{
  return(Attr.W+8);
}

TSpectrum::Height()

{
  return(Attr.H+27);
}

void TSpectrum::Stop_Sweep()

{
  need_lsb = need_usb = 0;
  MessageBox("  Spectrum Analyzer has attempted to "
             "sweep a frequency out of band.\n"
             "Sweep has been suspended.          \n"
             "To correct this problem, either    \n"
             "  A) Change the band limits.       \n"
             "  B) Change start & stop frequency.\n"
             "To restart sweep, press sweep.","ERROR",MB_ICONHAND | MB_OK);
}

int TSpectrum::Valid_Frequency(long freq)

{
  if (freq>=PFrequency->Get_Min() && freq<=PFrequency->Get_Max())
    return 1;
  else
    return 0;
}

void TSpectrum::start_sweep()

{
  int count;
  double fft_points;
  HRGN region;

  for(count=0;count<MAX_PIXELS;count++)
  {
    sweep[count][0] = 0;
    sweep[count][1] = 10;
  }
  pixels = Attr.W;
  fft_points = (PSpan->Get_Value()*(POINTS_IN_FFT-5)/(PStep->Get_Value()/(2.0-SSB_SWEEP)))/pixels;
  fft_points_per_pixel = fft_points;
  hz_per_point = PSpan->Get_Value()/(fft_points*pixels);
  need_usb = 1-SSB_SWEEP;
  need_lsb = 1;
  if (SSB_SWEEP)
    if (Valid_Frequency(PFrequency->Get_Value()+PStep->Get_Value()))
      PFrequency->Set_Value(PStart->Get_Value()+PStep->Get_Value());
    else
    {
      Stop_Sweep();
      return;
    }
  else
    if (Valid_Frequency(PFrequency->Get_Value()+PStep->Get_Value()/2))
      PFrequency->Set_Value(PStart->Get_Value()+PStep->Get_Value()/2);
    else
    {
      Stop_Sweep();
      return;
    }
  lsb_pixel = 0;
  lsb_points_done = 0;
  usb_pixel = fft_points_per_pixel/(POINTS_IN_FFT-5);
  usb_points_done = fft_points_per_pixel%(POINTS_IN_FFT-5);
  if (SSB_SWEEP)
    region = CreateRectRgn(1,0,1,252);
  else
    region = CreateRectRgn(0,0,252,252);
  InvalidateRgn(region,TRUE);
  UpdateWindow();
  DeleteObject(region);
}

/***************************************************
 * TSpectrumApp method implementations:
 ***************************************************/

// Construct the TSpectrum's MainWindow of type TMainWindow

void TSpectrum::Paint(TDC &DC,BOOL,TRect &)

{
  int count,pixel;

//  DSPSendMsg(msg_PLOT_TOGGLE,NULL,0);
  if (grid)
  {
    DC.SelectObject(Green);
    for(count=0;count<10;count++)
    {
      DC.MoveTo(1,25*count+26);
      DC.LineTo(251,25*count+26);
      DC.MoveTo(25*count+26,1);
      DC.LineTo(25*count+26,251);
    }
  }
  DC.SelectObject(Blue);
  DC.MoveTo(0,0);
  DC.LineTo(252,0);
  DC.LineTo(252,252);
  DC.LineTo(0,252);
  DC.LineTo(0,0);
  if (Scope == 2)
  {
    DC.MoveTo(sweep[0][0],sweep[0][1]);
    for(count=1;count<MAX_PIXELS;count++)
      DC.LineTo(sweep[count][0],sweep[count][1]);
  }
  else
    for(count=0;count<MAX_PIXELS;count++)
    {
      if (sweep[count][0] == sweep[count][1])
        DC.SetPixel(count+1,251-sweep[count][0],RGB(0,0,255));
      else
      {
        DC.MoveTo(count+1,251-sweep[count][0]);
        DC.LineTo(count+1,251-sweep[count][1]);
      }
    }
  if (send_to_null)
    DC.TextOut(10,255,"Simulation Mode",15);
  if (PMarker!=NULL)
  {
    pixel = (PMarker->Get_Value()-PStart->Get_Value())*1.0*MAX_PIXELS/(PStop->Get_Value()-PStart->Get_Value());
    DC.SelectObject(Red);
    DC.MoveTo(pixel+1,252-sweep[pixel][1]);
    DC.LineTo(pixel+2,251-sweep[pixel][1]);
    DC.LineTo(pixel+1,250-sweep[pixel][1]);
    DC.LineTo(pixel,251-sweep[pixel][1]);
    DC.LineTo(pixel+1,253-sweep[pixel][1]);
    DC.LineTo(pixel+3,251-sweep[pixel][1]);
    DC.LineTo(pixel+1,249-sweep[pixel][1]);
    DC.LineTo(pixel-1,251-sweep[pixel][1]);
  }
  DC.RestoreObjects();
  DeleteObject(DC);
//  DSPSendMsg(msg_PLOT_TOGGLE,NULL,0);
}

DEFINE_RESPONSE_TABLE1(TSpectrum,TWindow)
  EV_COMMAND(IDM_SWEEP,SWEEP),
  EV_COMMAND(IDM_ITEM2,CHANNEL),
  EV_COMMAND(IDM_ITEM3,ADDRESS),
  EV_COMMAND(IDM_START_FREQUENCY,START_FREQUENCY),
  EV_COMMAND(IDM_STOP_FREQUENCY,STOP_FREQUENCY),
  EV_COMMAND(IDM_CENTER_FREQUENCY,CENTER_FREQUENCY),
  EV_COMMAND(IDM_FREQUENCY_SPAN,FREQUENCY_SPAN),
  EV_COMMAND(IDM_STEP_SIZE,STEP_SIZE),
  EV_COMMAND(IDM_SWEEP_SPEED,SWEEP_SPEED),
  EV_WM_TIMER,
  EV_COMMAND(101,Marker101),
  EV_COMMAND(200,Sweep_Data),
  EV_COMMAND(202,Button_Frequency),
  EV_COMMAND(203,Button_Marker),
  EV_COMMAND(204,Button_Sweep),
  EV_COMMAND(205,LSB_Data),
  EV_COMMAND(206,USB_Data),
  EV_COMMAND(207,Button_AC_DC),
  EV_COMMAND(208,Button_Scale),
  EV_WM_LBUTTONUP,
  EV_WM_SIZE,
END_RESPONSE_TABLE;


