/***************************************************************************
//
//      ALG.C
//
//      Copyright (c) 1994 Analog Devices, Inc.
//
//-------------------------------------------------------------------------
//
//      Module:     alg.c
//      Purpose:    Main DSP module for the APP.EXE demo program
//
/**************************************************************************/

#include "dspshell.h"     /* Include dsp header files. */
#include "dspaudio.h"


#define PLOT_HANDSHAKING           /* This dictates how plot information
				      is transferred from the DSP to
				      the host... is it just dumped
				      as quickly as the DSP can go or
				      does the DSP wait for the host
				      to ask for it. IF DEFINED HERE, IT
				      MUST ALSO BE DEFINED IN APPMAIN.H!!! */

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

typedef struct msg        /* Create message structure MSG_STRUCT */
{                         /* used to send message information to host. */
  DSPMSGHEADER hdr;       /* DSPMSGHEADER is structure defined in dspshell.h */
  int data[50];
}MSG_STRUCT;

typedef struct plotmsg    /* Create measage structure PLOTMSG_STRUCT */
{                         /* used to send data to be plotted to host */
  DSPMSGHEADER hdr;
  int data[128];
}PLOTMSG_STRUCT;

MSG_STRUCT msg1,msg2;           
MSG_STRUCT echoMsg1,echoMsg2;
MSG_STRUCT spConfMsg;

PLOTMSG_STRUCT plotMsg;         

DSPIOCONF devConf;              
				
DSPIOCAPS devCaps;              
				
/*
 *      Define global integer variables
 */

int InputDevice = 0;
int OutputDevice = 1;
int whichEchoMsg = 1;
int hAlg;                       /* handle to algorithm... myself! */
int hApp;                       /* handle to application (on host) */
int algIoOverrun = 0;
int PlotOn = FALSE;
int FreqOn = TRUE;
int TalkOn = FALSE;
int LogOn = FALSE;
int EchoOn = FALSE;
int LpfOn = FALSE;
int HannOn = FALSE;

int hilbert,delay;

int GetErrCount = 0;
int GetLastErr = 0;
int SendErrCount = 0;
int SendLastErr = 0;
long SampleRate = 8000;

int buffer1[256];                  /* Define buffer arrays. */
int buffer2[256];
int buffer3[256];
int buffer4[256];
int buffert[256];
int *lastInputBuffer = buffer3;   /* Init pointer to base address of buffer3 */

/*
 *      Declare external functions
 */

extern int dofft(void);
extern void initSignal(void);
extern int genSignal(void);
extern void initFilter(void);
extern int sFilter(int);
extern int delayLine(int);
extern int MultiplyByVolume(int);
extern void ChangeVolume(int);
extern void AGC(int *);
extern int GetVolume(void);
/*
 *      Declare assembly language variables external
 */

extern int gain1,freq1;
extern int gain2,freq2;
extern int gain3,freq3;
extern int chirp_freq_min, chirp_freq_max, chirp_gain;
extern int chirp_freq_delta_l,chirp_freq_delta_h;
extern int ccitt_gain;

/*
 *      Message definitions
 */

#define msg_SET_NOISE   500
#define msg_ECHO_TOGGLE 501
#define msg_PLOT_TOGGLE 502
#define msg_GET_1848    503
#define msg_TALK_TOGGLE 504
#define msg_LOG_TOGGLE  505
#define msg_FREQ_TOGGLE 506
#define msg_LPF_TOGGLE  507
#define msg_HANN_TOGGLE 508
#define msg_PLOT_SEND   509

int PlotBufferFree = TRUE;

/****************************************************************************
//
//      void    ParseMsg(int wParam1, int wParam2, int wParam3)
//
// Description:
//              This function is called in response to dspAlgMain
//              receiving a DSPCB_GETMSG from the shell. Messages
//              received from the host that were initiated by the
//              dspGetMsg function are processed here.
//
// Parameters:
//              int wParam1-3           - message dependent parameters
//
// Return Value:
//              nothing
//
/***************************************************************************/

void ParseMsg(int wParam1, int wParam2, int wParam3)
{
  int i;                               /* Define local variables. */
  int vol;
  int wTemp;
  long *pLong;
  char string[20];

  MSG_STRUCT *pMsg;                    /* Define pointers of type MSG_STRUCT. */
  MSG_STRUCT *pEchoMsg;

  pMsg = (MSG_STRUCT *)wParam2;        /* pMsg =  address of the block of */
				       /* the message that was received. */
  if(EchoOn)                           /* Echo Message Back to Sender */
  {
    if(whichEchoMsg == 1)
    {
      pEchoMsg = &echoMsg1;      /*pEchoMsg = address of the structure echoMsg1. */
      whichEchoMsg = 2;
    }else
    {
      pEchoMsg = &echoMsg2;       /*pEchoMsg = address of the structure echoMsg2. */
      whichEchoMsg = 1;
    }                                     /* Fill EchoMsg structure */
    pEchoMsg->hdr.from = hAlg;
    pEchoMsg->hdr.to = hApp; 
/*    pEchoMsg->hdr.to = pMsg->hdr.from; */
    pEchoMsg->hdr.id = 1001;
    pEchoMsg->hdr.dataLen = pMsg->hdr.dataLen+1;
    pEchoMsg->data[0] = pMsg->hdr.id;
    for(i=0; i<pMsg->hdr.dataLen; i++)
      pEchoMsg->data[i+1] = pMsg->data[i];

    /* send message back to host */

    dspSendMsg(hAlg, (int *)pEchoMsg, sizeof(DSPMSGHEADER)+pEchoMsg->hdr.dataLen);
  }
     
  switch(pMsg->hdr.id)
  {
    case ALGMSG_SET_RATE:               /* Set 1848 sample rate */
    case ALGMSG_SET_MODE:    
	dspStopIo(hAlg, InputDevice);       /* Stop input device driver */
	dspStopIo(hAlg, OutputDevice);      /* Stop output device driver */

	/*Get current configuration of 1848 and stores in devConf */

	dspGetIoDevConf(InputDevice, &devConf, sizeof(DSPIOCONF));
        devConf.mode = DSPIOMODE_STEREO;
	dspSetIoDevConf(InputDevice, &devConf, sizeof(DSPIOCONF));
	dspGetIoDevConf(OutputDevice, &devConf, sizeof(DSPIOCONF));
        devConf.mode = DSPIOMODE_STEREO;
	dspSetIoDevConf(OutputDevice, &devConf, sizeof(DSPIOCONF));
	wTemp = dspGetIoDevConf(InputDevice, &devConf, sizeof(DSPIOCONF));
	if(wTemp == DSPERR_IO_DEVICE)
		dspUtilSendString(hAlg, hApp, "Get Conf Error(DEV)\n");
	else if(wTemp == DSPERR_SIZE)
		dspUtilSendString(hAlg, hApp, "Get Conf Error(SIZE)\n");

        devConf.mode = DSPIOMODE_STEREO;
/*	devConf.dataType = DSPIODT_MAX;
*/      if(pMsg->hdr.id == ALGMSG_SET_RATE)
	{
	  /* Fill structure devConf with new sample rate. */

	  wTemp = pMsg->data[0];          /* Swap the words because */
	  pMsg->data[0] = pMsg->data[1];  /*the pc swaps hi word/lo word */
	  pMsg->data[1] = wTemp;
	  pLong = (long *)&pMsg->data[0];
	  SampleRate = *pLong;             /* Save new sample rate for later */
	  devConf.rate = *pLong;
	 /* Receive ALGMSG_SET_MODE Message but don't set mode */
	}
     else if(pMsg->hdr.id == ALGMSG_SET_MODE)  
	  dspUtilSendString(hAlg, hApp, "Demo doesn't run in stereo; 1848 mode set to mono\n"); 

	/*Reconfigure 1848 with new sample rate or mode */

	if((wTemp = dspSetIoDevConf(InputDevice, &devConf, sizeof(DSPIOCONF)))
			!= DSPERR_SUCCESS)
		dspUtilSendString(hAlg, hApp, "Set Conf Error\n");

	if(((wTemp = dspStartIo(hAlg, OutputDevice)) != DSPERR_SUCCESS) ||
	   ((wTemp = dspStartIo(hAlg, InputDevice)) != DSPERR_SUCCESS))
		dspUtilSendString(hAlg, hApp, "Start Error\n");
      break;

    case msg_SET_NOISE:                      /* Set Noise Message */
      switch(pMsg->data[0])
      {
      case 0:                                /* Freq 1 */
	  gain1 = pMsg->data[1];                  /* Store gain1 value */
	  freq1 = pMsg->data[2];                  /* Store freq1 value */
	  freq1 = freq1 * 65536/SampleRate;       /* Convert to phase increment needed */
	  break;                                  /* to generate desired frequency */
					     /* in Hz based on sample rate */                                                                /* in Hz based on sample rate   */
      case 1:                                /* Freq 2 */
	  gain2 = pMsg->data[1];
	  freq2 = pMsg->data[2];
	  freq2 = freq2 * 65536/SampleRate;
	  break;

      case 2:                                /* Freq 3 */
	  gain3 = pMsg->data[1];
	  freq3 = pMsg->data[2];
	  freq3 = freq3 * 65536/SampleRate;
	  break;

      case 3:                                /* Chirp */
	  chirp_gain = pMsg->data[1];             /* Set chirp variables to values */
	  chirp_freq_min = pMsg->data[2];         /* received in message structure */
	  chirp_freq_min = chirp_freq_min * 65536/SampleRate;
	  chirp_freq_max = pMsg->data[3];
	  chirp_freq_max = chirp_freq_max * 65536/SampleRate;
	  chirp_freq_delta_l = pMsg->data[4];
	  chirp_freq_delta_l = chirp_freq_delta_l * 65536/SampleRate;
	  chirp_freq_delta_h = pMsg->data[5];
	  chirp_freq_delta_h = chirp_freq_delta_h * 65536/SampleRate;
	  break;

      case 4:                                 /* CCITT Noise */
	  ccitt_gain = pMsg->data[1];              /* Set ccitt_gain to value */
	  break;                                   /* received in message structure. */

      /* Send string to DSP Prints screen for undefined received messages */

      default:
	  dspUtilSendString(hAlg, hApp, "What noise was that?\n");
       break;
      }
      break;

    case msg_ECHO_TOGGLE:        /* ECHO_TOGGLE Message */
      if(EchoOn)                 /* Toggles EchoOn variable when Echo pushbutton */
	   EchoOn = 0;              /* by host windows application. */
      else
	  EchoOn = 1;
      break;
				 /* PLOT_TOGGLE Message */
    case msg_PLOT_TOGGLE:        /* Toggles PlotOn variable when Plot menu item */
      if(PlotOn)                 /* is selected by host windows application. */
	PlotOn = 0;
      else
      {
	int *pInt;
	pInt = lastInputBuffer;            /* Load pInt with address of */
	PlotOn = 1;                        /* buffer of data to be plotted. */
	for(wTemp=0; wTemp<128; wTemp++)
		plotMsg.data[wTemp] = *pInt++;   /* Fill plotMsg structure */
	plotMsg.hdr.from = hAlg;           /* with data to be plotted. */
	plotMsg.hdr.to = hApp;
	plotMsg.hdr.id = 500;
	   plotMsg.hdr.dataLen = 128;

	     /* Send plotMsg structure to host. */

	   dspSendMsg(hAlg, (int *)&plotMsg, sizeof(PLOTMSG_STRUCT));
	   PlotBufferFree = FALSE;
      }
      break;


    case msg_PLOT_SEND:
/*        dspUtilSendString(hAlg, hApp, "Got 509\n"); */
    
    if (TRUE)
    /*  if (PlotBufferFree == TRUE) */
      {
	int *pInt;        

    /*    if((int *)wParam2 == (int *)&plotMsg)   */
    /* Is address of message that was sent by */ 
	if(TRUE)
	{                 /* dspSendMsg function call = address of plotMsg. */
	   if(FreqOn)                   /* Test if plotting in frequency domain. */
	     pInt = (int *) dofft();    /* Store return value of fft call in pInt, */
	   else                         /* this is the starting address of the */
						    /* results buffer which contains the the */ 
						    /* spectrum of the buffer lastInputBuffer. */
           pInt = lastInputBuffer;    /* Store address of lastInputBuffer in pInt. */
/*           pInt = buffer2;
*/	     for(wTemp=0; wTemp<128; wTemp++)     /* Store data to be plotted in plotMsg */
             {
	       plotMsg.data[wTemp] = *pInt;     /* structure data field. */         
               pInt+=2;
             }

	  plotMsg.hdr.from = hAlg;
	  plotMsg.hdr.to = hApp;
	  plotMsg.hdr.id = 500;
	  plotMsg.hdr.dataLen = 128;
	  
	       /* Send plotMsg structure to host. */ 
	  dspSendMsg(hAlg, (int *)&plotMsg, sizeof(PLOTMSG_STRUCT));  
	  
/*        dspUtilSendString(hAlg, hApp, "Plot sent\n"); */
	  }
      }
	else
/*     dspUtilSendString(hAlg, hApp, "Plot buffer not free yet\n");*/
	PlotBufferFree=FALSE;
     break;


    case msg_GET_1848:                   /* Get_1848 Message */
      for(wTemp=0; wTemp<16; wTemp++)    /* Gets 1848 configuration and fills spConfMsg */
      {                                  /* structure to send to host. Configuration  */
	dspIoWrite(0x3440,wTemp<<8);    /* data gets displayed in dsp prints screen. */
	spConfMsg.data[wTemp]=dspIoRead(0x3448);
      }
      spConfMsg.hdr.from = hAlg;
      spConfMsg.hdr.to = hApp;
      spConfMsg.hdr.id = msg_GET_1848;
      spConfMsg.hdr.dataLen = 16;
      dspSendMsg(hAlg, (int *)&spConfMsg, sizeof(DSPMSGHEADER)+16);
      break;

    case msg_TALK_TOGGLE:               /* TALK_TOGGLE Message */
      if(TalkOn)                        /* Toggles TalkOn variable when Talk pushbutton */
	TalkOn = 0;                    /* by host windows application. */
      else
	TalkOn = 1;
      break;

    case msg_LOG_TOGGLE:                /* LOG_TOGGLE Message */
      if(LogOn)                         /* Toggles LogOn variable when Log menu item is */
	LogOn = 0;                     /* selected by host windows application. */
      else
	LogOn = 1;
      break;

    case msg_FREQ_TOGGLE:               /* Frequency Toggle message */
      if(FreqOn)                        /* Toggles FreqOn variable when Freq menu item is  */
	FreqOn = 0;                    /* selected by host windows application. */
      else
	FreqOn = 1;
      break;

    case msg_LPF_TOGGLE:               /* LPF Toggle message */
      if(LpfOn)                        /* Toggles LpfOn variable when LPF menu item is  */
	LpfOn = 0;                    /* selected by host windows application. */
      else
	LpfOn = 1;
      break;

    case msg_HANN_TOGGLE:               /* Hanning Toggle message */
      vol = GetVolume();
      if (vol<0)
      {
        string[0] = '-';
        vol*=-1;
      }
      else
        string[0] = '+'; 
      string[1] = vol/10000+'0';
      string[2] = ((vol/1000)%10)+'0';
      string[3] = ((vol/100)%10)+'0';
      string[4] = ((vol/10)%10)+'0';
      string[5] = (vol%10)+'0';
      string[6] = '\n';
      string[6] = 0;
      dspUtilSendString(hAlg,hApp,string);
      if(HannOn)                        /* Toggles HannOn variable when Hanning menu item is  */
	HannOn = 0;                    /* selected by host windows application. */
      else
	HannOn = 1;
      break;
   
    /* Send string to DSP Prints screen for undefined received messages */ 

    default:
	dspUtilSendString(hAlg, hApp, "Huh? What message was that?\n");
      break;
  }
}

/***************************************************************************
//
//      void    dspAlgMain( int wCbid, int wParam1, int wParam2, int wParam3)
//
// Description:
//              The dspAlgMain function is the entry point to the algorithm.
//              The DSP Shell calls dspAlgMain with messages.
//
// Parameters:
//              int wCbId               - callback id
//              int wParam1-3           - message dependent paramaters
//
// Return Value:
//              nothing
//
/***************************************************************************/

void dspAlgMain(int wCbId, int wParam1, int wParam2, int wParam3)
{
  int *pBuf;                      /* Define local variables */
  int wTemp;
  int i,wNumDevs;
/*  int hilbert,delay;
*/
  switch(wCbId)
  {
	    /* Shell sends this message to tell algorithm to initialize */
    case DSPCB_INIT:
      hAlg = wParam1;             /* Save algorithm handle   */
      hApp = wParam2;             /* Save application handle */
      msg1.hdr.dataLen = 50;
      msg2.hdr.dataLen = 50;

	/* give shell two buffers for incoming host messages */
      dspGetMsg(hAlg, (int *)&msg1, sizeof(MSG_STRUCT));
      dspGetMsg(hAlg, (int *)&msg2, sizeof(MSG_STRUCT));  

      initSignal();               /* Initalize all gain variables to zero.*/

      initFilter();               /* Clear filter delay line buffer */
      ChangeVolume(256);

	/* Find Out how many Devices there are */
      wNumDevs = dspGetNumIoDevs();
	/* Find the 1848 Input & Output Devices */
      for(i=0; i<wNumDevs; i++)
      {
	wTemp = dspGetIoDevCaps(i, &devCaps, sizeof(DSPIOCAPS));
	if(wTemp == DSPERR_SUCCESS)
	{
	  if(strcmp(devCaps.szDeviceName,"AD1848") == 0)
	  {
	    if(devCaps.dir == DSPIODIR_IN)
	      InputDevice = i;
	    else 
	      if(devCaps.dir == DSPIODIR_OUT)
	        OutputDevice = i;
	  }
	}
        else
	  dspUtilSendString(hAlg, hApp, "Dev Conf Error\n");
      }
      dspGetIoDevConf(OutputDevice, &devConf, sizeof(DSPIOCONF));
      devConf.mode = DSPIOMODE_STEREO;
      dspSetIoDevConf(OutputDevice, &devConf, sizeof(DSPIOCONF));
/*      dspGetIoDevConf(InputDevice, &devConf, sizeof(DSPIOCONF));
      devConf.mode = DSPIOMODE_STEREO;
      dspSetIoDevConf(InputDevice, &devConf, sizeof(DSPIOCONF));
*/      dspUtilSendString(hAlg, hApp, "Howdy\n");    /* Output message to     */

      for(wTemp=0; wTemp<256; wTemp++)      /* Initialize I/O buffers */
      {  
	   buffer1[wTemp]=0;
	   buffer2[wTemp]=0;
      }

      dspSendIo(hAlg, OutputDevice, buffer1, 256);     /* Send buffer data */
      dspSendIo(hAlg, OutputDevice, buffer2, 256);     /* to 1848          */

      for(wTemp=0; wTemp<256; wTemp++)      
      {  
	    buffer3[wTemp] = 0;
	    buffer4[wTemp] = 0;
	    buffert[wTemp] = -((wTemp*19)%256+10000);
      }

      dspGetIo(hAlg, InputDevice, buffer3, 256);       /* Give buffer3 and buffer4 to */
      dspGetIo(hAlg, InputDevice, buffer4, 256);       /* 1848 device driver to fill. */

      dspStartIo(hAlg, OutputDevice);            /* Starts 1848 input &   */
      dspStartIo(hAlg, InputDevice);             /* output device drivers */
						 /* dsp prints screen     */

      dspIoWrite(0x3440,0x0000);
      dspIoWrite(0x3448,0x4F00);   /* line input (L) */
      dspIoWrite(0x3440,0x0100);
      dspIoWrite(0x3448,0x4F00);   /* line input (R) */
      dspIoWrite(0x3440,0x0200);
      dspIoWrite(0x3448,0x8000);
      dspIoWrite(0x3440,0x0300);
      dspIoWrite(0x3448,0x8000);
      dspIoWrite(0x3440,0x0400);
      dspIoWrite(0x3448,0xBF00);
      dspIoWrite(0x3440,0x0500);
      dspIoWrite(0x3448,0xBF00);
      break;

	    /* Shell sends this message to tell algorithm to terminate. */
    case DSPCB_CLOSE:       
      dspStopIo(hAlg, InputDevice);              /* Stop 1848 input device driver */
      dspStopIo(hAlg, OutputDevice);             /* Stop 1848 output device driver */
      break;


    case DSPCB_SENDMSG:    /*Callback sent to dspSendMsg function call. */
      if(PlotOn == TRUE)   /* Test if plotting is on */
      {
#ifndef   PLOT_HANDSHAKING
	int *pInt;
	     /* Is address of message that was sent = address of plotMsg. */ 
	   if((int *)wParam2 == (int *)&plotMsg)   
	   {                                       
	     if(FreqOn)                    /* Test if plotting in frequency domain.*/
	       pInt = (int *) dofft();     /* Store return value of fft call in pInt,*/
	     else                          /* this is the starting address of the */
					/* results buffer which contains the the */
					/* spectrum of the buffer lastInputBuffer.*/
	       pInt = lastInputBuffer;     /* Store address of lastInputBuffer in pInt.*/

	   for(wTemp=0; wTemp<128; wTemp++)   /* Store data to be plotted in */
	     plotMsg.data[wTemp] = *pInt++;   /* plotMsg structure data field. */

	   plotMsg.hdr.from = hAlg;
	   plotMsg.hdr.to = hApp;
	   plotMsg.hdr.id = 500;
	   plotMsg.hdr.dataLen = 128;

	     /* Send plotMsg structure to host. */
	   dspSendMsg(hAlg, (int *)&plotMsg, sizeof(PLOTMSG_STRUCT));
	   }
#else
	PlotBufferFree=TRUE;
#endif
      }
      break;

    /* Callback indicates a buffer of data has been read from I/O device */
    case DSPCB_GETIO:
      if((wParam3 == DSPERR_SUCCESS) || (wParam3 == DSPERR_IO_ABORT))
      {
	     /* Save buffer address in lastInputBuffer for plot. */
	   lastInputBuffer = (int *)wParam2; 
	     /* Give the buffer back to the IO driver */
	   wTemp = dspGetIo(hAlg, InputDevice, (int *)wParam2, 256); 
	   if(wTemp != DSPERR_SUCCESS)                               
	   {
	     GetErrCount++;
	     GetLastErr = wTemp;
	   }
      }
      else if(wParam3 == DSPERR_NO_BUFFER)
	algIoOverrun++;
      break;

    case DSPCB_SENDIO:
      if((wParam3 == DSPERR_SUCCESS) || (wParam3 == DSPERR_IO_ABORT))
      {
	pBuf = (int *)wParam2;
	if(TalkOn)            /* If true, loop 1848 input to 1848 output */
	{
/*	  if (LpfOn) 
*/	  {
            if (LpfOn)
            {
              AGC(lastInputBuffer);
            }
            else
            {
              ChangeVolume(512);
            } 
	    for(wTemp=0; wTemp<256; wTemp+=2)
	    {  
              hilbert = sFilter(lastInputBuffer[wTemp]);
              delay = delayLine(lastInputBuffer[wTemp+1]);
/*              if (LpfOn)
              {
                pBuf[wTemp] = hilbert+delay;
                pBuf[wTemp+1] = hilbert-delay;
              }
              else
*/              {
                pBuf[wTemp] = MultiplyByVolume(hilbert);
                pBuf[wTemp+1] = MultiplyByVolume(delay);
              }
            }
          }
/*	  else
	  {
	    for(wTemp=0; wTemp<256; wTemp+=1)
	    {
              pBuf[wTemp+1] = lastInputBuffer[wTemp];
            }
	  }
*/	}
	else
	{
	     if(LpfOn)           /* If TalkOn = false and LpfOn = true */ 
	     {                   /* fill buffer1 with filtered samples from gen.dsp */  
    
	       for(wTemp=0; wTemp<256; wTemp++)
	       {
		 pBuf[wTemp] = sFilter(genSignal()); 
	       }                                                                    
	     }                   /* If TalkOn = false and LpfOn = false */  
	     else                /* fill buffer1 with samples from gen.dsp */     
	     {                                  
	       for(wTemp=0; wTemp<256; wTemp++) 
	         pBuf[wTemp] = genSignal();
	     }
	}

	     /* Send buffer to 1848 */
	   wTemp = dspSendIo(hAlg, OutputDevice, pBuf, 256);
	   if(wTemp != DSPERR_SUCCESS)
	   {
	     SendErrCount++;
	     SendLastErr = wTemp;
	   }
      }
      else if(wParam3 == DSPERR_NO_BUFFER)
	   algIoOverrun++;
      break;


    /* Shell sends callback to indicate that a message initiated by  */
    /* the dspGetMsg function call has been received from the host. */

    case DSPCB_GETMSG:
      if(wParam1 == DSPERR_SUCCESS)    /* Check if error */
      {
	   ParseMsg(wParam1, wParam2, wParam3);                 /* Call ParseMsg */
	   if(dspGetMsg(hAlg, (int *)wParam2, sizeof(MSG_STRUCT)) != 0)
	     dspUtilSendString(hAlg, hApp, "Get Msg Error\n");
      }
      break;
  }
}

/*---------------------------- END OF ALG.C ------------------------------*/

