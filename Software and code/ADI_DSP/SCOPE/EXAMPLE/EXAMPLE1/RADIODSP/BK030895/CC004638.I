# 1 "alg.c"
# 1 "d:\adi_dsp\scope\include\dspshell.h" 1
 













 

# 1 "d:\adi_dsp\scope\include\dspsdk.h" 1
 











 














































 

typedef struct {           
  int to;
  int from;
  int id;
  int size;
}DSPMSGHEADER;


 

 












 

typedef struct dspmsg_file_fopenreq {
  char FileName[100 ];
  char FileMode[10 ];
} DSPMSGFOPENREQ;

typedef struct dspmsg_file_fclosereq {
  short hFile;
} DSPMSGFCLOSEREQ;

typedef struct dspmsg_file_freadreq {
  short hFile;
  short nWordsToRead;
} DSPMSGFREADREQ;

typedef struct dspmsg_file_fwritereq {
  short hFile;
  short nWordsToWrite;
  short data[200 ];
} DSPMSGFWRITEREQ;


 

typedef struct dspmsg_file_fopenack {

  DSPMSGHEADER hdr;

  short hFile;
} DSPMSGFOPENACK;

typedef struct dspmsg_file_fcloseack {

  DSPMSGHEADER hdr;

  short nError;
} DSPMSGFCLOSEACK;

typedef struct dspmsg_file_freadack {

  DSPMSGHEADER hdr;

  short hFile;
  short nWordsRead;
  short data[200 ];
} DSPMSGFREADACK;

typedef struct dspmsg_file_fwriteack {

  DSPMSGHEADER hdr;

  short hFile;
  short nWordsWritten;
} DSPMSGFWRITEACK;

 





 
# 17 "d:\adi_dsp\scope\include\dspshell.h" 2



 














typedef enum
{
  DSPCB_INIT,
  DSPCB_CLOSE,
  DSPCB_GETIO,
  DSPCB_SENDIO,
  DSPCB_GETDATA,
  DSPCB_SENDDATA,
  DSPCB_GETMSG,
  DSPCB_SENDMSG,
  DSPCB_TIMER,
  DSPCB_IDLE,
  DSPCB_ERROR,
  DSPCB_DROPIT,
  MSG_END
}DSPCB;

typedef enum
{
  DSPERR_SUCCESS,
  DSPERR_HANDLE,
  DSPERR_IO_DEVICE,
  DSPERR_IO_CONF,
  DSPERR_IO_ABORT,
  DSPERR_NO_BUFFER,
  DSPERR_ARG,
  DSPERR_SIZE,
  DSPERR_QUEUE_FULL,
  DSPERR_FULL
}DSPERR;


typedef enum
{
  DSPIODIR_IN,
  DSPIODIR_OUT
}DSPIODIR;

typedef enum
{
  DSPIOMODE_STEREO,
  DSPIOMODE_MONO,
  DSPIOMODE_MAX
}DSPIOMODE;

typedef enum
{
  DSPIODT_LINEAR,
  DSPIODT_ULAW,
  DSPIODT_ALAW,
  DSPIODT_MAX
}DSPIODT;



 


typedef struct
{
  int wNum;
  DSPIODT types[DSPIODT_MAX];
}DSPIODTS;



typedef struct
{
  int wNum;
  long rate[23 ];
}DSPIORATES;

typedef struct
{
  int wNum;
  DSPIOMODE modes[DSPIOMODE_MAX];
}DSPIOMODES;

typedef struct
{
  int wNum;
  int wBits[2];
}DSPIOSIZES;

typedef struct
{
  char szDeviceName[20];
  DSPIOMODES io;
  DSPIODTS data;
  DSPIODIR dir;
  DSPIORATES sample;
  DSPIOSIZES word;
}DSPIOCAPS;

typedef struct
{
  DSPIOMODE mode;
  DSPIODT dataType;
  long rate;
  int size;
}DSPIOCONF;

typedef struct
{
  char szProcName[15];
  long dwSpeed;
  int wMemory;
}DSPSYSCAPS;

 

extern void dspAlgMain( unsigned int  wCbId,  unsigned int  wArg1,  unsigned int  wArg2,  unsigned int  wArg3);



 

extern  unsigned int  dspGetData(  unsigned int   hAlg,   int *  pData,  unsigned int  wSize);
extern  unsigned int  dspGetIo(  unsigned int   hAlg,   unsigned int   hDev,   int *  pData,  unsigned int  wSize);
extern  unsigned int  dspGetIoDevCaps(  unsigned int   hDev, DSPIOCAPS *pCaps,  unsigned int  wSize);
extern  unsigned int  dspGetIoDevConf(  unsigned int   hDev, DSPIOCONF *pConf,  unsigned int  wSize);
extern  unsigned int  dspGetMsg(  unsigned int   hAlg,   int *  pMsg,  unsigned int  wSize);
extern  unsigned int  dspGetNumIoDevs(void);
extern  unsigned int  dspGetSysCaps(DSPSYSCAPS *pCaps,  unsigned int  wSize);
extern  unsigned int  dspReleaseCheck(void);
extern  unsigned int  dspReleaseToShell(void);
extern  unsigned int  dspSendData(  unsigned int   hAlg,   int *  pData,  unsigned int  wSize);
extern  unsigned int  dspSendIo(  unsigned int   hAlg,   unsigned int   hDev,   int *  pData,  unsigned int  wSize);
extern  unsigned int  dspSendMsg(  unsigned int   hApp,   unsigned int   hAlg,  unsigned int  wId,  unsigned int  wSize,   int *  pMsg);
extern  unsigned int  dspSetIoDevConf(  unsigned int   hDev, DSPIOCONF *pConf,  unsigned int  wSize);
extern  unsigned int  dspStartIo(  unsigned int   hAlg,   unsigned int   hDev);
extern  unsigned int  dspStartTimer(  unsigned int   hAlg,   unsigned int   bScale,  unsigned int  wCount);
extern  unsigned int  dspStopIo(  unsigned int   hAlg,   unsigned int   hDev);
extern  unsigned int  dspStopTimer(  unsigned int   hAlg);




# 1 "alg.c" 2

# 1 "d:\adi_dsp\scope\include\dsp1848.h" 1
 











 
typedef struct soundportregs
{
  int wLeftInputControl;
  int wRightInputControl;
  int wLeftAux1Control;
  int wRightAux1Control;
  int wLeftAux2Control;
  int wRightAux2Control;
  int wLeftOutputControl;
  int wRightOutputControl;
  int wClockDataFormat;
  int wInterfaceConfig;
  int wPinControl;
  int wTestInitialization;
  int wMiscellaneous;
  int wDigitalMix;
  int wUpperBaseCount;
  int wLowerBaseCount;
}SOUNDPORTREGS;

 

extern void dspUtilProgram1848 (SOUNDPORTREGS *pSpregs);
extern void dspUtilGet1848 (SOUNDPORTREGS *pSpregs);




# 2 "alg.c" 2


















        



























        











typedef struct msg
{
  DSPMSGHEADER hdr;
  int data[  50 ];
}MSG_STRUCT;

MSG_STRUCT msg1,msg2;

int plotMsgData[  128 ];

DSPIOCONF devConf;
DSPIOCAPS devCaps;

char Morse[100 ]= {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int NewIFGain = -1;
int NewSampleRate = 1;
 
SOUNDPORTREGS Registers_1848,*pSpregs;
int USB_BUFFER[256];
int LSB_BUFFER[256];
int RTTY_BUFFER[256];
int USB_SPARE_BUFFER[128];
int LSB_SPARE_BUFFER[128];
int buffer1[256 ];                   
int buffer2[256 ];
int buffer3[256 ];
int buffer4[256 ];
 
int *USB = USB_BUFFER;
int *LSB = LSB_BUFFER;

int InputDevice = 0;
int OutputDevice = 1;
int hAlg;                        
int hApp;                        
int algIoOverrun = 0;
int AnalogAGC = 0;
int PlotOn = 0 ;
int PlotLSBS = 0 ;
int PlotUSBS = 0 ;
int PlotLSBT = 0 ;
int PlotUSBT = 0 ;
int FreqOn = 1 ;
int TalkOn = 0 ;
int Talk = 0 ;
int LogOn = 0 ;
int LpfOn = 0 ;
int HannOn = 0 ;
int SendingMorse = 0 ;
int DitLength = 1000;
int MorsePosition = 0;
int CurrentIFGain;

int hilbert,delay;
int fft_time = 10;

int GetErrCount = 0;
int GetLastErr = 0;
int SendErrCount = 0;
int SendLastErr = 0;
long SampleRate = 8000;
int smeter_on = 0;
int smeter_time = 1;

int mode,SB_DATA = 0;

int *lastInputBuffer = buffer3;
int Monitor;

extern int GetError(void);
extern int ChangeIFGain(int);
extern int CheckAnalog();
extern void ClearError(void);
extern void ClearOffset(void);
extern int dofft(void);
extern int dousbfft(void);
extern int dolsbfft(void);
extern int FreqCal(int *);
extern void initSignal(void);
extern int genSignal(void);
extern void initFilter(void);
extern int sFilter(int);
extern int Filter_60_Hz(int);
extern int UFilter(int);
extern int LFilter(int);
extern int delayLine(int);
extern int MultiplyByVolume(int);
extern void ChangeVolume(int);
extern void AGC(int *);
extern void InitAGC();
extern void SlowAGC();
extern int GetVolume(void);
extern int SelectFilter(int);
extern int RestoreVolume();
extern int RTTY_(int,int);
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
void ParseMsg(int wParam1, int wParam2, int wParam3)

{
MSG_STRUCT *pMsg;
  int i;
  int wTemp;
  int *pInt;
  long *pLong;


   pMsg = (MSG_STRUCT *)wParam2;      
   switch (pMsg->hdr.id) {
   case  1501 : wTemp = strlen(Morse);
                   		i = 0;
               			while (wTemp<100 -1 && pMsg->data[i] != 0)
               			  Morse[wTemp++] = pMsg->data[i++];
                                Morse[wTemp] == 0;
               			SendingMorse = 1;
               			break;
   case 1516  : DitLength = pMsg->data[0];
               			  break;
   case 1502  : SendingMorse = 1;
               			  gain1 = 2000;        
               			  freq1 = 1000 * (65536.0/SampleRate);        
               			  Morse[0] = 0;
               			  MorsePosition = 0;
               			  break;
   case  1503  : SendingMorse = 0;
               			 MorsePosition = 0;
               			 break;

   case        1506 : mode = pMsg->data[0];
                  if (NewSampleRate == -2 && mode > 1)
                    NewSampleRate += 3;
                  break;

   case    1505 : 
   case     1504 :  ChangeVolume(pMsg->data[1]);
                      TalkOn = pMsg->data[0];
                      LpfOn = pMsg->data[2];
                      NewIFGain = pMsg->data[3];
                      Monitor = pMsg->data[4];
                      SetTone(Monitor);
                      break;

   case      1507 : SelectFilter(pMsg->data[0]);
                    if (pMsg->data[0] == 5)
                      NewSampleRate == 2;
                    else
                      NewSampleRate == 1;
                    break;

   case     1508 : NewIFGain = pMsg->data[0];
 








                     break;
   case         1509 : LpfOn = pMsg->data[0];
                 InitAGC();
       		     ChangeVolume(pMsg->data[1]);
      		     Monitor = pMsg->data[2];
                     SetTone(Monitor);
      		     break;
   case        1515 : ChangeVolume(0);
                  SetTone(0);
                  break;
   case      1510 : ChangeVolume(pMsg->data[0]);
            	    SetTone(Monitor);
         		    break;
   case      1517 : smeter_on = pMsg->data[0];
                    break;
   case  1513 : PlotLSBT = pMsg->data[0];
			               break;
   case  1514 : PlotUSBT = pMsg->data[0];
                			break;
   case  1511 : PlotLSBS = pMsg->data[0];
               			break;
   case  1512 : PlotUSBS = pMsg->data[0];
               			break;
   case   500 : switch(pMsg->data[0])
                       {
                			 case 0: gain1 = pMsg->data[1];
                                 freq1 = pMsg->data[2];
                                 freq1 = freq1 * 65536/SampleRate;        
                                 break;                                   
               			 case 1: gain2 = pMsg->data[1];
                                 freq2 = pMsg->data[2];
                                 freq2 = freq2 * 65536/SampleRate;
                                 break;
               			 case 2: gain3 = pMsg->data[1];
                                 freq3 = pMsg->data[2];
                                 freq3 = freq3 * 65536/SampleRate;
                                 break;
                			 case 3:  
                			 case 4:  
                			 default:
                       }
                       break;
   case 502 : PlotOn = pMsg->data[0];
                  		 break;

   case   509 : if (FreqOn)
                         pInt = (int *) dofft();
                       else
                 			 pInt = lastInputBuffer;
                       for (wTemp=0; wTemp <   128 ; wTemp++)
                  		 plotMsgData[wTemp] = *pInt++;
                       dspSendMsg (hApp, hAlg,    500  ,   128 , plotMsgData);
                       break;

   case  505 : LogOn = pMsg->data[0];
                        break;
   case 506 : FreqOn = pMsg->data[0];
                         break;
   case  507 : LpfOn = pMsg->data[0];
               			break;
   case 508 : HannOn = pMsg->data[0];
               			 break;
   }
}

void dspAlgMain( unsigned int  wCbId,  unsigned int  wParam1,  unsigned int  wParam2,  unsigned int  wParam3)

{
  int *pBuf;
  int wTemp;
  int i,wNumDevs,count,count2;
  int *pInt;
  int vol;
  long center_usb,center_lsb;
  switch(wCbId)
  {

   case DSPCB_INIT:
     hAlg = wParam1;
     hApp = wParam2;
     msg1.hdr.size =   50 ;
     msg2.hdr.size =   50 ;

     dspGetMsg (hAlg, (int *)&msg1, sizeof(MSG_STRUCT));
     dspGetMsg (hAlg, (int *)&msg2, sizeof(MSG_STRUCT));

     initSignal();
     initFilter();
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
     for (wTemp = 0; wTemp < 256 ; wTemp++)
     {
       buffer1[wTemp]=0;
       buffer2[wTemp]=0;
       buffer3[wTemp]=0;
       buffer4[wTemp]=0;
 
     }
     dspSendIo (hAlg, OutputDevice, buffer1, 256 );
     dspSendIo (hAlg, OutputDevice, buffer2, 256 );
     dspGetIo (hAlg, InputDevice, buffer3, 256 );
     dspGetIo (hAlg, InputDevice, buffer4, 256 );
     dspStartIo (hAlg, OutputDevice);
     dspStartIo (hAlg, InputDevice);

     SetTone(Monitor);
     gain1 = 2000;        
     freq1 = 1000;                   
     freq1 = freq1 * 65536/SampleRate;        
     pSpregs = &Registers_1848;
     dspUtilGet1848(pSpregs);
     Registers_1848.wLeftInputControl = 0x4F4F+0x4040*TalkOn;
     Registers_1848.wRightInputControl = 0x4F4F+0x4040*TalkOn;
     Registers_1848.wLeftAux1Control = 0x9F9F;
     Registers_1848.wRightAux1Control = 0x9F9F;
     Registers_1848.wLeftAux2Control = 0x9F9F;   
     Registers_1848.wRightAux2Control = 0x9F9F;   
     Registers_1848.wClockDataFormat = (Registers_1848.wClockDataFormat & 0xFDFD);
     dspUtilProgram1848(pSpregs);

     dspSendMsg (hApp, hAlg,           (                (-400)  - 0) , sizeof (             "initialized" ),              "initialized" );
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
			      wTemp = dspGetIo(hAlg, InputDevice, (int *)wParam2, 256 );
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
			case (DSPERR_SUCCESS): pBuf = (int *)wParam2;
                                if (NewIFGain>=0 && NewIFGain<=15)
					                 {
                                  ChangeIFGain((1+TalkOn)*0x4040+NewIFGain*0x101);
                                  CurrentIFGain = NewIFGain;
		                            NewIFGain = -1;
                                  AnalogAGC = 0;
  		                          }
					                 if (NewSampleRate>0)
					                 {
						                pSpregs = &Registers_1848;
						                dspUtilGet1848(pSpregs);
						                if (NewSampleRate == 2)
						                  Registers_1848.wClockDataFormat = (Registers_1848.wClockDataFormat & 0xFDFD)+0x0202;
                                  else
						                  Registers_1848.wClockDataFormat = (Registers_1848.wClockDataFormat & 0xFDFD);
						                dspUtilProgram1848(pSpregs);
						                NewSampleRate *= -1;
					                 }
					                 if (SendingMorse==1)
					                 {
						                for(wTemp=0;wTemp<256;wTemp+=2)
						                {
						                  lastInputBuffer[wTemp] = genSignal();
						                  if (Morse[0]<=10)
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
					                 if (SendingMorse==2)
					                 {
						                for(wTemp=0;wTemp<256;wTemp+=2)
						                {
						                  lastInputBuffer[wTemp] = genSignal();
						                  lastInputBuffer[wTemp+1] = lastInputBuffer[wTemp];
                                  }
                                }
                                if (TalkOn)
                                {
                                  ClearError();
                                  AGC(lastInputBuffer);
                                  if (!LpfOn)
               						   RestoreVolume();
                                  switch(mode)
                                  {
                                    case 0 : for(wTemp=0; wTemp<256; wTemp+=2)
                                             {
                                               hilbert = lastInputBuffer[wTemp];
                                               delay = lastInputBuffer[wTemp];
                                               pBuf[wTemp] = MultiplyByVolume((hilbert));
                                               pBuf[wTemp+1] = MultiplyByVolume((delay));
                                             }
                                             break;
 








                                    case 1 : for(wTemp=0; wTemp<256; wTemp+=2)
                                             {
                                               pBuf[wTemp+1] = sFilter(MultiplyByVolume(lastInputBuffer[wTemp]));
                                               pBuf[wTemp] = delayLine(MultiplyByVolume(lastInputBuffer[wTemp]));
                                             }
                                             break;
                                    case 2 : for(wTemp=0; wTemp<256; wTemp+=2)
                                             {
                                               pBuf[wTemp] = sFilter(MultiplyByVolume(lastInputBuffer[wTemp]));
                                               pBuf[wTemp+1] = delayLine(MultiplyByVolume(lastInputBuffer[wTemp]));
                                             }
                                             break;
                                    case 3 : for(wTemp=0; wTemp<256; wTemp+=2)
                                             {
                                               hilbert = lastInputBuffer[wTemp];
                                               delay = lastInputBuffer[wTemp];
                                               pBuf[wTemp] = MultiplyByVolume((hilbert));
                                               pBuf[wTemp+1] = MultiplyByVolume((delay));
 




                                             }
                                             break;
                        			 }
					                 }
					                 else
					                 {
						                if (PlotLSBT && SB_DATA == 0)
						                {
						                  if (fft_time==2)
						                  {
						                    pInt = (int *) LSB_BUFFER;
						                    for (wTemp=0; wTemp < 256; wTemp++)
						                      plotMsgData[wTemp]  = *pInt++;
						                    dspSendMsg (hApp, hAlg,103 ,   128 , plotMsgData);
						                  }
						                }
						                ClearError();
 						                if (LpfOn==2)
                                  {
                                    AGC(lastInputBuffer);
                                    SlowAGC();
                                    plotMsgData[0]=GetVolume();
                                    plotMsgData[1]=0;
                                  }
                                  else
                                  {
                                    AGC(lastInputBuffer);
                                    plotMsgData[0]=GetVolume();
                                    plotMsgData[1]=0;
                                    if (LpfOn == 0)
                                      RestoreVolume();
                                    else
                                    {
                                      vol = GetVolume()+32767;
                                      if (vol<-29000)
                                        AnalogAGC+=1;
                                      else
                                        if (vol>10000)
                                          AnalogAGC-=1;
                                      ClearOffset();
                                    }
                                  }
                                  RestoreVolume();
                                  if (smeter_on)
                                    if (--smeter_time == 0)
                                    {
                                      smeter_time = 4;
                                      dspSendMsg(hApp,hAlg,   105 ,3,plotMsgData);
                                    }
                                  count = SB_DATA;
						                switch(mode)
						                {
						                  case 0 : for(wTemp=0; wTemp<256; wTemp+=2)
							                        {
							                          hilbert = sFilter(lastInputBuffer[wTemp]);
							                          delay = delayLine(lastInputBuffer[wTemp+1]);
 



							                          USB_BUFFER[count] = hilbert-delay;
							                          LSB_BUFFER[count] = hilbert+delay;
 




                                               pBuf[wTemp] = pBuf[wTemp+1] =  (MultiplyByVolume(UFilter(USB_BUFFER[count])));
 															  count++;
															}
						 

                           plotMsgData[0] = Center(USB_BUFFER)/2-Center(LSB_BUFFER)/2;
						 
                             dspSendMsg (hApp, hAlg,112 ,   128 , plotMsgData);
						  									break;
						                  case 10 : for(wTemp=0; wTemp<256; wTemp+=2)
							                        {
							                          hilbert = sFilter(lastInputBuffer[wTemp]);
							                          delay = delayLine(lastInputBuffer[wTemp+1]);
 



							                          USB_BUFFER[count] = hilbert-delay;
							                          LSB_BUFFER[count] = hilbert+delay;
 




                                               pBuf[wTemp] = pBuf[wTemp+1] =  (MultiplyByVolume(UFilter(LSB_BUFFER[count])));
 															  count++;
															}
						 

                           plotMsgData[0] = Center(USB_BUFFER)/2-Center(LSB_BUFFER)/2;
						 
                             dspSendMsg (hApp, hAlg,112 ,   128 , plotMsgData);
						  									break;
 






                                    case 1 : for(wTemp=0; wTemp<256; wTemp+=2)
                                             {
							                          hilbert = sFilter(lastInputBuffer[wTemp]);
							                          delay = delayLine(lastInputBuffer[wTemp+1]);
							                          USB_BUFFER[count] = hilbert-delay;
							                          LSB_BUFFER[count] = hilbert+delay;
							                          pBuf[wTemp] = pBuf[wTemp+1] = MultiplyByVolume(UFilter(hilbert+delay));
							                          count++;
							                        }
							                        break;
 







							                          count++;
							                        }
							                        break;
*/                                    case 2 : for(wTemp=0; wTemp<256; wTemp+=2)
                                             {
							                          hilbert = sFilter(lastInputBuffer[wTemp]);
							                          delay = delayLine(lastInputBuffer[wTemp+1]);
							                          USB_BUFFER[count] = hilbert-delay;
							                          LSB_BUFFER[count] = hilbert+delay;
							                          pBuf[wTemp] = pBuf[wTemp+1] = MultiplyByVolume(UFilter(hilbert-delay));
							                          count++;
							                        }
							                        break;
                                    case 3 : for(wTemp=0; wTemp<256; wTemp+=2)
							                        {
							                          hilbert = sFilter(lastInputBuffer[wTemp]);
                                               delay = delayLine(lastInputBuffer[wTemp+1]);
 

							                          pBuf[wTemp] = MultiplyByVolume(UFilter(USB_BUFFER[count] = hilbert-delay));
							                          pBuf[wTemp+1] = MultiplyByVolume(LFilter(LSB_BUFFER[count] = hilbert+delay));
							                          count++;
							                        }
							                        break;
												case 4 : for(wTemp=0; wTemp<256; wTemp+=2)
															{
                                               LSB_BUFFER[count] = FM(lastInputBuffer[wTemp],lastInputBuffer[wTemp+1]);
															  pBuf[wTemp] = pBuf[wTemp+1] = USB_BUFFER[count] = MultiplyByVolume( (LSB_BUFFER[count]));
 





															  count++;
															}
															break;
                                  }
                                  if (SB_DATA == 128)
                                  {
                                    if (fft_time==3 && PlotUSBT)
                                    {
                                      pInt = (int *) USB_BUFFER;
                                      for (wTemp=0; wTemp < 256; wTemp++)
						                      plotMsgData[wTemp]  = *pInt++;
						                    dspSendMsg (hApp, hAlg,104 ,   128 , plotMsgData);
						                  }
						                  if (fft_time == 2 && PlotUSBS)
						                  {
						                    pInt = (int *) dousbfft();
						                    for (wTemp=0; wTemp <   128 ; wTemp++)
						                      plotMsgData[wTemp]  = *pInt++;
						                    dspSendMsg (hApp, hAlg,101 ,   128 , plotMsgData);
						                  }
						                  if (--fft_time==0)
						                  {
						                    fft_time = 13;
						                    if (PlotLSBS)
						                    {
						                      pInt = (int *) dolsbfft();
						                      for (wTemp=0; wTemp <   128 ; wTemp++)
                                          plotMsgData[wTemp]  = *pInt++;
						                      dspSendMsg (hApp, hAlg,102 ,   128 , plotMsgData);
						                    }
						                  }
						                }
						                SB_DATA = 128 - SB_DATA;
                                }
 


                      wTemp = dspSendIo(hAlg, OutputDevice,pBuf, 256 );
                      plotMsgData[2] = 0;
                      if (LpfOn != 0)
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
                      if (NewIFGain>=0 || plotMsgData[2]!=0)
                      {
                        plotMsgData[0] = NewIFGain;
                        plotMsgData[1] = GetVolume();
                        AnalogAGC = 0;
                        dspSendMsg (hApp, hAlg,113  ,   128 , plotMsgData);
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
			    dspSendMsg (hApp, hAlg,           (                (-400)  - 0) , sizeof (       "requeueing error" ),        "requeueing error" );
			}
			break;
   }
}

int dit = 13;

DeleteMorse()

{
  int count;

  count = 0;
  for(count=0;count<100 -1;count++)
    Morse[count] = Morse[count+1];
  Morse[100 -1] = 0;
  if (Morse[0]==0)
  {
    dspSendMsg (hApp, hAlg, 111 ,256,"END MORSE");
    SendingMorse = 0;
  }
  else
    if (Morse[1]==0)
      dspSendMsg (hApp, hAlg, 110 ,256,"NEXT CHAR");
}


