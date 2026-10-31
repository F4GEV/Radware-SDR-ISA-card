
#include <d:\dsp1\thisCls.h>
#include <stdio.h>
#include <windows.h>
#include <mmsystem.h>
#include "dspmgr.h"
#include <string.h>
#include <d:\dsp1\smartdsp.h>
#include <d:\radware\code\echo.h>

HWND CallBack;

#define msg_GET_MSG 1000
#define TIMEOUT 4000000L
#define ECHO_ASIC_614_ID 4
#define ECHO_ASIC_615_ID 0

extern HWND TestWindow;
extern DWORD dsp_message;
static int sound_card;

extern HALG RadioWareAlg;

#include "smartdsp.h"
void Autodetect_Sound_Card();
void PrintInt(int);
unsigned int Read_DSP_Word();
int Write_DSP_Word(unsigned int value);
int Write_DSP_Boot_Word(unsigned int value);
long DSPSendMsg (LONG number,LPVOID message,DWORD length);
int Bootload_File(char *filename);
int pss_data,pss_control,pss_status;

static int AGC = 0,filter = 0,mute = 0, volume = 0;
static int simulation = 0;
static int last_send_data = 0;
static int last_get_data = 0;
extern int Need_More_Voice;
static int loaded = 0;
static int next_dsp_char = 0;
static int dsp_buffer = 0;
int Game_Port_Address;
int message_number = 0;
int message_length = 0;
int message_pointer;
long lost_transmit_charactors = 0;
int message_string[256];
int spectrum_string[256];
int lsb_string[256];
int usb_string[256];
int lsb_time_string[256];
int usb_time_string[256];
int smeter_string[256];
int frequency_calibrate_string[256];
int analog_string[256];
int voice_string[256];
int packet_string[256];
int get_1848_string[32];
int spectrum_length = 256;
int null_data[4][258];
int packet_length;
int timeout;

void Simulation_Mode()

{
  simulation = 1;
}

void DSPGetMsg()

{
  int count;
  long timer;
  int this_char;
  int char_ready = 0;
  int end_of_message = 0;

  if (simulation || !loaded)
    return;
  if (dsp_buffer)
  {
    this_char = next_dsp_char;
    char_ready = 1;
    dsp_buffer = 0;
  }
  else
    if (inpw(sound_card+pss_status) & PSS_RF)
    {
      this_char = inpw(sound_card+pss_data);
      char_ready = 1;
    }
  while (char_ready)
  {
    char_ready = 0;
    if (message_number == 0)
    {
                         /* check for valid message_number */
      if (this_char == 101 || this_char == 102 || this_char == 103 ||
          this_char == 104 || this_char == 105 || this_char == 110 ||
          this_char == 111 || this_char == 112 || this_char == 113 ||
          this_char == 116 || this_char == 120)
        message_number = this_char;
    }
    else
      if (message_length == 0)
      {
        if (message_length>256)    /* abort message */
          message_number = 0;
        {
          message_length = this_char;
          if (message_length == 0)
            end_of_message = 1;
          message_pointer = 0;
        }
      }
      else
      {
        switch(message_number)
        {
          case 101 : usb_string[message_pointer] = this_char;
                     break;
          case 102 : lsb_string[message_pointer] = this_char;
                     break;
          case 103 : lsb_time_string[message_pointer] = this_char;
                     break;
          case 104 : usb_time_string[message_pointer] = this_char;
                     break;
          case 105 : smeter_string[message_pointer] = this_char;
                     break;
          case 112 : frequency_calibrate_string[message_pointer] = this_char;
                     break;
          case 113 : analog_string[message_pointer] = this_char;
                     break;
          case 116 : packet_string[message_pointer] = this_char;
                     packet_length = message_pointer;
                     break;
          case 120 : get_1848_string[message_pointer] = this_char;
                     break;
        }
        message_pointer++;
        if (message_pointer >= message_length)
          end_of_message = 1;
      }
    if (end_of_message == 1)
    {
      if (message_number == 101 || message_number == 102 ||
          message_number == 103 || message_number == 104 ||
          message_number == 105 || message_number == 110 ||
          message_number == 111 || message_number == 112 ||
          message_number == 113 || message_number == 114 ||
          message_number == 115 || message_number == 116 ||
          message_number == 120)
        PostMessage(CallBack,WM_COMMAND,205+message_number,0);
      end_of_message = 0;
      message_number = 0;
      message_length = 0;
      message_pointer = 0;
    }
    else
      if (dsp_buffer)
      {
        this_char = next_dsp_char;
        char_ready = 1;
        dsp_buffer = 0;
      }
      else
        for(timer=0;timer<10000l;timer++)
          if (inpw(sound_card+pss_status) & PSS_RF)
          {
            this_char = inpw(sound_card+pss_data);
            char_ready = 1;
            break;
          }
  }
}

long DSPGetNumDevs(void)

{
  return 1;
}

long Load_DSP(HWND test_window)

{
  return DSPLoadAlg(0,NULL,"RADWARE.LD",test_window);
}

long DSPLoadAlg(DWORD dwDeviceID,DWORD dwInstance,LPSTR lpAlgSection,
					 HWND test_window)

{
  Autodetect_Sound_Card();
  CallBack = TestWindow;
  if (simulation)
	 return 0;
  loaded = Bootload_File("radware.ld");
  return loaded;
}

long DSPReset()

{
  return 1;
}


long DSPSendMsg (LONG number,LPVOID message,DWORD length)

{
  long count;
  int *Message;

  if (simulation)
	 return 0;
//  PrintInt(number);
  Message = (int *) message;
  Write_DSP_Word(number);
  Write_DSP_Word(length);
  for(count=0;count<length;count++)
    Write_DSP_Word(Message[count]);
  return 1;
}

int last_message_recieved = 0;

long DSPSendData (LPVOID lpMsgBody,DWORD dwSize)

{
//  if (simulation)
	 return 0;
//  last_send_data = dspSendData (RadioWareAlg,lpMsgBody,dwSize);
//  last_message_recieved = 116;
//  return last_send_data;
}

long DSPGetData(DWORD dwSize)

{
//  if (simulation)
    return 1;
//  last_get_data = dspGetData(RadioWareAlg,voice_string,dwSize);
//  return last_get_data;
}

long DSPUnloadAlg()

{
//  if (simulation)
	 return 0;
//  return dspUnloadAlg(RadioWareAlg);
}

int wait_for_message=0;

extern "C" void FAR PASCAL _export CallBackFunction(HALG Alg,DWORD Msg, DWORD Instance,
														DWORD Param1,DWORD Param2)

{
/*  dsp_message = Msg;
  if (dsp_message == 34048 && !wait_for_message)
  {
    switch (Param1)
    {
      case 101 : DSPGetMsg(& (LONG) Param1,usb_string,(DWORD) 1000);
                 break;
      case 102 : DSPGetMsg(& (LONG) Param1,lsb_string,(DWORD) 1000);
                 break;
      case 103 : DSPGetMsg(& (LONG) Param1,lsb_time_string,(DWORD) 1000);
                 break;
      case 104 : DSPGetMsg(& (LONG) Param1,usb_time_string,(DWORD) 1000);
                 break;
      case 105 : DSPGetMsg(& (LONG) Param1,smeter_string,(DWORD) 1000);
                 break;
      case 112 : DSPGetMsg(& (LONG) Param1,frequency_calibrate_string,(DWORD) 1000);
                 break;
      case 113 : DSPGetMsg(& (LONG) Param1,analog_string,(DWORD) 1000);
					  break;
		case 114 : DSPGetMsg(& (LONG) Param1,voice_string,(DWORD) 1000);
					  break;
		case 115 : DSPGetMsg(& (LONG) Param1,voice_string,(DWORD) 1000);
					  break;
 		case 116 : DSPGetMsg(& (LONG) Param1,packet_string,(DWORD) 300);
					  break;
		default  : DSPGetMsg(& (LONG) Param1,message_string,(DWORD) 1000);
	 }
	 wait_for_message = 1;
	 last_message_recieved = Param1;
	 message_length = Param2;
//	 PostMessage(CallBack,WM_COMMAND,101,0);
 }
 else
   if (dsp_message == 34049)
   {
      if ((last_get_data != 0 && Param1 == last_get_data) || Param2 == 268)
        PostMessage(CallBack,WM_COMMAND,319,0);
      else
        if (last_message_recieved == 115)
        {
          if (!Need_More_Voice)
          {
            Need_More_Voice = 1;
            PostMessage(CallBack,WM_COMMAND,320,0);
          }
          else
            DSPSendData(null_data,258);
        }
        else
          if (message_length == 256 || last_message_recieved == 105 ||
              last_message_recieved == 110 || last_message_recieved == 111
              || last_message_recieved == 1140 || last_message_recieved == 115
              || last_message_recieved == 116)
            PostMessage(CallBack,WM_COMMAND,205+last_message_recieved,0);
		wait_for_message = 0;
	 }
//  else
//	 printf("Unknown message from dsp %d",dsp_message);
*/}

DSPSendDataNull(int which)

{
  DSPSendData(null_data[which],258);
}

int Bootload_File(char *filename)

{
  FILE *stream;
  int count;
  int data;
  int config;

  stream = fopen(filename,"rb");
  if (!stream)
  {
    return -2;
  }
  outpw(sound_card+pss_control,PSS_RESET);       /* Boot DSP */
  data = fgetc(stream);
  outpw(sound_card+pss_data,data);
  config = outpw(sound_card+pss_control,0);
  count = 0;
  while (!feof(stream))
  {
    data = fgetc(stream);
    Write_DSP_Boot_Word(data);
    count++;
  }
  outpw(sound_card+pss_data,0);
  return Read_DSP_Word();
}

int Write_DSP_Boot_Word(unsigned int value)

{
  int count;

  for(count=0;count<1024;count++)
    if (inpw(sound_card+pss_status) & PSS_FLAG3)
    {
      outpw(sound_card+pss_data,value);
      return 1;
    }
//  printf("Timeout Error in Bootloading DSP \n");
  return 0;
}

int Write_DSP_Word(unsigned int value)

{
  long register count;
  int temp;

  for(count=0;count<100000l;count++)
    if (inpw(sound_card+pss_status) & PSS_WE)
    {
      outpw(sound_card+pss_data,value);
      if (inpw(sound_card+pss_status) & PSS_RF)
      {
        next_dsp_char = inpw(sound_card+pss_data);
        dsp_buffer = 1;
        DSPGetMsg();
      }
      else
        inpw(sound_card+pss_data);

//      Read_DSP_Word();
      return 1;
    }
  lost_transmit_charactors++;
  return 0;
}

unsigned int Read_DSP_Word()

{
  long register count;

  if (dsp_buffer)
  {
    dsp_buffer = 0;
    return next_dsp_char;
  }
  timeout = 0;
  for(count=0;count<TIMEOUT;count++)
    if (inpw(sound_card+pss_status) & PSS_RF)
      return inpw(sound_card+pss_data);
  timeout = 1;
  return 0;
}

void Autodetect_Sound_Card()

{
  int asic;

  sound_card = 0x220;
  Game_Port_Address = 0x230;
  asic = inpw(sound_card+ECHO_ASIC_614_ID)& 0xFF00;
  if (asic !=0x4500)
  {
    sound_card= 0x240;
    Game_Port_Address = 0x250;
    asic = inpw(sound_card+ECHO_ASIC_614_ID)& 0xFF00;
    if (asic!=0x4500)
    {
      sound_card=0x220;
      Game_Port_Address = 0x236;
      pss_control = 0x12;
      outpw(sound_card+pss_control,ECHO_ASIC_615_ID);
      asic = inpw(sound_card+0x16)&0xFF00;
      if (asic!=0x4500 && asic!=0x5000)
      {
        sound_card=0x240;
        Game_Port_Address = 0x256;
        outpw(sound_card+pss_control,ECHO_ASIC_615_ID);
        asic = inpw(sound_card+0x16)&0xFF00;
        if (asic!=0x4500 && asic!=0x5000)
        {
          simulation = 1;
          sound_card = 0;
          return;
        }
      }
      pss_data = 0x10;
      pss_status = 0x14;
      return;
    }
  }
  pss_data = 0;
  pss_control = 2;
  pss_status = 2;
}

int Get_Sound_Card()

{
  return sound_card;
}

