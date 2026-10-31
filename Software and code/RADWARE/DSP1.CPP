#define DIAGNOSTICS_MENU 1
#define ID_SPECTRUM     193
#define ID_LSB_SPECTRUM 194
#define ID_USB_SPECTRUM 195
#define ID_LSB_TIME     196
#define ID_USB_TIME     197
#define ID_XY_TIME      198
#define ID_PTT          199
#define ID_MODE         200
#define ID_VFO_B        201
#define ID_COMPRESSION  251
#define ID_SEND_MORSE   252
#include <thiscls.h>
#include <awindow.h>
#include <stdio.h>
#include "control.h"
//#include "dsplib.c"
#include <mmsystem.h>
#include <windows.h>
#include "dspmgr.h"
#include "dspapi2.h"
#include "freqwin2.h"
#include <string.h>
#include <math.h>
#include <bwcc.h>
#include <dos.h>
#include <io.h>

TStatic *PTCal;

struct single_station
{
  char name[16];
  long frequency;
  short mode;
  short band;
};

extern struct single_station stations[];
extern struct single_station quick_stations[];
extern int channels_selected[];

struct voice_memory voice_memories[MAX_VOICE_MEMORIES];

struct Band_Scan_Struct Band_Scan_Data;
struct Frequency_Scan_Struct Frequency_Scan_Data;

static LOGPEN lpBlue = {PS_SOLID,1,1,RGB(0,0,255)};
static LOGPEN lpGreen = {PS_SOLID,1,1,RGB(0,255,0)};
static LOGPEN lpRed = {PS_SOLID,1,1,RGB(255,0,0)};

HPEN Blue,Green,Red;

DWORD dsp_message = -1;
int knob_speed = 250;
char ON_STRING[3] = {1,1,0};
char OFF_STRING[3] = {0,0,0};

extern int banded;
extern int Game_Port_Address;
extern int band_pointers[];
extern int smeter_string[];
extern int analog_string[];
extern int packet_string[];
extern struct time frequency_time;

int Sound_Card;// = 0x240;
int Show_About = 0;
int Need_More_Voice = 0;
int TimerID;
int starting_places[NUMBER_OF_WINDOWS][2];
short int delete_class[NUMBER_OF_WINDOWS];
int starting_sizes[NUMBER_OF_WINDOW_SIZES][2];
short int windows_on_top[NUMBER_OF_WINDOWS];
char morse_button_names[MAX_MORSE_BUTTONS][6] = {"","","","","",""};
char morse_button_macros[MAX_MORSE_BUTTONS][41] = {"","","","","",""};
char agc_labels[3][20] = {"OFF","ON","HLD"};
char compression_labels[2][20] = {"OFF","ON"};
char ptt_labels[2][20] = {"RECV","XMIT"};
char filter_labels[6][20] = {".05",".2",".4","1.8","2.7","16"};
char split_labels[4][20] = {"A","B","rAxB","rBxA"};
char mode_labels[8][20] = {"AM","LSB","USB","ISB","FM","CW","FSK","PSK"};
char antenna_power_labels[2][20] = {"OFF","ON"};
char mixer_gain_labels[3][20]={"-10","0","+10"};
char volume_labels[1][20]={"MUTE"};
char places_name[40];
char settings_name[40];
char stations_name[40];
int screen_max_x = 700;
int screen_max_y = 500;
int am_offset_active = 1;
int ptt_via_joystick = 0;
int old_key=255;
int save_on_exit = 1;
int hardware_dsp_card = 0;
extern int message_string[],lsb_string[],usb_string[],spectrum_string[];
extern int lsb_time_string[],usb_time_string[],frequency_calibrate_string[];
extern long bands[][2];

TBitmap *knobs[MAX_KNOBS];
TBitmap *smeter_bitmap;
TBitmap *am_meter_bitmap;
TBitmap *sheet_bitmap[2];
TBitmap *tune_bitmap[2];

TDIAGNOSTICDlg *Diagnostics;
TCONTROLChld *PTAGC,*PTFilter,*PTVolume,*PTGain,*PTMode,*PTAntenna_Power,
				 *PTMixer_Gain,*PTMic_Level,*PTTXAntenna_Power,*PTCompression,
				 *PTCompression_Level,*PTSplit,*PTMonitor;
TFreq_Window2 *PTFreq,*PTCalibrate,*PTTXFreq = NULL,*PTMaxFreq = NULL,
				  *PTMinFreq = NULL,*PTAM_Offset;
TSTATChld *PTStations[9] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
TBANDChld *PTAmateurBands = NULL;
TBANDSCANChld *PTBandScan = NULL;
TFREQUENCY_SCANChld *PTFrequencyScan = NULL;
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
TCONTROLChld *PTSquelch = NULL;
TPACKETChld *PTPacket = NULL;
PSQUELCH_Control squelch_control;
PVolume_Control volume_control;
PMic_Level_Control mic_level_control;
PIF_GAIN_Control if_gain_control;
PAGC_Control agc_control;
PAM_Frequency_Offset_Control am_offset_control;
PSplit_Control split_control;
PCustom_Filter_Min_Control custom_filter_min_control;
PCustom_Filter_Max_Control custom_filter_max_control;
PCompression_Control compression_control;
PCompression_Level_Control compression_level_control;
PI_ADJUST_Control i_adjust_control;
PTX_I_ADJUST_Control tx_i_adjust_control;
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
/*PInt_Control lsb_ac_dc_control = NULL;
PInt_Control lsb_scale_control = NULL;
PInt_Control usb_ac_dc_control = NULL;
PInt_Control usb_scale_control = NULL;
*/
int closing = 0;
int leading_zeros,stations_reset_bands;
int key_config = 0;

HALG RadioWareAlg;
HWND TestWindow;

void load_places();
void save_stations();
void load_stations();
long get_long(FILE *stream);
void Set_Radware_Address(int address);
int Get_Radware_Address();
void Default_Places();
void Clear_Preselectors();
void set_am_side(int);
void Check_Key();

TMainWindow *MainWindow;

TMainWindow::TMainWindow(AWindow *AParent, LPSTR ATitle)
								 : AWindow(AParent, ATitle,-1)

{
  Attr.X = starting_places[9][0];
  Attr.Y = starting_places[9][1];
  Attr.W = starting_sizes[0][0];
  Attr.H = starting_sizes[0][1];
  if (DIAGNOSTICS_MENU)
    AssignMenu("TEST_MENU");
  else
    AssignMenu("RADIOWARE_MENU");
//  AssignMenu("RX_ONLY");
}

TMainWindow::~TMainWindow()

{
  int count;

  KillTimer(TimerID);
  ptt_control->Set_Value(0);
  closing = 1;
  Clear_Preselectors();
  if (save_on_exit)
  {
	 save_places();
	 save_settings();
	 save_stations();
  }
  DSPUnloadAlg();
  DSPReset();
  DSPLoadAlg("sc.ld");
  delete antenna_power_control;
  delete tx_antenna_power_control;
  delete frequency_control;
  delete volume_control;
  delete mic_level_control;
  delete agc_control;
  delete am_offset_control;
  delete if_gain_control;
  delete squelch_control;
  delete filter_control;
  delete mode_control;
  delete mixer_gain_control;
  delete split_control;
  delete tx_frequency_control;
  delete calibrate_control;
  delete max_frequency_control;
  delete min_frequency_control;
  delete ptt_control;
  delete monitor_control;
  delete compression_control;
  delete compression_level_control;
  delete i_adjust_control;
  delete tx_i_adjust_control;
  delete custom_filter_min_control;
  delete custom_filter_max_control;
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
  TDialog *About_Dlg;
  number_of_devices = DSPGetNumDevs();
  DSPReset();
  TestWindow = HWindow;
  if (number_of_devices>0)
  {
    if (DSPLoadAlg("RADIO"/*,TestWindow*/)!=1)
    {
      MessageBox("RADWARE.LD file did not load correctly.\n"
                 "RADWARE will continue in simulation mode.","ERROR",
                 MB_ICONSTOP | MB_OK);
      Simulation_Mode();
    }
    dly();
    if (DSPLoadAlg("RADIO"/*,TestWindow*/)!=1)
    {
      MessageBox("RADWARE.LD file did not load correctly.\n"
                 "RADWARE will continue in simulation mode.","ERROR",
                 MB_ICONSTOP | MB_OK);
      Simulation_Mode();
    }
    dly();
/*	 if ((error = DSPLoadAlg(0,(DWORD) HWindow,"RADIO",(DWORD) HWindow))<0)
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
	 }
*//*    else
    {
      sprintf(string,"DSP version %d",error);
      MessageBox(string,"DSP Load",MB_ICONSTOP | MB_OK);
    }
*/  }
  else
  {
	 MessageBox("ADSP 2115 (On Sound Card) not responding.","ERROR",
					MB_ICONSTOP | MB_OK);
    Simulation_Mode();
  }
  Sound_Card = Get_Sound_Card();
  frequency_control = new Frequency_Control();
  frequency_control->Set_Value(30000000); // to start up vco
  volume_control = new Volume_Control();
  mic_level_control = new Mic_Level_Control();
  agc_control = new AGC_Control();
  am_offset_control = new AM_Frequency_Offset_Control();
  antenna_power_control = new ANTENNA_POWER_Control();
  tx_antenna_power_control = new TXANTENNA_POWER_Control();
  if_gain_control = new IF_GAIN_Control();
  squelch_control = new SQUELCH_Control();
  filter_control = new FILTER_Control();
  custom_filter_min_control = new Custom_Filter_Min_Control();
  custom_filter_max_control = new Custom_Filter_Max_Control();
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
  tx_i_adjust_control = new TX_I_ADJUST_Control();

  PTCal = new TStatic(this,202,"",0,0,200,400);
  TFrameWindow::SetupWindow();
  load_settings();
  message[0] = 1;
  message[1] = 0;
  DSPSendMsg(msg_LOG_TOGGLE,(int *)message,2);
  dly();
  message[0] = 0;
  DSPSendMsg(msg_PLOT_TOGGLE,(int *)message,2);
  dly();
//  PostMessage(WM_COMMAND,206,0); /* Release Windows, Doesn't work yet! */
  for(count=0;count<NUMBER_OF_WINDOWS;count++)
	 temp_top[count] = windows_on_top[count];
  for(count2 = 100;count2>0;count2--)
	 for(count=0;count<NUMBER_OF_WINDOWS;count++)
		if (temp_top[count] == count2)
		  if (starting_places[count][1]>0)
			 Open_Window_Number(count);
//  print_int(joySetCapture(HWindow,JOYSTICKID1,1000,FALSE));
//  outport(Sound_Card+0x10,inport(Sound_Card+0x10)|0x0400);
//  PostMessage(WM_COMMAND,500,NULL);
/*  TimerID = SetTimer(1,1,NULL);
  if (TimerID == NULL)
  {
    sprintf(string,"Warning: Windows has no timer rescources left to "
                   "Allocate to the CW Keyer. The keyer will not be  "
                   "checked, and can not be used.");
	 MessageBox(string,"Warning",MB_OK | MB_ICONSTOP);
  }
*/  Blue = CreatePenIndirect(&lpBlue);
  Green = CreatePenIndirect(&lpGreen);
  Red = CreatePenIndirect(&lpRed);
  Show_About = 1;
/*  lsb_ac_dc_control = new Int_Control(0,1);
  lsb_scale_control = new Int_Control(0,8);
  usb_ac_dc_control = new Int_Control(0,1);
  usb_scale_control = new Int_Control(0,8);
*/}

void TMainWindow::Open_Window_Number(int which)

{
  char name[20];
  char string[80];

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
    case 4 :  PTMode = new TCONTROLChld(this,ID_MODE,"Mode",4,mode_control,mode_labels,MAX_MODES,"MODULATION",1);
              PTMode->Create();
              break;
    case 5 :  PTAntenna_Power = new TCONTROLChld(this,-1,"RX Ant DC Pwr",5,antenna_power_control,antenna_power_labels,2,"ANTENNA",1);
              PTAntenna_Power->Create();
              break;
    case 6 :  PTMixer_Gain = new TCONTROLChld(this,-1,"Mixer Gain",6,mixer_gain_control,mixer_gain_labels,3,"AMPATTN",0);
              PTMixer_Gain->Create();
				  break;
    case 7 :  PTFreq = new TFreq_Window2(this,192,"VFO A",frequency_control,7,1,1,1);
              PTFreq->Create();
              break;
    case 8 :  PTCalibrate = new TFreq_Window2(this,201,"Calibrate",calibrate_control,8,2,1,0);
              PTCalibrate->Create();
              break;
    case 10 : PTMaxFreq = new TFreq_Window2(this,190,"Max Frequency",max_frequency_control,10,3,1,2);
              PTMaxFreq->Create();
              break;
    case 11 : PTMinFreq = new TFreq_Window2(this,191,"Min Frequency",min_frequency_control,11,4,1,2);
              PTMinFreq->Create();
              break;
	 case 18 : Get_CallSign(string);
              PT_CallSign = new TCallSign(this,-1,"CallSign",18,11,string,"R232");
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
    case 26 : PTTXFreq = new TFreq_Window2(this,ID_VFO_B,"VFO B",tx_frequency_control,26,13,1,1);
              PTTXFreq->Create();
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
              PTStations[which-31] = new TSTATChld(this,name,which,which-31);
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
    case 54 : PTSquelch = new TCONTROLChld(this,-1,"FM Squelch",54,squelch_control,NULL,64,"SPEAKER",0);
              PTSquelch->Create();
				  break;
    case 55 : PTPacket = new TPACKETChld(this,-1,"Packet",55,400,200);
              PTPacket->Create();
				  break;
    case 56 : PTBandScan = new TBANDSCANChld(this,-1,"Memory Scanning",56);
              PTBandScan->Create();
              break;
    case 57 : // ALL Channels for Band Scanning
              break;
    case 58 : PTFrequencyScan = new TFREQUENCY_SCANChld(this,-1,"Frequency Scan",58);
              PTFrequencyScan->Create();
              break;
    }
}

void TMainWindow::Close_Window_Number(int which)

{
  switch (which)
  {
	 case 0 :  if (PTVolume && delete_class[which] == 0)
					 PTVolume->Destroy();
				  break;
	 case 1 :  if (PTAGC && delete_class[which] == 0)
					 PTAGC->Destroy();
				  break;
	 case 2 :  if (PTGain && delete_class[which] == 0)
					 PTGain->Destroy();
				  break;
	 case 3 :  if (PTFilter && delete_class[which] == 0)
					 PTFilter->Destroy();
				  break;
	 case 4 :  if (PTMode && delete_class[which] == 0)
					 PTMode->Destroy();
				  break;
	 case 5 :  if (PTAntenna_Power && delete_class[which] == 0)
					 PTAntenna_Power->Destroy();
				  break;
	 case 6 :  if (PTMixer_Gain && delete_class[which] == 0)
					 PTMixer_Gain->Destroy();
				  break;
	 case 7:  if (PTFreq && delete_class[which] == 0)
					 PTFreq->Destroy();
				  break;
	 case 8 :  if (PTCalibrate && delete_class[which] == 0)
					 PTCalibrate->Destroy();
				  break;
	 case 10:  if (PTMaxFreq && delete_class[which] == 0)
					 PTMaxFreq->Destroy();
				  break;
	 case 11:  if (PTMinFreq && delete_class[which] == 0)
					 PTMinFreq->Destroy();
				  break;
	 case 12:  if (PTSpectrum && delete_class[which] == 0)
					 PTSpectrum->Destroy();
				  break;
	 case 13:  if (PT_LSB_Spectrum && delete_class[which] == 0)
					 PT_LSB_Spectrum->Destroy();
				  break;
	 case 14:  if (PT_USB_Spectrum && delete_class[which] == 0)
					 PT_USB_Spectrum->Destroy();
				  break;
	 case 15:  if (PT_LSB_Time && delete_class[which] == 0)
					 PT_LSB_Time->Destroy();
				  break;
	 case 16:  if (PT_USB_Time && delete_class[which] == 0)
					 PT_USB_Time->Destroy();
				  break;
	 case 17:  if (PT_XY_Time && delete_class[which] == 0)
					 PT_XY_Time->Destroy();
				  break;
	 case 18 : if (PT_CallSign && delete_class[which] == 0)
					 PT_CallSign->Destroy();
				  break;
	 case 19 : if (PTS_Meter && delete_class[which] == 0)
					 PTS_Meter->Destroy();
				  break;
	 case 20 : if (PTPTT && delete_class[which] == 0)
					 PTPTT->Destroy();
				  break;
	 case 21 : if (PTMic_Level && delete_class[which] == 0)
					 PTMic_Level->Destroy();
				  break;
	 case 22 : if (PTTXAntenna_Power && delete_class[which] == 0)
					 PTTXAntenna_Power->Destroy();
				  break;
	 case 23 : if (PTCompression && delete_class[which] == 0)
					 PTCompression->Destroy();
				  break;
	 case 24 : if (PTCompression_Level && delete_class[which] == 0)
					 PTCompression_Level->Destroy();
				  break;
	 case 25 : if (PTSplit && delete_class[which] == 0)
					 PTSplit->Destroy();
				  break;
	 case 26 : if (PTTXFreq && delete_class[which] == 0)
					 PTTXFreq->Destroy();
				  break;
	 case 27 : if (PTMorse && delete_class[which] == 0)
					 PTMorse->Destroy();
				  break;
	 case 28 : if (PTSent_Morse && delete_class[which] == 0)
					 PTSent_Morse->Destroy();
				  break;
	 case 29 : if (PTMonitor && delete_class[which] == 0)
					 PTMonitor->Destroy();
				  break;
	 case 30 : if (PTAM_Meter && delete_class[which] == 0)
					 PTAM_Meter->Destroy();
				  break;
	 case 31 :
	 case 32 :
	 case 33 :
	 case 34 :
	 case 35 :
	 case 36 :
	 case 37 :
	 case 38 :
	 case 39 : if (PTStations[which-31] && delete_class[which] == 0)
					 PTStations[which-31]->Destroy();
				  break;
	 case 40 : if (PTAmateurBands && delete_class[which] == 0)
					 PTAmateurBands->Destroy();
				  break;
	 case 41 :
	 case 42 :
	 case 43 :
	 case 44 :
	 case 45 :
	 case 46 :
	 case 47 :
	 case 48 :
	 case 49 : if (PTVoice_Memories[which-41] && delete_class[which] == 0)
					 PTVoice_Memories[which-41]->Destroy();
				  break;
	 case 54 : if (PTSquelch && delete_class[which] == 0)
					 PTSquelch->Destroy();
				  break;
	 case 55 : if (PTPacket && delete_class[which] == 0)
					 PTPacket->Destroy();
				  break;
    case 56 : if (PTBandScan && delete_class[which] == 0)
                PTBandScan->Destroy();
              break;
    case 58 : if (PTFrequencyScan && delete_class[which] == 0)
                PTFrequencyScan->Destroy();
              break;
	 }
}

void TMainWindow::Window_Defaults()

{
  MoveWindow(0,0,639,479,1);
  if (PTVolume != NULL && delete_class[0]==0)
    PTVolume->Move(50,50,140,101);
  if (PTAGC != NULL && delete_class[1] == 0)
    PTAGC->Move(190,50,140,101);
  if (PTGain != NULL && delete_class[2] == 0)
	 PTGain->Move(330,50,140,101);
  if (PTFilter != NULL && delete_class[3] == 0)
	 PTFilter->Move(470,50,140,101);
  if (PTMode != NULL && delete_class[4] == 0)
    PTMode->Move(50,151,140,101);
  if (PTAntenna_Power != NULL && delete_class[5] == 0)
	 PTAntenna_Power->Move(190,151,140,101);
  if (PTMixer_Gain != NULL && delete_class[6] == 0)
    PTMixer_Gain->Move(330,151,140,101);
  if (PTPTT != NULL && delete_class[20] == 0)
    PTPTT->Move(470,252,140,101);
  if (PTMic_Level != NULL && delete_class[21]==0)
    PTMic_Level->Move(50,252,140,101);
  if (PTTXAntenna_Power != NULL && delete_class[22]==0)
    PTTXAntenna_Power->Move(190,252,140,101);
  if (PTCompression != NULL && delete_class[23]==0)
    PTCompression->Move(330,252,140,101);
  if (PTCompression_Level != NULL && delete_class[24]==0)
    PTCompression_Level->Move(470,252,140,101);
  if (PTSplit != NULL && delete_class[25]==0)
    PTSplit->Move(50,353,140,101);
  if (PTMonitor != NULL && delete_class[29]==0)
    PTMonitor->Move(190,353,140,101);
  if (PTStations[0] != NULL && delete_class[31] == 0)
    PTStations[0]->Move(330,353,140,101);
  if (PTStations[1] != NULL && delete_class[32] == 0)
    PTStations[1]->Move(340,363,140,101);
  if (PTStations[2] != NULL && delete_class[33] == 0)
    PTStations[2]->Move(350,373,140,101);
  if (PTStations[3] != NULL && delete_class[34] == 0)
	 PTStations[3]->Move(360,383,140,101);
  if (PTStations[4] != NULL && delete_class[35] == 0)
    PTStations[4]->Move(370,393,140,101);
  if (PTStations[5] != NULL && delete_class[36] == 0)
    PTStations[5]->Move(380,403,140,101);
  if (PTStations[6] != NULL && delete_class[37] == 0)
    PTStations[6]->Move(390,413,140,101);
  if (PTStations[7] != NULL && delete_class[38] == 0)
	 PTStations[7]->Move(400,423,140,101);
  if (PTStations[8] != NULL && delete_class[39] == 0)
	 PTStations[8]->Move(410,433,140,101);
  if (PTFreq != NULL && delete_class[7] == 0)
    PTFreq->Move(50,353,275,70);
  if (PTTXFreq != NULL && delete_class[26] == 0)
	 PTTXFreq->Move(50,423,275,70);
  if (PTCalibrate != NULL && delete_class[8] == 0)
    PTCalibrate->Move(50,453,275,70);
  if (PTMaxFreq != NULL && delete_class[10] == 0)
    PTMaxFreq->Move(325,353,275,70);
  if (PTMinFreq != NULL && delete_class[11] == 0)
    PTMinFreq->Move(325,423,275,70);
  if (PTSpectrum != NULL && delete_class[12] == 0)
    PTSpectrum->Move(150,150,256,296);
  if (PT_LSB_Spectrum != NULL && delete_class[13] == 0)
    PT_LSB_Spectrum->Move(0,50,135,281);
  if (PT_USB_Spectrum != NULL && delete_class[14] == 0)
    PT_USB_Spectrum->Move(400,50,135,281);
  if (PT_LSB_Time != NULL && delete_class[15] == 0)
    PT_LSB_Time->Move(0,350,135,150);
  if (PT_USB_Time != NULL && delete_class[16] == 0)
    PT_USB_Time->Move(400,350,135,150);
  if (PT_XY_Time != NULL && delete_class[17] == 0)
    PT_XY_Time->Move(150,200,264,304);
}


void TMainWindow::EvTimer(uint timerId)

{
  PostMessage(WM_COMMAND,500,NULL);
}

void TMainWindow::Check_Key()

{
  ::Check_Key();
}

void TMainWindow::Reload_Places()

{
  int count,count2;
  int temp_top[NUMBER_OF_WINDOWS];

  for(count=0;count<NUMBER_OF_WINDOWS;count++)
	 Close_Window_Number(count);
  load_places();
  for(count=0;count<NUMBER_OF_WINDOWS;count++)
	 temp_top[count] = windows_on_top[count];
  for(count2 = 100;count2>0;count2--)
	 for(count=0;count<NUMBER_OF_WINDOWS;count++)
		if (temp_top[count] == count2)
		  if (starting_places[count][1]>0)
			 Open_Window_Number(count);
}

void TMainWindow::Reload_Settings()

{
  load_settings();
}

void TMainWindow::Reload_Stations()

{
  load_stations();
}

void TMainWindow::Reload_All()

{
  Reload_Places();
  load_settings();
  load_stations();
}

void TMainWindow::Save_Places()

{
  save_places();
}

void TMainWindow::Save_Settings()

{
  save_settings();
}

void TMainWindow::Save_Stations()

{
  save_stations();
}

void TMainWindow::Save_All()

{
  save_places();
  save_settings();
  save_stations();
}

void TMainWindow::Load_Places()

{
  TFileOpenDialog::TData FilenameData(OFN_FILEMUSTEXIST | OFN_HIDEREADONLY|
												  OFN_PATHMUSTEXIST,
						  "Places Files (*.plc)|*.plc|",0,"","*");
  if (TFileOpenDialog(this,FilenameData).Execute() == IDOK)
  {
	 strcpy(places_name,FilenameData.FileName);
	 Reload_Places();
  }
}

void TMainWindow::Load_Settings()

{
  TFileOpenDialog::TData FilenameData(OFN_FILEMUSTEXIST | OFN_HIDEREADONLY|
												  OFN_PATHMUSTEXIST,
						  "Settings Files (*.set)|*.set|",0,"","*");
  if (TFileOpenDialog(this,FilenameData).Execute() == IDOK)
  {
	 strcpy(settings_name,FilenameData.FileName);
	 Reload_Settings();
  }
}

void TMainWindow::Load_Stations()

{
  TFileOpenDialog::TData FilenameData(OFN_FILEMUSTEXIST | OFN_HIDEREADONLY|
												  OFN_PATHMUSTEXIST,
						  "Station Files (*.sta)|*.sta|",0,"","*");
  if (TFileOpenDialog(this,FilenameData).Execute() == IDOK)
  {
	 strcpy(stations_name,FilenameData.FileName);
	 Reload_Stations();
  }
}

void TMainWindow::Save_Places_As()

{
  TFileSaveDialog::TData FilenameData(OFN_HIDEREADONLY | OFN_PATHMUSTEXIST,
						  "Places Files (*.plc)|*.plc|",0,"","*");
  if (TFileSaveDialog(this,FilenameData).Execute() == IDOK)
  {
	 strcpy(places_name,FilenameData.FileName);
	 save_places();
  }
}

void TMainWindow::Save_Settings_As()

{
  TFileSaveDialog::TData FilenameData(OFN_HIDEREADONLY | OFN_PATHMUSTEXIST,
						  "Settings Files (*.set)|*.set|",0,"","*");
  if (TFileSaveDialog(this,FilenameData).Execute() == IDOK)
  {
	 strcpy(settings_name,FilenameData.FileName);
	 save_settings();
  }
}

void TMainWindow::Save_Stations_As()

{
  TFileSaveDialog::TData FilenameData(OFN_HIDEREADONLY | OFN_PATHMUSTEXIST,
						  "Stations Files (*.sta)|*.sta|",0,"","*");
  if (TFileSaveDialog(this,FilenameData).Execute() == IDOK)
  {
	 strcpy(stations_name,FilenameData.FileName);
	 save_places();
  }
}

void TMainWindow::Window_AGC()

{
  if (!PTAGC || delete_class[1])
  {
	 PTAGC = new TCONTROLChld(this,-1,"AGC",1,agc_control,agc_labels,3,"AGC",1);
	 PTAGC->Create();
    delete_class[1] = 0;
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
    delete_class[30] = 0;
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
  if (!PTTXAntenna_Power || delete_class[22])
  {
    PTTXAntenna_Power = new TCONTROLChld(this,-1,"TX Ant DC Pwr",22,tx_antenna_power_control,antenna_power_labels,2,"ANTENNA",1);
    PTTXAntenna_Power->Create();
    delete_class[22] = 0;
  }
  else
    PTTXAntenna_Power->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTTXAntenna_Power->HWindow);
}

void TMainWindow::Window_Calibrate()

{
  if (PTCalibrate == NULL || delete_class[8] == 1)
  {
    PTCalibrate = new TFreq_Window2(this,201,"Calibrate",calibrate_control,8,
                                  2,1,0);
    PTCalibrate->Create();
  }
  else
    PTCalibrate->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTCalibrate->HWindow);
}

void TMainWindow::Window_Filter()

{
  if (!PTFilter || delete_class[3])
  {
    PTFilter = new TCONTROLChld(this,-1,"Filter",3,filter_control,filter_labels,6,"FILTER",1);
    PTFilter->Create();
    delete_class[3] = 0;
  }
  else
    PTFilter->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTFilter->HWindow);
}

void TMainWindow::Window_Frequency()

{
  if (!PTFreq || delete_class[7])
  {
    PTFreq = new TFreq_Window2(this,192,"VFO A",frequency_control,7,
                                   1,1,1);
    PTFreq->Create();
  }
  else
    PTFreq->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTFreq->HWindow);
}

void TMainWindow::Window_TX_Frequency()

{
  if (PTTXFreq == NULL || delete_class[26])
  {
    PTTXFreq = new TFreq_Window2(this,ID_VFO_B,"VFO B",tx_frequency_control,26,
                                 13,1,1);
    PTTXFreq->Create();
  }
  else
    PTTXFreq->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTTXFreq->HWindow);
}

void TMainWindow::Window_Max_Frequency()

{
  if (PTMaxFreq == NULL || delete_class[10])
  {
    PTMaxFreq = new TFreq_Window2(this,190,"Max Frequency",max_frequency_control,10,
                                  3,1,2);
    PTMaxFreq->Create();
  }
  else
    PTMaxFreq->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTMaxFreq->HWindow);
}

void TMainWindow::Window_Min_Frequency()

{
  if (PTMinFreq == NULL || delete_class[11])
  {
    PTMinFreq = new TFreq_Window2(this,191,"Min Frequency",min_frequency_control,11,
                                  4,1,2);
    PTMinFreq->Create();
  }
  else
    PTMinFreq->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTMinFreq->HWindow);
}

void TMainWindow::Window_Mixer_Gain()

{
  if (!PTMixer_Gain || delete_class[6])
  {
    PTMixer_Gain = new TCONTROLChld(this,-1,"Mixer Gain",6,mixer_gain_control,mixer_gain_labels,3,"AMPATTN",0);
    PTMixer_Gain->Create();
    delete_class[6] = 0;
  }
  else
    PTMixer_Gain->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTMixer_Gain->HWindow);
}

void TMainWindow::Window_Mode()

{
  if (!PTMode || delete_class[4])
  {
    PTMode = new TCONTROLChld(this,ID_MODE,"Mode",4,mode_control,mode_labels,MAX_MODES,"MODULATION",1);
    PTMode->Create();
    delete_class[4] = 0;
  }
  else
    PTMode->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTMode->HWindow);
}

void TMainWindow::Window_PTT()

{
  if (!PTPTT || delete_class[20])
  {
    PTPTT = new TCONTROLChld(this,ID_PTT,"PTT",20,ptt_control,ptt_labels,2,"R232",1);
    PTPTT->Create();
    delete_class[20] = 0;
  }
  else
    PTPTT->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTPTT->HWindow);
}

void TMainWindow::Window_Band_Scan()

{
  if (!PTBandScan || delete_class[56])
  {
    PTBandScan = new TBANDSCANChld(this,-1,"Memmory Scanning",56);
    PTBandScan->Create();
    delete_class[56] = 0;
  }
  else
    PTBandScan->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTBandScan->HWindow);
}

void TMainWindow::Window_Frequency_Scan()

{
  if (!PTFrequencyScan || delete_class[58])
  {
    PTFrequencyScan = new TFREQUENCY_SCANChld(this,-1,"Frequency Scan",58);
    PTFrequencyScan->Create();
    delete_class[58] = 0;
  }
  else
    PTFrequencyScan->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTFrequencyScan->HWindow);
}

void TMainWindow::Window_Spectrum_Analyzer()

{
  if (PTSpectrum == NULL || delete_class[12])
  {
    PTSpectrum = new TSpectrum(this,ID_SPECTRUM,"Spectrum",12,5,
                               frequency_control,lsb_string,usb_string,
                               -2/*,NULL,NULL*/);
    PTSpectrum->Create();
    delete_class[12] = 0;
    DSPSendMsg(msg_PLOT_LSB_S,(int *)ON_STRING,3);
  }
  else
    PTSpectrum->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTSpectrum->HWindow);
}

void TMainWindow::Window_LSB_Spectrum()

{
  if (PT_LSB_Spectrum == NULL || delete_class[13])
  {
    PT_LSB_Spectrum = new TSpectrum(this,ID_LSB_SPECTRUM,"LSB",13,6,
                                    frequency_control,lsb_string,NULL,
                                    -1/*,NULL,NULL*/);
    PT_LSB_Spectrum->Create();
    delete_class[13] = 0;
    DSPSendMsg(msg_PLOT_LSB_S,(int *) ON_STRING,3);
  }
  else
  {
    PT_LSB_Spectrum->Show(SW_SHOWNORMAL);
    DSPSendMsg(msg_PLOT_LSB_S,(int *) ON_STRING,3);
  }
  ::BringWindowToTop(PT_LSB_Spectrum->HWindow);
}

void TMainWindow::Window_USB_Spectrum()

{
  if (PT_USB_Spectrum == NULL || delete_class[14])
  {
    PT_USB_Spectrum = new TSpectrum(this,ID_USB_SPECTRUM,"USB",14,7,
                                    frequency_control,usb_string,NULL,
                                    0/*,NULL,NULL*/);
    PT_USB_Spectrum->Create();
    delete_class[14] = 0;
    DSPSendMsg(msg_PLOT_USB_S,(int *) ON_STRING,3);
  }
  else
  {
    PT_USB_Spectrum->Show(SW_SHOWNORMAL);
    DSPSendMsg(msg_PLOT_USB_S,(int *) ON_STRING,3);
  }
  ::BringWindowToTop(PT_USB_Spectrum->HWindow);
}

void TMainWindow::Window_LSB_Time()

{
  if (PT_LSB_Time == NULL || delete_class[15])
  {
    PT_LSB_Time = new TSpectrum(this,ID_LSB_TIME,"LSB Time",15,8,
                                frequency_control,lsb_time_string,
                                NULL,1/*,lsb_ac_dc_control,
                                lsb_scale_control*/);
    PT_LSB_Time->Create();
    delete_class[15] = 0;
    DSPSendMsg(msg_PLOT_LSB_T,(int *) ON_STRING,3);
  }
  else
  {
    PT_LSB_Time->Show(SW_SHOWNORMAL);
    DSPSendMsg(msg_PLOT_LSB_T,(int *) ON_STRING,3);
  }
  ::BringWindowToTop(PT_LSB_Time->HWindow);
}

void TMainWindow::Window_USB_Time()

{
  if (PT_USB_Time == NULL || delete_class[16])
  {
    PT_USB_Time = new TSpectrum(this,ID_USB_TIME,"USB Time",16,9,
                                frequency_control,usb_time_string,NULL,
                                1/*,usb_ac_dc_control,usb_scale_control*/);
    PT_USB_Time->Create();
    delete_class[16] = 0;
    DSPSendMsg(msg_PLOT_USB_T,(int *) ON_STRING,3);
  }
  else
  {
    PT_USB_Time->Show(SW_SHOWNORMAL);
    DSPSendMsg(msg_PLOT_USB_T,(int *) ON_STRING,3);
  }
  ::BringWindowToTop(PT_USB_Time->HWindow);
}

void TMainWindow::Window_XY_Time()

{
  if (PT_XY_Time == NULL || delete_class[17])
  {
	 PT_XY_Time = new TSpectrum(this,ID_XY_TIME,"XY Time",17,10,
                               frequency_control,usb_time_string,
                               lsb_time_string,2/*,NULL,NULL*/);
	 PT_XY_Time->Create();
    delete_class[17] = 0;
	 DSPSendMsg(msg_PLOT_USB_T,(int *) ON_STRING,3);
    dly();
	 DSPSendMsg(msg_PLOT_LSB_T,(int *) ON_STRING,3);
  }
  else
  {
    PT_XY_Time->Show(SW_SHOWNORMAL);
    DSPSendMsg(msg_PLOT_USB_T,(int *) ON_STRING,3);
    DSPSendMsg(msg_PLOT_LSB_T,(int *) ON_STRING,3);
  }
  ::BringWindowToTop(PT_XY_Time->HWindow);
}

void TMainWindow::Window_S_Meter()

{
  if (PTS_Meter==NULL || delete_class[19])
  {
	 PTS_Meter = new TSMTRChld(this,-1,"S Meter",19,starting_sizes[12][0],
										  starting_sizes[12][1],"SMETER",1);
	 PTS_Meter->Create();
    delete_class[19] = 0;
  }
  else
    PTS_Meter->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTS_Meter->HWindow);
}

void TMainWindow::Window_Morse()

{
  int x,y;

  if (PTMorse==NULL || delete_class[27])
  {
	 PTMorse = new TSEND_MORSEChld(this,ID_SEND_MORSE,"Morse to be sent",27,400,69);
	 PTMorse->Create();
    delete_class[27];
  }
  else
    PTMorse->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTMorse->HWindow);
}

void TMainWindow::Window_Sent_Morse()

{
  if (PTSent_Morse==NULL || delete_class[28])
  {
	 PTSent_Morse = new TSENT_MORSEChld(this,"Sent Morse",28,400,300);
	 PTSent_Morse->Create();
    delete_class[28]=0;
  }
  else
    PTSent_Morse->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTSent_Morse->HWindow);
}

void TMainWindow::Window_Recieved_Morse()

{
/*  if (PTSent_Morse==NULL || delete_class[28] == 0)
  {
	 PTSent_Morse = new TSENT_MORSEChld(this,"Sent Morse",28,400,300);
	 PTSent_Morse->Create();
  }
  else
	 PTSent_Morse->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTSent_Morse->HWindow);
*/}

void TMainWindow::Window_Recieved_Packet()

{
  if (PTPacket==NULL || delete_class[55])
  {
	 PTPacket = new TPACKETChld(this,-1,"Packet",55,400,200);
	 PTPacket->Create();
    delete_class[55]=0;
  }
  else
    PTPacket->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTPacket->HWindow);
}

void TMainWindow::Window_Volume()

{
  if (!PTVolume || delete_class[0])
  {
    PTVolume = new TCONTROLChld(this,-1,"Volume",0,volume_control,volume_labels,-64,"SPEAKER",0);
    PTVolume->Create();
    delete_class[0] = 0;
  }
  else
    PTVolume->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTVolume->HWindow);
}

void TMainWindow::Window_Squelch()

{
  if (!PTSquelch || delete_class[54])
  {
    PTSquelch = new TCONTROLChld(this,-1,"Squelch",54,squelch_control,volume_labels,-64,"SPEAKER",0);
    PTSquelch->Create();
    delete_class[54] = 0;
  }
  else
    PTSquelch->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTSquelch->HWindow);
}

void TMainWindow::Window_Fast_Channels_1()

{
  if (!PTStations[0] || delete_class[31])
  {
    PTStations[0] = new TSTATChld(this,"Channels 1",31,0);
    PTStations[0]->Create();
    delete_class[31] = 0;
  }
    PTStations[0]->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTStations[0]->HWindow);
}

void TMainWindow::Window_Fast_Channels_2()

{
  if (!PTStations[1] || delete_class[32])
  {
    PTStations[1] = new TSTATChld(this,"Channels 2",32,1);
    PTStations[1]->Create();
    delete_class[32] = 0;
  }
  else
    PTStations[1]->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTStations[1]->HWindow);
}

void TMainWindow::Window_Fast_Channels_3()

{
  if (!PTStations[2] || delete_class[33])
  {
    PTStations[2] = new TSTATChld(this,"Channels 3",33,2);
    PTStations[2]->Create();
    delete_class[33] = 0;
  }
    PTStations[2]->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTStations[2]->HWindow);
}

void TMainWindow::Window_Fast_Channels_4()

{
  if (!PTStations[3] || delete_class[34])
  {
    PTStations[3] = new TSTATChld(this,"Channels 4",34,3);
    PTStations[3]->Create();
    delete_class[34] = 0;
  }
    PTStations[3]->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTStations[3]->HWindow);
}

void TMainWindow::Window_Fast_Channels_5()

{
  if (!PTStations[4] || delete_class[35])
  {
    PTStations[4] = new TSTATChld(this,"Channels 5",35,4);
    PTStations[4]->Create();
    delete_class[35] = 0;
  }
    PTStations[4]->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTStations[4]->HWindow);
}

void TMainWindow::Window_Fast_Channels_6()

{
  if (!PTStations[5] || delete_class[36])
  {
    PTStations[5] = new TSTATChld(this,"Channels 6",36,5);
    PTStations[5]->Create();
    delete_class[36] = 0;
  }
    PTStations[5]->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTStations[5]->HWindow);
}

void TMainWindow::Window_Fast_Channels_7()

{
  if (!PTStations[6] || delete_class[37])
  {
    PTStations[6] = new TSTATChld(this,"Channels 7",37,6);
    PTStations[6]->Create();
    delete_class[37] = 0;
  }
    PTStations[6]->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTStations[6]->HWindow);
}

void TMainWindow::Window_Fast_Channels_8()

{
  if (!PTStations[7] || delete_class[38])
  {
    PTStations[7] = new TSTATChld(this,"Channels 8",38,7);
    PTStations[7]->Create();
    delete_class[38] = 0;
  }
    PTStations[7]->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTStations[7]->HWindow);
}

void TMainWindow::Window_Fast_Channels_9()

{
  if (!PTStations[8] || delete_class[39])
  {
	 PTStations[8] = new TSTATChld(this,"Channels 9",39,8);
	 PTStations[8]->Create();
    delete_class[39] = 0;
  }
    PTStations[8]->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTStations[8]->HWindow);
}

void TMainWindow::Window_Amateur_Bands()

{
  if (!PTAmateurBands || delete_class[40])
  {
	 PTAmateurBands = new TBANDChld(this,"Amateur Bands",40);
	 PTAmateurBands->Create();
  }
    PTAmateurBands->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTAmateurBands->HWindow);
}

void TMainWindow::Window_Voice_Memories(WPARAM ID)

{
  int which;
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
  char string[80];

  if (PT_CallSign==NULL || delete_class[18])
  {
    Get_CallSign(string);
	 PT_CallSign = new TCallSign(this,-1,"CallSign",18,11,string,"R232");
	 PT_CallSign->Create();
    delete_class[18] = 0;
  }
  else
	 PT_CallSign->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PT_CallSign->HWindow);
}

void TMainWindow::Window_Compression()

{
  if (!PTCompression || delete_class[23])
  {
    PTCompression = new TCONTROLChld(this,ID_COMPRESSION,"Compression",23,compression_control,compression_labels,2,"AGC",1);
    PTCompression->Create();
    delete_class[23] = 0;
  }
  else
    PTCompression->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTCompression->HWindow);
}

void TMainWindow::Window_Compression_Level()

{
  if (!PTCompression_Level || delete_class[24])
  {
    PTCompression_Level = new TCONTROLChld(this,-1,"Comp Level",24,compression_level_control,volume_labels,64,"SPEAKER",0);
    PTCompression_Level->Create();
    delete_class[24] = 0;
  }
  else
    PTCompression_Level->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTCompression_Level->HWindow);
}

void TMainWindow::Window_Split()

{
  if (!PTSplit || delete_class[25])
  {
    PTSplit = new TCONTROLChld(this,-1,"Split Freq.",25,split_control,split_labels,4,"R232",1);
    PTSplit->Create();
    delete_class[25] = 0;
  }
  else
    PTSplit->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTSplit->HWindow);
}

void TMainWindow::Window_Monitor()

{
  if (!PTMonitor || delete_class[29])
  {
    PTMonitor = new TCONTROLChld(this,-1,"Monitor",29,monitor_control,NULL,37,"R232",0);
    PTMonitor->Create();
    delete_class[29] = 0;
  }
  else
    PTMonitor->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTMonitor->HWindow);
}

void TMainWindow::Window_Mic_Gain()

{
  if (!PTMic_Level || delete_class[21])
  {
    PTMic_Level = new TCONTROLChld(this,-1,"Mic Gain",21,mic_level_control,volume_labels,64,"SPEAKER",0);
    PTMic_Level->Create();
    delete_class[21] = 0;
  }
  else
    PTMic_Level->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTMic_Level->HWindow);
}

void TMainWindow::About()

{
  char string[80];

  GetModule()->ExecDialog(new TAboutDlg(this, "ABOUT"));
  old_key = inportb(0x201);
  Show_About = 0;
  if (TimerID == NULL)
  {
    TimerID = SetTimer(1,1,NULL);
    if (TimerID == NULL)
    {
      sprintf(string,"Warning: Windows has no timer rescources left to "
                     "Allocate to the CW Keyer. The keyer will not be  "
                     "checked, and can not be used.");
      MessageBox(string,"Warning",MB_OK | MB_ICONSTOP);
    }
  }
}

void TMainWindow::Help_Contents()

{
  WinExec("WINHELP RADHELP.HLP",SW_SHOWNORMAL);
}

void TMainWindow::Configure()

{
  TCONFIGUREDlg(this, "CONFIGURE").Execute();
  if (PTFreq!= NULL && delete_class[7] == 0)
    PTFreq->Redraw();
  if (PTTXFreq!= NULL && delete_class[26] == 0)
    PTTXFreq->Redraw();
  if (PTMaxFreq!= NULL && delete_class[10] == 0)
    PTMaxFreq->Redraw();
  if (PTMinFreq!= NULL && delete_class[11] == 0)
    PTMinFreq->Redraw();
  if (PTCalibrate != NULL && delete_class[8] == 0)
    PTCalibrate->Redraw();
  if (PT_CallSign != NULL && delete_class[18] == 0)
    PT_CallSign->Redraw();
}

void TMainWindow::Configure_Hardware()

{
  TCONFIGURE_HARDWAREDlg(this, "CONFIGURE_HARDWARE").Execute();
}

void TMainWindow::Window_Diagnostic()

{
  Diagnostics = new TDIAGNOSTICDlg(this, "Alan's Diagnostics");
  Diagnostics->Execute();
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
  DSPSendMsg(msg_MORSE_CHAR,(int *) message,15);
}

void TMainWindow::TakePlot()

{
  DSPSendMsg(msg_PLOT_SEND,(int *) NULL,0);
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
  if (PT_LSB_Spectrum != NULL && delete_class[13] == 0)
    ::PostMessage(PT_LSB_Spectrum->HWindow,WM_COMMAND,200,0);
  if (PTSpectrum != NULL && delete_class[12] == 0)
  {
    ::PostMessage(PTSpectrum->HWindow,WM_COMMAND,205,0);
    return;
  }
  if (!(PT_LSB_Spectrum != NULL && delete_class[13] == 0))
    DSPSendMsg(msg_PLOT_LSB_S,(int *) OFF_STRING,3);
}

void TMainWindow::Notify_USB_Spectrum_Analyzer()

{
  PostMessage(WM_COMMAND,500,NULL);
  if (PT_USB_Spectrum != NULL && delete_class[14] == 0)
    ::PostMessage(PT_USB_Spectrum->HWindow,WM_COMMAND,200,0);
/*  if (PTSpectrum != NULL && delete_class[12] == 0)
  {
    ::PostMessage(PTSpectrum->HWindow,WM_COMMAND,206,0);
    return;
  }
*/  if (!(PT_USB_Spectrum != NULL && delete_class[14] == 0))
    DSPSendMsg(msg_PLOT_USB_S,(int *) OFF_STRING,3);
}

void TMainWindow::Notify_LSB_Time_Spectrum_Analyzer()

{
  if (PT_LSB_Time != NULL && delete_class[15] == 0)
    ::PostMessage(PT_LSB_Time->HWindow,WM_COMMAND,200,0);
  else
    if (!(PT_XY_Time != NULL && delete_class[17] == 0))
      DSPSendMsg(msg_PLOT_LSB_T,(int *) OFF_STRING,3);
  if (PT_XY_Time != NULL && delete_class[17] == 0)
    ::PostMessage(PT_XY_Time->HWindow,WM_COMMAND,200,0);
}

void TMainWindow::Notify_USB_Time_Spectrum_Analyzer()

{
  if (PT_USB_Time != NULL && delete_class[16] == 0)
    ::PostMessage(PT_USB_Time->HWindow,WM_COMMAND,200,0);
  else
    if (!(PT_XY_Time != NULL && delete_class[17] == 0))
      DSPSendMsg(msg_PLOT_USB_T,(int *) OFF_STRING,3);
}

void TMainWindow::Notify_S_Meter()

{
  double value,value2;
  char string[80];
  struct time t;

  value = smeter_string[0];
  if (value<0)
    value += 65536;
  if (value == 0)
    value = .001;
  value2 = 20*log10(value)-98-1.5*if_gain_control->Get_Value()-
                               8*mixer_gain_control->Get_Value();
/*  gettime(&t);
  if ((t.ti_sec%11)<9)
    value2 = -107+6*(t.ti_sec%11);
  else
    value2 = -59+20*((t.ti_sec%11)-8);
*///  sprintf(string,"%5.0lf %lf",value2,Band_Scan_Data.min_ss);
//  PTCal->SetText(string);
  if (PTS_Meter != NULL && delete_class[19] == 0)
    PTS_Meter->Set_Value(value2); //40*log10(value+250)-50);
  if (PTBandScan != NULL && delete_class[56] == 0)
    PTBandScan->S_Meter_Report(value2);
  if (PTFrequencyScan != NULL && delete_class[58] == 0)
    PTFrequencyScan->S_Meter_Report(value2);
}

void TMainWindow::Notify_Morse_Next_Char()

{
  if (PTMorse != NULL && delete_class[27] == 0)
    ::PostMessage(PTMorse->HWindow,WM_COMMAND,201,NULL);
}

void TMainWindow::Notify_Morse_End_Morse()

{
  if (PTMorse != NULL && delete_class[27] == 0)
    ::PostMessage(PTMorse->HWindow,WM_COMMAND,202,NULL);
}

int change_calibrate = 1;
long calibrate_data = 0;
long calibrate_amount = 0;
int calibrate_last_direction = 1;
int calibrate_last_distance = 0;
int calibrate_last = 0;
int calibrate_increment;

void TMainWindow::Notify_Frequency_Calibrate()

{
  char string[240];
  char last_ptcal[240];
  char this_ptcal[480];
  int multiplier;
  struct time now;
  long time_change;
  int temp_cal;

//  calibrate_data = 0;
  if (!am_offset_active)
    return;
/*  temp_cal = frequency_calibrate_string[0];
  frequency_calibrate_string[0] = 1l*calibrate_last-frequency_calibrate_string[0];
  calibrate_last = temp_cal;
*/  if (calibrate_amount<-5 || calibrate_amount>5)
	 calibrate_increment = 5;
  else
	 calibrate_increment = 1;
  calibrate_data+=frequency_calibrate_string[0];
  if (--change_calibrate==0)
  {
	 change_calibrate = 1;
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
		sprintf(string,"-%10ld   %-6d",-calibrate_data,frequency_calibrate_string[0]);
		if (calibrate_amount>=3)
		{
        if (time_change<=3)
          set_am_side(0);
        am_offset_control->Set_Value(am_offset_control->Get_Value()-(calibrate_amount-3)*multiplier);
        if (PTAM_Meter && delete_class[30] == 0)
          PTAM_Meter->Set_Value(am_offset_control->Get_Value()/50+126);
      }
	 }
	 if (calibrate_data>0)
	 {
		if (calibrate_amount <0)
		  calibrate_amount-=calibrate_increment;
		else
		  calibrate_amount = -1;
		sprintf(string,"X%10ld %-6d",calibrate_data,frequency_calibrate_string[0]);
		if (calibrate_amount<=-3)
		{
        if (time_change<=3)
          set_am_side(1);
        am_offset_control->Set_Value(am_offset_control->Get_Value()-(calibrate_amount+3)*multiplier);
        if (PTAM_Meter && delete_class[30] == 0)
          PTAM_Meter->Set_Value(am_offset_control->Get_Value()/50+126);
      }
    }
    if (calibrate_data==0)
    {
      calibrate_amount = 0;
      sprintf(string,"          0");
	 }
/*    PTCal->GetText(last_ptcal,239);
    sprintf(this_ptcal,"%s\n%s",string,last_ptcal);
	 PTCal->SetText(this_ptcal);
*/	 calibrate_data = 0;
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

void TMainWindow::Notify_Packet()

{
  if (PTPacket != NULL && delete_class[55] != 1)
    PTPacket->Add_Int_String(packet_string);
}

void TMainWindow::Notify_Get_1848()

{
  Diagnostics->Recieved_1848();
}

void TMainWindow::Update_From_Frequency()

{
  if (PTMinFreq != NULL && delete_class[11] == 0)
    PTMinFreq->Check_For_Repaint();
  if (PTMaxFreq != NULL && delete_class[10] == 0)
    PTMaxFreq->Check_For_Repaint();
  if (PTMode != NULL && delete_class[4] == 0)
    PTMode->Check_For_Repaint();
  if (PTTXFreq != NULL && delete_class[26] == 0)
    PTTXFreq->Check_For_Repaint();
  if (PTSplit != NULL && delete_class[25] == 0)
    PTSplit->Check_For_Repaint();
  if (PTBandScan && delete_class[56] == 0)
    PTBandScan->Stop_Scanning();
  if (PTFrequencyScan && delete_class[58] == 0)
    PTFrequencyScan->Stop_Scanning();
}

void TMainWindow::Update_From_Max_Frequency()

{
  if (PTMinFreq != NULL && delete_class[11] == 0)
    PTMinFreq->Check_For_Repaint();
  if (PTFreq != NULL && delete_class[7])
    PTFreq->Check_For_Repaint();
  if (PTTXFreq != NULL && delete_class[26] == 0)
    PTTXFreq->Check_For_Repaint();
}

void TMainWindow::Update_From_Min_Frequency()

{
  if (PTMaxFreq != NULL && delete_class[10] == 0)
    PTMaxFreq->Check_For_Repaint();
  if (PTFreq != NULL && delete_class[7] == 0)
    PTFreq->Check_For_Repaint();
  if (PTTXFreq != NULL && delete_class[26] == 0)
    PTTXFreq->Check_For_Repaint();
}

void TMainWindow::Update_From_VFO_B()

{
  if (PTMinFreq != NULL && delete_class[11] == 0)
    PTMinFreq->Check_For_Repaint();
  if (PTMaxFreq != NULL && delete_class[10] == 0)
    PTMaxFreq->Check_For_Repaint();
  if (PTMode != NULL && delete_class[4] == 0)
    PTMode->Check_For_Repaint();
  if (PTFreq != NULL && delete_class[7] == 0)
    PTFreq->Check_For_Repaint();
  if (PTSplit != NULL && delete_class[25] == 0)
    PTSplit->Check_For_Repaint();
  if (PTBandScan && delete_class[56] == 0)
    PTBandScan->Stop_Scanning();
  if (PTFrequencyScan && delete_class[58] == 0)
    PTFrequencyScan->Stop_Scanning();
}

void TMainWindow::Update_From_Spectrum_Analyzer()

{
  if (PTFreq != NULL && delete_class[7] == 0)
    PTFreq->Check_For_Repaint();
}

void TMainWindow::Update_From_PTT()

{
  if (PTGain != NULL && delete_class[2] == 0)
    PTGain->Redraw();
}

void TMainWindow::Update_From_Mode()

{
  if (mode_control->Get_Value()==0)
    strcpy(filter_labels[5],"16");
  else
    strcpy(filter_labels[5],"4");
  if (PTFilter != NULL && delete_class[4] == 0)
    PTFilter->Relabel_Buttons(filter_labels);
  if (PTAM_Meter != NULL && delete_class[30] == 0)
    PTAM_Meter->Redraw();
}

void TMainWindow::Update_From_Send_Morse()

{
  if (PTSent_Morse != NULL && !closing)
    PTSent_Morse->Add_Char(PTMorse->Last_Char);
}

void TMainWindow::Select_Band(WPARAM ID)

{
  if (PTSpectrum==NULL || delete_class[12] == 1)
  {
    if (ID == 161) /* CB IS OUT OF PLACE */
      ID = 201;
    if (PTMinFreq && delete_class[11] == 0)
      PTMinFreq->Set_Value(bands[ID-113][0]);
    else
      min_frequency_control->Set_Value(bands[ID-113][0]);
    if (PTMaxFreq && delete_class[10] == 0)
      PTMaxFreq->Set_Value(bands[ID-113][1]);
    else
      max_frequency_control->Set_Value(bands[ID-113][1]);
    if (PTFreq && delete_class[7] == 0)
      PTFreq->Redraw/*_Frequency*/();
    if (PTTXFreq && delete_class[26] == 0)
      PTTXFreq->Redraw/*_Frequency*/();
    return;
  }
  MessageBox("You may not change the band while "
             "Spectrum Analyzer is running","Error",MB_ICONHAND | MB_OK);
}

void TMainWindow::Select_All_Band(WPARAM ID)

{
  int min_band;

  if (PTSpectrum==NULL || delete_class[12] == 1)
  {
    banded = ID - 163;
    min_band = band_pointers[banded];
    if (PTMinFreq && delete_class[11] == 0)
      PTMinFreq->Set_Value(bands[min_band][0]);
    else
      min_frequency_control->Set_Value(bands[min_band][0]);
    if (PTMaxFreq && delete_class[10] == 0)
      PTMaxFreq->Set_Value(bands[min_band][1]);
    else
      max_frequency_control->Set_Value(bands[min_band][1]);
    if (PTFreq && delete_class[7] == 0)
      PTFreq->Redraw_Frequency();
    if (PTTXFreq && delete_class[26] == 0)
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
  if (PTFreq && delete_class[7] == 0)
    PTFreq->Redraw_Frequency();
  if (PTMode && delete_class[4] == 0)
  {
    ::InvalidateRgn(PTMode->HWindow,NULL,0);
    ::UpdateWindow(PTMode->HWindow);
  }
  if (PTFilter && delete_class[3] == 0)
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

void TMainWindow::load_settings()

{
  FILE *stream;
  char string[80];
  int count,split;
  long vol;
  long temp_rx,temp_tx,temp_min,temp_max,temp_mode;

  volume_control->Mute();
  if (!(stream = fopen(settings_name,"r")))
  {
    sprintf(string,"Unable to find file %s.\nUsing Defaults",settings_name);
	 MessageBox(string,"File Missing",MB_OK | MB_ICONINFORMATION);
	 if (PTPTT && !delete_class[20])
		PTPTT->Set_Value(0);
	 else
		ptt_control->Set_Value(0);
	 dly();
	 if (PTAGC && !delete_class[1])
		PTAGC->Set_Value(0);
	 else
		agc_control->Set_Value(0);
	 dly();
	 if (PTGain && !delete_class[2])
		PTGain->Set_Value(15);
	 else
		if_gain_control->Set_Value(15);
	 dly();
	 if (PTFilter && !delete_class[3])
		PTFilter->Set_Value(3);
	 else
		filter_control->Set_Value(3);
	 dly();
	 if (PTMode && !delete_class[4])
		PTMode->Set_Value(1);
	 else
		mode_control->Set_Value(1);
	 dly();
	 if (PTAntenna_Power && !delete_class[5])
		PTAntenna_Power->Set_Value(0);
	 else
		antenna_power_control->Set_Value(0);
	 dly();
	 if (PTMixer_Gain && !delete_class[6])
		PTMixer_Gain->Set_Value(1);
	 else
		mixer_gain_control->Set_Value(1);
	 dly();
	 if (PTFreq && !delete_class[7])
		PTFreq->Set_Value(10000000l);
	 else
		frequency_control->Set_Value(10000000l);
	 if (PTSquelch && !delete_class[54])
		PTSquelch->Set_Value(10);
	 else
		squelch_control->Set_Value(10);
	 if (PTTXAntenna_Power && !delete_class[22])
		PTTXAntenna_Power->Set_Value(0);
	 else
		tx_antenna_power_control->Set_Value(0);
	 dly();
	 banded = 0;
	 leading_zeros = 0;
	 Set_Frequency_Wrap(0);
	 stations_reset_bands = 0;
    hardware_dsp_card = 0;
    Set_DSP_Card(hardware_dsp_card);
	 Set_CallSign("UNKNOWN");
	 Set_Radware_Address(0x358);
	 for(count=0;count<MAX_VOICE_MEMORIES;count++)
	 {
		strcpy(voice_memories[count].label,"NONE");
		strcpy(voice_memories[count].filename,"VOICE");
		voice_memories[count].record = 0;
		voice_memories[count].play = 0;
	 }
	 for(count=0;count<9;count++)
		if (PTVoice_Memories[count] && !delete_class[31+count])
		  PTVoice_Memories[count]->UpdateAllButtons();
	 dly();
	 if (PTVolume && !delete_class[0])
		PTVolume->Set_Value(32);
	 else
		volume_control->Set_Value(32);
//	 Sound_Card = 0x220;
	 outport(Sound_Card+0x10,inport(Game_Port_Address)|0x0400);
  }
  else
  {
	 leading_zeros = get_long(stream);
	 Set_Radware_Address(get_long(stream));
    frequency_control->Set_Value(30000000l); /*start VCO */
	 vol = get_long(stream);
	 if (PTAGC && !delete_class[1])
		PTAGC->Set_Value(get_long(stream));
	 else
		agc_control->Set_Value(get_long(stream));
	 dly();
	 if (PTGain && !delete_class[6])
		PTGain->Set_Value(get_long(stream));
	 else
		if_gain_control->Set_Value(get_long(stream));
	 dly();
	 if (PTFilter && !delete_class[3])
		PTFilter->Set_Value(get_long(stream));
	 else
		filter_control->Set_Value(get_long(stream));
	 dly();
    temp_mode = get_long(stream);
	 dly();
	 if (PTAntenna_Power && !delete_class[5])
		PTAntenna_Power->Set_Value(get_long(stream));
	 else
		antenna_power_control->Set_Value(get_long(stream));
	 dly();
	 if (PTTXAntenna_Power && !delete_class[22])
		PTTXAntenna_Power->Set_Value(get_long(stream));
	 else
		tx_antenna_power_control->Set_Value(get_long(stream));
	 dly();
	 if (PTCompression && !delete_class[23])
		PTCompression->Set_Value(get_long(stream));
	 else
		compression_control->Set_Value(get_long(stream));
	 dly();
	 if (PTCompression_Level && !delete_class[24])
		PTCompression_Level->Set_Value(get_long(stream));
	 else
		compression_level_control->Set_Value(get_long(stream));
	 dly();
	 if (PTMixer_Gain && !delete_class[6])
		PTMixer_Gain->Set_Value(get_long(stream));
	 else
		mixer_gain_control->Set_Value(get_long(stream));
	 dly();
    split = get_long(stream); /*must be set after freq & tx freq*/
    temp_rx = get_long(stream);
	 dly();
    temp_tx = get_long(stream);
	 if (PTCalibrate!=NULL && !delete_class[8])
		PTCalibrate->Set_Value(get_long(stream));
	 else
		calibrate_control->Set_Value(get_long(stream));
	 dly();
    temp_max = get_long(stream);
    temp_min = get_long(stream);
	 if (PTMic_Level && !delete_class[21])
		PTMic_Level->Set_Value(get_long(stream));
	 else
		mic_level_control->Set_Value(get_long(stream));
	 dly();
	 if (PTMonitor && !delete_class[29])
		PTMonitor->Set_Value(get_long(stream));
	 else
		monitor_control->Set_Value(get_long(stream));
	 dly();
	 banded = get_long(stream);
	 Set_Frequency_Wrap(get_long(stream));
	 stations_reset_bands = get_long(stream);
	 set_tx_analog_gain(get_long(stream));
/*	 Sound_Card = */get_long(stream);
	 outport(Game_Port_Address,inport(Game_Port_Address)|0x0400);
	 ptt_via_joystick = get_long(stream);
	 if (PTSquelch && !delete_class[54])
		PTSquelch->Set_Value(get_long(stream));
	 else
		squelch_control->Set_Value(get_long(stream));
	 if (PTSplit && !delete_class[25])
		PTSplit->Set_Value(split);
	 else
		split_control->Set_Value(split);
	 dly();
	 key_config = get_long(stream);
	 i_adjust_control->Set_Value(get_long(stream));
	 tx_i_adjust_control->Set_Value(get_long(stream));
    set_port_on_off(get_long(stream));
    hardware_dsp_card = get_long(stream);
    Set_DSP_Card(hardware_dsp_card);
    set_winwave(get_long(stream));
	 for(count=0;count<10;count++)
		get_long(stream);
	 fgets(string,19,stream);
	 string[strlen(string)-1] = 0;
    Set_CallSign(string);
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
	 for(count=0;count<MAX_MORSE_BUTTONS;count++)
	 {
		fgets(morse_button_names[count],6,stream);
		morse_button_names[count][strlen(morse_button_names[count])-1] = 0;
		fgets(morse_button_macros[count],40,stream);
		morse_button_macros[count][strlen(morse_button_macros[count])-1] = 0;
		if (strcmp(morse_button_names[count],"NONE")==0)
		  morse_button_names[count][0] = 0;
		if (strcmp(morse_button_macros[count],"NONE")==0)
		  morse_button_macros[count][0] = 0;
	 }
	 for(count=0;count<9;count++)
		if (PTVoice_Memories[count] && !delete_class[31+count])
		  PTVoice_Memories[count]->UpdateAllButtons();
	 fclose(stream);
	 dly();
	 if (PTPTT && !delete_class[20])
		PTPTT->Set_Value(0);
	 else
		ptt_control->Set_Value(0);
    dly();
	 if (PTMode && !delete_class[4])
		PTMode->Set_Value(temp_mode);
	 else
		mode_control->Set_Value(temp_mode);
	 dly();
	 if (PTMaxFreq && !delete_class[10])
		PTMaxFreq->Set_Value(temp_max);
	 else
		max_frequency_control->Set_Value(temp_max);
	 dly();
	 if (PTMinFreq && !delete_class[11])
		PTMinFreq->Set_Value(temp_min);
	 else
		min_frequency_control->Set_Value(temp_min);
	 dly();
    if (PTFreq && !delete_class[7])
      PTFreq->Set_Value(temp_rx);
    else
      frequency_control->Set_Value(temp_rx);
    dly();
	 if (PTTXFreq!=NULL && !delete_class[26])
		PTTXFreq->Set_Value(temp_tx);
	 else
		tx_frequency_control->Set_Value(temp_tx);
	 dly();
	 if (PTVolume && !delete_class[0])
		PTVolume->Set_Value(vol);
	 else
		volume_control->Set_Value(vol);
  }
  dly();
  if (mode_control->Get_Value()==0)
    strcpy(filter_labels[5],"16");
  else
    strcpy(filter_labels[5],"4");
  if (PTFilter != NULL && delete_class[4] == 0)
    PTFilter->Relabel_Buttons(filter_labels);
  custom_filter_min_control->Set_Value(0);
  custom_filter_max_control->Set_Value(4000);
}

void TMainWindow::save_settings()

{
  FILE *stream;
  int count;
  char string[80];

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
			 Get_Frequency_Wrap(),stations_reset_bands);
  fprintf(stream,"%d\n%d\n%d\n%ld\n%d\n%ld\n%ld\n%d\n%d\n%d\n",
			 get_tx_analog_gain(),Sound_Card,ptt_via_joystick,
			 squelch_control->Get_Value(),key_config,i_adjust_control->Get_Value(),
			 tx_i_adjust_control->Get_Value(),get_port_on_off(),hardware_dsp_card,
          get_winwave());
  fprintf(stream,"%ld\n%ld\n%ld\n%ld\n%ld\n%ld\n%ld\n%ld\n%ld\n%ld\n",
			 0,0,0,0,0,0,0,0,0,0);
  Get_CallSign(string);
  fprintf(stream,"%s\n",string);
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
  for(count=0;count<MAX_MORSE_BUTTONS;count++)
  {
	 if (morse_button_names[count][0] == 0)
		strcpy(morse_button_names[count],"NONE");
	 if (morse_button_macros[count][0] == 0)
		strcpy(morse_button_macros[count],"NONE");
	 fprintf(stream,"%s\n%s\n",morse_button_names[count],
				morse_button_macros[count]);
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
  fprintf(stream,"%d\n%d\n",starting_places[7][0],starting_places[7][1]);
  fprintf(stream,"%d\n%d\n",starting_places[8][0],starting_places[8][1]);
  fprintf(stream,"%d\n%d\n",Where_X(),Where_Y()-41);
  fprintf(stream,"%d\n%d\n",starting_places[10][0],starting_places[10][1]);
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
  fprintf(stream,"%d\n%d\n",starting_sizes[1][0],starting_sizes[1][1]);
  fprintf(stream,"%d\n%d\n",starting_sizes[2][0],starting_sizes[2][1]);
  fprintf(stream,"%d\n%d\n",starting_sizes[3][0],starting_sizes[3][1]);
  fprintf(stream,"%d\n%d\n",starting_sizes[4][0],starting_sizes[4][1]);
  fprintf(stream,"%d\n%d\n",starting_sizes[5][0],starting_sizes[5][1]);
  fprintf(stream,"%d\n%d\n",starting_sizes[6][0],starting_sizes[6][1]);
  fprintf(stream,"%d\n%d\n",starting_sizes[7][0],starting_sizes[7][1]);
  fprintf(stream,"%d\n%d\n",starting_sizes[8][0],starting_sizes[8][1]);
  fprintf(stream,"%d\n%d\n",starting_sizes[9][0],starting_sizes[9][1]);
  fprintf(stream,"%d\n%d\n",starting_sizes[10][0],starting_sizes[10][1]);
  fprintf(stream,"%d\n%d\n",starting_sizes[11][0],starting_sizes[11][1]);
  fprintf(stream,"%d\n%d\n",starting_sizes[12][0],starting_sizes[12][1]);
  fprintf(stream,"%d\n%d\n",starting_sizes[13][0],starting_sizes[13][1]);
  fprintf(stream,"%d\n%d\n",starting_sizes[14][0],starting_sizes[14][1]);
  fprintf(stream,"%d\n%d\n",starting_sizes[15][0],starting_sizes[15][1]);
  for(count=0;count<NUMBER_OF_WINDOWS;count++)
	 fprintf(stream,"%d\n",windows_on_top[count]);
  fclose(stream);
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
  EV_COMMAND(189,Window_Squelch),
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
  EV_COMMAND(229,Reload_Places),
  EV_COMMAND(230,Reload_Settings),
  EV_COMMAND(231,Reload_Stations),
  EV_COMMAND(232,Load_Places),
  EV_COMMAND(233,Load_Settings),
  EV_COMMAND(234,Load_Stations),
  EV_COMMAND(235,Save_Places),
  EV_COMMAND(236,Save_Settings),
  EV_COMMAND(237,Save_Stations),
  EV_COMMAND(238,Save_Places_As),
  EV_COMMAND(239,Save_Settings_As),
  EV_COMMAND(240,Save_Stations_As),
  EV_COMMAND(241,Reload_All),
  EV_COMMAND(242,Save_All),
  EV_COMMAND(243,Window_Recieved_Packet),
  EV_COMMAND(244,Help_Contents),
  EV_COMMAND(245,Window_Band_Scan),
  EV_COMMAND(246,Window_Frequency_Scan),
  EV_COMMAND(247,Configure_Hardware),
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
  EV_COMMAND(ID_MODE,Update_From_Mode),
  EV_COMMAND(ID_SEND_MORSE,Update_From_Send_Morse),
  EV_COMMAND(ID_VFO_B,Update_From_VFO_B),
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
  EV_COMMAND(321,Notify_Packet),
  EV_COMMAND(325,Notify_Get_1848),
  EV_COMMAND(500,Check_Key),
//  EV_COMMAND(501,Remove_Old_Classes),
  EV_COMMAND(502,Window_Diagnostic),
  EV_WM_TIMER,
END_RESPONSE_TABLE;
