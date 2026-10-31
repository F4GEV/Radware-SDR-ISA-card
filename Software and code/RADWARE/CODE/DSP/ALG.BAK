#include "dspshell.h"
#include "dsp1848.h"

#ifndef TRUE
#define TRUE 1
#define FALSE 0

#endif

#define SET_MASTER_COMMAND  0x0010
#define MASTER_VOLUME_LEFT  0x0000
#define MASTER_VOLUME_RIGHT 0x0100
#define MASTER_BASS         0x0200
#define MASTER_TREBLE       0x0300
#define MASTER_SWITCH       0x0800
#define MAX_MORSE 100

#define STR_INITCOMPLETE              "initialized"
#define STR_GETMSGREQUEUEERROR        "requeueing error"

       /* Host to DSP */
#define msg_SET_NOISE   500
#define msg_PLOT_TOGGLE 502
#define msg_TALK_TOGGLE 504
#define msg_LOG_TOGGLE  505
#define msg_FREQ_TOGGLE 506
#define msg_LPF_TOGGLE  507
#define msg_HANN_TOGGLE 508
#define msg_PLOT_SEND   509

#define msg_MORSE_CHAR   1501
#define msg_START_MORSE  1502
#define msg_STOP_MORSE   1503
#define msg_RECIEVE      1504
#define msg_TRANSMIT     1505
#define msg_MODE         1506
#define msg_FILTER       1507
#define msg_IF_GAIN      1508
#define msg_AGC          1509
#define msg_VOLUME       1510
#define msg_PLOT_LSB_S   1511
#define msg_PLOT_USB_S   1512
#define msg_PLOT_LSB_T   1513
#define msg_PLOT_USB_T   1514
#define msg_MUTE         1515
#define msg_MORSE_SPEED  1516
#define msg_SMETER       1517
#define msg_START_RECORD 1518
#define msg_STOP_RECORD  1519
#define msg_CHANGE_I     1520
#define msg_START_PLAY   1521
#define msg_MORSE_KEY    1522
#define msg_SQUELCH      1523
#define msg_STOP_PLAY    1498
#define msg_PLAY_DATA    1499
       /* DSP to HOST */
#define msg_PLOT_DATA     500
#define msg_1848_REG_DUMP 503
#define MSG_SMETER_LEVEL    105
#define MSG_MORSE_NEXT_CHAR 110
#define MSG_MORSE_END_MORSE 111
#define msg_FREQ_CAL 112
#define MSG_AGC_ANALOG 113
#define msg_VOICE_DATA 114
#define msg_REQUEST_PLAY_DATA  115
#define msg_RTTY 116
#define SIZE_DATAMSG   50
#define SIZE_PLOTMSG   128
#define SIZE_IO_BUFFER 256
#define GETDATASIZE    129
typedef struct msg
{
  DSPMSGHEADER hdr;
  int data[SIZE_DATAMSG];
}MSG_STRUCT;

MSG_STRUCT msg1,msg2;

int plotMsgData[SIZE_PLOTMSG];

DSPIOCONF devConf;
DSPIOCAPS devCaps;

char Morse[MAX_MORSE]= {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int NewIFGain = -1;
int NewSampleRate = 1;

SOUNDPORTREGS Registers_1848,*pSpregs;
int USB_BUFFER[256];
int LSB_BUFFER[256];
int RTTY_BUFFER[256];
/*int USB_SPARE_BUFFER[128];
int LSB_SPARE_BUFFER[128];
*/int buffer1[SIZE_IO_BUFFER];                  /* Define buffer arrays. */
int buffer2[SIZE_IO_BUFFER];
int buffer3[SIZE_IO_BUFFER];
int buffer4[SIZE_IO_BUFFER];
int Temp_Buffer[SIZE_IO_BUFFER];
int play_buffer[128];
int data_buffer[4][GETDATASIZE];
int current_buffer = 0;
int buffer_write = 0;
int buffer_read = 0;
int buffer_ready = 1;
int squelch = -31500;
/*int buffer_null[256];
*/int *USB = USB_BUFFER;
int *LSB = LSB_BUFFER;

int InputDevice = 0;
int OutputDevice = 1;
int hAlg;                       /* handle to algorithm... myself! */
int hApp;                       /* handle to application (on host) */
int algIoOverrun = 0;
int AnalogAGC = 0;
int PlotOn = FALSE;
int PlotLSBS = FALSE;
int PlotUSBS = FALSE;
int PlotLSBT = FALSE;
int PlotUSBT = FALSE;
int FreqOn = TRUE;
int TalkOn = FALSE;
int Talk = FALSE;
int LogOn = FALSE;
int AGC_On = FALSE;
int HannOn = FALSE;
int SendingMorse = FALSE;
int DitLength = 1000;
int MorsePosition = 0;
int CurrentIFGain;
int record_voice = 0;
int play_voice = 0;

int hilbert,delay;
int fft_time = 10;

int GetErrCount = 0;
int GetLastErr = 0;
int SendErrCount = 0;
int SendLastErr = 0;
long SampleRate = 8000;
int smeter_on = 0;
int smeter_time = 1;
int NewSetTone = 0;
int mode,SB_DATA = 0;

int *lastInputBuffer = buffer3;
int Monitor;

extern void Adjust_I(int *);
extern int GetError(void);
extern int ChangeIFGain(int);
extern int ChangeIFGain2(int);
extern void Copy_Data_128(int *,int *);
extern void Copy_Data_256(int *,int *);
extern void Copy_Data_256_to_128(int *,int *);
extern void Copy_Data_128_to_256(int *,int *);
extern void Decimate_And_Filter(int *);
extern void Mute_Samples_(int *);
extern void Remove_Noise(int *);
extern void Remove_Noise2(int *);
extern void Remove_Noise3(int *);
extern int Change_I(int);
extern int CheckAnalog();
extern void ClearError(void);
extern void ClearOffset(void);
extern int dofft(void);
extern int dousbfft(void);
extern int dolsbfft(void);
extern int FreqCal(int *);
extern void initSignal(void);
extern int genSignal(void);
extern int HighPass(int *);
extern int Highpass_Both(int *);
extern void initFilter(void);
extern int sFilter(int);
extern int Filter_60_Hz(int *);
extern int Double_Filter_60_Hz(int *);
extern int UFilter(int);
extern void FillSin(int *);
extern int FM_Filter(int);
extern int FM3(int,int);
extern void FM2(int *,int *);
extern int FM_Filter2(int);
extern int LFilter(int);
extern int delayLine(int);
extern int MultiplyByVolume(int);
extern int MultiplyAllByVolume(int *);
extern void ChangeVolume(int);
extern void AGC(int *);
extern void SSBAGC(int *);
extern void InitAGC();
extern void SlowAGC();
extern int GetVolume(void);
extern void Hilbert_And_Delay_(int *,int *);
extern Xmit_LSB_H_And_D(int *,int *);
extern Xmit_USB_H_And_D(int *,int *);
extern void Make_SSB_(int *,int *);
extern void Make_SSB_FM(int *,int *);
extern void Filter_Both(int *);
extern void Filter_Both_And_Copy(int *);
extern void Filter_First(int *);
extern void Remove_USB(int *);
extern int SelectFilter(int);
extern int RestoreVolume();
extern int RTTY_(int,int);
extern int FM_RTTY_(int,int);
extern int RTTY_Block_(int *,int *);
extern int CW_Block_(int *,int *);
extern void SetTone(int);
extern int Multiply(int,int);
extern int Pythagoras(int,int);
extern int FreqCal2(int *,int *);
extern int FreqCal3(int *,int *);
extern int FreqCal4(int *,int *);
extern int gain1,freq1;
extern int gain2,freq2;
extern int gain3,freq3;
extern int Center(int *);
extern void CenterAll(int *);
void ParseMsg(int wParam1, int wParam2, int wParam3)

{
MSG_STRUCT *pMsg;
  int i;
  int wTemp;
  int *pInt;
  long *pLong;
  int count;

   pMsg = (MSG_STRUCT *)wParam2;
   switch (pMsg->hdr.id) {
	case msg_TRANSMIT: TalkOn = 1;
							 SetTone(0);
							 NewIFGain = pMsg->data[3];
/*                      ChangeIFGain2((1+TalkOn)*0x4040+NewIFGain*0x101);
*/                      ChangeVolume(pMsg->data[1]);
							 InitAGC();
							 AGC_On = pMsg->data[2];
							 Monitor = pMsg->data[4];
							 Change_I(pMsg->data[5]);
			  pSpregs = &Registers_1848;
/*		     dspUtilGet1848(pSpregs);
*/			  Registers_1848.wLeftInputControl = (1+TalkOn)*0x4040+NewIFGain*0x101;
			  Registers_1848.wRightInputControl =(1+TalkOn)*0x4040+NewIFGain*0x101;
			  dspUtilProgram1848(pSpregs);
							 CurrentIFGain = NewIFGain;
							 NewIFGain = -1;
							 AnalogAGC = 0;
							 NewSetTone = Monitor;
                      break;


   case msg_RECIEVE:  TalkOn = 0;
                      SetTone(0);
							 NewIFGain = pMsg->data[3];
/*                      ChangeIFGain2((1+TalkOn)*0x4040+NewIFGain*0x101);
*/                      ChangeVolume(pMsg->data[1]);
							 InitAGC();
							 AGC_On = pMsg->data[2];
							 Monitor = pMsg->data[4];
							 Change_I(pMsg->data[5]);
							 pSpregs = &Registers_1848;
/*		     dspUtilGet1848(pSpregs);
*/			  Registers_1848.wLeftInputControl = (1+TalkOn)*0x4040+NewIFGain*0x101;
			  Registers_1848.wRightInputControl =(1+TalkOn)*0x4040+NewIFGain*0x101;
			  dspUtilProgram1848(pSpregs);
							 CurrentIFGain = NewIFGain;
							 NewIFGain = -1;
                      AnalogAGC = 0;
                      NewSetTone = Monitor;
                      break;

   case msg_MORSE_CHAR: wTemp = strlen(Morse);
                   		i = 0;
               			while (wTemp<MAX_MORSE-1 && pMsg->data[i] != 0)
               			  Morse[wTemp++] = pMsg->data[i++];
                        Morse[wTemp] == 0;
               			SendingMorse = 1;
               			break;
	case msg_SQUELCH : squelch = pMsg->data[0];
							 break;
	case msg_MORSE_SPEED : DitLength = pMsg->data[0];
               			  break;
   case msg_MORSE_KEY   : SendingMorse = 2;
               			  gain1 = 2000;       /* Store gain1 value */
               			  freq1 = 1000 * (65536.0/SampleRate);       /* Convert to phase increment needed */
               			  break;
   case msg_START_MORSE : SendingMorse = 1;
								  gain1 = 2000;       /* Store gain1 value */
               			  freq1 = 1000 * (65536.0/SampleRate);       /* Convert to phase increment needed */
               			  Morse[0] = 0;
               			  MorsePosition = 0;
               			  break;
   case msg_STOP_MORSE : SendingMorse = 0;
               			 MorsePosition = 0;
               			 break;

	case msg_MODE: mode = pMsg->data[0];
						if (NewSampleRate == -2 && mode !=4)
						  NewSampleRate = 1;
						else
						  if (mode == 4 && NewSampleRate == -1)
							 NewSampleRate = 2;
						SB_DATA = 0;
						break;

	case msg_FILTER: SelectFilter(pMsg->data[0]);
/*                    if (pMsg->data[0] == 5)
                      NewSampleRate == 2;
                    else
                      NewSampleRate == 1;
*/                    break;

   case msg_IF_GAIN: NewIFGain = pMsg->data[0];
/*                     dspStopIo(hAlg, InputDevice);
			  dspStopIo(hAlg, OutputDevice);
		     pSpregs = &Registers_1848;
		     dspUtilGet1848(pSpregs);
		     Registers_1848.wLeftInputControl = 0x4040+wTemp*0x101;
		     Registers_1848.wRightInputControl = 0x4040+wTemp*0x101;
		     dspUtilProgram1848(pSpregs);
		     dspStartIo(hAlg, InputDevice);
		     dspStartIo(hAlg, OutputDevice);
*/                     break;
   case msg_AGC: AGC_On = pMsg->data[0];
                 InitAGC();
       		     ChangeVolume(pMsg->data[1]);
      		     Monitor = pMsg->data[2];
                 NewSetTone = Monitor;
      		     break;
   case msg_MUTE: /*ChangeVolume(0);
                  InitAGC();
                  */if (Monitor>=0)
                    Monitor = Monitor*-1 -1;
                  NewSetTone = 15;
                  break;
   case msg_VOLUME: ChangeVolume(pMsg->data[0]);
                    InitAGC();
                    play_voice = 0;
                   if (Monitor<0)
                   {
							Monitor = Monitor*-1 -1;
                     NewSetTone = Monitor;
                   }
         		    break;
   case msg_SMETER: smeter_on = pMsg->data[0];
                    break;
   case msg_CHANGE_I :  Change_I(pMsg->data[0]);
                        break;
   case msg_PLOT_LSB_T: PlotLSBT = pMsg->data[0];
			               break;
   case msg_PLOT_USB_T: PlotUSBT = pMsg->data[0];
                			break;
   case msg_PLOT_LSB_S: PlotLSBS = pMsg->data[0];
               			break;
   case msg_PLOT_USB_S: PlotUSBS = pMsg->data[0];
               			break;
   case msg_SET_NOISE: switch(pMsg->data[0])
                       {
                			 case 0: gain1 = pMsg->data[1];
                                 freq1 = pMsg->data[2];
                                 freq1 = freq1 * 65536/SampleRate;       /* Convert to phase increment needed */
                                 break;                                  /* to generate desired frequency */
               			 case 1: gain2 = pMsg->data[1];
                                 freq2 = pMsg->data[2];
                                 freq2 = freq2 * 65536/SampleRate;
                                 break;
								 case 2: gain3 = pMsg->data[1];
                                 freq3 = pMsg->data[2];
                                 freq3 = freq3 * 65536/SampleRate;
                                 break;
                			 case 3: /* no chirp allowed anymore */
                			 case 4: /* no ccitt allowed anymore */
                			 default:
                       }
                       break;
   case msg_PLOT_TOGGLE: PlotOn = pMsg->data[0];
                  		 break;

   case msg_PLOT_SEND: if (FreqOn)
                         pInt = (int *) dofft();
                       else
                 			 pInt = lastInputBuffer;
                       for (wTemp=0; wTemp < SIZE_PLOTMSG; wTemp++)
                  		 plotMsgData[wTemp] = *pInt++;
                       dspSendMsg (hApp, hAlg,msg_PLOT_DATA , SIZE_PLOTMSG, plotMsgData);
                       break;

   case msg_LOG_TOGGLE: LogOn = pMsg->data[0];
                        break;
   case msg_FREQ_TOGGLE: FreqOn = pMsg->data[0];
                         break;
   case msg_LPF_TOGGLE: AGC_On = pMsg->data[0];
								break;
   case msg_HANN_TOGGLE: HannOn = pMsg->data[0];
               			 break;
   case msg_START_RECORD: record_voice = 1;
                          break;
   case msg_STOP_RECORD:  record_voice = 0;
                          break;
   case msg_START_PLAY:   play_voice = 1;
                        if (buffer_ready)
                          {
/*                            dspGetData(hAlg,(int *)data_buffer[0],GETDATASIZE);
*/                            buffer_ready = 0;
                          }
                          buffer_write = 0;
                          buffer_read = 0;
                          if (++current_buffer>3)
                            current_buffer = 0;
                          break;
   case msg_STOP_PLAY:    play_voice = 0;
                          break;
                          break;
   }
}

int offset_agc_toggle = 0;
int request_toggle = 1;
int which;

void dspAlgMain(WORD wCbId, WORD wParam1, WORD wParam2, WORD wParam3)

{
  int *pBuf;
  int wTemp;
  int i,wNumDevs,count,count2,vol;
  int *pInt;
  long center_usb,center_lsb;
  int request_more_data = 0;

  switch(wCbId)
  {
   case DSPCB_GETDATA: Copy_Data_128(play_buffer,(data_buffer[buffer_read])+1);
                       if (++buffer_read>1)
                         buffer_read = 0;
                       play_buffer[0] = 0;
                       buffer_ready = 1;
                       break;

   case DSPCB_INIT:
     hAlg = wParam1;
     hApp = wParam2;
     msg1.hdr.size = SIZE_DATAMSG;
     msg2.hdr.size = SIZE_DATAMSG;

     pSpregs = &Registers_1848;
     dspUtilGet1848(pSpregs);
     Registers_1848.wLeftOutputControl = 0xBFBF;
     Registers_1848.wRightOutputControl = 0xBFBF;
     Registers_1848.wLeftAux1Control = 0x9F9F;
     Registers_1848.wRightAux1Control = 0x9F9F;
     Registers_1848.wLeftAux2Control = 0x9F9F;
     Registers_1848.wRightAux2Control = 0x9F9F;
     dspUtilProgram1848(pSpregs);
     Registers_1848.wInterfaceConfig = (Registers_1848.wInterfaceConfig | 0x0808);
     dspUtilProgram1848(pSpregs);
     dspGetMsg (hAlg, (int *)&msg1, sizeof(MSG_STRUCT));
     dspGetMsg (hAlg, (int *)&msg2, sizeof(MSG_STRUCT));

     initSignal();
     initFilter();
     for(count=0;count<32000;count++)
       {};
     ChangeVolume(256);

     wNumDevs = dspGetNumIoDevs();
     for (i = 0; i < wNumDevs; i++)
     {
       wTemp = dspGetIoDevCaps (i, &devCaps, sizeof(DSPIOCAPS));
       if (wTemp == DSPERR_SUCCESS)
       {
         if (strcmp (devCaps.szDeviceName,"AD1848") == 0)
           if (devCaps.dir == DSPIODIR_IN)
             InputDevice = i;
           else
             if (devCaps.dir == DSPIODIR_OUT)
               OutputDevice = i;
       }
     }
     dspGetIoDevConf(OutputDevice, &devConf, sizeof(DSPIOCONF));
     devConf.mode = DSPIOMODE_STEREO;
     dspSetIoDevConf(OutputDevice, &devConf, sizeof(DSPIOCONF));
     for (wTemp = 0; wTemp < SIZE_IO_BUFFER; wTemp++)
     {
       buffer1[wTemp]=0;
       buffer2[wTemp]=0;
       buffer3[wTemp]=0;
       buffer4[wTemp]=0;
/*       buffer_null[wTemp] = 0;
*/     }
     dspSendIo (hAlg, OutputDevice, buffer1, SIZE_IO_BUFFER);
     dspSendIo (hAlg, OutputDevice, buffer2, SIZE_IO_BUFFER);
     dspGetIo (hAlg, InputDevice, buffer3, SIZE_IO_BUFFER);
     dspGetIo (hAlg, InputDevice, buffer4, SIZE_IO_BUFFER);
     dspStartIo (hAlg, OutputDevice);
     dspStartIo (hAlg, InputDevice);

     SetTone(Monitor);
     gain1 = 2000;       /* Store gain1 value */
     freq1 = 1000;                  /* Store freq1 value */
     freq1 = freq1 * 65536/SampleRate;       /* Convert to phase increment needed */
     pSpregs = &Registers_1848;
     dspUtilGet1848(pSpregs);
     Registers_1848.wLeftInputControl = 0x4F4F+0x4040*TalkOn;
     Registers_1848.wRightInputControl = 0x4F4F+0x4040*TalkOn;
     Registers_1848.wLeftAux1Control = 0x9F9F;
     Registers_1848.wRightAux1Control = 0x9F9F;
     Registers_1848.wLeftAux2Control = 0x9F9F;  
     Registers_1848.wRightAux2Control = 0x9F9F;
     Registers_1848.wClockDataFormat = (Registers_1848.wClockDataFormat & 0xF0F0);
     dspUtilProgram1848(pSpregs);

     dspSendMsg (hApp, hAlg, DSPMSGID_STRING_PRINT, sizeof (STR_INITCOMPLETE), STR_INITCOMPLETE);
     break;

   case DSPCB_CLOSE:
     dspStopIo(hAlg, InputDevice);
     dspStopIo(hAlg, OutputDevice);
     break;

   case DSPCB_SENDMSG: break;

   case DSPCB_GETIO:
     switch (wParam3)
     {
       case (DSPERR_SUCCESS): lastInputBuffer = (int *)wParam2;
			      wTemp = dspGetIo(hAlg, InputDevice, (int *)wParam2, SIZE_IO_BUFFER);
			      if (wTemp != DSPERR_SUCCESS)
			      {
                 GetErrCount++;
                 GetLastErr = wTemp;
			      }
			      break;
       case (DSPERR_NO_BUFFER): algIoOverrun++;
                                break;
     }
     break;

   case DSPCB_SENDIO: switch (wParam3)
	                   {
			case (DSPERR_SUCCESS): if (play_voice)
                                {
                                  play_buffer[0] = 0;
                                  if (dspGetData(hAlg,(int *) data_buffer[buffer_write],GETDATASIZE)==0)
                                    Copy_Data_128(play_buffer,data_buffer[buffer_write]+1);
                                  else
                                    if (++buffer_write >1)
                                      buffer_write = 0;
                                  plotMsgData[0] = play_buffer[0];
                                  plotMsgData[1] = play_buffer[1];
                                  dspSendMsg (hApp, hAlg,msg_REQUEST_PLAY_DATA , 2 , plotMsgData);
                                }
                                if (NewSetTone>=0)
                                {
/*                                  dspStopIo(hAlg, InputDevice);
                                  dspStopIo(hAlg, OutputDevice);
*/                                  SetTone(NewSetTone);
                                  NewSetTone = NewSetTone*-1-1;
/*                                  dspStartIo(hAlg, InputDevice);
                                  dspStartIo(hAlg, OutputDevice);
*/                                }
                                pBuf = (int *)wParam2;
                                if (NewIFGain>=0 && NewIFGain<=15)
					                 {
/*                                  pSpregs = &Registers_1848;
                                  Registers_1848.wLeftInputControl = (1+TalkOn)*0x4040+NewIFGain*0x101);
                                  Registers_1848.wRightInputControl = (1+TalkOn)*0x4040+NewIFGain*0x101);
                                  dspUtilProgram1848(pSpregs);
*/                                  ChangeIFGain2((1+TalkOn)*0x4040+NewIFGain*0x101);
                                  CurrentIFGain = NewIFGain;
		                            NewIFGain = -1;
                                  AnalogAGC = 0;
  		                          }
					                 if (NewSampleRate>0)
					                 {
						                pSpregs = &Registers_1848;
/*						                dspUtilGet1848(pSpregs);
*/						                if (NewSampleRate == 2)
						                  Registers_1848.wClockDataFormat = (Registers_1848.wClockDataFormat & 0xF0F0)+0x0202; /* 0x0404 for 32 KHz 0x0707 for 22.5 KHz*/
                                  else
						                  Registers_1848.wClockDataFormat = (Registers_1848.wClockDataFormat & 0xF0F0);
						                dspUtilProgram1848(pSpregs);
						                NewSampleRate *= -1;
					                 }

					                 if (SendingMorse==1)
					                 {
                                  FillSin(lastInputBuffer);
						                for(wTemp=0;wTemp<256;wTemp+=2)
						                {
/*						                  lastInputBuffer[wTemp] = genSignal();
*/                                  if (Morse[0]<=10)
						                    lastInputBuffer[wTemp] = 0;
						                  lastInputBuffer[wTemp+1] = lastInputBuffer[wTemp];
												if (++MorsePosition>=DitLength)
						                  {
						                    MorsePosition = 0;
                 						     if (Morse[0]>10)
					               	       Morse[0]--;
                                      else
                                        if (Morse[0]<10 && Morse[0] != 0)
                                          Morse[0]++;
                     				     if (Morse[0] == 10)
					               	       DeleteMorse();
                                    }
                                  }
                                }
                                if (SendingMorse == 2)
                                  FillSin(lastInputBuffer);
                                if (TalkOn)
                                {
                                  ClearError();
											 if (record_voice)
											 {
                                    Copy_Data_256(pBuf,lastInputBuffer);
                                    pBuf[0] = lastInputBuffer[0];
/*                                    MultiplyAllByVolume(pBuf);
*//*                                  Double_Filter_60_Hz(pBuf);
/*                                  Filter_First(pBuf);
*/                                    Copy_Data_256_to_128(plotMsgData+4,pBuf); /*lastInputBuffer);*/
                                    dspSendData(hAlg,plotMsgData,134);
                                  }
                                  else
                                  {
/*                                    AGC(lastInputBuffer);
                                    ClearOffset();
                                    if (!AGC_On)
               						     RestoreVolume();
*/                                    switch(mode)
                                    {
                                      case 0 : /*for(wTemp=0; wTemp<256; wTemp+=2)
                                               {
                                                 hilbert = lastInputBuffer[wTemp];
                                                 delay = lastInputBuffer[wTemp];
                                                 pBuf[wTemp] = hilbert;
                                                 pBuf[wTemp+1] = delay;
                                               } */
                                               Copy_Data_256(pBuf,lastInputBuffer);
                                               pBuf[0] = lastInputBuffer[0];
                                               if (!SendingMorse)
                                               {
                                                 Filter_60_Hz(pBuf);
                                                 if (AGC_On)
                                                   AGC(pBuf);
															  }
															  else
																 Filter_Both(pBuf);
															  MultiplyAllByVolume(pBuf);
															  break;
												  case 2 : for(wTemp=0; wTemp<256; wTemp+=2)
															  {
																 pBuf[wTemp+1] = sFilter(lastInputBuffer[wTemp]);
																 pBuf[wTemp] = delayLine(lastInputBuffer[wTemp]);
															  }
/*                                               Xmit_USB_H_And_D(lastInputBuffer,pBuf);
*/                                               if (!SendingMorse)
															  {
																 Filter_60_Hz(pBuf);
																 if (AGC_On)
																	AGC(pBuf);
															  }
 /*															  else
 */																 Filter_Both(pBuf);
															  MultiplyAllByVolume(pBuf);
															  Adjust_I(pBuf);
															  break;
												  case 1 : for(wTemp=0; wTemp<256; wTemp+=2)
															  {
																 pBuf[wTemp] = sFilter(lastInputBuffer[wTemp]);
																 pBuf[wTemp+1] = delayLine(lastInputBuffer[wTemp]);
															  }
/*                                               Xmit_LSB_H_And_D(lastInputBuffer,pBuf);
*/                                               if (!SendingMorse)
															  {
																 Filter_60_Hz(pBuf);
																 if (AGC_On)
																	AGC(pBuf);
															  }
 /*															  else
 */																 Filter_Both(pBuf);
															  MultiplyAllByVolume(pBuf);
															  Adjust_I(pBuf);
															  break;
												  case 3 : for(wTemp=0; wTemp<256; wTemp+=2)
															  {
																 hilbert = lastInputBuffer[wTemp];
																 delay = lastInputBuffer[wTemp];
																 pBuf[wTemp] = hilbert;
																 pBuf[wTemp+1] = delay;
															  }
															  if (!SendingMorse)
															  {
																 Filter_60_Hz(pBuf);
																 if (AGC_On)
																	AGC(pBuf);
															  }
 /*															  else
 */																 Filter_Both(pBuf);
															  MultiplyAllByVolume(pBuf);
															  Adjust_I(pBuf);
															  break;
												  case 4 : for(wTemp=0; wTemp<256; wTemp+=2)
															  {
																 hilbert = lastInputBuffer[wTemp];
																 delay = lastInputBuffer[wTemp];
																 pBuf[wTemp] = hilbert;
																 pBuf[wTemp+1] = delay;
															  }
/*															  else
  */																 Filter_60_Hz(pBuf);
															  if (AGC_On)
																 AGC(pBuf);
															  MultiplyAllByVolume(pBuf);
															  break;
												}
											 }
										  }
										  else
										  {
                                  if (play_voice)
                                  {
                                    Copy_Data_128_to_256(lastInputBuffer,play_buffer);
/*                                    buffer_ready = 0;
                                    if (dspGetData(hAlg,(int *) data_buffer[0],128)==0)
                                    {
                                      Copy_Data_128(play_buffer,data_buffer[0]);
                                      buffer_ready = 1;
                                    }
                                    dspSendMsg (hApp, hAlg,msg_REQUEST_PLAY_DATA , 0 , plotMsgData);
*/                                  }
                                  if (PlotLSBT && SB_DATA == 0 && !record_voice && !play_voice)
											 {
												if (fft_time==2)
												{
												  pInt = (int *) LSB_BUFFER;
												  for (wTemp=0; wTemp < 256; wTemp++)
													 plotMsgData[wTemp]  = *pInt++;
												  dspSendMsg (hApp, hAlg,103 , SIZE_PLOTMSG, plotMsgData);
												}
											 }
											 ClearError();
											 count = SB_DATA;
											 switch(mode)
											 {
												case 0 : /*Copy_Data_256(pBuf,lastInputBuffer);
                                             */Hilbert_And_Delay(lastInputBuffer,pBuf);
															Make_SSB(pBuf,&LSB_BUFFER[count]);
															Make_SSB(pBuf+1,&USB_BUFFER[count]);
															Filter_Both(pBuf);
															Remove_USB(pBuf);
															Which_AGC(pBuf);
															MultiplyAllByVolume(pBuf);
															Filter_60_Hz(pBuf);
															count +=128;
															plotMsgData[0] = Center(USB_BUFFER)/2-Center(LSB_BUFFER)/2;
															if (!play_voice && !record_voice)
															  dspSendMsg (hApp, hAlg,msg_FREQ_CAL, SIZE_PLOTMSG, plotMsgData);
/*															AnalogAGC = 0;
*/															break;
												case 10 : /*Copy_Data_256(pBuf,lastInputBuffer);
															*/Hilbert_And_Delay(lastInputBuffer,pBuf);
															Make_SSB(pBuf,&LSB_BUFFER[count]);
															Make_SSB(pBuf+1,&USB_BUFFER[count]);
															Filter_Both(pBuf);
															Remove_LSB(pBuf);
															Which_AGC(pBuf);
															MultiplyAllByVolume(pBuf);
															Filter_60_Hz(pBuf);
															count +=128;
															plotMsgData[0] = Center(USB_BUFFER)/2-Center(LSB_BUFFER)/2;
															if (!play_voice && !record_voice)
															  dspSendMsg (hApp, hAlg,msg_FREQ_CAL, SIZE_PLOTMSG, plotMsgData);
/*															AnalogAGC = 0;
*/															break;
												case 2 : Adjust_I(lastInputBuffer);
															Hilbert_And_Delay(lastInputBuffer,pBuf);
															Filter_Both(pBuf);
															Make_SSB(lastInputBuffer,&LSB_BUFFER[count]);
															Make_SSB(lastInputBuffer+1,&USB_BUFFER[count]);
															Remove_USB(pBuf);
															Filter_60_Hz(pBuf);
															Which_AGC(pBuf);
															MultiplyAllByVolume(pBuf);
/*                                             CW_Block(&USB_BUFFER[count],plotMsgData);
*/                                             count +=128;
															break;
												case 1 : Adjust_I(lastInputBuffer);
                                             Hilbert_And_Delay(lastInputBuffer,pBuf);
															Filter_Both(pBuf);
															Make_SSB(pBuf,&LSB_BUFFER[count]);
															Make_SSB(pBuf+1,&USB_BUFFER[count]);
															Remove_LSB(pBuf);
															Filter_60_Hz(pBuf);
															Which_AGC(pBuf);
															MultiplyAllByVolume(pBuf);
															count +=128;
															break;
												case 3 : Adjust_I(lastInputBuffer);
															Hilbert_And_Delay(lastInputBuffer,pBuf);
															Which_AGC(pBuf);
															MultiplyAllByVolume(pBuf);
															Filter_Both(pBuf);
															Make_SSB(pBuf,&LSB_BUFFER[count]);
															Make_SSB(pBuf+1,&USB_BUFFER[count]);
															Filter_60_Hz(pBuf);
															count +=128;
															break;
												case 4 : Copy_Data_256(Temp_Buffer,lastInputBuffer);
															AGC(Temp_Buffer);
															plotMsgData[0]=GetVolume();
															plotMsgData[1]=0;
															if (--smeter_time == 0)
															{
															  smeter_time = 8;
															  dspSendMsg(hApp,hAlg,MSG_SMETER_LEVEL,3,plotMsgData);
															}
															vol = GetVolume()+32768;
															RestoreVolume();
															if (vol<squelch)
															  Mute_Samples(pBuf);
															else
															{
/*                                             Adjust_I(Temp_Buffer);
*/                                             Highpass_Both(Temp_Buffer);
															  FM2(Temp_Buffer,pBuf);
															  MultiplyAllByVolume(pBuf);
/*                                             Filter_Both_And_Copy(pBuf);
*/	                                            Decimate_And_Filter(pBuf);
															  Make_SSB_FM(pBuf+1,&LSB_BUFFER[count]);
															}
															count +=128;
															break;
												case 6 : Copy_Data_256(Temp_Buffer,lastInputBuffer);
															AGC(Temp_Buffer);
															plotMsgData[0]=GetVolume();
															plotMsgData[1]=0;
															if (--smeter_time == 0)
															{
															  smeter_time = 4;
															  dspSendMsg(hApp,hAlg,MSG_SMETER_LEVEL,3,plotMsgData);
															}
															vol = GetVolume()+32768;
															RestoreVolume();
															if (vol<squelch)
															{
                                               Mute_Samples(pBuf);
															  Make_SSB(pBuf,&USB_BUFFER[count]);
															  Make_SSB(pBuf+1,&LSB_BUFFER[count]);
															}
															else
															{
															  Highpass_Both(Temp_Buffer);
															  FM2(Temp_Buffer,pBuf);
															  MultiplyAllByVolume(pBuf);
                                               FM_RTTY(pBuf);
                                               dspSendMsg(hApp,hAlg,msg_RTTY,
                                                          Convert_RTTY_To_Bits(pBuf+1,plotMsgData),
                                                          plotMsgData);
															  Make_SSB(pBuf+1,&LSB_BUFFER[count]);
															}
															count +=128;
															break;
												case 7 : Copy_Data_256(Temp_Buffer,lastInputBuffer);
															AGC(Temp_Buffer);
															plotMsgData[0]=GetVolume();
															plotMsgData[1]=0;
															if (--smeter_time == 0)
															{
															  smeter_time = 4;
															  dspSendMsg(hApp,hAlg,MSG_SMETER_LEVEL,3,plotMsgData);
															}
															vol = GetVolume()+32768;
															RestoreVolume();
															if (vol<squelch)
															{
                                               Mute_Samples(pBuf);
															  Make_SSB(pBuf,&USB_BUFFER[count]);
															  Make_SSB(pBuf+1,&LSB_BUFFER[count]);
															}
															else
															{
															  Highpass_Both(Temp_Buffer);
															  PSK(Temp_Buffer,pBuf);
															  MultiplyAllByVolume(pBuf);
                                               FM_RTTY(pBuf);
															  Make_SSB(pBuf+1,&LSB_BUFFER[count]);
															}
															count +=128;
															break;
											 }
											 if (record_voice)
											 {
												int which = 0;
/*												for (wTemp=0; wTemp < 128; wTemp++)
												{
												  plotMsgData[wTemp] = lastInputBuffer[which];
												  which +=2;
												}
*/                                    for(wTemp=0;wTemp<128;wTemp++);
												  plotMsgData[wTemp] = wTemp;
												dspSendMsg(hApp,hAlg,msg_VOICE_DATA,128,plotMsgData);
											 }
											 else
											 {
												if (!play_voice)
												  if ((mode !=4 && SB_DATA == 128) || (mode ==4 && SB_DATA == 192))
												  {
													 if (fft_time==3 && PlotUSBT)
													 {
														pInt = (int *) USB_BUFFER;
														for (wTemp=0; wTemp < 256; wTemp++)
														  plotMsgData[wTemp]  = *pInt++;
														dspSendMsg (hApp, hAlg,104 , SIZE_PLOTMSG, plotMsgData);
													 }
													 if (fft_time == 2 && PlotUSBS)
													 {
														pInt = (int *) dousbfft();
														for (wTemp=0; wTemp < SIZE_PLOTMSG; wTemp++)
														  plotMsgData[wTemp]  = *pInt++;
														dspSendMsg (hApp, hAlg,101 , SIZE_PLOTMSG, plotMsgData);
													 }
													 if (--fft_time==0)
													 {
														fft_time = 26; /*13*/
														if (PlotLSBS)
														{
														  pInt = (int *) dolsbfft();
														  for (wTemp=0; wTemp < SIZE_PLOTMSG; wTemp++)
															 plotMsgData[wTemp]  = *pInt++;
														  dspSendMsg (hApp, hAlg,102 , SIZE_PLOTMSG, plotMsgData);
														}
													 }
												}
												if (mode != 4)
												  SB_DATA = 128 - SB_DATA;
												else
												  if ((SB_DATA+=64)>192)
													 SB_DATA = 0;
											 }
										  }
/*                    if (GetError()!=0)
						 wTemp = dspSendIo(hAlg, OutputDevice, buffer_null, SIZE_IO_BUFFER);
							 else
*/                      wTemp = dspSendIo(hAlg, OutputDevice,pBuf, SIZE_IO_BUFFER);
							 plotMsgData[2] = 0;
							 if (AGC_On != 0 && !TalkOn/* && mode != 0 && mode !=10*/)
							 {
								if (AnalogAGC<-1)
								  if (CurrentIFGain == 0)
									 plotMsgData[2] = -1;
                          else
                            NewIFGain=CurrentIFGain-1;
                        if (AnalogAGC>64)
                          if (CurrentIFGain == 15)
                            plotMsgData[2] = 1;
                          else
									 NewIFGain=CurrentIFGain+1;
                      }
							 if ((NewIFGain>=0 || plotMsgData[2]!=0)/* && mode != 0 && mode !=10*/)
                      {
                        plotMsgData[0] = NewIFGain;
                        plotMsgData[1] = GetVolume();
                        AnalogAGC = 0;
								dspSendMsg (hApp, hAlg,MSG_AGC_ANALOG , SIZE_PLOTMSG, plotMsgData);
  					       }
                      if (wTemp != DSPERR_SUCCESS)
					       {
						      SendErrCount++;
						      SendLastErr = wTemp;
							 }
					       break;
			case (DSPERR_IO_ABORT): break;
			case (DSPERR_NO_BUFFER): algIoOverrun++;
						 break;
		      }
		      break;
   case DSPCB_GETMSG:   if (wParam1 == DSPERR_SUCCESS)
			{
			  ParseMsg(wParam1, wParam2, wParam3);
			  if (dspGetMsg (hAlg, (int *)wParam2, sizeof(MSG_STRUCT)) != 0)
			    dspSendMsg (hApp, hAlg, DSPMSGID_STRING_PRINT, sizeof (STR_GETMSGREQUEUEERROR), STR_GETMSGREQUEUEERROR);
			}
			break;
   }
	if (request_more_data)
   {
     Copy_Data_128_to_256(lastInputBuffer,play_buffer);
     buffer_ready = 0;
     if (dspGetData(hAlg,(int *) data_buffer[0],128)==0)
	  {
       Copy_Data_128(play_buffer,data_buffer[0]);
       buffer_ready = 1;
     }
     dspSendMsg (hApp, hAlg,msg_REQUEST_PLAY_DATA , 0 , plotMsgData);
   }
}

int dit = 13;

Which_AGC(int *Buffer)

{
  int vol;

  if (AGC_On==2)
  {
	 AGC(Buffer);
	 SlowAGC();
	 plotMsgData[0]=GetVolume();
	 plotMsgData[1]=0;
	 vol = GetVolume()+32768;
	 if (vol<-31500)
		AnalogAGC+=1;
	 else
		if (vol>-10000)
        AnalogAGC-=1;
  }
  else
  {
	 AGC(Buffer);
	 plotMsgData[0]=GetVolume();
	 plotMsgData[1]=0;
	 if (AGC_On == 0)
		RestoreVolume();
	 else
	 {
		vol = GetVolume()+32768;
		if (vol<-31500)
		  AnalogAGC+=1;
		else
		  if (vol>-10000)
			 AnalogAGC-=1;
		ClearOffset();
	 }
  }
  if (/*smeter_on && */!TalkOn && !record_voice && !play_voice && mode != 4)
	 if (--smeter_time == 0)
	 {
		smeter_time = 4;
		dspSendMsg(hApp,hAlg,MSG_SMETER_LEVEL,3,plotMsgData);
	 }
}

DeleteMorse()

{
  int count;

  count = 0;
  for(count=0;count<MAX_MORSE-1;count++)
    Morse[count] = Morse[count+1];
  Morse[MAX_MORSE-1] = 0;
  if (Morse[0]==0)
  {
    dspSendMsg (hApp, hAlg, MSG_MORSE_END_MORSE,256,"END MORSE");
    SendingMorse = 0;
  }
  else
    if (Morse[1]==0)
      dspSendMsg (hApp, hAlg, MSG_MORSE_NEXT_CHAR,256,"NEXT CHAR");
}


