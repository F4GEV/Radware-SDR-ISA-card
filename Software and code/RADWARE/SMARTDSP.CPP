#include <d:\dsp1\thisCls.h>
#include <stdio.h>
#include <windows.h>
#include <mmsystem.h>
#include "dspmgr.h"
#include <string.h>
#include <d:\dsp1\smartdsp.h>

HWND CallBack;

extern FARPROC CallBackInstance;
extern HWND TestWindow;
extern DWORD dsp_message;
extern "C" long pascal _export dspGetNumDevs(void);
extern "C" long pascal _export dspReset();
extern "C" long pascal _export dspGetMsg(HALG hAlg,
													  LPLONG lpMsgID,
													  LPVOID lpMsgBody,
													  DWORD dwSize);
extern "C" long pascal _export dspLoadAlg (DWORD dwDeviceID,
														 DWORD dwInstance,
														 LPHALG lphAlg,
														 LPSTR lpAlgSection,
														 DWORD dwCallback,
														 DWORD dwFlags);
extern "C" long pascal _export dspSendMsg (HALG hAlg,LONG lMsgID,
														 LPVOID lpMsgBody,DWORD dwSize);
extern "C" long pascal _export dspSendData (HALG hAlg,LPVOID lpMsgBody,
                                            DWORD dwSize);
extern "C" long pascal _export dspGetData (HALG hAlg,LPVOID lpMsgBody,
                                           DWORD dwSize);
extern "C" long pascal _export dspUnloadAlg (HALG hAlg);

extern HALG RadioWareAlg;

#include "smartdsp.h"

static int AGC = 0,filter = 0,mute = 0, volume = 0;
static int simulation = 0;
static int last_send_data = 0;
static int last_get_data = 0;
extern int Need_More_Voice;
int message_string[1000];
int spectrum_string[1000];
int lsb_string[1000];
int usb_string[1000];
int lsb_time_string[1000];
int usb_time_string[1000];
int smeter_string[1000];
int frequency_calibrate_string[1000];
int analog_string[1000];
int voice_string[1000];
int packet_string[300];
int message_length,spectrum_length = 256;
int null_data[4][258];
void Simulation_Mode()

{
  simulation = 1;
}

long DSPGetMsg(LPLONG MsgID,LPVOID string,DWORD size)

{
  if (simulation)
	 return 0;
  return dspGetMsg(RadioWareAlg,MsgID,string,size);
}

long DSPGetNumDevs(void)

{
  return 1; //dspGetNumDevs();
}

long DSPLoadAlg(DWORD dwDeviceID,DWORD dwInstance,LPSTR lpAlgSection,
					 DWORD dwCallBack)

{
  int count;

  for(count = 0;count<258;count++)
  {
    null_data[0][count] = 0;
    null_data[1][count] = 0;
    null_data[2][count] = 0;
    null_data[3][count] = 0;
  }
  CallBack = TestWindow; //dwCallBack;
  if (simulation)
	 return 0;
  return dspLoadAlg(dwDeviceID,dwInstance,&RadioWareAlg,lpAlgSection,
						  (DWORD) CallBackInstance,CALLBACK_FUNCTION);
}

long DSPReset()

{
  return 1;
}


long DSPSendMsg (LONG lMsgID,LPVOID lpMsgBody,DWORD dwSize)

{
  long count;

  if (simulation)
	 return 0;
  return dspSendMsg (RadioWareAlg,lMsgID,lpMsgBody,dwSize);
}

int last_message_recieved = 0;

long DSPSendData (LPVOID lpMsgBody,DWORD dwSize)

{
  if (simulation)
	 return 0;
  last_send_data = dspSendData (RadioWareAlg,lpMsgBody,dwSize);
  last_message_recieved = 116;
  return last_send_data;
}

long DSPGetData(DWORD dwSize)

{
  if (simulation)
    return 1;
  last_get_data = dspGetData(RadioWareAlg,voice_string,dwSize);
  return last_get_data;
}

long DSPUnloadAlg()

{
  if (simulation)
	 return 0;
  return dspUnloadAlg(RadioWareAlg);
}

int wait_for_message=0;

extern "C" void FAR PASCAL _export CallBackFunction(HALG Alg,DWORD Msg, DWORD Instance,
														DWORD Param1,DWORD Param2)

{
  dsp_message = Msg;
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
}

DSPSendDataNull(int which)

{
  DSPSendData(null_data[which],258);
}


