#define MAX_KNOBS 17
#define NUMBER_OF_WINDOWS 75
#define NUMBER_OF_WINDOW_SIZES 14
#define ID_SPECTRUM     193
#define ID_LSB_SPECTRUM 194
#define ID_USB_SPECTRUM 195                 
#define ID_LSB_TIME     196
#define ID_USB_TIME     197
#define ID_XY_TIME      198
#define ID_PTT          199
#define ID_COMPRESSION  251
#define ID_SEND_MORSE   252
#include <D:\proradio\awindow.h>
#include <stdio.h>
#include "dsp1lib.h"
#include "control.h"
//#include "dsplib.c"
#include <mmsystem.h>
#include <windows.h>
#include "dspmgr.h"
#include "dspapi2.h"
#include "D:\proradio\freqwin2.h"
#include <string.h>
#include <thiscls.h>
#include <math.h>
#include <bwcc.h>
#include <dos.h>

TStatic *PTCal;

struct single_station
{
  char name[16];
  long frequency;
  short mode;
  short band;
};

struct single_station stations[MAX_STATIONS];
struct single_station quick_stations[MAX_QUICK_STATIONS];

struct voice_memory voice_memories[MAX_VOICE_MEMORIES];

static LOGPEN lpBlue = {PS_SOLID,1,1,RGB(0,0,255)};
static LOGPEN lpGreen = {PS_SOLID,1,1,RGB(0,255,0)};
static LOGPEN lpRed = {PS_SOLID,1,1,RGB(255,0,0)};

HPEN Blue,Green,Red;

DWORD dsp_message = -1;
int knob_speed = 250;
char ON_STRING[3] = {1,1,0};
char OFF_STRING[3] = {0,0,0};
char CallSign[20] = "UNKNOWN";

extern int banded;
extern int band_pointers[];
extern int smeter_string[];
extern int analog_string[];
extern struct time frequency_time;

int Need_More_Voice = 0;
int TimerID;
int starting_places[NUMBER_OF_WINDOWS][2];
short int delete_class[NUMBER_OF_WINDOWS];
int starting_sizes[NUMBER_OF_WINDOW_SIZES][2];
short int windows_on_top[NUMBER_OF_WINDOWS];
char agc_labels[3][20] = {"OFF","ON","HLD"};
char compression_labels[2][20] = {"OFF","ON"};
char ptt_labels[2][20] = {"RECV","XMIT"};
char filter_labels[6][20] = {"50","200","400","1800","2700","16K"};
char split_labels[4][20] = {"A","B","rAxB","rBxA"};
char mode_labels[5][20] = {"AM","LSB","USB","ISB","FM"};
char antenna_power_labels[2][20] = {"OFF","ON"};
char mixer_gain_labels[3][20]={"-10","0","+10"};
char volume_labels[1][20]={"MUTE"};
char places_name[80];
char settings_name[80];
char stations_name[80];
int screen_max_x = 700;
int screen_max_y = 500;
extern int message_string[],lsb_string[],usb_string[],spectrum_string[];
extern int lsb_time_string[],usb_time_string[],frequency_calibrate_string[];
extern long bands[][2];
FARPROC CallBackInstance;
extern "C" void CALLBACK CallBackFunction(HALG Alg,DWORD Msg, DWORD Instance,
										 DWORD Param1,DWORD Param2);

TBitmap *knobs[MAX_KNOBS];
TBitmap *smeter_bitmap;
TBitmap *sheet_bitmap[2];
TBitmap *tune_bitmap[2];

TCONTROLChld *PTAGC,*PTFilter,*PTVolume,*PTGain,*PTMode,*PTAntenna_Power,
				 *PTMixer_Gain,*PTMic_Level,*PTTXAntenna_Power,*PTCompression,
				 *PTCompression_Level,*PTSplit,*PTMonitor;
TFreq_Window2 *PTFreq,*PTCalibrate,*PTTXFreq = NULL,*PTMaxFreq = NULL,
				  *PTMinFreq = NULL,*PTAM_Offset;
TSTATChld *PTStations[9] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
TBANDChld *PTAmateurBands = NULL;
TVoiceChld *PTVoice_Memories[9] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
TSpectrum *PTSpectrum = NULL;
TSpectrum *PT_USB_Spectrum = NULL;
TSpectrum *PT_LSB_Spectrum = NULL;
TSpectrum *PT_USB_Time = NULL;
TSpectrum *PT_LSB_Time = NULL;
TSpectrum *PT_XY_Time = NULL;
TCallSign *PT_CallSign = NULL;
TSMTRChld *PTS_Meter = NULL;
TSMTRChld *PTAM_Meter = NULL;
TCONTROLChld *PTPTT = NULL;
TSEND_MORSEChld *PTMorse = NULL;
TSENT_MORSEChld *PTSent_Morse = NULL;
//TPTTChld *PTPTT = NULL;
PVolume_Control volume_control;
PMic_Level_Control mic_level_control;
PIF_GAIN_Control if_gain_control;
PAGC_Control agc_control;
PAM_Frequency_Offset_Control am_offset_control;
PSplit_Control split_control;
PCompression_Control compression_control;
PCompression_Level_Control compression_level_control;
PI_ADJUST_Control i_adjust_control;
PANTENNA_POWER_Control antenna_power_control;
PTXANTENNA_POWER_Control tx_antenna_power_control;
PMIXER_GAIN_Control mixer_gain_control;
PFILTER_Control filter_control;
PMODE_Control mode_control;
PFrequency_Control frequency_control;
PTX_Frequency_Control tx_frequency_control;
PMax_Frequency_Control max_frequency_control;
PMin_Frequency_Control min_frequency_control;
PCalibrate_Control calibrate_control;
PPTT_Control ptt_control;
PMonitor_Control monitor_control;

int closing = 0;
int leading_zeros,frequency_wrap,stations_reset_bands;

HALG RadioWareAlg;
HWND TestWindow;

void dly();
void load_places();
void load_stations();
long get_long(FILE *stream);
void Set_Radware_Address(int address);
int Get_Radware_Address();
void Default_Places();
void Clear_Preselectors();
void set_am_side(int);

class DSP1:public TApplication

{
  public:
  DSP1(char far *Name): TApplication(Name) {}
  void InitMainWindow();
  bool ProcessAppMsg(MSG &msg);
};

class TMainWindow:public AWindow

{
  public:
  TMainWindow(AWindow *AParent, LPSTR ATitle);
  ~TMainWindow();
  void GetWindowClass(WNDCLASS _FAR & AWndClass);
  void SetupWindow();
  void GetMsg();
  void Window_Defaults();
  void Window_AGC();
  void Window_AM_Offset();
  void Window_Analog_Gain();
  void Window_Antenna_Power();
  void Window_Calibrate();
  void Window_Filter();
  void Window_Frequency();
  void Window_TX_Frequency();
  void Window_Max_Frequency();
  void Window_Min_Frequency();
  void Window_Mic_Gain();
  void Window_Mixer_Gain();
  void Window_Mode();
  void Window_Monitor();
  void Window_Spectrum_Analyzer();
  void Window_LSB_Spectrum();
  void Window_USB_Spectrum();
  void Window_LSB_Time();
  void Window_USB_Time();
  void Window_XY_Time();
  void Window_S_Meter();
  void Window_Volume();
  void Window_PTT();
  void Window_Call_Sign();
  void Window_TX_Antenna();
  void Window_Compression();
  void Window_Compression_Level();
  void Window_Split();
  void Window_Morse();
  void Window_Sent_Morse();
  void Window_Recieved_Morse();
  void Window_Fast_Channels_1();
  void Window_Fast_Channels_2();
  void Window_Fast_Channels_3();
  void Window_Fast_Channels_4();
  void Window_Fast_Channels_5();
  void Window_Fast_Channels_6();
  void Window_Fast_Channels_7();
  void Window_Fast_Channels_8();
  void Window_Fast_Channels_9();
  void Window_Voice_Memories(WPARAM ID);
  void Window_Amateur_Bands();
  void EvTimer(uint timerId);
  void About();
  void Select_Band(WPARAM ID);
  void Select_All_Band(WPARAM ID);
  LRESULT GetMsg2(WPARAM ID,LPARAM);
  void Post();
  void TakePlot();
  void Reset();
  void load_settings();
  void save_settings();
  void save_places();
  void Notify_LSB_Spectrum_Analyzer();
  void Notify_USB_Spectrum_Analyzer();
  void Notify_LSB_Time_Spectrum_Analyzer();
  void Notify_USB_Time_Spectrum_Analyzer();
  void Notify_S_Meter();
  void Notify_Morse_Next_Char();
  void Notify_Morse_End_Morse();
  void Notify_Frequency_Calibrate();
  void Notify_AGC_Analog();
  void Notify_Save_Voice();
  void Notify_Play_Voice();
  void Update_From_Frequency();
  void Update_From_Max_Frequency();
  void Update_From_Min_Frequency();
  void Update_From_Spectrum_Analyzer();
  void Update_From_PTT();
  void Update_From_Send_Morse();
  void STATIONS();
  void Release_Windows();
  void Configure();
  void Check_Key();
  void Open_Window_Number(int);
//  void Remove_Old_Classes();
  DECLARE_RESPONSE_TABLE(TMainWindow);
};

TMainWindow *MainWindow;

TMainWindow::TMainWindow(AWindow *AParent, LPSTR ATitle)
								 : AWindow(AParent, ATitle,-1)

{
  Attr.X = starting_places[9][0];
  Attr.Y = starting_places[9][1];
  Attr.W = starting_sizes[0][0];
  Attr.H = starting_sizes[0][1];
  AssignMenu("RADIOWARE_MENU");
  CallBackInstance = MakeProcInstance((FARPROC) CallBackFunction,_hInstance);
}

TMainWindow::~TMainWindow()

{
  FILE *stream;
  int count;

  KillTimer(TimerID);
  ptt_control->Set_Value(0);
  closing = 1;
  Clear_Preselectors();
  save_places();
  save_settings();
  DSPUnloadAlg();
  DSPReset();
  DSPLoadAlg( 0, NULL, "dsp001", NULL);
  stream = fopen(stations_name,"w");
  for(count=0;count<MAX_STATIONS;count++)
    if (stations[count].name[0] == 0)
      fprintf(stream,"NULL\n0\n0\n0\n");
    else
      fprintf(stream,"%s\n%ld\n%d\n%d\n",stations[count].name,
    	      stations[count].frequency,stations[count].mode,
	      stations[count].band);
  for(count=0;count<MAX_QUICK_STATIONS;count++)
    if (quick_stations[count].name[0] == 0)
      fprintf(stream,"%d\n%ld\n%d\n%d\n",count,
              quick_stations[count].frequency,quick_stations[count].mode,
	           quick_stations[count].band);
    else
      fprintf(stream,"%s\n%ld\n%d\n%d\n",quick_stations[count].name,
    	        quick_stations[count].frequency,quick_stations[count].mode,
	           quick_stations[count].band);
  fclose(stream);
}

void TMainWindow::GetWindowClass(WNDCLASS _FAR & AWndClass)

{
  TFrameWindow::GetWindowClass(AWndClass);
  AWndClass.hIcon = LoadIcon(_hInstance,"R232");
}

void TMainWindow::SetupWindow()

{
  int number_of_devices,error;
  char string[600];
  char message[6];
  int count,count2;
  int temp_top[NUMBER_OF_WINDOWS];

  number_of_devices = DSPGetNumDevs();
  DSPReset();
  TestWindow = HWindow;
  if (number_of_devices>0)
  {
	 if ((error = DSPLoadAlg(0,(DWORD) HWindow,"RADIO",(DWORD) HWindow))<0)
	 {
      if (error == -3)
        sprintf(string,"PSA compatible sound card not found.\n"
                       "Continuing in simulation mode.\n\n"
                       "  If you have a PSA sound card installed,\n"
                       "  the most likely cause of this error is\n"
                       "  the driver being disabled. To remedy,\n"
                       "  go to the program manager, select the\n"
                       "  'Main' Group, 'Control Panel', 'Drivers'.\n"
                       "  then select 'PSA SDK ...' & click on setup.\n"
                       "  under status, click on enabled.\n"
                       "  then close down these windows.\n"
                       "  this should solve the problem if it was\n"
                       "  disabled.\n");
      else
        sprintf(string,"DSP Load Failure %d",error);
		MessageBox(string,"WARNING",MB_ICONSTOP | MB_OK);
		Simulation_Mode();
//		abort();
	 }
  }
  else
  {
	 MessageBox("ADSP 2115 (On Sound Card) not responding.","ERROR",
					MB_ICONSTOP | MB_OK);
    Simulation_Mode();
  }
  frequency_control = new Frequency_Control();
  frequency_control->Set_Value(30000000); // to start up vco
  volume_control = new Volume_Control();
  mic_level_control = new Mic_Level_Control();
  agc_control = new AGC_Control();
  am_offset_control = new AM_Frequency_Offset_Control();
  antenna_power_control = new ANTENNA_POWER_Control();
  tx_antenna_power_control = new TXANTENNA_POWER_Control();
  if_gain_control = new IF_GAIN_Control();
  filter_control = new FILTER_Control();
  mode_control = new MODE_Control();
  mixer_gain_control = new MIXER_GAIN_Control();
  split_control = new Split_Control();
  tx_frequency_control = new TX_Frequency_Control();
  calibrate_control = new Calibrate_Control();
  max_frequency_control = new Max_Frequency_Control();
  min_frequency_control = new Min_Frequency_Control();
  ptt_control = new PTT_Control();
  monitor_control = new Monitor_Control();
  compression_control = new Compression_Control();
  compression_level_control = new Compression_Level_Control();
  i_adjust_control = new I_ADJUST_Control();

  if (starting_sizes[2][1]>0)
    PTCalibrate = new TFreq_Window2(this,201,"Calibrate",calibrate_control,2,
                                    starting_sizes[2][0],starting_sizes[2][1],1,0);
  PTCal = new TStatic(this,202,"",0,0,200,100);
  if (starting_sizes[13][1]>0)
    PTTXFreq = new TFreq_Window2(this,-1,"VFO B",tx_frequency_control,26,
                                  starting_sizes[13][0],starting_sizes[13][1],1,1);
  if (starting_sizes[3][1]>0)
    PTMaxFreq = new TFreq_Window2(this,190,"Max Frequency",max_frequency_control,
											 10,starting_sizes[3][0],starting_sizes[3][1],1,2);
  if (starting_sizes[4][1]>0)
    PTMinFreq = new TFreq_Window2(this,191,"Min Frequency",min_frequency_control,
                                  11,starting_sizes[4][0],starting_sizes[4][1],1,2);
  if (starting_sizes[1][1]>0)
    PTFreq = new TFreq_Window2(this,192,"VFO A",frequency_control,7,
                                   starting_sizes[1][0],starting_sizes[1][1],1,1);
  TFrameWindow::SetupWindow();
  load_settings();
  if (PTFreq)
    PTFreq->Redraw_Frequency();
  message[0] = 1;
  message[1] = 0;
  DSPSendMsg(msg_LOG_TOGGLE,message,2);
  dly();
  message[0] = 0;
  DSPSendMsg(msg_PLOT_TOGGLE,message,2);
  dly();
//  PostMessage(WM_COMMAND,206,0); /* Release Windows, Doesn't work yet! */
  for(count=0;count<NUMBER_OF_WINDOWS;count++)
    temp_top[count] = windows_on_top[count];
  for(count2 = 100;count2>0;count2--)
    for(count=0;count<50;count++)
      if (temp_top[count] == count2)
        if (starting_places[count][1]>0)
          Open_Window_Number(count);
//  print_int(joySetCapture(HWindow,JOYSTICKID1,1000,FALSE));
  outport(0x230,inport(0x230)|0x0400);
//  PostMessage(WM_COMMAND,500,NULL);
  TimerID = SetTimer(1,1,NULL);
  if (TimerID == NULL)
  {
    sprintf(string,"Warning: Windows has no timer rescources left to "
                   "Allocate to the CW Keyer. The keyer will not be  "
                   "checked, and can not be used.");
    MessageBox(string,"Warning",MB_OK | MB_ICONSTOP);
  }
  Blue = CreatePenIndirect(&lpBlue);
  Green = CreatePenIndirect(&lpGreen);
  Red = CreatePenIndirect(&lpRed);
}

void TMainWindow::Open_Window_Number(int which)

{
  char name[20];
  switch (which)
  {
    case 0 :  PTVolume = new TCONTROLChld(this,-1,"Volume",0,volume_control,volume_labels,-64,"SPEAKER",0);
              PTVolume->Create();
              break;
    case 1 :  PTAGC = new TCONTROLChld(this,-1,"AGC",1,agc_control,agc_labels,3,"AGC",1);
              PTAGC->Create();
              break;
    case 2 :  PTGain = new TCONTROLChld(this,-1,"Analog Gain",2,if_gain_control,NULL,16,"AMP",0);
              PTGain->Create();
              break;
    case 3 :  PTFilter = new TCONTROLChld(this,-1,"Filter",3,filter_control,filter_labels,6,"FILTER",1);
              PTFilter->Create();
              break;
    case 4 :  PTMode = new TCONTROLChld(this,-1,"Mode",4,mode_control,mode_labels,5,"MODULATION",1);
              PTMode->Create();
              break;
    case 5 :  PTAntenna_Power = new TCONTROLChld(this,-1,"RX Ant DC Pwr",5,antenna_power_control,antenna_power_labels,2,"ANTENNA",1);
              PTAntenna_Power->Create();
              break;
    case 6 :  PTMixer_Gain = new TCONTROLChld(this,-1,"Mixer Gain",6,mixer_gain_control,mixer_gain_labels,3,"AMPATTN",0);
              PTMixer_Gain->Create();
              break;
	 case 18 : PT_CallSign = new TCallSign(this,-1,"CallSign",18,starting_sizes[11][0],
				                      starting_sizes[11][1],CallSign,"R232");
              PT_CallSign->Create();
              break;
  	 case 19 : PTS_Meter = new TSMTRChld(this,-1,"S Meter",19,starting_sizes[12][0],
										  starting_sizes[12][1],"SMETER",1);
              PTS_Meter->Create();
              break;
    case 20 : PTPTT = new TCONTROLChld(this,ID_PTT,"PTT",20,ptt_control,ptt_labels,2,"R232",1);
              PTPTT->Create();
              break;
    case 21 : PTMic_Level = new TCONTROLChld(this,-1,"Mic Gain",21,mic_level_control,volume_labels,64,"SPEAKER",0);
              PTMic_Level->Create();
              break;
    case 22 : PTTXAntenna_Power = new TCONTROLChld(this,-1,"TX Ant DC Pwr",22,tx_antenna_power_control,antenna_power_labels,2,"ANTENNA",1);
              PTTXAntenna_Power->Create();
              break;
    case 23 : PTCompression = new TCONTROLChld(this,ID_COMPRESSION,"Compression",23,compression_control,compression_labels,2,"AGC",1);
              PTCompression->Create();
              break;
    case 24 : PTCompression_Level = new TCONTROLChld(this,-1,"Comp Level",24,compression_level_control,volume_labels,64,"SPEAKER",0);
              PTCompression_Level->Create();
              break;
    case 25 : PTSplit = new TCONTROLChld(this,-1,"Split Freq.",25,split_control,split_labels,4,"R232",1);
              PTSplit->Create();
              break;
/*    case 27 : PTMorse = new TSEND_MORSEChld(this,ID_SEND_MORSE,"Morse to be sent",27,400,69);
              PTMorse->Create();
              break;
*/    case 28 : PTSent_Morse = new TSENT_MORSEChld(this,"Sent Morse",28,400,200);
              PTSent_Morse->Create();
              break;
    case 29 : PTMonitor = new TCONTROLChld(this,-1,"Monitor",29,monitor_control,NULL,37,"R232",0);
              PTMonitor->Create();
              break;
    case 30 : PTAM_Meter = new TSMTRChld(this,-1,"AM Offset",30,140,95,"SMETER",0);
              PTAM_Meter->Create();
              break;
    case 31 :
    case 32 :
    case 33 :
    case 34 :
    case 35 :
    case 36 :
    case 37 :
    case 38 :
    case 39 : sprintf(name,"Channels %d",which-30);
              PTStations[which-31] = new TSTATChld(this,name,which,0);
              PTStations[which-31]->Create();
              break;
    case 40 : PTAmateurBands = new TBANDChld(this,"Amateur Bands",40);
              PTAmateurBands->Create();
              break;
    case 41 :
    case 42 :
    case 43 :
    case 44 :
    case 45 :
    case 46 :
    case 47 :
    case 48 :
    case 49 : sprintf(name,"Voice Mem. %c",which-41+'A');
              PTVoice_Memories[which-41] = new TVoiceChld(this,name,0,which);
              PTVoice_Memories[which-41]->Create();
              break;
    }
}

void TMainWindow::Window_Defaults()

{
  MoveWindow(0,0,639,479,1);
  if (PTVolume != NULL && PTVolume->Attr.H>=0)
    PTVolume->Move(50,50,140,101);
  if (PTAGC != NULL && PTAGC->Attr.H>=0)
    PTAGC->Move(190,50,140,101);
  if (PTGain != NULL && PTGain->Attr.H>=0)
    PTGain->Move(330,50,140,101);
  if (PTFilter != NULL && PTFilter->Attr.H>=0)
    PTFilter->Move(470,50,140,101);
  if (PTMode != NULL && PTMode->Attr.H>=0)
    PTMode->Move(50,151,140,101);
  if (PTAntenna_Power != NULL && PTAntenna_Power->Attr.H>=0)
    PTAntenna_Power->Move(190,151,140,101);
  if (PTMixer_Gain != NULL && PTMixer_Gain->Attr.H>=0)
    PTMixer_Gain->Move(330,151,140,101);
  if (PTPTT != NULL && PTPTT->Attr.H>=0)
    PTPTT->Move(470,252,140,101);
  if (PTMic_Level != NULL && PTMic_Level->Attr.H>=0)
    PTMic_Level->Move(50,252,140,101);
  if (PTTXAntenna_Power != NULL && PTTXAntenna_Power->Attr.H>=0)
    PTTXAntenna_Power->Move(190,252,140,101);
  if (PTCompression != NULL && PTCompression->Attr.H>=0)
    PTCompression->Move(330,252,140,101);
  if (PTCompression_Level != NULL && PTCompression_Level->Attr.H>=0)
    PTCompression_Level->Move(470,252,140,101);
  if (PTSplit != NULL && PTSplit->Attr.H>=0)
    PTSplit->Move(50,353,140,101);
  if (PTMonitor != NULL && PTMonitor->Attr.H>=0)
    PTMonitor->Move(190,353,140,101);
  if (PTStations[0] != NULL && PTStations[0]->Attr.H>=0)
    PTStations[0]->Move(330,353,140,101);
  if (PTStations[1] != NULL && PTStations[1]->Attr.H>=0)
    PTStations[1]->Move(340,363,140,101);
  if (PTStations[2] != NULL && PTStations[2]->Attr.H>=0)
    PTStations[2]->Move(350,373,140,101);
  if (PTStations[3] != NULL && PTStations[3]->Attr.H>=0)
    PTStations[3]->Move(360,383,140,101);
  if (PTStations[4] != NULL && PTStations[4]->Attr.H>=0)
    PTStations[4]->Move(370,393,140,101);
  if (PTStations[5] != NULL && PTStations[5]->Attr.H>=0)
    PTStations[5]->Move(380,403,140,101);
  if (PTStations[6] != NULL && PTStations[6]->Attr.H>=0)
    PTStations[6]->Move(390,413,140,101);
  if (PTStations[7] != NULL && PTStations[7]->Attr.H>=0)
    PTStations[7]->Move(400,423,140,101);
  if (PTStations[8] != NULL && PTStations[8]->Attr.H>=0)
    PTStations[8]->Move(410,433,140,101);
  if (PTFreq != NULL && PTFreq->Attr.H>=0)
    PTFreq->Move(50,353,275,70);
  if (PTTXFreq != NULL && PTTXFreq->Attr.H>=0)
    PTTXFreq->Move(50,423,275,70);
  if (PTCalibrate != NULL && PTCalibrate->Attr.H>=0)
    PTCalibrate->Move(50,453,275,70);
  if (PTMaxFreq != NULL && PTMaxFreq->Attr.H>=0)
    PTMaxFreq->Move(325,353,275,70);
  if (PTMinFreq != NULL && PTMinFreq->Attr.H>=0)
    PTMinFreq->Move(325,423,275,70);
  if (PTSpectrum != NULL && PTSpectrum->Attr.H>=0)
    PTSpectrum->Move(150,150,256,296);
  if (PT_LSB_Spectrum != NULL && PT_LSB_Spectrum->Attr.H>=0)
    PT_LSB_Spectrum->Move(0,50,135,281);
  if (PT_USB_Spectrum != NULL && PT_USB_Spectrum->Attr.H>=0)
    PT_USB_Spectrum->Move(400,50,135,281);
  if (PT_LSB_Time != NULL && PT_LSB_Time->Attr.H>=0)
    PT_LSB_Time->Move(0,350,135,150);
  if (PT_USB_Time != NULL && PT_USB_Time->Attr.H>=0)
    PT_USB_Time->Move(400,350,135,150);
  if (PT_XY_Time != NULL && PT_XY_Time->Attr.H>=0)
    PT_XY_Time->Move(150,200,264,304);
}

int old_key=254;

void TMainWindow::EvTimer(uint timerId)

{
  PostMessage(WM_COMMAND,500,NULL);
}

void TMainWindow::Check_Key()

{
  char string[80];
  int new_key;

  new_key = inportb(0x201);
  if (new_key!=old_key)
  {
    Set_Morse(1-new_key/128);
    old_key = new_key;
  }
}

void TMainWindow::Window_AGC()

{
  if (!PTAGC)
  {
    PTAGC = new TCONTROLChld(this,-1,"AGC",1,agc_control,agc_labels,3,"AGC",1);
    PTAGC->Create();
  }
  else
    if (PTAGC->Attr.Y < 0)
    {
      PTAGC = new TCONTROLChld(this,-1,"AGC",1,agc_control,agc_labels,3,"AGC",1);
      PTAGC->Create();
    }
    else
      PTAGC->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTAGC->HWindow);
}

void TMainWindow::Window_AM_Offset()

{
  if (!PTAM_Meter || delete_class[30])
  {
    PTAM_Meter = new TSMTRChld(this,-1,"AM Offset",30,140,95,"SMETER",0);
    PTAM_Meter->Create();
  }
  else
    PTAM_Meter->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTAM_Meter->HWindow);
}

void TMainWindow::Window_Analog_Gain()

{
  if (!PTGain || delete_class[2])
  {
    PTGain = new TCONTROLChld(this,-1,"Analog Gain",2,if_gain_control,NULL,16,"AMP",0);
    PTGain->Create();
    delete_class[2] = 0;
  }
  else
    PTGain->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTGain->HWindow);
}

void TMainWindow::Window_Antenna_Power()

{

  if (!PTAntenna_Power || delete_class[5])
  {
    PTAntenna_Power = new TCONTROLChld(this,-1,"RX Ant DC Pwr",5,antenna_power_control,antenna_power_labels,2,"ANTENNA",1);
    PTAntenna_Power->Create();
    delete_class[5] = 0;
  }
  else
    PTAntenna_Power->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTAntenna_Power->HWindow);
}

void TMainWindow::Window_TX_Antenna()

{
  if (!PTTXAntenna_Power)
  {
    PTTXAntenna_Power = new TCONTROLChld(this,-1,"TX Ant DC Pwr",22,tx_antenna_power_control,antenna_power_labels,2,"ANTENNA",1);
    PTTXAntenna_Power->Create();
  }
  else
    if (PTTXAntenna_Power->Attr.Y < 0)
    {
      PTTXAntenna_Power = new TCONTROLChld(this,-1,"Tx Ant DC Pwr",22,
                      tx_antenna_power_control,antenna_power_labels,2,"ANTENNA",1);
      PTTXAntenna_Power->Create();
    }
    else
      PTTXAntenna_Power->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTTXAntenna_Power->HWindow);
}

void TMainWindow::Window_Calibrate()

{
  if (PTCalibrate == NULL)
  {
    PTCalibrate = new TFreq_Window2(this,103,"Calibrate",calibrate_control,8,
                                  starting_sizes[2][0]-8,-starting_sizes[2][1]+27,1,0);
    PTCalibrate->Create();
  }
  else
    if (PTCalibrate->Attr.H <0)
    {
      PTCalibrate = new TFreq_Window2(this,103,"Calibrate",calibrate_control,8,
                                          PTCalibrate->Attr.W,-PTCalibrate->Attr.H,1,0);
      PTCalibrate->Create();
    }
    else
      PTCalibrate->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTCalibrate->HWindow);
}

void TMainWindow::Window_Filter()

{
  int x,y;

  if (!PTFilter)
  {
    PTFilter = new TCONTROLChld(this,-1,"Filter",3,filter_control,filter_labels,6,"FILTER",1);
    PTFilter->Create();
  }
  else
    if (PTFilter->Attr.Y < 0)
    {
      x = PTFilter->Attr.X;
      y = PTFilter->Attr.Y;
      PTFilter = new TCONTROLChld(this,-1,"Filter",3,filter_control,filter_labels,6,"FILTER",1);
      PTFilter->Create();
    }
    else
      PTFilter->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTFilter->HWindow);
}

void TMainWindow::Window_Frequency()

{
  if (!PTFreq)
  {
    PTFreq = new TFreq_Window2(this,192,"VFO A",frequency_control,7,
                                   starting_sizes[1][0]-8,-starting_sizes[1][1]+27,1,1);
    PTFreq->Create();
  }
  else
    if (PTFreq->Attr.H <0)
    {
      PTFreq = new TFreq_Window2(this,200,"VFO A",frequency_control,7,
                                 PTFreq->Attr.W,-PTFreq->Attr.H,1,1);
      PTFreq->Create();
    }
    else
      PTFreq->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTFreq->HWindow);
}

void TMainWindow::Window_TX_Frequency()

{
  if (PTTXFreq == NULL)
  {
    PTTXFreq = new TFreq_Window2(this,-1,"VFO B",tx_frequency_control,26,
                                  starting_sizes[13][0]-8,-starting_sizes[13][1]+27,1,1);
    PTTXFreq->Create();
  }
  else
    if (PTTXFreq->Attr.H <0)
    {
      PTTXFreq = new TFreq_Window2(this,-1,"VFO B",tx_frequency_control,26,
                                          PTTXFreq->Attr.W,-PTTXFreq->Attr.H,1,1);
      PTTXFreq->Create();
    }
    else
      PTTXFreq->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTTXFreq->HWindow);
}

void TMainWindow::Window_Max_Frequency()

{
  if (PTMaxFreq == NULL)
  {
    PTMaxFreq = new TFreq_Window2(this,190,"Max Frequency",max_frequency_control,10,
                                  starting_sizes[3][0]-8,-starting_sizes[3][1]+27,1,2);
    PTMaxFreq->Create();
  }
  else
    if (PTMaxFreq->Attr.H <0)
    {
      if (PTMaxFreq->Attr.H == 0)
        PTMaxFreq->Attr.H = -100;
      PTMaxFreq = new TFreq_Window2(this,190,"Max Frequency",max_frequency_control,10,
                                    PTMaxFreq->Attr.W,-PTMaxFreq->Attr.H,1,2);
      PTMaxFreq->Create();
    }
    else
      PTMaxFreq->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTMaxFreq->HWindow);
}

void TMainWindow::Window_Min_Frequency()

{
  if (PTMinFreq == NULL)
  {
    PTMinFreq = new TFreq_Window2(this,191,"Min Frequency",min_frequency_control,11,
                                  starting_sizes[4][0]-8,-starting_sizes[4][1]+27,1,2);
    PTMinFreq->Create();
  }
  else
    if (PTMinFreq->Attr.H < 0)
    {
      if (PTMinFreq->Attr.H == 0)
        PTMinFreq->Attr.H = -100;
      PTMinFreq = new TFreq_Window2(this,191,"Min Frequency",min_frequency_control,11,
                                    PTMinFreq->Attr.W,-PTMinFreq->Attr.H,1,2);
      PTMinFreq->Create();
    }
    else
      PTMinFreq->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTMinFreq->HWindow);
}

void TMainWindow::Window_Mixer_Gain()

{
  int x,y;
  if (!PTMixer_Gain)
  {
    PTMixer_Gain = new TCONTROLChld(this,-1,"Mixer Gain",6,mixer_gain_control,mixer_gain_labels,3,"AMPATTN",0);
    PTMixer_Gain->Create();
  }
  else
    if (PTMixer_Gain->Attr.Y < 0)
    {
      x = PTMixer_Gain->Attr.X;
      y = PTMixer_Gain->Attr.Y;
      PTMixer_Gain = new TCONTROLChld(this,-1,"Mixer Gain",6,mixer_gain_control,
                                      mixer_gain_labels,3,"AMPATTN",0);
      PTMixer_Gain->Create();
    }
    else
      PTMixer_Gain->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTMixer_Gain->HWindow);
}

void TMainWindow::Window_Mode()

{
  int x,y;

  if (!PTMode)
  {
    PTMode = new TCONTROLChld(this,-1,"Mode",4,mode_control,mode_labels,5,"MODULATION",1);
    PTMode->Create();
  }
  else
    if (PTMode->Attr.Y < 0)
    {
      x = PTMode->Attr.X;
      y = PTMode->Attr.Y;
      PTMode = new TCONTROLChld(this,-1,"Mode",4,mode_control,mode_labels,5,"MODULATION",1);
      PTMode->Create();
    }
    else
      PTMode->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTMode->HWindow);
}

void TMainWindow::Window_PTT()

{
  int x,y;

  if (!PTPTT)
  {
    PTPTT = new TCONTROLChld(this,ID_PTT,"PTT",20,ptt_control,ptt_labels,2,"R232",1);
    PTPTT->Create();
  }
  else
    if (PTPTT->Attr.Y < 0)
    {
      x = PTPTT->Attr.X;
      y = PTPTT->Attr.Y;
      PTPTT = new TCONTROLChld(this,-1,"PTT",20,ptt_control,ptt_labels,2,"R232",1);
      PTPTT->Create();
    }
    else
      PTPTT->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTPTT->HWindow);
}

void TMainWindow::Window_Spectrum_Analyzer()

{
  if (PTSpectrum == NULL)
  {
    PTSpectrum = new TSpectrum(this,ID_SPECTRUM,"Spectrum",12,starting_sizes[5][0],
                               starting_sizes[5][1],frequency_control,
                               lsb_string,usb_string,-2);
    PTSpectrum->Create();
    DSPSendMsg(msg_PLOT_LSB_S,ON_STRING,3);
/*    dly();
    DSPSendMsg(msg_PLOT_USB_S,ON_STRING,3);
*/  }
  else
    if (PTSpectrum->Attr.H < 0)
    {
      PTSpectrum = new TSpectrum(this,ID_SPECTRUM,"Spectrum",12,PTSpectrum->Attr.W,
                                 -PTSpectrum->Attr.H,frequency_control,
                                 lsb_string,usb_string,-2);
      PTSpectrum->Create();
      DSPSendMsg(msg_PLOT_LSB_S,ON_STRING,3);
/*      dly();
      DSPSendMsg(msg_PLOT_USB_S,ON_STRING,3);
*/    }
    else
      PTSpectrum->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTSpectrum->HWindow);
}

void TMainWindow::Window_LSB_Spectrum()

{
  if (PT_LSB_Spectrum == NULL)
  {
    PT_LSB_Spectrum = new TSpectrum(this,ID_LSB_SPECTRUM,"LSB",13,starting_sizes[6][0],
                                 starting_sizes[6][1],frequency_control,
                                 lsb_string,NULL,-1);
    PT_LSB_Spectrum->Create();
    DSPSendMsg(msg_PLOT_LSB_S,ON_STRING,3);
  }
  else
    if (PT_LSB_Spectrum->Attr.H < 0)
    {
      PT_LSB_Spectrum = new TSpectrum(this,ID_LSB_SPECTRUM,"LSB",13,PT_LSB_Spectrum->Attr.W,
                                 -PT_LSB_Spectrum->Attr.H,frequency_control,
                                 lsb_string,NULL,-1);
      PT_LSB_Spectrum->Create();
      DSPSendMsg(msg_PLOT_LSB_S,ON_STRING,3);
    }
    else
    {
      PT_LSB_Spectrum->Show(SW_SHOWNORMAL);
      DSPSendMsg(msg_PLOT_LSB_S,ON_STRING,3);
    }
  ::BringWindowToTop(PT_LSB_Spectrum->HWindow);
}

void TMainWindow::Window_USB_Spectrum()

{
  if (PT_USB_Spectrum == NULL)
  {
    PT_USB_Spectrum = new TSpectrum(this,ID_USB_SPECTRUM,"USB",14,starting_sizes[7][0],
                                 starting_sizes[7][1],frequency_control,
                                 usb_string,NULL,0);
    PT_USB_Spectrum->Create();
    DSPSendMsg(msg_PLOT_USB_S,ON_STRING,3);
  }
  else
    if (PT_USB_Spectrum->Attr.H < 0)
    {
      PT_USB_Spectrum = new TSpectrum(this,ID_USB_SPECTRUM,"USB",14,PT_USB_Spectrum->Attr.W,
                                 -PT_USB_Spectrum->Attr.H,frequency_control,
                                 usb_string,NULL,0);
      PT_USB_Spectrum->Create();
      DSPSendMsg(msg_PLOT_USB_S,ON_STRING,3);
    }
    else
    {
      PT_USB_Spectrum->Show(SW_SHOWNORMAL);
      DSPSendMsg(msg_PLOT_USB_S,ON_STRING,3);
    }
  ::BringWindowToTop(PT_USB_Spectrum->HWindow);
}

void TMainWindow::Window_LSB_Time()

{
  if (PT_LSB_Time == NULL)
  {
    PT_LSB_Time = new TSpectrum(this,ID_LSB_TIME,"LSB Time",15,starting_sizes[8][0],
                                 starting_sizes[8][1],frequency_control,
                                 lsb_time_string,NULL,1);
    PT_LSB_Time->Create();
    DSPSendMsg(msg_PLOT_LSB_T,ON_STRING,3);
  }
  else
    if (PT_LSB_Time->Attr.H < 0)
    {
      PT_LSB_Time = new TSpectrum(this,ID_LSB_TIME,"LSB Time",15,PT_LSB_Time->Attr.W,
                                 -PT_LSB_Time->Attr.H,frequency_control,
                                 lsb_time_string,NULL,1);
      PT_LSB_Time->Create();
      DSPSendMsg(msg_PLOT_LSB_T,ON_STRING,3);
    }
    else
    {
      PT_LSB_Time->Show(SW_SHOWNORMAL);
      DSPSendMsg(msg_PLOT_LSB_T,ON_STRING,3);
    }
  ::BringWindowToTop(PT_LSB_Time->HWindow);
}

void TMainWindow::Window_USB_Time()

{
  if (PT_USB_Time == NULL)
  {
    PT_USB_Time = new TSpectrum(this,ID_USB_TIME,"USB Time",16,starting_sizes[9][0],
                                 starting_sizes[9][1],frequency_control,
                                 usb_time_string,NULL,1);
    PT_USB_Time->Create();
    DSPSendMsg(msg_PLOT_USB_T,ON_STRING,3);
  }
  else
    if (PT_USB_Time->Attr.H < 0)
    {
      PT_USB_Time = new TSpectrum(this,ID_USB_TIME,"USB Time",16,PT_USB_Time->Attr.W,
                                 -PT_USB_Time->Attr.H,frequency_control,
                                 usb_time_string,NULL,1);
      PT_USB_Time->Create();
      DSPSendMsg(msg_PLOT_USB_T,ON_STRING,3);
    }
    else
    {
      PT_USB_Time->Show(SW_SHOWNORMAL);
      DSPSendMsg(msg_PLOT_USB_T,ON_STRING,3);
    }
  ::BringWindowToTop(PT_USB_Time->HWindow);
}

void TMainWindow::Window_XY_Time()

{
  if (PT_XY_Time == NULL)
  {
	 PT_XY_Time = new TSpectrum(this,ID_XY_TIME,"XY Time",17,starting_sizes[10][0],
											starting_sizes[10][1],frequency_control,
											usb_time_string,lsb_time_string,2);
	 PT_XY_Time->Create();
	 DSPSendMsg(msg_PLOT_USB_T,ON_STRING,3);
    dly();
	 DSPSendMsg(msg_PLOT_LSB_T,ON_STRING,3);
  }
  else
    if (PT_XY_Time->Attr.H < 0)
    {
      PT_XY_Time = new TSpectrum(this,ID_XY_TIME,"XY Time",17,PT_XY_Time->Attr.W,
                                 -PT_XY_Time->Attr.H,frequency_control,
                                 usb_time_string,lsb_time_string,2);
      PT_XY_Time->Create();
      DSPSendMsg(msg_PLOT_USB_T,ON_STRING,3);
      DSPSendMsg(msg_PLOT_LSB_T,ON_STRING,3);
    }
    else
	 {
      PT_XY_Time->Show(SW_SHOWNORMAL);
		DSPSendMsg(msg_PLOT_USB_T,ON_STRING,3);
      DSPSendMsg(msg_PLOT_LSB_T,ON_STRING,3);
    }
  ::BringWindowToTop(PT_XY_Time->HWindow);
}

void TMainWindow::Window_S_Meter()

{
  int x,y,w,h;

  if (PTS_Meter==NULL)
  {
	 PTS_Meter = new TSMTRChld(this,-1,"S Meter",19,starting_sizes[12][0],
										  starting_sizes[12][1],"SMETER",1);
	 PTS_Meter->Create();
  }
  else
    if (PTS_Meter->Attr.Y < 0)
    {
      x = PTS_Meter->Attr.X;
	   y = -PTS_Meter->Attr.Y;
      w = PTS_Meter->Attr.W;
      h = PTS_Meter->Attr.H;
//    delete PTS_Meter;
	   PTS_Meter = new TSMTRChld(this,-1,"S Meter",19,w,h,"SMETER",1);
	   PTS_Meter->Create();
    }
    else
	   PTS_Meter->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTS_Meter->HWindow);
}

void TMainWindow::Window_Morse()

{
  int x,y;

  if (PTMorse==NULL)
  {
	 PTMorse = new TSEND_MORSEChld(this,ID_SEND_MORSE,"Morse to be sent",27,400,69);
	 PTMorse->Create();
  }
  else
    if (PTMorse->Attr.Y < 0)
    {
	   x = PTMorse->Attr.X;
      y = -PTMorse->Attr.Y;
      PTMorse = new TSEND_MORSEChld(this,ID_SEND_MORSE,"Morse to be sent",27,400,69);
      PTMorse->Create();
    }
    else
      PTMorse->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTMorse->HWindow);
}

void TMainWindow::Window_Sent_Morse()

{
  int x,y;

  if (PTSent_Morse==NULL)
  {
	 PTSent_Morse = new TSENT_MORSEChld(this,"Sent Morse",28,400,300);
	 PTSent_Morse->Create();
  }
  else
    if (PTSent_Morse->Attr.Y < 0)
    {
	   x = PTSent_Morse->Attr.X;
      y = -PTSent_Morse->Attr.Y;
	   PTSent_Morse = new TSENT_MORSEChld(this,"Sent Morse",28,400,300);
	   PTSent_Morse->Create();
    }
    else
	   PTSent_Morse->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTSent_Morse->HWindow);
}

void TMainWindow::Window_Recieved_Morse()

{
  int x,y;

/*  if (PTSent_Morse==NULL)
  {
	 PTSent_Morse = new TSENT_MORSEChld(this,"Sent Morse",starting_places[28][0],
										  starting_places[28][1],400,300);
	 PTSent_Morse->Create();
  }
  if (PTSent_Morse->Attr.H < 0)
  {
	 x = PTSent_Morse->Attr.X;
	 y = PTSent_Morse->Attr.Y;
	 PTSent_Morse = new TSENT_MORSEChld(this,"Sent Morse",x,y,400,300);
	 PTSent_Morse->Create();
  }
  else
	 PTSent_Morse->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTSent_Morse->HWindow);
*/}

void TMainWindow::Window_Volume()

{
  int x,y;

  if (!PTVolume)
  {
    PTVolume = new TCONTROLChld(this,-1,"Volume",0,volume_control,volume_labels,-64,"SPEAKER",0);
    PTVolume->Create();
  }
  else
    if (PTVolume->Attr.Y < 0)
    {
	   x = PTVolume->Attr.X;
      y = PTVolume->Attr.Y;
      PTVolume = new TCONTROLChld(this,-1,"Volume",0,volume_control,volume_labels,-64,"SPEAKER",0);
      PTVolume->Create();
    }
    else
	   PTVolume->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTVolume->HWindow);
}

void TMainWindow::Window_Fast_Channels_1()

{
  int x,y;

  if (!PTStations[0])
  {
    PTStations[0] = new TSTATChld(this,"Channels 1",31,0);
    PTStations[0]->Create();
  }
  else
    if (PTStations[0]->Attr.Y<0)
    {
      x = PTStations[0]->Attr.X;
      y = -PTStations[0]->Attr.Y;
      PTStations[0] = new TSTATChld(this,"Channels 1",31,0);
      PTStations[0]->Create();
    }
    else
		PTStations[0]->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTStations[0]->HWindow);
}

void TMainWindow::Window_Fast_Channels_2()

{
  int x,y;

  if (!PTStations[1])
  {
    PTStations[1] = new TSTATChld(this,"Channels 2",32,1);
    PTStations[1]->Create();
  }
  else
    if (PTStations[1]->Attr.Y<0)
    {
      x = PTStations[1]->Attr.X;
      y = -PTStations[1]->Attr.Y;
		PTStations[1] = new TSTATChld(this,"Channels 2",32,1);
      PTStations[1]->Create();
    }
    else
	   PTStations[1]->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTStations[1]->HWindow);
}

void TMainWindow::Window_Fast_Channels_3()

{
  int x,y;

  if (!PTStations[2])
  {
    PTStations[2] = new TSTATChld(this,"Channels 3",33,2);
    PTStations[2]->Create();
  }
  else
	 if (PTStations[2]->Attr.Y<0)
    {
      x = PTStations[2]->Attr.X;
      y = -PTStations[2]->Attr.Y;
      PTStations[2] = new TSTATChld(this,"Channels 3",33,2);
      PTStations[2]->Create();
    }
    else
	   PTStations[2]->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTStations[2]->HWindow);
}

void TMainWindow::Window_Fast_Channels_4()

{
  int x,y;

  if (!PTStations[3])
  {
    PTStations[3] = new TSTATChld(this,"Channels 4",34,3);
    PTStations[3]->Create();
  }
  else
    if (PTStations[3]->Attr.Y<0)
    {
      x = PTStations[3]->Attr.X;
      y = -PTStations[3]->Attr.Y;
      PTStations[3] = new TSTATChld(this,"Channels 4",34,3);
      PTStations[3]->Create();
    }
    else
	   PTStations[3]->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTStations[3]->HWindow);
}

void TMainWindow::Window_Fast_Channels_5()

{
  int x,y;

  if (!PTStations[4])
  {
    PTStations[4] = new TSTATChld(this,"Channels 5",35,4);
    PTStations[4]->Create();
  }
  else
    if (PTStations[4]->Attr.Y<0)
    {
      x = PTStations[4]->Attr.X;
      y = -PTStations[4]->Attr.Y;
      PTStations[4] = new TSTATChld(this,"Channels 5",35,4);
      PTStations[4]->Create();
    }
    else
	   PTStations[4]->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTStations[4]->HWindow);
}

void TMainWindow::Window_Fast_Channels_6()

{
  int x,y;

  if (!PTStations[5])
  {
    PTStations[5] = new TSTATChld(this,"Channels 6",36,5);
    PTStations[5]->Create();
  }
  else
    if (PTStations[5]->Attr.Y<0)
    {
      x = PTStations[5]->Attr.X;
      y = -PTStations[5]->Attr.Y;
      PTStations[5] = new TSTATChld(this,"Channels 6",36,5);
      PTStations[5]->Create();
    }
    else
	   PTStations[5]->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTStations[5]->HWindow);
}

void TMainWindow::Window_Fast_Channels_7()

{
  int x,y;

  if (!PTStations[6])
  {
    PTStations[6] = new TSTATChld(this,"Channels 7",37,6);
    PTStations[6]->Create();
  }
  else
    if (PTStations[6]->Attr.Y<0)
    {
      x = PTStations[6]->Attr.X;
      y = -PTStations[6]->Attr.Y;
      PTStations[6] = new TSTATChld(this,"Channels 7",37,6);
      PTStations[6]->Create();
    }
    else
	   PTStations[6]->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTStations[6]->HWindow);
}

void TMainWindow::Window_Fast_Channels_8()

{
  int x,y;

  if (!PTStations[7])
  {
    PTStations[7] = new TSTATChld(this,"Channels 8",38,7);
    PTStations[7]->Create();
  }
  else
    if (PTStations[7]->Attr.Y<0)
    {
      x = PTStations[7]->Attr.X;
      y = -PTStations[7]->Attr.Y;
      PTStations[7] = new TSTATChld(this,"Channels 8",38,7);
      PTStations[7]->Create();
    }
    else
	   PTStations[7]->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTStations[7]->HWindow);
}

void TMainWindow::Window_Fast_Channels_9()

{
  int x,y;

  if (!PTStations[8])
  {
	 PTStations[8] = new TSTATChld(this,"Channels 9",39,8);
	 PTStations[8]->Create();
  }
  else
	 if (PTStations[8]->Attr.Y<0)
	 {
		x = PTStations[8]->Attr.X;
		y = -PTStations[8]->Attr.Y;
		PTStations[8] = new TSTATChld(this,"Channels 9",39,8);
		PTStations[8]->Create();
	 }
	 else
		PTStations[8]->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTStations[8]->HWindow);
}

void TMainWindow::Window_Amateur_Bands()

{
  if (!PTAmateurBands)
  {
	 PTAmateurBands = new TBANDChld(this,"Amateur Bands",40);
	 PTAmateurBands->Create();
  }
  else
	 if (PTAmateurBands->Attr.Y<0)
	 {
		PTAmateurBands = new TBANDChld(this,"Amateur Bands",40);
		PTAmateurBands->Create();
	 }
	 else
		PTAmateurBands->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTAmateurBands->HWindow);
}

void TMainWindow::Window_Voice_Memories(WPARAM ID)

{
  int x,y,which;
  char name[80];

  Remove_Old_Classes();
  which = ID-220;
  if (which>=0 && which<=9)
  {
    sprintf(name,"Voice Mem. %c",which+'A');
    if (!PTVoice_Memories[which])
    {
      PTVoice_Memories[which] = new TVoiceChld(this,name,0,41+which);
      PTVoice_Memories[which]->Create();
    }
    else
      PTVoice_Memories[which]->Show(SW_SHOWNORMAL);
    ::BringWindowToTop(PTVoice_Memories[which]->HWindow);
  }
}

void TMainWindow::Window_Call_Sign()

{
  int x,y;

  if (PT_CallSign==NULL)
  {
	 PT_CallSign = new TCallSign(this,-1,"CallSign",18,starting_sizes[11][0],
										  starting_sizes[11][1],CallSign,"R232");
	 PT_CallSign->Create();
  }
  if (PT_CallSign->Attr.H <= 0)
  {
	 x = PT_CallSign->Attr.X;
	 y = PT_CallSign->Attr.Y;
	 PT_CallSign = new TCallSign(this,-1,"CallSign",18,PT_CallSign->Attr.W,
										  -PT_CallSign->Attr.H,CallSign,"R232");
	 PT_CallSign->Create();
  }
  else
	 PT_CallSign->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PT_CallSign->HWindow);
}

void TMainWindow::Window_Compression()

{
  int x,y;

  if (!PTCompression)
  {
    PTCompression = new TCONTROLChld(this,ID_COMPRESSION,"Compression",23,compression_control,compression_labels,2,"AGC",1);
    PTCompression->Create();
  }
  else
    if (PTCompression->Attr.Y<0)
    {
		x = PTCompression->Attr.X;
      y = PTCompression->Attr.Y;
      PTCompression = new TCONTROLChld(this,ID_COMPRESSION,"Compression",23,
                                       compression_control,compression_labels,2,"AGC",1);
      PTCompression->Create();
    }
    else
      PTCompression->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTCompression->HWindow);
}

void TMainWindow::Window_Compression_Level()

{
  int x,y;

  if (!PTCompression_Level)
  {
    PTCompression_Level = new TCONTROLChld(this,-1,"Comp Level",24,compression_level_control,volume_labels,64,"SPEAKER",0);
    PTCompression_Level->Create();
  }
  else
    if (PTCompression_Level->Attr.Y<0)
    {
      x = PTCompression_Level->Attr.X;
      y = PTCompression_Level->Attr.Y;
      PTCompression_Level = new TCONTROLChld(this,-1,"Comp Level",24,
                   compression_level_control,volume_labels,64,"SPEAKER",0);
      PTCompression_Level->Create();
    }
    else
      PTCompression_Level->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTCompression_Level->HWindow);
}

void TMainWindow::Window_Split()

{
  int x,y;

  if (!PTSplit)
  {
    PTSplit = new TCONTROLChld(this,-1,"Split Freq.",25,split_control,split_labels,4,"R232",1);
    PTSplit->Create();
  }
  else
    if (PTSplit->Attr.Y<0)
    {
      x = PTSplit->Attr.X;
      y = PTSplit->Attr.Y;
      PTSplit = new TCONTROLChld(this,-1,"Split Freq.",25,split_control,
                                 split_labels,4,"R232",1);
      PTSplit->Create();
    }
    else
      PTSplit->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTSplit->HWindow);
}

void TMainWindow::Window_Monitor()

{
  int x,y;

  if (!PTMonitor)
  {
    PTMonitor = new TCONTROLChld(this,-1,"Monitor",29,monitor_control,NULL,37,"R232",0);
    PTMonitor->Create();
  }
  else
    if (PTMonitor->Attr.Y<0)
    {
      x = PTMonitor->Attr.X;
      y = PTMonitor->Attr.Y;
      PTMonitor = new TCONTROLChld(this,-1,"Monitor",29,monitor_control,
                                 NULL,37,"R232",0);
      PTMonitor->Create();
    }
    else
      PTMonitor->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTMonitor->HWindow);
}

void TMainWindow::Window_Mic_Gain()

{
  int x,y;

  if (!PTMic_Level)
  {
    PTMic_Level = new TCONTROLChld(this,-1,"Mic Gain",21,mic_level_control,volume_labels,64,"SPEAKER",0);
    PTMic_Level->Create();
  }
  else
    if (PTMic_Level->Attr.Y<0)
    {
      x = PTMic_Level->Attr.X;
      y = PTMic_Level->Attr.Y;
      PTMic_Level = new TCONTROLChld(this,-1,"Mic Gain",21,mic_level_control,volume_labels,64,"SPEAKER",0);
      PTMic_Level->Create();
    }
    else
      PTMic_Level->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTMic_Level->HWindow);
}

void TMainWindow::About()

{
  GetModule()->ExecDialog(new TDialog(this, "ABOUT"));
}

void TMainWindow::Configure()

{
  TCONFIGUREDlg(this, "CONFIGURE").Execute();
  if (PTFreq!= NULL && PTFreq->Attr.H >0)
    PTFreq->Redraw();
  if (PTTXFreq!= NULL && PTTXFreq->Attr.H >0)
    PTTXFreq->Redraw();
  if (PTMaxFreq!= NULL && PTMaxFreq->Attr.H >0)
    PTMaxFreq->Redraw();
  if (PTMinFreq!= NULL && PTMinFreq->Attr.H >0)
    PTMinFreq->Redraw();
  if (PTCalibrate != NULL && PTCalibrate->Attr.H >0)
    PTCalibrate->Redraw();
  if (PT_CallSign != NULL && PT_CallSign->Attr.H >0)
    PT_CallSign->Redraw();
}

void TMainWindow::GetMsg()

{
//  printf("string = %s %d.....",message_string,strlen(message_string));
}

LRESULT TMainWindow::GetMsg2(WPARAM ID,LPARAM)

{
//  printf("string = %s %d\n",message_string,strlen(message_string));
}

static morse_on = 0;

void TMainWindow::Post()

{
  char message[15];

  if (morse_on)
  {
    DSPSendMsg(msg_STOP_MORSE,NULL,0);
    morse_on = 0;
    return;
  }
  morse_on = 1;
  DSPSendMsg(msg_START_MORSE,NULL,0);
  message[1] = 0;
  message[0] = 11;
  message[3] = 0;
  message[2] = 9;
  message[5] = 0;
  message[4] = 13;
  message[7] = 0;
  message[6] = 9;
  message[9] = 0;
  message[8] = 11;
  message[11] = 0;
  message[10] = 9;
  message[13] = 0;
  message[12] = 0;
  message[15] = 0;
  message[14] = 0;
  DSPSendMsg(msg_MORSE_CHAR,message,15);
}

void TMainWindow::TakePlot()

{
  DSPSendMsg(msg_PLOT_SEND,NULL,0);
}

int xmit = 0;

void /*TMainWindow::*/Remove_Old_Classes()

{
  int count;

/*  for(count=41;count<50;count++)
    if (delete_class[count])
    {
      delete_class[count] = 0;
      delete PTVoice_Memories[count-41];
      PTVoice_Memories[count-41] = NULL;
      MessageBox(NULL,"Deleted old class",NULL,MB_OK);
    }
*/}

void TMainWindow::Reset()

{
  if (xmit)
    DSPSendMsg(msg_RECIEVE,NULL,0);
  else
    DSPSendMsg(msg_TRANSMIT,NULL,0);
  xmit = 1-xmit;
//  DSPReset();
}

void TMainWindow::Notify_LSB_Spectrum_Analyzer()

{
  if (PT_LSB_Spectrum != NULL && PT_LSB_Spectrum->Attr.H >= 0)
    ::PostMessage(PT_LSB_Spectrum->HWindow,WM_COMMAND,200,0);
  if (PTSpectrum != NULL && PTSpectrum->Attr.H >= 0)
  {
    ::PostMessage(PTSpectrum->HWindow,WM_COMMAND,205,0);
    return;
  }
  if (!(PT_LSB_Spectrum != NULL && PT_LSB_Spectrum->Attr.H >= 0))
    DSPSendMsg(msg_PLOT_LSB_S,OFF_STRING,3);
}

void TMainWindow::Notify_USB_Spectrum_Analyzer()

{
  PostMessage(WM_COMMAND,500,NULL);
  if (PT_USB_Spectrum != NULL && PT_USB_Spectrum->Attr.H >= 0)
    ::PostMessage(PT_USB_Spectrum->HWindow,WM_COMMAND,200,0);
/*  if (PTSpectrum != NULL && PTSpectrum->Attr.H >= 0)
  {
    ::PostMessage(PTSpectrum->HWindow,WM_COMMAND,206,0);
    return;
  }
*/  if (!(PT_USB_Spectrum != NULL && PT_USB_Spectrum->Attr.H >= 0))
    DSPSendMsg(msg_PLOT_USB_S,OFF_STRING,3);
}

void TMainWindow::Notify_LSB_Time_Spectrum_Analyzer()

{
  if (PT_LSB_Time != NULL && PT_LSB_Time->Attr.H != -1)
    ::PostMessage(PT_LSB_Time->HWindow,WM_COMMAND,200,0);
  else
    if (!(PT_XY_Time != NULL && PT_XY_Time->Attr.H != -1))
      DSPSendMsg(msg_PLOT_LSB_T,OFF_STRING,3);
  if (PT_XY_Time != NULL && PT_XY_Time->Attr.H != -1)
    ::PostMessage(PT_XY_Time->HWindow,WM_COMMAND,200,0);
}

void TMainWindow::Notify_USB_Time_Spectrum_Analyzer()

{
  if (PT_USB_Time != NULL && PT_USB_Time->Attr.H != -1)
    ::PostMessage(PT_USB_Time->HWindow,WM_COMMAND,200,0);
  else
    if (!(PT_XY_Time != NULL && PT_XY_Time->Attr.H != -1))
      DSPSendMsg(msg_PLOT_USB_T,OFF_STRING,3);
}

void TMainWindow::Notify_S_Meter()

{
  double value;
  char string[80];

  if (PTS_Meter != NULL && PTS_Meter->Attr.Y > -1)
  {
    value = smeter_string[0];
    if (value<0)
      value += 65536;
    value-=250;
    if (value<=0)
      value=1;
    sprintf(string,"%5.0lf",value);
//    PTCal->SetText(string);
    PTS_Meter->Set_Value(40*log10(value+350));
  }
}

void TMainWindow::Notify_Morse_Next_Char()

{
  if (PTMorse != NULL && PTMorse->Attr.H != -1)
    ::PostMessage(PTMorse->HWindow,WM_COMMAND,201,NULL);
}

void TMainWindow::Notify_Morse_End_Morse()

{
  if (PTMorse != NULL && PTMorse->Attr.H != -1)
    ::PostMessage(PTMorse->HWindow,WM_COMMAND,202,NULL);
}

int change_calibrate = 1;
long calibrate_data = 0;
long calibrate_amount = 0;
int calibrate_last_direction = 1;
int calibrate_last_distance = 0;
int calibrate_increment;

void TMainWindow::Notify_Frequency_Calibrate()

{
  char string[80];
  int multiplier;
  struct time now;
  long time_change;

//  calibrate_data = 0;
  if (calibrate_amount<-5 || calibrate_amount>5)
	 calibrate_increment = 5;
  else
	 calibrate_increment = 1;
  calibrate_data+=frequency_calibrate_string[0];
  if (--change_calibrate==0)
  {
	 change_calibrate = 3;
	 gettime(&now);
	 time_change = labs(now.ti_hour-frequency_time.ti_hour)*3600l+
							 (now.ti_min-frequency_time.ti_min)*60+
							 (now.ti_sec-frequency_time.ti_sec);
	 if (calibrate_data<-1000 || calibrate_data>1000)
	 {
		if (time_change>3)
		  multiplier = 1;
		else
		  multiplier = 5;
	 }
	 else
		if (calibrate_amount<4 && calibrate_amount>-4)
		  multiplier = 0;
		else
		{
		  if (time_change>3)
			 if (calibrate_amount<-3)
				calibrate_amount = -3;
			 else
				if (calibrate_amount>3)
				  calibrate_amount = 3;
		  multiplier = 1;
		}
//    calibrate_amount=labs(calibrate_data/100);
//    if (calibrate_amount == 0)
//      calibrate_amount = 1;
	 if (calibrate_data<0)
	 {
		if (calibrate_amount >0)
		  calibrate_amount+=calibrate_increment;
		else
		  calibrate_amount = 1;
		sprintf(string,"-%10ld   %-6d",-calibrate_data,frequency_calibrate_string[1]);
		if (calibrate_amount>=3)
		{
        if (time_change<=3)
          set_am_side(0);
        if (PTAM_Offset && PTAM_Offset->Attr.H>=0)
			 PTAM_Offset->Set_Value(am_offset_control->Get_Value()-(calibrate_amount-3)*multiplier);
		  else
			 am_offset_control->Set_Value(am_offset_control->Get_Value()-(calibrate_amount-3)*multiplier);
        if (PTAM_Meter && PTAM_Meter->Attr.H>=0)
          PTAM_Meter->Set_Value(am_offset_control->Get_Value()/50+126);
      }
	 }
	 if (calibrate_data>0)
	 {
		if (calibrate_amount <0)
		  calibrate_amount-=calibrate_increment;
		else
		  calibrate_amount = -1;
		sprintf(string,"X%10ld %-6d",calibrate_data,frequency_calibrate_string[1]);
		if (calibrate_amount<=-3)
		{
        if (time_change<=3)
          set_am_side(1);
        if (PTAM_Offset && PTAM_Offset->Attr.H>=0)
			 PTAM_Offset->Set_Value(am_offset_control->Get_Value()-(calibrate_amount+3)*multiplier);
		  else
			 am_offset_control->Set_Value(am_offset_control->Get_Value()-(calibrate_amount+3)*multiplier);
        if (PTAM_Meter && PTAM_Meter->Attr.H>=0)
          PTAM_Meter->Set_Value(am_offset_control->Get_Value()/50+126);
      }
    }
    if (calibrate_data==0)
    {
      calibrate_amount = 0;
      sprintf(string,"          0");
	 }
//	 PTCal->SetText(string);
	 calibrate_data = 0;
  }
}

void TMainWindow::Notify_AGC_Analog()

{
  char string[80];

  if (analog_string[2] == 0)
	 if (PTGain)
		PTGain->Set_Value(analog_string[0]);
	 else
		if_gain_control->Set_Value(analog_string[0]);
  if (analog_string[2]==-1)
	 if (PTMixer_Gain)
	 {
		if (mixer_gain_control->Get_Value()!=mixer_gain_control->Get_Min())
		  PTMixer_Gain->Set_Value(mixer_gain_control->Get_Value()-1);
	 }
	 else
	 {
		if (mixer_gain_control->Get_Value()!=mixer_gain_control->Get_Min())
		  mixer_gain_control->Set_Value(mixer_gain_control->Get_Value()-1);
	 }
  if (analog_string[2]==1)
	 if (PTMixer_Gain)
	 {
		if (mixer_gain_control->Get_Value()!=mixer_gain_control->Get_Max())
		  PTMixer_Gain->Set_Value(mixer_gain_control->Get_Value()+1);
	 }
	 else
	 {
		if (mixer_gain_control->Get_Value()!=mixer_gain_control->Get_Max())
		  mixer_gain_control->Set_Value(mixer_gain_control->Get_Value()+1);
	 }
}

void TMainWindow::Notify_Save_Voice()

{
  save_voice_memory();
}

void TMainWindow::Notify_Play_Voice()

{
  play_voice_memory();
  Need_More_Voice = 0;
}

void TMainWindow::Update_From_Frequency()

{
  if (PTMinFreq != NULL && PTMinFreq->Attr.H > 0)
    PTMinFreq->Check_For_Repaint();
  if (PTMaxFreq != NULL && PTMaxFreq->Attr.H > 0)
    PTMaxFreq->Check_For_Repaint();
  if (PTMode != NULL && PTMode->Attr.H > 0)
    PTMode->Check_For_Repaint();
}

void TMainWindow::Update_From_Max_Frequency()

{
  if (PTMinFreq != NULL && PTMinFreq->Attr.H > 0)
    PTMinFreq->Check_For_Repaint();
  if (PTFreq != NULL && PTFreq->Attr.H > 0)
    PTFreq->Check_For_Repaint();
}

void TMainWindow::Update_From_Min_Frequency()

{
  if (PTMaxFreq != NULL && PTMaxFreq->Attr.H > 0)
    PTMaxFreq->Check_For_Repaint();
  if (PTFreq != NULL && PTFreq->Attr.H > 0)
    PTFreq->Check_For_Repaint();
}

void TMainWindow::Update_From_Spectrum_Analyzer()

{
  if (PTFreq != NULL && PTFreq->Attr.H > 0)
    PTFreq->Check_For_Repaint();
}

void TMainWindow::Update_From_PTT()

{
  if (PTGain != NULL && PTGain->Attr.H > 0)
    PTGain->Redraw();
}

void TMainWindow::Update_From_Send_Morse()

{
  if (PTSent_Morse != NULL && !closing)//PTSent_Morse->Attr.H > 0)
    PTSent_Morse->Add_Char(PTMorse->Last_Char);
}

void TMainWindow::Select_Band(WPARAM ID)

{
  if (PTSpectrum==NULL || PTSpectrum->Attr.H<0)
  {
    if (ID == 161) /* CB IS OUT OF PLACE */
      ID = 201;
    if (PTMinFreq && PTMinFreq->Attr.H >= 0)
      PTMinFreq->Set_Value(bands[ID-113][0]);
    else
      min_frequency_control->Set_Value(bands[ID-113][0]);
    if (PTMaxFreq && PTMaxFreq->Attr.H >= 0)
      PTMaxFreq->Set_Value(bands[ID-113][1]);
    else
      max_frequency_control->Set_Value(bands[ID-113][1]);
    if (PTFreq && PTFreq->Attr.H >= 0)
      PTFreq->Redraw_Frequency();
    if (PTTXFreq && PTTXFreq->Attr.H >= 0)
      PTTXFreq->Redraw_Frequency();
    return;
  }
  MessageBox("You may not change the band while "
             "Spectrum Analyzer is running","Error",MB_ICONHAND | MB_OK);
}

void TMainWindow::Select_All_Band(WPARAM ID)

{
  int min_band;

  if (PTSpectrum==NULL || PTSpectrum->Attr.H<0)
  {
    banded = ID - 163;
    min_band = band_pointers[banded];
    if (PTMinFreq && PTMinFreq->Attr.H >= 0)
      PTMinFreq->Set_Value(bands[min_band][0]);
    else
      min_frequency_control->Set_Value(bands[min_band][0]);
    if (PTMaxFreq && PTMaxFreq->Attr.H >= 0)
      PTMaxFreq->Set_Value(bands[min_band][1]);
    else
      max_frequency_control->Set_Value(bands[min_band][1]);
    if (PTFreq && PTFreq->Attr.H >= 0)
      PTFreq->Redraw_Frequency();
    if (PTTXFreq && PTTXFreq->Attr.H >= 0)
      PTTXFreq->Redraw_Frequency();
    banded = ID - 163;
    return;
  }
  MessageBox("You may not change the band while "
             "Spectrum Analyzer is running","Error",MB_ICONHAND | MB_OK);
}

void TMainWindow::STATIONS()

{
  TSTATIONSDlg(this, "STATIONS").Execute();
  if (PTFreq && PTFreq->Attr.H > 0)
    PTFreq->Redraw_Frequency();
  if (PTMode && PTMode->Attr.H > 0)
  {
    ::InvalidateRgn(PTMode->HWindow,NULL,0);
    ::UpdateWindow(PTMode->HWindow);
  }
  if (PTFilter && PTFilter->Attr.H > 0)
  {
    ::InvalidateRgn(PTFilter->HWindow,NULL,0);
    ::UpdateWindow(PTFilter->HWindow);
  }
}

void TMainWindow::Release_Windows()

{
  if (starting_places[19][1]>=0)
  {
  	 PTS_Meter = new TSMTRChld(this,-1,"S Meter",19,starting_sizes[12][0],
										  starting_sizes[12][1],"SMETER",1);
	 PTS_Meter->Create();
  }
//  PTMode->SetParent(NULL);
}

void DSP1::InitMainWindow()

{
  SetMainWindow(MainWindow = new TMainWindow(NULL,"RADWARE"));
}

bool DSP1::ProcessAppMsg(MSG &msg)

{
  if (msg.message==WM_CHAR)
  {
    if (msg.wParam == '`')
      BringWindowToTop(MainWindow->HWindow);
    if (msg.wParam == VK_SPACE)
      if (PTPTT)
      {
        PTPTT->Set_Value(ptt_control->Get_Value()?0:1);
        return 0;
      }
      else
      {
        ptt_control->Set_Value(ptt_control->Get_Value()?0:1);
        return 0;
      }
  }
  return TApplication::ProcessAppMsg(msg);
}

int OwlMain(int argc,char *argv[])

{
	TResId *ID;
	int count;
	char string[80];
	RECT desktop;

  GetWindowRect(GetDesktopWindow(),&desktop);
  screen_max_x = desktop.right-50;
  screen_max_y = desktop.bottom-25;
  HINSTANCE hBorLibrary;

  if (argc>1)
  {
    strcpy(places_name,argv[1]);
    strcat(places_name,".plc");
  }
  else
    strcpy(places_name,"places.plc");
  if (argc>2)
  {
    strcpy(settings_name,argv[2]);
    strcat(settings_name,".set");
  }
  else
    if (argc == 2)
    {
      strcpy(settings_name,argv[1]);
      strcat(settings_name,".set");
    }
    else
      strcpy(settings_name,"settings.set");
  if (argc>3)
  {
    strcpy(stations_name,argv[3]);
    strcat(stations_name,".sta");
  }
  else
    if (argc == 2)
    {
      strcpy(stations_name,argv[1]);
      strcat(stations_name,".sta");
    }
    else
      strcpy(stations_name,"stations.sta");
  hBorLibrary = LoadLibrary("bwcc.dll");

  if((UINT) hBorLibrary <= 32)
     MessageBox(NULL, "Unable to load Borland Controls", "System Error", MB_OK | MB_ICONHAND);
//  Simulation_Mode();
  DSP1 DSP("DSP 1");
  load_places();
  load_stations();
  for(count=0;count<NUMBER_OF_WINDOWS;count++)
    delete_class[count] = 0;
  for(count=0;count<MAX_KNOBS;count++)
  {
	 sprintf(string,"KNOB%d",count+1);
	 ID = new TResId(string);
	 knobs[count] = new TBitmap(_hInstance,*ID);
	 delete ID;
  }
  ID = new TResId("SMETER");
  smeter_bitmap = new TBitmap(_hInstance,*ID);
  delete ID;
  ID = new TResId("SHEET_UNPRESSED");
  sheet_bitmap[0] = new TBitmap(_hInstance,*ID);
  delete ID;
  ID = new TResId("SHEET_PRESSED");
  sheet_bitmap[1] = new TBitmap(_hInstance,*ID);
  delete ID;
  ID = new TResId("TUNE_UNPRESSED");
  tune_bitmap[0] = new TBitmap(_hInstance,*ID);
  delete ID;
  ID = new TResId("TUNE_PRESSED");
  tune_bitmap[1] = new TBitmap(_hInstance,*ID);
  delete ID;
  DSP.Run();
  FreeProcInstance(CallBackInstance);
  if((UINT) hBorLibrary > 32)
    FreeLibrary(hBorLibrary);
  Clear_Preselectors();
  return DSP.Status;
}

void load_places()

{
  FILE *stream;
  int count;
  char string[80];

  stream = fopen(places_name,"r");
  if (!stream)
  {
    sprintf(string,"Unable to find file %s.\nUsing Defaults",places_name);
    MessageBox(NULL,string,"File Missing",MB_OK | MB_ICONINFORMATION);
    Default_Places();
  }
  else
  {
    for(count=0;count<NUMBER_OF_WINDOWS;count++)
    {
      fscanf(stream,"%d\n%d\n",&starting_places[count][0],&starting_places[count][1]);
      if (starting_places[count][0]>screen_max_x)
        starting_places[count][0] = screen_max_x;
      if (starting_places[count][0]<-screen_max_x)
        starting_places[count][0] = -screen_max_x;
      if (starting_places[count][1]>screen_max_y)
        starting_places[count][1] = screen_max_y;
      if (starting_places[count][1]<-screen_max_y)
        starting_places[count][1] = -screen_max_y;
    }
    if (feof(stream))
    {
      sprintf(string,"%s appears to be corrupt.\nUsing Defaults",places_name);
      MessageBox(NULL,string,"File Corrupt",MB_OK | MB_ICONINFORMATION);
      Default_Places();
    }
    else
    {
      for(count=0;count<NUMBER_OF_WINDOW_SIZES;count++)
        fscanf(stream,"%d\n%d\n",&starting_sizes[count][0],&starting_sizes[count][1]);
      for(count=0;count<NUMBER_OF_WINDOWS;count++)
      {
        fscanf(stream,"%d\n",&windows_on_top[count]);
        if (windows_on_top[count]<1)
          windows_on_top[count] = 100;
      }
    }
    fclose(stream);
  }
}

void Default_Places()

{
  starting_places[0][0] = 50;
  starting_places[0][1] = 50;
  starting_places[1][0] = 190;
  starting_places[1][1] = 50;
  starting_places[2][0] = 330;
  starting_places[2][1] = 50;
  starting_places[3][0] = 470;
  starting_places[3][1] = 50;
  starting_places[4][0] = 50;
  starting_places[4][1] = 151;
  starting_places[5][0] = 190;
  starting_places[5][1] = 151;
  starting_places[6][0] = 330;
  starting_places[6][1] = 151;
  starting_places[7][0] = 50;
  starting_places[7][1] = 252;
  starting_places[8][0] = 50;
  starting_places[8][1] = 352;
  starting_places[9][0] = 0;
  starting_places[9][1] = 0;
  starting_places[10][0] = 214;
  starting_places[10][1] = 460;
  starting_places[11][0] = 140;
  starting_places[11][1] = 441;
  starting_places[12][0] = 446;
  starting_places[12][1] = 92;
  starting_places[13][0] = 112;
  starting_places[13][1] = 210;
  starting_places[14][0] = 157;
  starting_places[14][1] = 170;
  starting_places[15][0] = 490;
  starting_places[15][1] = 84;
  starting_places[16][0] = 600;
  starting_places[16][1] = 113;
  starting_places[17][0] = 150;
  starting_places[17][1] = 200;
  starting_places[18][0] = 0;
  starting_places[18][1] = 0;
  starting_places[19][0] = 471;
  starting_places[19][1] = 151;
  starting_places[20][0] = 470;
  starting_places[20][1] = 151;
  starting_places[21][0] = 619;
  starting_places[21][1] = -268;
  starting_places[22][0] = 190;
  starting_places[22][1] = -250;
  starting_places[23][0] = 330;
  starting_places[23][1] = -250;
  starting_places[24][0] = 470;
  starting_places[24][1] = -250;
  starting_places[25][0] = 471;
  starting_places[25][1] = -253;
  starting_places[26][0] = 18;
  starting_places[26][1] = 408;
  starting_places[27][0] = 400;
  starting_places[27][1] = -300;
  starting_places[28][0] = 358;
  starting_places[28][1] = -349;
  starting_places[29][0] = 622;
  starting_places[29][1] = -53;
  starting_places[30][0] = 449;
  starting_places[30][1] = 350;
  starting_places[31][0] = 459;
  starting_places[31][1] = 360;
  starting_places[32][0] = 459;
  starting_places[32][1] = -360;
  starting_places[33][0] = 469;
  starting_places[33][1] = -370;
  starting_places[34][0] = 479;
  starting_places[34][1] = -380;
  starting_places[35][0] = 489;
  starting_places[35][1] = -390;
  starting_places[36][0] = 499;
  starting_places[36][1] = -400;
  starting_places[37][0] = 509;
  starting_places[37][1] = -410;
  starting_places[38][0] = 519;
  starting_places[38][1] = -420;
  starting_places[39][0] = 529;
  starting_places[39][1] = -430;
  starting_places[40][0] = 249;
  starting_places[40][1] = 250;
  starting_places[41][0] = 249;
  starting_places[41][1] = -350;
  starting_places[42][0] = 259;
  starting_places[42][1] = -360;
  starting_places[43][0] = 269;
  starting_places[43][1] = -370;
  starting_places[44][0] = 279;
  starting_places[44][1] = -380;
  starting_places[45][0] = 289;
  starting_places[45][1] = -390;
  starting_places[46][0] = 299;
  starting_places[46][1] = -400;
  starting_places[47][0] = 309;
  starting_places[47][1] = -410;
  starting_places[48][0] = 319;
  starting_places[48][1] = -420;
  starting_places[49][0] = 329;
  starting_places[49][1] = -430;
  starting_sizes[0][0] = 639;
  starting_sizes[0][1] = 479;
  starting_sizes[1][0] = 562;
  starting_sizes[1][1] = 169;
  starting_sizes[2][0] = 283;
  starting_sizes[2][1] = -43;
  starting_sizes[3][0] = 308;
  starting_sizes[3][1] = -123;
  starting_sizes[4][0] = 135;
  starting_sizes[4][1] = -215;
  starting_sizes[5][0] = 262;
  starting_sizes[5][1] = -248;
  starting_sizes[6][0] = 137;
  starting_sizes[6][1] = 248;
  starting_sizes[7][0] = 135;
  starting_sizes[7][1] = -244;
  starting_sizes[8][0] = 138;
  starting_sizes[8][1] = 288;
  starting_sizes[9][0] = 138;
  starting_sizes[9][1] = 296;
  starting_sizes[10][0] = 0;
  starting_sizes[10][1] = 0;
  starting_sizes[13][0] = 140;
  starting_sizes[13][1] = 101;
}

void load_stations()

{
  FILE *stream;
  int count;
  char string[80];

  stream = fopen(stations_name,"r");
  if (stream)
  {
    for(count=0;count<MAX_STATIONS && !feof(stream);count++)
    {
      fgets(stations[count].name,40,stream);
      if (stations[count].name[strlen(stations[count].name)-1]=='\n')
        stations[count].name[strlen(stations[count].name)-1] = 0;
      if (strcmp(stations[count].name,"NULL") == 0)
        stations[count].name[0] = 0;
      if (strcmp(stations[count].name,"0") == 0)
        stations[count].name[0] = 0;
      fgets(string,16,stream);
      sscanf(string,"%ld",&stations[count].frequency);
      fgets(string,16,stream);
      sscanf(string,"%d",&stations[count].mode);
      fgets(string,16,stream);
      sscanf(string,"%d",&stations[count].band);
    }
    while(count<MAX_STATIONS)
      stations[count++].name[0] = 0;
    for(count=0;count<MAX_QUICK_STATIONS && !feof(stream);count++)
    {
      fgets(quick_stations[count].name,40,stream);
      if (quick_stations[count].name[strlen(quick_stations[count].name)-1]=='\n')
        quick_stations[count].name[strlen(quick_stations[count].name)-1] = 0;
      if (strcmp(quick_stations[count].name,"NULL") == 0)
        quick_stations[count].name[0] = 0;
      if (strcmp(quick_stations[count].name,"0") == 0)
        quick_stations[count].name[0] = 0;
      fgets(string,16,stream);
      sscanf(string,"%ld",&quick_stations[count].frequency);
      if (quick_stations[count].frequency<450000l ||
          quick_stations[count].frequency>30000000l)
        quick_stations[count].frequency = 10000000l;
      fgets(string,16,stream);
      sscanf(string,"%d",&quick_stations[count].mode);
      fgets(string,16,stream);
      sscanf(string,"%d",&quick_stations[count].band);
    }
    while(count<MAX_QUICK_STATIONS)
      quick_stations[count++].name[0] = 0;
  }
  else
  {
    for(count = 0;count<MAX_STATIONS;count++)
      stations[count].name[0] = 0;
    for(count = 0;count<MAX_QUICK_STATIONS;count++)
      quick_stations[count].name[0] = 0;
  }
}

void TMainWindow::load_settings()

{
  FILE *stream;
  char string[80];
  int count;

  if (!(stream = fopen(settings_name,"r")))
  {
    sprintf(string,"Unable to find file %s.\nUsing Defaults",settings_name);
	 MessageBox(string,"File Missing",MB_OK | MB_ICONINFORMATION);
    if (PTPTT)
      PTPTT->Set_Value(0);
    else
      ptt_control->Set_Value(0);
    dly();
    if (PTVolume)
      PTVolume->Set_Value(32);
    else
      volume_control->Set_Value(32);
    dly();
    if (PTAGC)
      PTAGC->Set_Value(0);
    else
      agc_control->Set_Value(0);
    dly();
    if (PTGain)
      PTGain->Set_Value(15);
    else
      if_gain_control->Set_Value(15);
    dly();
    if (PTFilter)
      PTFilter->Set_Value(3);
    else
      filter_control->Set_Value(3);
    dly();
    if (PTMode)
      PTMode->Set_Value(1);
    else
      mode_control->Set_Value(1);
    dly();
    if (PTAntenna_Power)
      PTAntenna_Power->Set_Value(0);
    else
      antenna_power_control->Set_Value(0);
    dly();
    if (PTMixer_Gain)
      PTMixer_Gain->Set_Value(1);
    else
      mixer_gain_control->Set_Value(1);
    dly();
    if (PTFreq)
      PTFreq->Set_Value(10000000l);
    else
      frequency_control->Set_Value(10000000l);
    dly();
	 banded = 0;
	 leading_zeros = 0;
	 frequency_wrap = 0;
	 stations_reset_bands = 0;
	 strcpy(CallSign,"UNKNOWN");
    Set_Radware_Address(0x358);
    for(count<0;count<MAX_VOICE_MEMORIES;count++)
    {
      strcpy(voice_memories[count].label,"NONE");
      strcpy(voice_memories[count].filename,"VOICE");
      voice_memories[count].record = 0;
      voice_memories[count].play = 0;
    }
    for(count=0;count<9;count++)
      if (PTVoice_Memories[count])
        PTVoice_Memories[count]->UpdateAllButtons();
  }
  else
  {
	 leading_zeros = get_long(stream);
    Set_Radware_Address(get_long(stream));
    if (PTVolume)
      PTVolume->Set_Value(get_long(stream));
    else
      volume_control->Set_Value(get_long(stream));
    dly();
    if (PTAGC)
      PTAGC->Set_Value(get_long(stream));
    else
      agc_control->Set_Value(get_long(stream));
    dly();
    if (PTGain)
      PTGain->Set_Value(get_long(stream));
    else
      if_gain_control->Set_Value(get_long(stream));
    dly();
    if (PTFilter)
      PTFilter->Set_Value(get_long(stream));
    else
      filter_control->Set_Value(get_long(stream));
    dly();
    if (PTMode)
      PTMode->Set_Value(get_long(stream));
    else
      mode_control->Set_Value(get_long(stream));
    dly();
    dly();
    if (PTAntenna_Power)
      PTAntenna_Power->Set_Value(get_long(stream));
    else
      antenna_power_control->Set_Value(get_long(stream));
    dly();
    if (PTTXAntenna_Power)
      PTTXAntenna_Power->Set_Value(get_long(stream));
    else
      tx_antenna_power_control->Set_Value(get_long(stream));
    dly();
    if (PTCompression)
      PTCompression->Set_Value(get_long(stream));
    else
      compression_control->Set_Value(get_long(stream));
    dly();
    if (PTCompression_Level)
      PTCompression_Level->Set_Value(get_long(stream));
    else
      compression_level_control->Set_Value(get_long(stream));
    dly();
    if (PTMixer_Gain)
      PTMixer_Gain->Set_Value(get_long(stream));
    else
      mixer_gain_control->Set_Value(get_long(stream));
    dly();
    if (PTSplit)
      PTSplit->Set_Value(get_long(stream));
    else
      split_control->Set_Value(get_long(stream));
    dly();
    if (PTFreq)
      PTFreq->Set_Value(get_long(stream));
    else
      frequency_control->Set_Value(get_long(stream));
    dly();
    if (PTTXFreq!=NULL)
      PTTXFreq->Set_Value(get_long(stream));
    else
      tx_frequency_control->Set_Value(get_long(stream));
    dly();
    if (PTCalibrate!=NULL)
      PTCalibrate->Set_Value(get_long(stream));
    else
      calibrate_control->Set_Value(get_long(stream));
    dly();
    if (PTMaxFreq)
      PTMaxFreq->Set_Value(get_long(stream));
    else
      max_frequency_control->Set_Value(get_long(stream));
    dly();
    if (PTMinFreq)
      PTMinFreq->Set_Value(get_long(stream));
    else
      min_frequency_control->Set_Value(get_long(stream));
    dly();
    if (PTMic_Level)
      PTMic_Level->Set_Value(get_long(stream));
    else
      mic_level_control->Set_Value(get_long(stream));
    dly();
    if (PTMonitor)
      PTMonitor->Set_Value(get_long(stream));
    else
      monitor_control->Set_Value(get_long(stream));
    dly();
    if (PTPTT)
      PTPTT->Set_Value(0);
    else
      ptt_control->Set_Value(0);
    dly();
	 banded = get_long(stream);
	 frequency_wrap = get_long(stream);
	 stations_reset_bands = get_long(stream);
    set_tx_analog_gain(get_long(stream));
    for(count=0;count<19;count++)
      get_long(stream);
	 fgets(CallSign,19,stream);
	 CallSign[strlen(CallSign)-1] = 0;
    for(count=0;count<MAX_VOICE_MEMORIES;count++)
    {
      fgets(voice_memories[count].label,19,stream);
      voice_memories[count].label[strlen(voice_memories[count].label)-1] = 0;
      fgets(voice_memories[count].filename,79,stream);
      voice_memories[count].filename[strlen(voice_memories[count].filename)-1] = 0;
      fgets(string,20,stream);
      sscanf(string,"%d\n",&voice_memories[count].record);
      fgets(string,20,stream);
      sscanf(string,"%d\n",&voice_memories[count].play);
    }
    for(count=0;count<9;count++)
      if (PTVoice_Memories[count])
        PTVoice_Memories[count]->UpdateAllButtons();
	 fclose(stream);
  }
  dly();
}

void TMainWindow::save_settings()

{
  FILE *stream;
  int count;

  stream = fopen(settings_name,"w");
  fprintf(stream,"%d\n%d\n%ld\n%ld\n%ld\n%ld\n%ld\n%ld\n%ld\n%ld\n%ld\n%ld\n%ld\n%ld\n%ld\n%ld\n%ld\n%ld\n%ld\n%ld\n%d\n%d\n%d\n",
			 leading_zeros,Get_Radware_Address(),volume_control->Get_Value(),agc_control->Get_Value(),
			 if_gain_control->Get_Value(),filter_control->Get_Value(),
			 mode_control->Get_Value(),antenna_power_control->Get_Value(),
          tx_antenna_power_control->Get_Value(),
          compression_control->Get_Value(),compression_level_control->Get_Value(),
			 mixer_gain_control->Get_Value(),split_control->Get_Value(),
          frequency_control->Get_Value(),tx_frequency_control->Get_Value(),
			 calibrate_control->Get_Value(),max_frequency_control->Get_Value(),
			 min_frequency_control->Get_Value(),mic_level_control->Get_Value(),
          monitor_control->Get_Value(),banded,
			 frequency_wrap,stations_reset_bands);
  fprintf(stream,"%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n",
          get_tx_analog_gain(),0,0,0,0,0,0,0,0,0);
  fprintf(stream,"%ld\n%ld\n%ld\n%ld\n%ld\n%ld\n%ld\n%ld\n%ld\n%ld\n",
          0,0,0,0,0,0,0,0,0,0);
  fprintf(stream,"%s\n",CallSign);
  for(count=0;count<MAX_VOICE_MEMORIES;count++)
  {
    if (strlen(voice_memories[count].label)==0)
      strcpy(voice_memories[count].label,"NONE");
    if (strlen(voice_memories[count].filename)==0)
      strcpy(voice_memories[count].filename,"VOICE");
    fprintf(stream,"%s\n%s\n%d\n%d\n",voice_memories[count].label,
            voice_memories[count].filename,voice_memories[count].record,
            voice_memories[count].play);
  }
  fclose(stream);
}

void TMainWindow::save_places()

{
  FILE *stream;
  int count;

  Remove_Old_Classes();
  stream = fopen(places_name,"w");
  fprintf(stream,"%d\n%d\n",starting_places[0][0],starting_places[0][1]);
  fprintf(stream,"%d\n%d\n",starting_places[1][0],starting_places[1][1]);
  fprintf(stream,"%d\n%d\n",starting_places[2][0],starting_places[2][1]);
  fprintf(stream,"%d\n%d\n",starting_places[3][0],starting_places[3][1]);
  fprintf(stream,"%d\n%d\n",starting_places[4][0],starting_places[4][1]);
  fprintf(stream,"%d\n%d\n",starting_places[5][0],starting_places[5][1]);
  fprintf(stream,"%d\n%d\n",starting_places[6][0],starting_places[6][1]);
  if (PTFreq)
    fprintf(stream,"%d\n%d\n",PTFreq->Where_X(),PTFreq->Where_Y()-41);
  else
    fprintf(stream,"%d\n%d\n",starting_places[7][0],starting_places[7][1]);
  if (PTCalibrate!=NULL)
    fprintf(stream,"%d\n%d\n",PTCalibrate->Where_X(),PTCalibrate->Where_Y()-41);
  else
    fprintf(stream,"%d\n%d\n",starting_places[8][0],starting_places[8][1]);
  fprintf(stream,"%d\n%d\n",Where_X(),Where_Y()-41);
  if (PTMaxFreq!=NULL)
    fprintf(stream,"%d\n%d\n",PTMaxFreq->Where_X(),PTMaxFreq->Where_Y()-41);
  else
    fprintf(stream,"%d\n%d\n",starting_places[10][0],starting_places[10][1]);
  if (PTMinFreq!=NULL)
    fprintf(stream,"%d\n%d\n",PTMinFreq->Where_X(),PTMinFreq->Where_Y()-41);
  else
    fprintf(stream,"%d\n%d\n",starting_places[11][0],starting_places[11][1]);
  fprintf(stream,"%d\n%d\n",starting_places[12][0],starting_places[12][1]);
  fprintf(stream,"%d\n%d\n",starting_places[13][0],starting_places[13][1]);
  fprintf(stream,"%d\n%d\n",starting_places[14][0],starting_places[14][1]);
  fprintf(stream,"%d\n%d\n",starting_places[15][0],starting_places[15][1]);
  fprintf(stream,"%d\n%d\n",starting_places[16][0],starting_places[16][1]);
  fprintf(stream,"%d\n%d\n",starting_places[17][0],starting_places[17][1]);
  fprintf(stream,"%d\n%d\n",starting_places[18][0],starting_places[18][1]);
  fprintf(stream,"%d\n%d\n",starting_places[19][0],starting_places[19][1]);
  fprintf(stream,"%d\n%d\n",starting_places[20][0],starting_places[20][1]);
  fprintf(stream,"%d\n%d\n",starting_places[21][0],starting_places[21][1]);
  fprintf(stream,"%d\n%d\n",starting_places[22][0],starting_places[22][1]);
  fprintf(stream,"%d\n%d\n",starting_places[23][0],starting_places[23][1]);
  fprintf(stream,"%d\n%d\n",starting_places[24][0],starting_places[24][1]);
  fprintf(stream,"%d\n%d\n",starting_places[25][0],starting_places[25][1]);
  if (PTTXFreq!=NULL)
    fprintf(stream,"%d\n%d\n",PTTXFreq->Where_X(),PTTXFreq->Where_Y()-41);
  else
    fprintf(stream,"%d\n%d\n",starting_places[26][0],starting_places[26][1]);
  fprintf(stream,"%d\n%d\n",starting_places[27][0],starting_places[27][1]);
  fprintf(stream,"%d\n%d\n",starting_places[28][0],starting_places[28][1]);
  fprintf(stream,"%d\n%d\n",starting_places[29][0],starting_places[29][1]);
  fprintf(stream,"%d\n%d\n",starting_places[30][0],starting_places[30][1]);
  fprintf(stream,"%d\n%d\n",starting_places[31][0],starting_places[31][1]);
  fprintf(stream,"%d\n%d\n",starting_places[32][0],starting_places[32][1]);
  fprintf(stream,"%d\n%d\n",starting_places[33][0],starting_places[33][1]);
  fprintf(stream,"%d\n%d\n",starting_places[34][0],starting_places[34][1]);
  fprintf(stream,"%d\n%d\n",starting_places[35][0],starting_places[35][1]);
  fprintf(stream,"%d\n%d\n",starting_places[36][0],starting_places[36][1]);
  fprintf(stream,"%d\n%d\n",starting_places[37][0],starting_places[37][1]);
  fprintf(stream,"%d\n%d\n",starting_places[38][0],starting_places[38][1]);
  fprintf(stream,"%d\n%d\n",starting_places[39][0],starting_places[39][1]);
  fprintf(stream,"%d\n%d\n",starting_places[40][0],starting_places[40][1]);
  fprintf(stream,"%d\n%d\n",starting_places[41][0],starting_places[41][1]);
  fprintf(stream,"%d\n%d\n",starting_places[42][0],starting_places[42][1]);
  fprintf(stream,"%d\n%d\n",starting_places[43][0],starting_places[43][1]);
  fprintf(stream,"%d\n%d\n",starting_places[44][0],starting_places[44][1]);
  fprintf(stream,"%d\n%d\n",starting_places[45][0],starting_places[45][1]);
  fprintf(stream,"%d\n%d\n",starting_places[46][0],starting_places[46][1]);
  fprintf(stream,"%d\n%d\n",starting_places[47][0],starting_places[47][1]);
  fprintf(stream,"%d\n%d\n",starting_places[48][0],starting_places[48][1]);
  fprintf(stream,"%d\n%d\n",starting_places[49][0],starting_places[49][1]);
  fprintf(stream,"%d\n%d\n",starting_places[50][0],starting_places[50][1]);
  fprintf(stream,"%d\n%d\n",starting_places[51][0],starting_places[51][1]);
  fprintf(stream,"%d\n%d\n",starting_places[52][0],starting_places[52][1]);
  fprintf(stream,"%d\n%d\n",starting_places[53][0],starting_places[53][1]);
  fprintf(stream,"%d\n%d\n",starting_places[54][0],starting_places[54][1]);
  fprintf(stream,"%d\n%d\n",starting_places[55][0],starting_places[55][1]);
  fprintf(stream,"%d\n%d\n",starting_places[56][0],starting_places[56][1]);
  fprintf(stream,"%d\n%d\n",starting_places[57][0],starting_places[57][1]);
  fprintf(stream,"%d\n%d\n",starting_places[58][0],starting_places[58][1]);
  fprintf(stream,"%d\n%d\n",starting_places[59][0],starting_places[59][1]);
  fprintf(stream,"%d\n%d\n",starting_places[60][0],starting_places[60][1]);
  fprintf(stream,"%d\n%d\n",starting_places[61][0],starting_places[61][1]);
  fprintf(stream,"%d\n%d\n",starting_places[62][0],starting_places[62][1]);
  fprintf(stream,"%d\n%d\n",starting_places[63][0],starting_places[63][1]);
  fprintf(stream,"%d\n%d\n",starting_places[64][0],starting_places[64][1]);
  fprintf(stream,"%d\n%d\n",starting_places[65][0],starting_places[65][1]);
  fprintf(stream,"%d\n%d\n",starting_places[66][0],starting_places[66][1]);
  fprintf(stream,"%d\n%d\n",starting_places[67][0],starting_places[67][1]);
  fprintf(stream,"%d\n%d\n",starting_places[68][0],starting_places[68][1]);
  fprintf(stream,"%d\n%d\n",starting_places[69][0],starting_places[69][1]);
  fprintf(stream,"%d\n%d\n",starting_places[70][0],starting_places[70][1]);
  fprintf(stream,"%d\n%d\n",starting_places[71][0],starting_places[71][1]);
  fprintf(stream,"%d\n%d\n",starting_places[72][0],starting_places[72][1]);
  fprintf(stream,"%d\n%d\n",starting_places[73][0],starting_places[73][1]);
  fprintf(stream,"%d\n%d\n",starting_places[74][0],starting_places[74][1]);
  fprintf(stream,"%d\n%d\n",Attr.W,Attr.H);
  if (PTFreq)
    fprintf(stream,"%d\n%d\n",PTFreq->Width(),PTFreq->Height());
  else
    fprintf(stream,"%d\n%d\n",starting_sizes[1][0],starting_sizes[1][1]);
  if (PTCalibrate!=NULL)
    fprintf(stream,"%d\n%d\n",PTCalibrate->Width(),PTCalibrate->Height());
  else
    fprintf(stream,"%d\n%d\n",starting_sizes[2][0],starting_sizes[2][1]);
  if (PTMaxFreq!=NULL)
	 fprintf(stream,"%d\n%d\n",PTMaxFreq->Width(),PTMaxFreq->Height());
  else
	 fprintf(stream,"%d\n%d\n",starting_sizes[3][0],starting_sizes[3][1]);
  if (PTMinFreq!=NULL)
	 fprintf(stream,"%d\n%d\n",PTMinFreq->Width(),PTMinFreq->Height());
  else
	 fprintf(stream,"%d\n%d\n",starting_sizes[4][0],starting_sizes[4][1]);
  if (PTSpectrum!=NULL)
	 fprintf(stream,"%d\n%d\n",PTSpectrum->Width(),PTSpectrum->Height());
  else
	 fprintf(stream,"%d\n%d\n",starting_sizes[5][0],starting_sizes[5][1]);
  if (PT_LSB_Spectrum!=NULL)
	 fprintf(stream,"%d\n%d\n",PT_LSB_Spectrum->Width(),PT_LSB_Spectrum->Height());
  else
	 fprintf(stream,"%d\n%d\n",starting_sizes[6][0],starting_sizes[6][1]);
  if (PT_USB_Spectrum!=NULL)
	 fprintf(stream,"%d\n%d\n",PT_USB_Spectrum->Width(),PT_USB_Spectrum->Height());
  else
	 fprintf(stream,"%d\n%d\n",starting_sizes[7][0],starting_sizes[7][1]);
  if (PT_LSB_Time!=NULL)
	 fprintf(stream,"%d\n%d\n",PT_LSB_Time->Width(),PT_LSB_Time->Height());
  else
	 fprintf(stream,"%d\n%d\n",starting_sizes[8][0],starting_sizes[8][1]);
  if (PT_USB_Time!=NULL)
	 fprintf(stream,"%d\n%d\n",PT_USB_Time->Width(),PT_USB_Time->Height());
  else
	 fprintf(stream,"%d\n%d\n",starting_sizes[9][0],starting_sizes[9][1]);
  if (PT_XY_Time!=NULL)
	 fprintf(stream,"%d\n%d\n",PT_XY_Time->Width(),PT_XY_Time->Height());
  else
	 fprintf(stream,"%d\n%d\n",starting_sizes[10][0],starting_sizes[10][1]);
  if (PT_CallSign!=NULL)
	 fprintf(stream,"%d\n%d\n",PT_CallSign->Width(),PT_CallSign->Height());
  else
	 fprintf(stream,"%d\n%d\n",starting_sizes[11][0],starting_sizes[11][1]);
  if (PTS_Meter!=NULL)
	 fprintf(stream,"%d\n%d\n",PTS_Meter->Width(),PTS_Meter->Height());
  else
	 fprintf(stream,"%d\n%d\n",starting_sizes[12][0],starting_sizes[12][1]);
  if (PTTXFreq!=NULL)
	 fprintf(stream,"%d\n%d\n",PTTXFreq->Width(),PTTXFreq->Height());
  else
	 fprintf(stream,"%d\n%d\n",starting_sizes[13][0],starting_sizes[13][1]);
  for(count=0;count<NUMBER_OF_WINDOWS;count++)
    fprintf(stream,"%d\n",windows_on_top[count]);
  fclose(stream);
}

long get_long(FILE *stream)

{
  long answer;

  fscanf(stream,"%ld\n",&answer);
  return answer;
}

void dly()

{
  struct time start_time,end_time;
  int count;
  char string[80];

  for(count=0;count<2;count++)
  {
    gettime(&start_time);
    do
      gettime(&end_time);
    while (start_time.ti_hund == end_time.ti_hund);
  }
}

int Keyboard(uint key, uint repeatCount, uint flags)

{
  if (key == 13)
  {
    ClipCursor(NULL);
    ShowCursor(0);
//    Attr.H = -1;
    while (ShowCursor(1)<0)
      {}
  }
  PTFreq->Set_Value(frequency_control->Get_Value()-10);
//  MessageBox(NULL,"A key has been pressed",NULL,MB_OK);
}

void MoveWindowToTopOfList(int which)

{
  int current;
  int count;

  current = windows_on_top[which];
  if (current<1)
    current = NUMBER_OF_WINDOWS;
  while (current>1)
  {
    for(count=0;count<NUMBER_OF_WINDOWS;count++)
      if (windows_on_top[count] == current-1)
        windows_on_top[count] = current;
    current--;
  }
  windows_on_top[which] = 1;
}

DEFINE_RESPONSE_TABLE1(TMainWindow,AWindow)
  EV_COMMAND(101,Window_AGC),
  EV_COMMAND(102,Window_Analog_Gain),
  EV_COMMAND(103,Window_Antenna_Power),
  EV_COMMAND(104,Window_Calibrate),
  EV_COMMAND(105,Window_Filter),
  EV_COMMAND(106,Window_Frequency),
  EV_COMMAND(111,Window_Max_Frequency),
  EV_COMMAND(112,Window_Min_Frequency),
  EV_COMMAND(107,Window_Mixer_Gain),
  EV_COMMAND(108,Window_Mode),
  EV_COMMAND(109,Window_Volume),
  EV_COMMAND(110,Window_Spectrum_Analyzer),
  EV_COMMAND_AND_ID(113,Select_Band),
  EV_COMMAND_AND_ID(114,Select_Band),
  EV_COMMAND_AND_ID(115,Select_Band),
  EV_COMMAND_AND_ID(116,Select_Band),
  EV_COMMAND_AND_ID(117,Select_Band),
  EV_COMMAND_AND_ID(118,Select_Band),
  EV_COMMAND_AND_ID(119,Select_Band),
  EV_COMMAND_AND_ID(120,Select_Band),
  EV_COMMAND_AND_ID(121,Select_Band),
  EV_COMMAND_AND_ID(122,Select_Band),
  EV_COMMAND_AND_ID(123,Select_Band),
  EV_COMMAND_AND_ID(124,Select_Band),
  EV_COMMAND_AND_ID(125,Select_Band),
  EV_COMMAND_AND_ID(126,Select_Band),
  EV_COMMAND_AND_ID(127,Select_Band),
  EV_COMMAND_AND_ID(128,Select_Band),
  EV_COMMAND_AND_ID(129,Select_Band),
  EV_COMMAND_AND_ID(130,Select_Band),
  EV_COMMAND_AND_ID(131,Select_Band),
  EV_COMMAND_AND_ID(132,Select_Band),
  EV_COMMAND_AND_ID(133,Select_Band),
  EV_COMMAND_AND_ID(134,Select_Band),
  EV_COMMAND_AND_ID(135,Select_Band),
  EV_COMMAND_AND_ID(136,Select_Band),
  EV_COMMAND_AND_ID(137,Select_Band),
  EV_COMMAND_AND_ID(138,Select_Band),
  EV_COMMAND_AND_ID(139,Select_Band),
  EV_COMMAND_AND_ID(140,Select_Band),
  EV_COMMAND_AND_ID(141,Select_Band),
  EV_COMMAND_AND_ID(142,Select_Band),
  EV_COMMAND_AND_ID(143,Select_Band),
  EV_COMMAND_AND_ID(144,Select_Band),
  EV_COMMAND_AND_ID(145,Select_Band),
  EV_COMMAND_AND_ID(146,Select_Band),
  EV_COMMAND_AND_ID(147,Select_Band),
  EV_COMMAND_AND_ID(148,Select_Band),
  EV_COMMAND_AND_ID(149,Select_Band),
  EV_COMMAND_AND_ID(150,Select_Band),
  EV_COMMAND_AND_ID(151,Select_Band),
  EV_COMMAND_AND_ID(152,Select_Band),
  EV_COMMAND_AND_ID(153,Select_Band),
  EV_COMMAND_AND_ID(154,Select_Band),
  EV_COMMAND_AND_ID(155,Select_Band),
  EV_COMMAND_AND_ID(156,Select_Band),
  EV_COMMAND_AND_ID(157,Select_Band),
  EV_COMMAND_AND_ID(158,Select_Band),
  EV_COMMAND_AND_ID(159,Select_Band),
  EV_COMMAND_AND_ID(160,Select_Band),
  EV_COMMAND_AND_ID(161,Select_Band),
  EV_COMMAND(162,STATIONS),
  EV_COMMAND(163,Configure),
  EV_COMMAND_AND_ID(164,Select_All_Band),
  EV_COMMAND_AND_ID(165,Select_All_Band),
  EV_COMMAND_AND_ID(166,Select_All_Band),
  EV_COMMAND_AND_ID(167,Select_All_Band),
  EV_COMMAND_AND_ID(168,Select_All_Band),
  EV_COMMAND(169,Window_Defaults),
  EV_COMMAND(170,Window_LSB_Spectrum),
  EV_COMMAND(171,Window_USB_Spectrum),
  EV_COMMAND(172,Window_LSB_Time),
  EV_COMMAND(173,Window_USB_Time),
  EV_COMMAND(174,Window_XY_Time),
  EV_COMMAND(175,Window_Call_Sign),
  EV_COMMAND(176,Window_S_Meter),
  EV_COMMAND(177,Window_PTT),
  EV_COMMAND(178,Window_TX_Frequency),
  EV_COMMAND(179,Window_TX_Antenna),
  EV_COMMAND(180,Window_Compression),
  EV_COMMAND(181,Window_Compression_Level),
  EV_COMMAND(182,Window_Split),
  EV_COMMAND(183,About),
  EV_COMMAND(184,Window_Morse),
  EV_COMMAND(185,Window_Sent_Morse),
  EV_COMMAND(186,Window_Recieved_Morse),
  EV_COMMAND(187,Window_Mic_Gain),
  EV_COMMAND(188,Window_Monitor),
  EV_COMMAND(210,Window_Fast_Channels_1),
  EV_COMMAND(211,Window_Fast_Channels_2),
  EV_COMMAND(212,Window_Fast_Channels_3),
  EV_COMMAND(213,Window_Fast_Channels_4),
  EV_COMMAND(214,Window_Fast_Channels_5),
  EV_COMMAND(215,Window_Fast_Channels_6),
  EV_COMMAND(216,Window_Fast_Channels_7),
  EV_COMMAND(217,Window_Fast_Channels_8),
  EV_COMMAND(218,Window_Fast_Channels_9),
  EV_COMMAND(219,Window_Amateur_Bands),
  EV_COMMAND_AND_ID(220,Window_Voice_Memories),
  EV_COMMAND_AND_ID(221,Window_Voice_Memories),
  EV_COMMAND_AND_ID(222,Window_Voice_Memories),
  EV_COMMAND_AND_ID(223,Window_Voice_Memories),
  EV_COMMAND_AND_ID(224,Window_Voice_Memories),
  EV_COMMAND_AND_ID(225,Window_Voice_Memories),
  EV_COMMAND_AND_ID(226,Window_Voice_Memories),
  EV_COMMAND_AND_ID(227,Window_Voice_Memories),
  EV_COMMAND_AND_ID(228,Window_Voice_Memories),
  EV_COMMAND(400,Window_AM_Offset),
  EV_COMMAND(202,Post),
  EV_COMMAND(203,TakePlot),
  EV_COMMAND(204,Reset),
  EV_COMMAND(206,Release_Windows),
  EV_COMMAND(190,Update_From_Max_Frequency),
  EV_COMMAND(191,Update_From_Min_Frequency),
  EV_COMMAND(192,Update_From_Frequency),
  EV_COMMAND(ID_SPECTRUM,Update_From_Spectrum_Analyzer),
  EV_COMMAND(ID_PTT,Update_From_PTT),
  EV_COMMAND(ID_SEND_MORSE,Update_From_Send_Morse),
  EV_COMMAND(306,Notify_USB_Spectrum_Analyzer),
  EV_COMMAND(307,Notify_LSB_Spectrum_Analyzer),
  EV_COMMAND(308,Notify_LSB_Time_Spectrum_Analyzer),
  EV_COMMAND(309,Notify_USB_Time_Spectrum_Analyzer),
  EV_COMMAND(310,Notify_S_Meter),
  EV_COMMAND(315,Notify_Morse_Next_Char),
  EV_COMMAND(316,Notify_Morse_End_Morse),
  EV_COMMAND(317,Notify_Frequency_Calibrate),
  EV_COMMAND(318,Notify_AGC_Analog),
  EV_COMMAND(319,Notify_Save_Voice),
  EV_COMMAND(320,Notify_Play_Voice),
  EV_COMMAND(500,Check_Key),
//  EV_COMMAND(501,Remove_Old_Classes),
  EV_WM_TIMER,
END_RESPONSE_TABLE;
