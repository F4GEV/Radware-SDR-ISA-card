#define OVERSEAS 1
#define OLD_RADIO 0
#define MIN_AMATEUR_BAND 1
#define MAX_AMATEUR_BAND 9
#define MINIADJUST 0x52AA
#define MAXIADJUST 0xC000
#define MINTXIADJUST 0x52AA
#define MAXTXIADJUST 0xC000
#define MINSQUELCH 0
#define MAXSQUELCH 63
#define MINVOLUME 0
#define MAXVOLUME 63
#define MIN_AM_OFFSET -4000
#define MAX_AM_OFFSET  4000
#define MINMICLEVEL 0
#define MAXMICLEVEL 63
#define MINMONITOR 0
#define MAXMONITOR 36
#define MINSPLIT 0
#define MAXSPLIT 3
#define MINAGC 0
#define MAXAGC 2
#define MINCALIBRATE 14300000L
#define MAXCALIBRATE 14350000L
#define MINCOMPRESSION 0
#define MAXCOMPRESSION 1
#define MINCOMPRESSIONLEVEL 0
#define MAXCOMPRESSIONLEVEL 63
#define MINFILTER 0
#define MAXFILTER 5
#define MAXSSBFILTERS 4
#define MINIFGAIN 0
#define MAXIFGAIN 15
#define MINMODE 0
#define MAXMODE 7
#define AM_MODES 0
#define MINANTENNAPOWER 0
#define MAXANTENNAPOWER 1
#define MINTXANTENNAPOWER 0
#define MAXTXANTENNAPOWER 1
#define MINMIXERGAIN 0
#define MAXMIXERGAIN 2
//#define MINFREQUENCY 200000000
#define MINFREQUENCY 450000
//#define MAXFREQUENCY 225000000
#define MAXFREQUENCY 30000000
#define MINPRESELECTOR1 18000000
#define MINPRESELECTOR2 10800000
#define MINPRESELECTOR3  6400000
#define MINPRESELECTOR4  3880000
#define MINPRESELECTOR5  2330000
#define MINPRESELECTOR6  1400000
#define MINPRESELECTOR7   800000
#define MINPRESELECTOR8   500000

#define TDA_8425 60
#define msg_LPF_TOGGLE  507

#define msg_RECIEVE      1504
#define msg_TRANSMIT     1505
#define msg_MODE         1506
#define msg_FILTER       1507
#define msg_IF_GAIN      1508
#define msg_AGC          1509
#define msg_VOLUME       1510
#define msg_START_RECORD 1518
#define msg_STOP_RECORD  1519
#define msg_I_ADJUST     1520
#define msg_START_PLAY   1521
#define msg_MORSE_KEY    1522
#define msg_SQUELCH      1523
#define msg_STOP_PLAY    1498
#define msg_PLAY_DATA    1499
#define msg_GET_1848     1524
#define msg_AGC_10_DB    1525
  
long bands[89][2] = {{  450000l,30000000l},
                     { 1800000l, 2000000l},   /* Ham Bands */
                     { 3500000l, 4000000l},
                     { 7000000l, 7300000l},
                     {10100000l,10150000l},
                     {14000000l,14350000l},
                     {18068000l,18168000l},
                     {21000000l,21450000l},
                     {24890000l,24990000l},
                     {28000000l,29700000l},
							{ 2850000l, 3155000l},   /* Aeronatical Bands */
                     { 3400000l, 3500000l},
                     { 4600000l, 4750000l},
                     { 5450000l, 5730000l},
                     { 6250000l, 6765000l},
                     { 8815000l, 9040000l},
                     {10005000l,10100000l},
                     {11175000l,11400000l},
										 {13200000l,13360000l},
                     {15010000l,15100000l},
                     {17900000l,18030000l},
                     {21870000l,22000000l},
                     {23200000l,23350000l},
                     {  500000l, 1750000l},   /* SW Bands */
                     { 5950000l, 6200000l},
                     { 9500000l, 9775000l},
                     {11700000l,11975000l},
                     {15100000l,15450000l},
                     {17700000l,17900000l},
                     {21450000l,21750000l},
                     {25600000l,26100000l},
                     { 1605000l, 1750000l},   /* Marine Bands */
                     { 2000000l, 2035000l},
                     { 2092500l, 2170000l},
							{ 2190500l, 2495000l},
                     { 2505000l, 2850000l},
                     { 3155000l, 3400000l},
										 { 4139500l, 4166000l},
                     { 6210400l, 6216500l},
                     { 6244500l, 6248000l},
                     { 8281200l, 8288000l},
                     { 8328000l, 8331500l},
                     {12421000l,12431500l},
                     {12479500l,12483000l},
                     {16565000l,16576000l},
                     {16636500l,16640000l},
                     {22094500l,22112000l},
                     {22160500l,22164000l},
                     {26965000l,26965000l},   /* CB Bands   */
                     {26975000l,26975000l},
                     {26985000l,26985000l},
                     {27005000l,27005000l},
                     {27015000l,27015000l},
                     {27025000l,27025000l},
                     {27035000l,27035000l},
                     {27055000l,27055000l},
                     {27065000l,27065000l},
                     {27075000l,27075000l},
							{27085000l,27085000l},
                     {27105000l,27105000l},
                     {27115000l,27115000l},
                     {27125000l,27125000l},
                     {27135000l,27135000l},
                     {27155000l,27155000l},
                     {27165000l,27165000l},
                     {27175000l,27175000l},
                     {27185000l,27185000l},
                     {27205000l,27205000l},
                     {27215000l,27215000l},
                     {27225000l,27225000l},
                     {27255000l,27255000l},
                     {27235000l,27235000l},
                     {27245000l,27245000l},
                     {27265000l,27265000l},
                     {27275000l,27275000l},
                     {27285000l,27285000l},
                     {27295000l,27295000l},
                     {27305000l,27305000l},
                     {27315000l,27315000l},
                     {27325000l,27325000l},
                     {27335000l,27335000l},
                     {27345000l,27345000l},
							{27355000l,27355000l},
                     {27365000l,27365000l},
                     {27375000l,27375000l},
                     {27385000l,27385000l},
                     {27395000l,27395000l},
                     {27405000l,27405000l},
                     {26960000l,27405000l}};

int Baudot[3][32] = {{ 0 ,'E', 10,'A',' ','S','I','U', 13,'D','R','J','N','F','C','K',
                      'T','Z','L','W','H','Y','P','Q','O','B','G', 2 ,'M','X','V', 3 },
                     { 0 ,'3', 10,'-',' ','`','8','7', 13, 4 ,'4', 5 ,',', 0 ,':','(',
                      '5','+',')','2', 0 ,'6','0','1','9','?', 0 , 2 ,'.','/','=', 3 },
                     { 0 ,'3', 10,'-',' ', 7 ,'8','7', 13,'$','4','`',',','!',':','(',
                      '5','"',')','2', 6 ,'6','0','1','9','?','&', 2 ,'.','/',';', 3 }};
   /* these codes & more can be found in  "REFERENCE DATA FOR RADIO ENGINEERS" (ITT) */

int baudot_recieve_shift = 0,baudot_transmit_shift = 0,baudot_alphabet = 1;

int Volume_Levels[64]={0,20,40,60,80,100,120,140,
							  160,180,205,225,250,275,300,330,
							  360,395,435,495,520,570,625,685,
							  750,825,905,995,1095,1205,1325,1455,
							  1600,1760,1935,2125,2335,2565,2820,3100,
							  3400,3750,4125,4535,5000,5500,6000,6600,
							  7250,8000,8800,9500,10450,11500,12500,13700,
							  15400,16940,18700,20550,22500,25000,27500,40000}; //6050};

/*int Volume_Levels[64]={16,17,19,21,23,25,28,31,
							  34,37,41,45,50,55,60,66,
							  72,79,87,95,104,114,125,137,
							  150,165,181,199,219,241,265,291,
							  320,352,387,425,467,513,564,620,
							  682,750,825,907,997,1096,1200,1320,
							  1450,1600,1760,1900,2090,2300,2530,2783,
							  3080,3388,3740,4110,4500,4950,5500,8000}; //6050};
*/
int band_pointers[7] = {0,1,10,23,30,48,88};

#include <d:\dsp1\thisCls.h>;
#include <d:\proradio\awindow.h>
#include <stdio.h>
#include "radlib.h"
#include "radlib2.h"
#include <mmsystem.h>
#include <windows.h>
#include "dspmgr.h"
#include "smartdsp.h"
#include "control.h"
#include "dos.h"
#include "math.h"

static int frequency_wrap;
extern char voice_string[];
static char CallSign[40]="NONE";

void Update_DDS_Control();
void delay(long);
void send_compression();
void set_frequency();
void set_parallel_port(int value);
void Set_Preselectors();
void select_previous_band();
void select_next_band();
int change_mode();
int change_filter();
int Valid_CallSign(int display_error);
int Valid_TX_Frequency(int display_error);

static int AGC = 0,filter = 4,mute = 0, volume = 0,tx_if_gain = 15,if_gain = 15,
           mode = 0,PTT = 0,compression = 0,compression_level = 63,split = 0,
           monitor = 0,am_side = 0,last_parallel = -1;
static long i_adjust = 0x8000;
static long tx_i_adjust = 0x8000;
int dds_control_address = 0x35C,dds_freq_address = 0x358,mic_level = 0,
           preselector_address = 0x35A;
static int antenna_power = 0,mixer_gain = 0,tx_antenna_power = 0;;
static long frequency=1280000l,tx_frequency = 1280000l;
static int AM_Offset = 0,squelch = 31;
static int dds_initialized = 0,dds_load = 0,dds_vco = 4,preselector = 0x0002;
static long DDSCRYSTAL=14318180l;
static long min_frequency = MINFREQUENCY,max_frequency = MAXFREQUENCY;
struct time frequency_time;
struct time ptt_time;
int banded = 0,parallel_port = 0;

int get_agc()

{
  return AGC;
}

int get_filter()

{
  return filter;
}

char *I_ADJUST_Control::Get_Name(void)

{
  return "RX SSB";
}

char *I_ADJUST_Control::Get_Error_Message(void)

{
  return "RX Sideband Rejection adjust out of range.";
}

long I_ADJUST_Control::Get_Value(void)

{
  return i_adjust;
}

int I_ADJUST_Control::Set_Value(long value)

{
  char message[6];

  if (value>=MINIADJUST && value<=MAXIADJUST)
  {
	 i_adjust = value;
	 if (!PTT)
	 {
		message[0] = i_adjust%256;
		message[1] = i_adjust/256;
		DSPSendMsg(msg_I_ADJUST,message,3);
	 }
	 return 1;
	}
  return 0;
}

long I_ADJUST_Control::Get_Min(void)

{
  return MINIADJUST;
}

long I_ADJUST_Control::Get_Max(void)

{
  return MAXIADJUST;
}

void I_ADJUST_Control::Mute(void)

{
  /* does not exist */
}

void I_ADJUST_Control::Restore(void)

{
  /* does not exist */
}

char *TX_I_ADJUST_Control::Get_Name(void)

{
  return "TX SSB";
}

char *TX_I_ADJUST_Control::Get_Error_Message(void)

{
  return "TX Sideband Rejection adjust out of range.";
}

long TX_I_ADJUST_Control::Get_Value(void)

{
  return tx_i_adjust;
}

int TX_I_ADJUST_Control::Set_Value(long value)

{
  char message[6];

  if (value>=MINTXIADJUST && value<=MAXTXIADJUST)
  {
	 tx_i_adjust = value;
	 if (PTT)
	 {
		message[0] = tx_i_adjust%256;
		message[1] = tx_i_adjust/256;
		DSPSendMsg(msg_I_ADJUST,message,3);
	 }
	 return 1;
  }
  return 0;
}

long TX_I_ADJUST_Control::Get_Min(void)

{
  return MINTXIADJUST;
}

long TX_I_ADJUST_Control::Get_Max(void)

{
  return MAXTXIADJUST;
}

void TX_I_ADJUST_Control::Mute(void)

{
  /* does not exist */
}

void TX_I_ADJUST_Control::Restore(void)

{
  /* does not exist */
}

char *TXANTENNA_POWER_Control::Get_Name(void)

{
  return "TX ANTENNA POWER";
}

char *TXANTENNA_POWER_Control::Get_Error_Message(void)

{
  return "Transmit Antenna Power out of range.";
}

long TXANTENNA_POWER_Control::Get_Value(void)

{
  return tx_antenna_power;
}

int TXANTENNA_POWER_Control::Set_Value(long value)

{
  if (value == 1)
  {
    if (MessageBox(NULL,"Warning: This control applies +12 Volts DC to\n"
                        "the TX antenna port. This should only be used\n"
                        "with the RADTOM module or other applications \n"
                        "specifically requiring +12 Volt DC. Use of   \n"
                        "this control can damage devices connected to \n"
                        "the Radware TX antenna port if they were not \n"
                        "designed to be powered from their input.     \n\n"
                        "   Do you REALLY want to turn this power on? ",
                        "Warning",MB_ICONSTOP | MB_YESNO)!=IDYES)
      return 0;
  }
  if (value>=MINTXANTENNAPOWER && value<=MAXTXANTENNAPOWER)
  {
	 tx_antenna_power = value;
	 Update_DDS_Control();
	 return 1;
  }
  return 0;
}

long TXANTENNA_POWER_Control::Get_Min(void)

{
  return MINTXANTENNAPOWER;
}

long TXANTENNA_POWER_Control::Get_Max(void)

{
  return MAXTXANTENNAPOWER;
}

void TXANTENNA_POWER_Control::Mute(void)

{
  /* does not exist */
}

void TXANTENNA_POWER_Control::Restore(void)

{
  /* does not exist */
}

char *ANTENNA_POWER_Control::Get_Name(void)

{
  return "ANTENNA POWER";
}

char *ANTENNA_POWER_Control::Get_Error_Message(void)

{
  return "Antenna Power out of range.";
}

long ANTENNA_POWER_Control::Get_Value(void)

{
  return antenna_power;
}

int ANTENNA_POWER_Control::Set_Value(long value)

{
  if (value == 1)
  {
    if (MessageBox(NULL,"Warning: This control applies +12 Volts DC to\n"
                        "the RX antenna port. This should only be used\n"
                        "with the Comer Communications Active Antenna \n"
                        " or other applications specifically requiring\n"
                        " +12 Volt DC. Use of this control can damage \n"
                        "devices connected to the Radware RX antenna  \n"
                        "port if they were not designed to be powered \n"
                        "from their input.                            \n\n"
                        "   Do you REALLY want to turn this power on? ",
                        "Warning",MB_ICONSTOP | MB_YESNO)!=IDYES)
      return 0;
  }
  if (value>=MINANTENNAPOWER && value<=MAXANTENNAPOWER)
  {
	 antenna_power = value;
	 Update_DDS_Control();
	 return 1;
  }
  return 0;
}

long ANTENNA_POWER_Control::Get_Min(void)

{
  return MINANTENNAPOWER;
}

long ANTENNA_POWER_Control::Get_Max(void)

{
  return MAXANTENNAPOWER;
}

void ANTENNA_POWER_Control::Mute(void)

{
  /* does not exist */
}

void ANTENNA_POWER_Control::Restore(void)

{
  /* does not exist */
}

char *Calibrate_Control::Get_Name(void)

{
  return "Calibrate";
}

char *Calibrate_Control::Get_Error_Message(void)

{
  return "Calibrate out of range.";
}

long Calibrate_Control::Get_Value(void)

{
  return DDSCRYSTAL;
}

int Calibrate_Control::Set_Value(long value)

{
  if (value>=MINCALIBRATE && value<=MAXCALIBRATE/* && mode == 0*/)
  {
    DDSCRYSTAL = value;
    set_frequency();
    return 1;
  }
  return 0;
}

long Calibrate_Control::Get_Min(void)

{
  return MINCALIBRATE;
}

long Calibrate_Control::Get_Max(void)

{
  return MAXCALIBRATE;
}

void Calibrate_Control::Mute(void)

{
  /* does not exist */
}

void Calibrate_Control::Restore(void)

{
  /* does not exist */
}

char *Split_Control::Get_Name(void)

{
  return "Split Freq.";
}

char *Split_Control::Get_Error_Message(void)

{
  return "Split Freq. out of range.";
}

long Split_Control::Get_Value(void)

{
  return split;
}

int Split_Control::Set_Value(long value)

{
  if (value>=Get_Min() && value<=Get_Max())
  {
    split = value;
    set_frequency();
  }
  else
    return 0;
}

long Split_Control::Get_Min(void)

{
  return MINSPLIT;
}

long Split_Control::Get_Max(void)

{
  return MAXSPLIT;
}

void Split_Control::Mute(void)

{
  /* does not exist */
}

void Split_Control::Restore(void)

{
  /* does not exist */
}

char *FILTER_Control::Get_Name(void)

{
  return "FILTER";
}

char *FILTER_Control::Get_Error_Message(void)

{
  return "Filter out of range.";
}

long FILTER_Control::Get_Value(void)

{
  return filter;
}

int FILTER_Control::Set_Value(long value)

{
  if (value>=Get_Min() && value<=Get_Max())
  {
	 filter = value;
	 return change_filter();
  }
  else
	 return 0;
}

long FILTER_Control::Get_Min(void)

{
  return MINFILTER;
}

long FILTER_Control::Get_Max(void)

{
  return MAXFILTER;
}

void FILTER_Control::Mute(void)

{
  /* does not exist */
}

void FILTER_Control::Restore(void)

{
  /* does not exist */
}

int change_filter()

{
  char message[6];

  message[0] = filter;
  message[1] = 0;
  DSPSendMsg(msg_FILTER,message,3);
  return 1;
}

char *AM_Frequency_Offset_Control::Get_Name(void)

{
  return "AM Offset";
}

char *AM_Frequency_Offset_Control::Get_Error_Message(void)

{
  return "AM Frequency Offset out of range.";
}

long AM_Frequency_Offset_Control::Get_Value(void)

{
  return AM_Offset;
}

int AM_Frequency_Offset_Control::Set_Value(long value)

{
  if (value>=MIN_AM_OFFSET && value<=MAX_AM_OFFSET)
  {
	 AM_Offset = value;
	 set_frequency();
    return 1;
  }
  return 0;
}

long AM_Frequency_Offset_Control::Get_Min(void)

{
  return MIN_AM_OFFSET;
}

long AM_Frequency_Offset_Control::Get_Max(void)

{
  return MAX_AM_OFFSET;
}

void AM_Frequency_Offset_Control::Mute(void)

{
  if (mode == 0)
    set_frequency();
}

void AM_Frequency_Offset_Control::Restore(void)

{
  if (mode == 0)
    set_frequency();
}

char *Frequency_Control::Get_Name(void)

{
  return "Freq. A";
}

char *Frequency_Control::Get_Error_Message(void)

{
  return "Frequency out of range.";
}

long Frequency_Control::Get_Value(void)

{
  return frequency;
}

int Frequency_Control::Set_Value(long value)

{
  if (PTT)
    return 0;
  if (split == 1)
    split = 0;
  if (value>=min_frequency && value<=max_frequency)
  {
	 frequency = value;
	 if (split == 0 || (split == 2 && !PTT) || (split == 3 && PTT))
	 {
		gettime(&frequency_time);
		set_frequency();
	 }
	 return 1;
  }
  if (banded)
	 if (value<min_frequency)
    {
      select_previous_band();
      return 1;
    }
    else
    {
      select_next_band();
      return 1;
    }
  else
    if (frequency_wrap)
      if (value<min_frequency)
      {
        frequency = max_frequency;
        if (split == 0 || (split == 2 && !PTT) || (split == 3 && PTT))
          set_frequency();
        return 1;
      }
      else
        if (value>max_frequency)
        {
          frequency = min_frequency;
          if (split == 0 || (split == 2 && !PTT) || (split == 3 && PTT))
            set_frequency();
          return 1;
        }
  return 0;
}

long Frequency_Control::Get_Min(void)

{
  return min_frequency;
}

long Frequency_Control::Get_Max(void)

{
  return max_frequency;
}

void Frequency_Control::Mute(void)

{
  /* does not exist */
}

void Frequency_Control::Restore(void)

{
  /* does not exist */
}

char *TX_Frequency_Control::Get_Name(void)

{
  return "Freq. B";
}

char *TX_Frequency_Control::Get_Error_Message(void)

{
  return "Tx Frequency out of range.";
}

long TX_Frequency_Control::Get_Value(void)

{
  return tx_frequency;
}

int TX_Frequency_Control::Set_Value(long value)

{
  if (PTT)
    return 0;
  if (split == 0)
    split = 1;
  if (value>=min_frequency && value<=max_frequency)
  {
	 tx_frequency = value;
	 if (split == 1 || (PTT && split == 2) || (!PTT && split == 3))
	 {
      gettime(&frequency_time);
		set_frequency();
    }
	 return 1;
  }
  if (banded)
    if (value<min_frequency)
    {
      select_previous_band();
      return 1;
    }
    else
    {
      select_next_band();
      return 1;
    }
  else
    if (frequency_wrap)
      if (value<min_frequency)
      {
        tx_frequency = max_frequency;
        if (split == 1 || (PTT && split == 2) || (!PTT && split == 3))
          set_frequency();
        return 1;
      }
      else
        if (value>max_frequency)
        {
          tx_frequency = min_frequency;
          if (split == 1 || (PTT && split == 2) || (!PTT && split == 3))
            set_frequency();
          return 1;
        }
  return 0;
}

long TX_Frequency_Control::Get_Min(void)

{
  return min_frequency;
}

long TX_Frequency_Control::Get_Max(void)

{
  return max_frequency;
}

void TX_Frequency_Control::Mute(void)

{
  /* does not exist */
}

void TX_Frequency_Control::Restore(void)

{
  /* does not exist */
}

char *Max_Frequency_Control::Get_Name(void)

{
  return "Max Freq.";
}

char *Max_Frequency_Control::Get_Error_Message(void)

{
  return "Max Frequency out of range.";
}

long Max_Frequency_Control::Get_Value(void)

{
  return max_frequency;
}

int Max_Frequency_Control::Set_Value(long value)

{
  if (value<=MAXFREQUENCY && value>=MINFREQUENCY)
  {
    max_frequency = value;
    banded = 0;
    if (frequency>max_frequency)
    {
      if (max_frequency<min_frequency)
        min_frequency = max_frequency;
      frequency = max_frequency;
      if (split == 0 || (!PTT && split == 2) || (PTT && split == 3))
        set_frequency();
    }
    if (tx_frequency>max_frequency)
    {
      tx_frequency = max_frequency;
      if (split == 1 || (PTT && split == 2) || (!PTT && split == 3))
        set_frequency();
    }
    return 1;
  }
  return 0;
}

long Max_Frequency_Control::Get_Min(void)

{
  return MINFREQUENCY;
}

long Max_Frequency_Control::Get_Max(void)

{
  return MAXFREQUENCY;
}

void Max_Frequency_Control::Mute(void)

{
  /* does not exist */
}

void Max_Frequency_Control::Restore(void)

{
  /* does not exist */
}

char *Min_Frequency_Control::Get_Name(void)

{
  return "Min Freq.";
}

char *Min_Frequency_Control::Get_Error_Message(void)

{
  return "Min Frequency out of range.";
}

long Min_Frequency_Control::Get_Value(void)

{
  return min_frequency;
}

int Min_Frequency_Control::Set_Value(long value)

{
  if (value>=MINFREQUENCY && value<=MAXFREQUENCY)
  {
    min_frequency = value;
    banded = 0;
    if (frequency<min_frequency)
    {
      if (min_frequency>max_frequency)
        max_frequency = min_frequency;
      frequency = min_frequency;
      if (split == 0 || (!PTT && split == 2) || (PTT && split == 3))
        set_frequency();
    }
    if (tx_frequency<min_frequency)
    {
      tx_frequency = min_frequency;
      if (split == 1 || (PTT && split == 2) || (!PTT && split == 3))
        set_frequency();
    }
    return 1;
  }
  return 0;
}

long Min_Frequency_Control::Get_Min(void)

{
  return MINFREQUENCY;
}

long Min_Frequency_Control::Get_Max(void)

{
  return MAXFREQUENCY;
}

void Min_Frequency_Control::Mute(void)

{
  /* does not exist */
}

void Min_Frequency_Control::Restore(void)

{
  /* does not exist */
}

void set_frequency()

{
  double dds_freq;
  unsigned int freq_unsigned;
  long freq;


  if (split == 0 || (!PTT && split == 2) || (PTT && split == 3))
	 if (mode == 0 && !PTT)
		freq = frequency+AM_Offset;
    else
      freq = frequency;
  else
    if (mode == 0 && !PTT)
      freq = tx_frequency+AM_Offset;
    else
      freq = tx_frequency;
  if (!dds_initialized)
  {
    dds_load = 0x0000;
	 Update_DDS_Control();
	 dds_load = 0x0008;
	 Update_DDS_Control();
	 outport(dds_freq_address,0x0008);
	 dds_load = 0x0000;
	 Update_DDS_Control();
	 dds_initialized = 1;
  }
  if (!OLD_RADIO)
  {
	 if (freq>15000000l)
	 {
		dds_vco = 0;
		dds_freq = freq/16.0;
	 }
	 else
		if (freq>7500000l)
		{
		  dds_vco = 2;
		  dds_freq = freq/8.0;
		}
		else
		  if (freq>3750000l)
		  {
			 dds_vco = 1;
			 dds_freq = freq/4.0;
		  }
		  else
			 if (freq>1875000l)
			 {
				dds_vco = 3;
				dds_freq = freq/2.0;
			 }
			 else
				if (freq>937500l)
				{
				  dds_vco = 4;
				  dds_freq = freq;
				}
				else
				{
				  dds_vco = 6;
				  dds_freq = freq*2;
				}
	 dds_freq *= (128.0/41);
  }
  else
  {
	 if (freq>15000000l)
	 {
		dds_vco = 7;
		dds_freq = freq/16.0;
	 }
	 else
		if (freq>7500000l)
		{
		  dds_vco = 5;
		  dds_freq = freq/8.0;
		}
		else
		  if (freq>3750000l)
		  {
			 dds_vco = 6;
			 dds_freq = freq/4.0;
		  }
		  else
			 if (freq>1875000l)
			 {
				dds_vco = 4;
				dds_freq = freq/2.0;
			 }
			 else
				if (freq>937500l)
				{
				  dds_vco = 3;
				  dds_freq = freq;
				}
				else
				{
				  dds_vco = 2;
				  dds_freq = freq*2;
				}
    dds_freq;
  }
  dds_freq = dds_freq/DDSCRYSTAL*65536;
  freq_unsigned = dds_freq;
  outport(dds_freq_address,freq_unsigned/256);
  delay(1);
  outport(dds_freq_address,freq_unsigned%256);
  delay(1);
  dds_freq -=freq_unsigned;
  dds_freq *= 65536;
  freq_unsigned = dds_freq;
  outport(dds_freq_address,freq_unsigned/256);
  delay(1);
  outport(dds_freq_address,freq_unsigned%256);
  delay(1);
  dds_load = 0x0010;
  Update_DDS_Control();
  dds_load = 0x0018;
  Update_DDS_Control();
  dds_load = 0x0010;
  Update_DDS_Control();
  if (freq<2000000L)
    set_parallel_port(1);
  else
    if (freq<4000000L)
      set_parallel_port(2);
    else
      if (freq<7300000L)
        set_parallel_port(4);
      else
        if (freq<14400000L)
          set_parallel_port(8);
        else
          set_parallel_port(16);
  Set_Preselectors();
}

char *MIXER_GAIN_Control::Get_Name(void)

{
  return "MIXER GAIN";
}

char *MIXER_GAIN_Control::Get_Error_Message(void)

{
  return "Antenna Power out of range.";
}

long MIXER_GAIN_Control::Get_Value(void)

{
  return mixer_gain;
}

int MIXER_GAIN_Control::Set_Value(long value)

{
  if (value>=MINMIXERGAIN && value<=MAXMIXERGAIN)
  {
	 mixer_gain = value;
	 Update_DDS_Control();
	 return 1;
  }
  return 0;
}

long MIXER_GAIN_Control::Get_Min(void)

{
  return MINMIXERGAIN;
}

long MIXER_GAIN_Control::Get_Max(void)

{
  return MAXMIXERGAIN;
}

void MIXER_GAIN_Control::Mute(void)

{
  /* does not exist */
}

void MIXER_GAIN_Control::Restore(void)

{
  /* does not exist */
}

char *MODE_Control::Get_Name(void)

{
  return "MODE";
}

char *MODE_Control::Get_Error_Message(void)

{
  return "Mode out of range.";
}

long MODE_Control::Get_Value(void)

{
  return mode;
}

int MODE_Control::Set_Value(long value)

{
  if (value>=MINMODE && value <=MAXMODE)
  {
    mode = value;
    return change_mode();
  }
  return 0;
}

int change_mode()

{
  int message[3];

  if (mode == 0 && am_side == 1)
    message[0] = 10;
  else
    message[0] = mode;
  if (message[0] == 5)
    message[0] = 2;    /* CW is USB */    
  message[1] = message[0];
  DSPSendMsg(msg_MODE,message,3);
  set_frequency();
  return 1;
}

long MODE_Control::Get_Min(void)

{
  return MINMODE;
}

long MODE_Control::Get_Max(void)

{
  return MAXMODE;
}

void MODE_Control::Mute(void)

{
  /* does not exist */
}

void MODE_Control::Restore(void)

{
  /* does not exist */
}

char *IF_GAIN_Control::Get_Name(void)

{
  return "IF GAIN";
}

char *IF_GAIN_Control::Get_Error_Message(void)

{
  return "IF Gain out of range.";
}

long IF_GAIN_Control::Get_Value(void)

{
  if (PTT)
    return tx_if_gain;
  return if_gain;
}

int IF_GAIN_Control::Set_Value(long value)

{
  int message[3];

  if (value>=MINIFGAIN && value<=MAXIFGAIN)
  {
    if (PTT)
    {
      tx_if_gain = value;
      message[0] = tx_if_gain;
      message[1] = tx_if_gain;
      DSPSendMsg(msg_IF_GAIN,message,3);
      return 1;
    }
    if_gain = value;
    message[0] = if_gain;
    message[1] = if_gain;
    DSPSendMsg(msg_IF_GAIN,message,3);
    return 1;
  }
  else
    return 0;
}

long IF_GAIN_Control::Get_Min(void)

{
  return MINIFGAIN;
}

long IF_GAIN_Control::Get_Max(void)

{
  return MAXIFGAIN;
}

void IF_GAIN_Control::Mute(void)

{
  /* does not exist */
}

void IF_GAIN_Control::Restore(void)

{
  /* does not exist */
}

char *SQUELCH_Control::Get_Name(void)

{
  return "SQUELCH";
}

char *SQUELCH_Control::Get_Error_Message(void)

{
  return "Squelch out of range.";
}

long SQUELCH_Control::Get_Value(void)

{
  return squelch;
}

int SQUELCH_Control::Set_Value(long value)

{
  char message[6];

  if (value>=MINSQUELCH && value<=MAXSQUELCH)
  {
	 squelch = value;
	 message[0] = ((Volume_Levels[squelch]-16259)%128)*2;
	 message[1] = (Volume_Levels[squelch]-16259)/128;
    DSPSendMsg(msg_SQUELCH,message,2);
    return 1;
  }
  else
    return 0;
}

long SQUELCH_Control::Get_Min(void)

{
  return MINSQUELCH;
}

long SQUELCH_Control::Get_Max(void)

{
  return MAXSQUELCH;
}

void SQUELCH_Control::Mute(void)

{
  /* does not exist */
}

void SQUELCH_Control::Restore(void)

{
  /* does not exist */
}

char *Compression_Control::Get_Name(void)

{
  return "COMPRESSION";
}

char *Compression_Control::Get_Error_Message(void)

{
  return "Compression out of range.";
}

long Compression_Control::Get_Value(void)

{
  return compression;
}

int Compression_Control::Set_Value(long value)

{
  int message[5];

  if (value>=MINCOMPRESSION && value <= MAXCOMPRESSION)
  {
    compression = value;
    if (PTT)
      send_compression();
    return 1;
  }
  return 0;
}

long Compression_Control::Get_Min(void)

{
  return MINCOMPRESSION;
}

long Compression_Control::Get_Max(void)

{
  return MAXCOMPRESSION;
}

void Compression_Control::Mute(void)

{
  /* does not exist */
}

void Compression_Control::Restore(void)

{
  /* does not exist */
}

void send_compression()

{
  char message[14];
  int message2;

  if (PTT)
  {
    message[0] = compression;
    if (compression)
      message2 = Volume_Levels[compression_level];
    else
      message2 = Volume_Levels[mic_level];
    message[4] = monitor+27;
  }
  else
  {
    message[0] = AGC;
    message2 = Volume_Levels[volume];
    message[4] = TDA_8425;
  }
  message[1] = 0;
  message[2] = message2%256;
  message[3] = message2/256;
  message[5] = 0;
  DSPSendMsg(msg_AGC,message,7);
}


char *AGC_Control::Get_Name(void)

{
  return "AGC";
}

char *AGC_Control::Get_Error_Message(void)

{
  return "AGC out of range.";
}

long AGC_Control::Get_Value(void)

{
  return AGC;
}

int AGC_Control::Set_Value(long value)

{
  int message[3];

  AGC = value;
  if (!PTT)
  {
    send_compression();
    message[0] = 1;
    message[1] = 1;
    if (AGC==0)
      message[1] = 0;
    if (AGC==2)
      message[0] = 0;
    DSPSendMsg(msg_AGC_10_DB,message,2);
  }
  return 1;
}

long AGC_Control::Get_Min(void)

{
  return MINAGC;
}

long AGC_Control::Get_Max(void)

{
  return MAXAGC;
}

void AGC_Control::Mute(void)

{
  /* does not exist */
}

void AGC_Control::Restore(void)

{
  /* does not exist */
}

char *Mic_Level_Control::Get_Name(void)

{
  return "Mic_Level";
}

char *Mic_Level_Control::Get_Error_Message(void)

{
  return "Mic Level out of range.";
}

long Mic_Level_Control::Get_Value(void)

{
  return mic_level;
}

int Mic_Level_Control::Set_Value(long value)

{
  int message[3];

  mute = 0;
  mic_level = value;
  message[0] = Volume_Levels[mic_level];
  message[1] = Volume_Levels[mic_level];
  if (PTT && !compression)
    DSPSendMsg(msg_VOLUME,message,2);
  return 1;
}

long Mic_Level_Control::Get_Min(void)

{
  return MINMICLEVEL;
}

long Mic_Level_Control::Get_Max(void)

{
  return MAXMICLEVEL;
}

void Mic_Level_Control::Mute(void)

{
  // Mute does not exist
}

void Mic_Level_Control::Restore(void)

{
  // Restore does not exist
}

char *Compression_Level_Control::Get_Name(void)

{
  return "Compression_Level";
}

char *Compression_Level_Control::Get_Error_Message(void)

{
  return "Compression Level out of range.";
}

long Compression_Level_Control::Get_Value(void)

{
  return compression_level;
}

int Compression_Level_Control::Set_Value(long value)

{
  int message[3];

  mute = 0;
  compression_level = value;
  message[0] = Volume_Levels[compression_level];
  message[1] = Volume_Levels[compression_level];
  if (PTT && compression)
    DSPSendMsg(msg_VOLUME,message,2);
  return 1;
}

long Compression_Level_Control::Get_Min(void)

{
  return MINCOMPRESSIONLEVEL;
}

long Compression_Level_Control::Get_Max(void)

{
  return MAXCOMPRESSIONLEVEL;
}

void Compression_Level_Control::Mute(void)

{
  // Mute does not exist
}

void Compression_Level_Control::Restore(void)

{
  // Restore does not exist
}

char *Volume_Control::Get_Name(void)

{
  return "Volume";
}

char *Volume_Control::Get_Error_Message(void)

{
  return "Volume out of range.";
}

long Volume_Control::Get_Value(void)

{
  return volume;
}

int Volume_Control::Set_Value(long value)

{
  int message[3];

//  AGC = 0;
  mute = 0;
  volume = value;
  message[0] = Volume_Levels[volume];
  message[1] = Volume_Levels[volume];
  if (!PTT)
    DSPSendMsg(msg_VOLUME,message,2);
  return 1;
}

long Volume_Control::Get_Min(void)

{
  return MINVOLUME;
}

long Volume_Control::Get_Max(void)

{
  return MAXVOLUME;
}

void Volume_Control::Mute(void)

{
  int message[3];

  if (mute == 0)
  {
    mute = 1;
    AGC = 0;
    DSPSendMsg(msg_MUTE,NULL,0);
  }
}

void Volume_Control::Restore(void)

{
  Set_Value(volume);
}

char *Monitor_Control::Get_Name(void)

{
  return "Monitor";
}

char *Monitor_Control::Get_Error_Message(void)

{
  return "Monitor out of range.";
}

long Monitor_Control::Get_Value(void)

{
  return monitor;
}

int Monitor_Control::Set_Value(long value)

{
  if (monitor>=MINMONITOR && monitor<=MAXMONITOR)
  {
    monitor = value;
    send_compression();
    return 1;
  }
  return 0;
}

long Monitor_Control::Get_Min(void)

{
  return MINMONITOR;
}

long Monitor_Control::Get_Max(void)

{
  return MAXMONITOR;
}

void Monitor_Control::Mute(void)

{
  /* function does not exist */
}

void Monitor_Control::Restore(void)

{
  /* function does not exist */
}

char *PTT_Control::Get_Name(void)

{
  return "PTT";
}

char *PTT_Control::Get_Error_Message(void)

{
  return "PTT out of range.";
}

long PTT_Control::Get_Value(void)

{
  return PTT;
}

int PTT_Control::Set_Value(long value)

{
  return Set_PTT(value);
}

int Set_PTT(long value)

{
  char message[24];

  ptt_time.ti_hour = 25;
  if (value==1)
  {
	 if (!Valid_Xmit(1))
		return 0;
	 PTT = 1;
//    send_compression();
	 if (compression)
	 {
		message[2] = (Volume_Levels[compression_level])%256;
		message[3] = (Volume_Levels[compression_level])/256;
		message[4] = 1;
		message[6] = tx_if_gain;
	 }
	 else
	 {
		message[2] = (Volume_Levels[mic_level])%256;
		message[3] = (Volume_Levels[mic_level])/256;
		message[4] = 0;
		message[6] = tx_if_gain;
	 }
	 message[0] = 1;
	 message[1] = 0;
	 message[5] = 0;
	 message[7] = 0;
	 message[8] = monitor+27;
	 message[9] = 0;
	 message[10] = tx_i_adjust%256;
	 message[11] = tx_i_adjust/256;
	 DSPSendMsg(msg_TRANSMIT,message,12);
  }
  if (value==0)
  {
	 PTT = 0;
//    send_compression();
	 message[2] = (Volume_Levels[volume])%256;
	 message[3] = (Volume_Levels[volume])/256;
	 message[0] = 0;
	 message[1] = 0;
	 message[4] = AGC;
	 message[5] = 0;
	 message[6] = if_gain;
	 message[7] = 0;
	 message[8] = TDA_8425;
	 message[9] = 0;
	 message[10] = i_adjust%256;
	 message[11] = i_adjust/256;
    DSPSendMsg(msg_RECIEVE,message,12);
  }
  if (split >1 && tx_frequency!=frequency)
	 set_frequency();
  else
  {
	 Update_DDS_Control();
	 Set_Preselectors();
  }
  return PTT;
}

long PTT_Control::Get_Min(void)

{
  return 0;
}

long PTT_Control::Get_Max(void)

{
  return 1;
}

void PTT_Control::Mute(void)

{
  /* does not exist */
}

void PTT_Control::Restore(void)

{
  /* does not exist */
}

char *Radio_Control::Get_Name(void)

{
  return "Radio";
}

char *Radio_Control::Get_Error_Message(void)

{
  return "Radio Control was called. (Dummy routine)";
}

long Radio_Control::Get_Value(void)

{
  return 0;
}

int Radio_Control::Set_Value(long value)

{
  return 0;
}

long Radio_Control::Get_Min(void)

{
  return 0;
}

long Radio_Control::Get_Max(void)

{
  return 0;
}

void Radio_Control::Mute(void)

{
  /* does not exist */
}

void Radio_Control::Restore(void)

{
  /* does not exist */
}

Int_Control::Int_Control(int min,int max)

{
  Min = min;
  Max = max;
  Value = min;
}

char *Int_Control::Get_Name(void)

{
  return "Int";
}

char *Int_Control::Get_Error_Message(void)

{
  return "Int out of range.";
}

long Int_Control::Get_Value(void)

{
  return Value;
}

int Int_Control::Set_Value(long value)

{
  if (value>=Get_Min() && value<=Get_Max())
  {
    Value = value;
    return 1;
  }
  else
    return 0;
}

long Int_Control::Get_Min(void)

{
  return Min;
}

long Int_Control::Get_Max(void)

{
  return Max;
}

void Int_Control::Mute(void)

{
  /* does not exist */
}

void Int_Control::Restore(void)

{
  /* does not exist */
}

void Update_DDS_Control()

{
  unsigned int dds_binary;

  switch(dds_vco)
  {
    case 0 :
    case 1 : dds_binary = dds_vco*0x4000;
             break;
    case 2 :
    case 3 : dds_binary = (dds_vco-2)*0x4000+0x8000;
             break;
    case 4 :
    case 5 : dds_binary = (dds_vco-4)*0x4000+0x0100;
             break;
    case 6 :
    case 7 : dds_binary = (dds_vco-6)*0x4000+0x8100;
  }
  outport(dds_control_address,(1-PTT)*(antenna_power*0x1000+0x0800)+
          (mixer_gain+1)+dds_load+dds_binary+PTT*(tx_antenna_power*0x2000));
  delay(1);
}

void delay(long value)

{
  long count;

  for(count=0;count<100l*value;count++);
}

void select_previous_band()

{
  int count;

  count = band_pointers[banded];
  while (min_frequency != bands[count][0] && count <= band_pointers[banded+1])
    count ++;
  if (count == band_pointers[banded+1]) /* ERROR */
  {
    min_frequency = MINFREQUENCY;
    max_frequency = MAXFREQUENCY;
  }
  if (count == band_pointers[banded])
  {
    count = band_pointers[banded+1];
    if (mode == 1)
      mode = 2;
  }
  count --;
  if (count == 3  && mode == 2)
    mode = 1;
  min_frequency = bands[count][0];
  max_frequency = bands[count][1];
  tx_frequency = frequency = bands[count][1];
  change_mode();
}

void select_next_band()

{
  int count;

  count = band_pointers[banded];
  while (min_frequency != bands[count][0] && count <= band_pointers[banded+1])
    count ++;
  if (count == band_pointers[banded+1]) /* ERROR */
  {
    min_frequency = MINFREQUENCY;
    max_frequency = MAXFREQUENCY;
  }
  if (count == band_pointers[banded+1]-1)
  {
    count = band_pointers[banded]-1;
    if (mode == 2)
      mode = 1;
  }
  count++;
  if (count == 4)
    if (mode == 1)
      mode = 2;
  min_frequency = bands[count][0];
  max_frequency = bands[count][1];
  tx_frequency = frequency = bands[count][0];
  change_mode();
}

Valid_Xmit(int display_error)

{
  if (OVERSEAS)
    return 1;
  if (Valid_CallSign(display_error))
    if (Valid_TX_Frequency(display_error))
      return 1;
  return 0;
}

Valid_CallSign(int display_error)

{
  char string[20];
  int answer = 1;

  strcpy(string,CallSign);
  strupr(string);
  if (!isalpha(string[0]))
    answer = 0;
  if (!(isalpha(string[1]) && string[2]>='0' && string[2]<='9') &&
      !(isalpha(string[2]) && string[1]>='0' && string[1]<='9'))
    answer = 0;
  if (!isalpha(string[3]))
    answer = 0;
  if (strlen(string)>4)
  {
    if (!isalpha(string[4]))
      answer = 0;
    if (strlen(string)>5)
      if (!isalpha(string[5]))
        answer = 0;
  }
  if (answer == 0 && display_error)
    MessageBox(NULL,"In order to transmit, you must tell RADWARE your amateur "
                    "radio callsign. Please go into the Configure Menu & type "
                    "in your callsign.","Warning",MB_ICONEXCLAMATION | MB_OK);
  return(answer);
}

Valid_TX_Frequency(int display_error)

{
  long freq;
  int count;
  int answer = 0;

  if (split == 1 || split == 2)
    freq = tx_frequency;
  else
    freq = frequency;
  for(count = 1;count<10;count++)
    if (bands[count][0]<= freq && bands[count][1]>=freq)
      answer = 1;
  if (answer == 0 && display_error)
    MessageBox(NULL,"You are attempting to transmit outside legal amateur bands.\n"
                    "Please tune to an appropriate frequency.","ERROR",MB_ICONHAND | MB_OK);
  return(answer);
}

void Set_Radware_Address(int address)

{
  dds_freq_address = address;
  preselector_address = address+2;
  dds_control_address = address+4;
}

int Get_Radware_Address()

{
  return dds_freq_address;
}

void set_am_side(int value)

{
  if (am_side != value)
  {
    am_side = value;
    change_mode();
  }
}

void Clear_Preselectors()

{
  outport(preselector_address,0);
}

void Set_Preselectors()

{
  long freq;

  if (split == 0 || (!PTT && split == 2) || (PTT && split == 3))
	 if (mode == 0 && (!PTT))
		freq = frequency+AM_Offset;
    else
      freq = frequency;
  else
    if (mode == 0 && !PTT)
      freq = tx_frequency+AM_Offset;
    else
      freq = tx_frequency;
  if (freq>MINPRESELECTOR1)
    preselector = 0x0080;
  else
	 if (freq>MINPRESELECTOR2)
		preselector = 0x0040;
	 else
		if (freq>MINPRESELECTOR3)
		  preselector = 0x0020;
		else
		  if (freq>MINPRESELECTOR4)
			 preselector = 0x0010;
        else
		    if (freq>MINPRESELECTOR5)
			   preselector = 0x0008;
          else
            if (freq>MINPRESELECTOR6)
		        preselector = 0x0004;
            else
              if (freq>MINPRESELECTOR7)
	             preselector = 0x0002;
              else
  	             preselector = 0x0001;
  if (PTT)
    preselector = 0x0000;
  outport(preselector_address,preselector);
}

char convert_from_baudot(int which)

{
  int count;
  char c;

  if (baudot_recieve_shift)
    c = Baudot[baudot_alphabet][which];
  if (baudot_recieve_shift)
    c = Baudot[0][which];
  switch (c)
  {
    case 2 : baudot_recieve_shift = 1;
						 return 0;
    case 3 : baudot_recieve_shift = 0;
             return 0;
    default : return c;
  }
}

void start_play()

{
  DSPSendMsg(msg_START_PLAY,NULL,0);
}

void end_play()

{
//  DSPSendDataNull(0);
//  DSPSendDataNull(1);
//  DSPSendDataNull(3);
  DSPSendMsg(msg_STOP_PLAY,NULL,0);
}

void end_play_and_null()

{
//  DSPSendDataNull(0);
//  DSPSendDataNull(1);
  DSPSendDataNull(2);
//  DSPSendDataNull(3);
  DSPSendMsg(msg_STOP_PLAY,NULL,0);
}

int sent = 0;
UINT send_play_message(int *string)

{
  return DSPSendData(string,258);
}

static FILE *voice_stream = NULL;
int old_ptt;

int save_voice_memory_start(char *filename,int which)

{
	voice_stream = fopen(filename,"wb");
	if (voice_stream)
	{
		old_ptt = PTT;
		PTT = 1;
      DSPGetData(264);
		DSPSendMsg(msg_START_RECORD,NULL,0);
      return 1;
	}
   return 0;
}

int sine;

void save_voice_memory()

{
  int count,value;
  if (voice_stream)
    do
    {
      for(count=8;count<264;count+=2)
      {
        fputc(voice_string[count+1],voice_stream);
        fputc(voice_string[count],voice_stream);
      }
      value = DSPGetData(264);
    }
    while (!value);
    fflush(voice_stream);
}

void save_voice_memory_end()

{
	if (voice_stream)
	{
		DSPSendMsg(msg_STOP_RECORD,NULL,0);
		PTT = old_ptt;
		fclose(voice_stream);
		voice_stream = NULL;
	}
}

int save_voice_memory_open()

{
	if (voice_stream)
		return 1;
	return 0;
}

int set_tx_analog_gain(int value)

{
  if (value>= MINIFGAIN && value<=MAXIFGAIN)
  {
    tx_if_gain = value;
    return 1;
  }
  return 0;
}

int get_tx_analog_gain()

{
  return tx_if_gain;
}

void get_1848()

{
  int message[2];

  message[0] = 0;
  DSPSendMsg(msg_GET_1848,message,1);
}

void Set_Morse(int how)

{
  if (how)
  {
    if (Valid_Xmit(0))
      if (!PTT)
        Set_PTT(1);
    DSPSendMsg(msg_MORSE_KEY,NULL,0);
  }
  else
  {
    DSPSendMsg(msg_STOP_MORSE,NULL,0);
    gettime(&ptt_time);
  }
}

long Check_PTT_Time()

{
  long time_change=-1;
  struct time now;

  if (ptt_time.ti_hour!=25)
  {
    gettime(&now);
  	 time_change = labs(now.ti_hour-ptt_time.ti_hour)*3600l+
							 (now.ti_min-ptt_time.ti_min)*60+
							 (now.ti_sec-ptt_time.ti_sec);
    if (time_change>1)
      Set_PTT(0);
  }
  return (time_change);
}

int get_port_on_off()

{
  return parallel_port;
}

void set_port_on_off(int value)

{
  if (value == 0 || value == 1)
  {
    parallel_port = value;
    set_frequency();
  }
}

void set_parallel_port(int value)

{
  int strobe,not_strobe;

  if (parallel_port && last_parallel != value)
  {
    strobe = inportb(0x37A) & 0xFE;
    not_strobe = strobe | 0x01;
    outport(0x0378,value);
    outport(0x037A,not_strobe);
    outport(0x037A,strobe);
    last_parallel = value;
  }
}

void Set_Frequency_Wrap(int value)

{
  if (value == 0 || value == 1)
    frequency_wrap = value;
}

int Get_Frequency_Wrap()

{
  return frequency_wrap;
}

void Set_CallSign(char *string)

{
  strncpy(CallSign,string,38);
  CallSign[39] = 0;
}

void Get_CallSign(char *string)

{
  strcpy(string,CallSign);
}

int Is_Frequency_Amateur(long frequency)

{
  int count;

  for(count = MIN_AMATEUR_BAND;count<=MAX_AMATEUR_BAND;count++)
    if (frequency>=bands[count][0] && frequency<=bands[count][1])
      return 1;
  return 0;
}

