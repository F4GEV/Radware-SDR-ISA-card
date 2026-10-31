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

#include "dspshell.h" 


#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

/*************************************************** strings */

#define STR_DEMOMSG                   "Demo doesn't run in stereo; overriding to mono..."
#define STR_GETCONFDEVERROR           "Get Conf Device Error"
#define STR_GETCONFSIZEERROR          "Get Conf Size Error"
#define STR_SETCONFERROR              "Set Conf Error"
#define STR_STARTERROR                "Start Error"
#define STR_UNKNOWNNOISETYPE          "Unknown noise type received by algorithm."
#define STR_BADPLOTREQUEST            "Request for plot data received by algorithm when plotting is disabled."
#define STR_UNKNOWNMSG                "Algorithm received an unknown message ID."
#define STR_NOIODEVICES               "Algorithm can't locate IO devices"
#define STR_INITCOMPLETE              "Algorithm initialization complete"
#define STR_INPUTBUFFERREQUEUEFAILED  "Algorithm failed to requeue an input buffer."
#define STR_IOABORTINPUTBUFFER        "Algorithm received an IO abort when filling an input buffer."
#define STR_OVERRUNINPUTBUFFER        "Algorithm received an overrun error when filling an input buffer."
#define STR_OUTPUTBUFFERREQUEUEFAILED "Algorithm failed to requeue an output buffer."
#define STR_IOABORTOUTPUTBUFFER       "Algorithm received an IO abort when filling an output buffer."
#define STR_UNDERRUNOUTPUTBUFFER      "Algorithm received an underrun error when sending an output buffer."
#define STR_GETMSGREQUEUEERROR        "Algorithm received a get message error when requeueing."
#define STR_GETMSGERROR               "Algorithm received a get message error."
#define STR_UNEXPECTEDCALLBACK        "Algorithm received an unexpected callback message."
#define STR_SYSTEMERROR               "Algorithm received a System Error message."
#define STR_UNKNOWNCALLBACK           "Algorithm received an unknown callback message."
/********************************************** Message definitions */

       /* Host to DSP */
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

       /* DSP to HOST */
#define msg_PLOT_DATA     500
#define msg_1848_REG_DUMP 503
#define msg_ECHO_DATA     1001



/************************************************ Message Stuff */

#define SIZE_DATAMSG   50  /* size of message buffers */
#define SIZE_PLOTMSG   128 /* size of plot buffers */
#define SIZE_IO_BUFFER 256 /* size of 1848 IO buffers */




typedef struct msg        /* Create message structure MSG_STRUCT */
{                         /* used to send message information to host. */
  DSPMSGHEADER hdr;      /* DSPMSGHEADER is structure defined in dspsdk.h */
  int data[SIZE_DATAMSG];
}MSG_STRUCT;

MSG_STRUCT msg1,msg2;

int plotMsgData[SIZE_PLOTMSG];

DSPIOCONF devConf;              
DSPIOCAPS devCaps;              
				
/************************************************ globals */

int InputDevice = 0;
int OutputDevice = 1;
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

int GetErrCount = 0;
int GetLastErr = 0;
int SendErrCount = 0;
int SendLastErr = 0;
long SampleRate = 8000;

int buffer1[SIZE_IO_BUFFER];                  /* Define buffer arrays. */
int buffer2[SIZE_IO_BUFFER];
int buffer3[SIZE_IO_BUFFER];
int buffer4[SIZE_IO_BUFFER];

int *lastInputBuffer = buffer3;   /* Init pointer to base address of buffer3 */



/***************************************************** external functions and data */

extern int dofft(void);
extern void initSignal(void);
extern int genSignal(void);
extern void initFilter(void);
extern int sFilter(void);

extern int gain1,freq1;
extern int gain2,freq2;
extern int gain3,freq3;
extern int chirp_freq_min, chirp_freq_max, chirp_gain;
extern int chirp_freq_delta_l,chirp_freq_delta_h;
extern int ccitt_gain;


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

void ParseMsg(int wParam1, int wParam2, int wParam3) {

  int i;
  int wTemp;
  int *pInt;
  long *pLong;
  MSG_STRUCT *pMsg;

/* point to message */
   pMsg = (MSG_STRUCT *)wParam2;        /* pMsg =  address of the block of */
				       /* the message that was received. */

/* echo it back if required */
   if (EchoOn) {
     dspSendMsg(hApp, hAlg, msg_ECHO_DATA, pMsg->hdr.size, pMsg->data);
   }

/* CASEOF (message ID) */        
   switch (pMsg->hdr.id) {

/* CASE (1848 update) */
   case DSPMSGID_AUDIO_SETRATE:
   case DSPMSGID_AUDIO_SETMODE:

/* . Don't allow mode changes to mono as applications group wants to do this in the SDK class */
     if(pMsg->hdr.id == DSPMSGID_AUDIO_SETMODE) {
       if (pMsg->data[0] == 1) {
         dspSendMsg (hApp, hAlg, DSPMSGID_STRING_PRINT, sizeof (STR_DEMOMSG), STR_DEMOMSG);
       }
       break;
     }
	
/* . pause the 1848 IO */
     dspStopIo(hAlg, InputDevice);
     dspStopIo(hAlg, OutputDevice);

/* . get the current configuration */
     wTemp = dspGetIoDevConf(InputDevice, &devConf, sizeof(DSPIOCONF));
     if (wTemp == DSPERR_IO_DEVICE) {
       dspSendMsg (hApp, hAlg, DSPMSGID_STRING_PRINT, sizeof (STR_GETCONFDEVERROR), STR_GETCONFDEVERROR);
     } else {
       if (wTemp == DSPERR_SIZE) {
	 dspSendMsg (hApp, hAlg, DSPMSGID_STRING_PRINT, sizeof (STR_GETCONFSIZEERROR), STR_GETCONFSIZEERROR);
       }
     }

/* . IF (changing the rate) */
     if (pMsg->hdr.id == DSPMSGID_AUDIO_SETRATE) {

/* . . prepare the new rate info */
       wTemp = pMsg->data[0];          /* Swap the words because */
       pMsg->data[0] = pMsg->data[1];  /*the pc swaps hi word/lo word */
       pMsg->data[1] = wTemp;
       pLong = (long *)&pMsg->data[0];
       SampleRate = *pLong;             /* Save new sample rate for later */
       devConf.rate = *pLong;

/* . ENDIF */
     }

/* . update the 1848 configuration */
     if ((wTemp = dspSetIoDevConf(InputDevice, &devConf, sizeof(DSPIOCONF))) != DSPERR_SUCCESS) {
       dspSendMsg (hApp, hAlg, DSPMSGID_STRING_PRINT, sizeof (STR_SETCONFERROR), STR_SETCONFERROR);
     }

/* . initialize and requeue the IO buffers, 2 for transmit and 2 for reception */
     for (i = 0; i < SIZE_IO_BUFFER; i++) {
       buffer1[i]=0;
       buffer2[i]=0;
       buffer3[i]=0;
       buffer4[i]=0;
     }
     dspSendIo (hAlg, OutputDevice, buffer1, SIZE_IO_BUFFER);
     dspSendIo (hAlg, OutputDevice, buffer2, SIZE_IO_BUFFER);
     dspGetIo (hAlg, InputDevice, buffer3, SIZE_IO_BUFFER);
     dspGetIo (hAlg, InputDevice, buffer4, SIZE_IO_BUFFER);

/* . restart the 1848 */
     if (((wTemp = dspStartIo(hAlg, OutputDevice)) != DSPERR_SUCCESS) ||
	   ((wTemp = dspStartIo(hAlg, InputDevice)) != DSPERR_SUCCESS)) {
       dspSendMsg (hApp, hAlg, DSPMSGID_STRING_PRINT, sizeof (STR_STARTERROR), STR_STARTERROR);
     }
     break;

/* CASE (setting a noise value) */
   case msg_SET_NOISE:

/* . CASEOF (noise type) */
     switch(pMsg->data[0]) {

/* . . CASE (SIN1) */
       case 0:

/* . . . save the data */
	 gain1 = pMsg->data[1];                  /* Store gain1 value */
	 freq1 = pMsg->data[2];                  /* Store freq1 value */
	 freq1 = freq1 * 65536/SampleRate;       /* Convert to phase increment needed */
	 break;                                  /* to generate desired frequency */

/* . . CASE (SIN2) */
       case 1:

/* . . . save the data */
	 gain2 = pMsg->data[1];
	 freq2 = pMsg->data[2];
	 freq2 = freq2 * 65536/SampleRate;
	 break;

/* . . CASE (SIN3) */
       case 2:

/* . . . save the data */
	 gain3 = pMsg->data[1];
	 freq3 = pMsg->data[2];
	 freq3 = freq3 * 65536/SampleRate;
	 break;

/* . . CASE (Chirp) */
       case 3:

/* . . . save the data */
	 chirp_gain = pMsg->data[1];
	 chirp_freq_min = pMsg->data[2];
	 chirp_freq_min = chirp_freq_min * 65536/SampleRate;
	 chirp_freq_max = pMsg->data[3];
	 chirp_freq_max = chirp_freq_max * 65536/SampleRate;
	 chirp_freq_delta_l = pMsg->data[4];
	 chirp_freq_delta_l = chirp_freq_delta_l * 65536/SampleRate;
	 chirp_freq_delta_h = pMsg->data[5];
	 chirp_freq_delta_h = chirp_freq_delta_h * 65536/SampleRate;
	 break;


/* . . CASE (CCITT) */
       case 4:

/* . . . save the data */
	 ccitt_gain = pMsg->data[1];
	 break;

/* . . CASEELSE */
       default:

/* . . . what kind of noise was that */
	 dspSendMsg (hApp, hAlg, DSPMSGID_STRING_PRINT, sizeof (STR_UNKNOWNNOISETYPE), STR_UNKNOWNNOISETYPE);
	 break;

/* . . ENDCASE */
       }
     break;

/* CASE (echo toggle) */
   case msg_ECHO_TOGGLE:

/* . update flag */
     if (EchoOn) {
       EchoOn = FALSE;
     } else {
       EchoOn = TRUE;
     }
     break;

/* CASE (plot toggle) */
   case msg_PLOT_TOGGLE:

/* . update flag */
     if (PlotOn) {
       PlotOn = FALSE;
     } else {
       PlotOn = TRUE;
     }
     break;

/* CASE (plot data request) */
   case msg_PLOT_SEND:

/* . be sure plotting is enabled */
     if (!PlotOn) {
       dspSendMsg (hApp, hAlg, DSPMSGID_STRING_PRINT, sizeof (STR_BADPLOTREQUEST), STR_BADPLOTREQUEST);
       break;
     }

/* . IF (plotting in frequency domain) */
     if (FreqOn) {

/* . . do the FFT and use that data */
       pInt = (int *) dofft();

/* . ELSE */
     } else {

/* . . use unprocessed data */
       pInt = lastInputBuffer;

/* . ENDIF */
     }

/* . copy the data to the buffer */
     for (wTemp=0; wTemp < SIZE_PLOTMSG; wTemp++) {
       plotMsgData[wTemp] = *pInt++;
     }

/* . send it to the host */
     dspSendMsg (hApp, hAlg, msg_PLOT_DATA, SIZE_PLOTMSG, plotMsgData);
     break;

/* CASE (1848 register dump) */
   case msg_GET_1848:

/* . get the device configuration information and copy it into our buffer */
     wTemp = dspGetIoDevConf(InputDevice, &devConf, sizeof(DSPIOCONF));
     if (wTemp == DSPERR_IO_DEVICE) {
       dspSendMsg (hApp, hAlg, DSPMSGID_STRING_PRINT, sizeof (STR_GETCONFDEVERROR), STR_GETCONFDEVERROR);
       break;
     }
     if (wTemp == DSPERR_SIZE) {
       dspSendMsg (hApp, hAlg, DSPMSGID_STRING_PRINT, sizeof (STR_GETCONFSIZEERROR), STR_GETCONFSIZEERROR);
       break;
     }

/* . send the message */
     dspSendMsg(hApp, hAlg, msg_GET_1848, sizeof (DSPIOCONF), (PTR) &devConf);
     break;

/* CASE (talk toggle) */
   case msg_TALK_TOGGLE:

/* . update our flag */
     if (TalkOn) {
       TalkOn = FALSE;
     } else {
       TalkOn = TRUE;
     }
     break;

/* CASE (log toggle) */
   case msg_LOG_TOGGLE:

/* . toggle our flag */
     if (LogOn) {
       LogOn = FALSE;
     } else {
       LogOn = TRUE;
     }
     break;

/* CASE (frequency toggle) */
   case msg_FREQ_TOGGLE:

/* . toggle our flag */
     if (FreqOn) {
       FreqOn = FALSE;
     } else {
       FreqOn = TRUE;
     }
     break;

/* CASE (low pass filter toggle) */
   case msg_LPF_TOGGLE:

/* . toggle our flag */
     if (LpfOn) {
       LpfOn = FALSE;
     } else {
       LpfOn = TRUE;
     }
     break;

/* CASE (hanning filter toggle) */
   case msg_HANN_TOGGLE:

/* . toggle our flag */
     if (HannOn) {
       HannOn = FALSE;
     } else {
       HannOn = TRUE;
     }
     break;

/* CASEELSE */
   default:

/* . tell the application we got something we didn't understand */
     dspSendMsg (hApp, hAlg, DSPMSGID_STRING_PRINT, sizeof (STR_UNKNOWNMSG), STR_UNKNOWNMSG);
     break;

/* ENDCASE */
   }

/* return */
   }

/***************************************************************************
//
//      void    dspAlgMain( WORD wCbid, WORD wParam1, WORD wParam2, WORD wParam3)
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

void dspAlgMain(WORD wCbId, WORD wParam1, WORD wParam2, WORD wParam3) {

  int *pBuf;
  int wTemp;
  int i,wNumDevs;

/* CASEOF (callback message) */
   switch(wCbId) {

/* CASE (initialization) */
   case DSPCB_INIT:

/* . save the IDs */
     hAlg = wParam1;
     hApp = wParam2;
     msg1.hdr.size = SIZE_DATAMSG;
     msg2.hdr.size = SIZE_DATAMSG;

/* . load a couple of message structures */
     dspGetMsg (hAlg, (int *)&msg1, sizeof(MSG_STRUCT));
     dspGetMsg (hAlg, (int *)&msg2, sizeof(MSG_STRUCT));  

/* . initialize signals and filters */
     initSignal ();
     initFilter ();

/* . initialize the devices */
     wNumDevs = dspGetNumIoDevs();
     for (i = 0; i < wNumDevs; i++) {
       wTemp = dspGetIoDevCaps (i, &devCaps, sizeof(DSPIOCAPS));
       if (wTemp == DSPERR_SUCCESS) {
	 if (strcmp (devCaps.szDeviceName,"AD1848") == 0) {
	   if (devCaps.dir == DSPIODIR_IN) {
	     InputDevice = i;
	   } else {
	     if (devCaps.dir == DSPIODIR_OUT) {
	       OutputDevice = i;
	     }
	   }
	 }
       } else {
	 dspSendMsg (hApp, hAlg, DSPMSGID_STRING_PRINT, sizeof (STR_NOIODEVICES), STR_NOIODEVICES);
       }
     }

/* . initialize IO buffers, 2 for transmit and 2 for reception */
     for (wTemp = 0; wTemp < SIZE_IO_BUFFER; wTemp++) {
       buffer1[wTemp]=0;
       buffer2[wTemp]=0;
       buffer3[wTemp]=0;
       buffer4[wTemp]=0;
     }
     dspSendIo (hAlg, OutputDevice, buffer1, SIZE_IO_BUFFER);
     dspSendIo (hAlg, OutputDevice, buffer2, SIZE_IO_BUFFER);
     dspGetIo (hAlg, InputDevice, buffer3, SIZE_IO_BUFFER);
     dspGetIo (hAlg, InputDevice, buffer4, SIZE_IO_BUFFER);

/* . start the devices */
     dspStartIo (hAlg, OutputDevice);
     dspStartIo (hAlg, InputDevice);
     dspSendMsg (hApp, hAlg, DSPMSGID_STRING_PRINT, sizeof (STR_INITCOMPLETE), STR_INITCOMPLETE);
     break;


/* CASE (terminating) */
   case DSPCB_CLOSE:

/* . halt the IO devices */          
     dspStopIo(hAlg, InputDevice);
     dspStopIo(hAlg, OutputDevice);
     break;

/* CASE (send message callback) */
   case DSPCB_SENDMSG:

/* . nothing special to do */
     break;

/* CASE (1848 input buffer filled) */
   case DSPCB_GETIO:

/* . CASEOF (error code) */
     switch (wParam3) {

/* . CASE (success) */
     case (DSPERR_SUCCESS):

/* . . save buffer address */
       lastInputBuffer = (int *)wParam2;

/* . . requeue the buffer */
       wTemp = dspGetIo(hAlg, InputDevice, (int *)wParam2, SIZE_IO_BUFFER);
       if (wTemp != DSPERR_SUCCESS) {
	 GetErrCount++;
	 GetLastErr = wTemp;
	 dspSendMsg (hApp, hAlg, DSPMSGID_STRING_PRINT, sizeof (STR_INPUTBUFFERREQUEUEFAILED), STR_INPUTBUFFERREQUEUEFAILED);
       }
       break;

/* . CASE (aborted) */
     case (DSPERR_IO_ABORT):

/* . . tell the application */
       dspSendMsg (hApp, hAlg, DSPMSGID_STRING_PRINT, sizeof (STR_IOABORTINPUTBUFFER), STR_IOABORTINPUTBUFFER);
       break;

/* . CASE (overrun) */
     case (DSPERR_NO_BUFFER):

/* . . tell the application */
       dspSendMsg (hApp, hAlg, DSPMSGID_STRING_PRINT, sizeof (STR_OVERRUNINPUTBUFFER), STR_OVERRUNINPUTBUFFER);
       algIoOverrun++;
       break;

/* . ENDCASE */
     }
     break;

/* CASE (1848 output buffer sent) */
   case DSPCB_SENDIO:

/* . CASEOF (error code) */
     switch (wParam3) {

/* . CASE (success) */
     case (DSPERR_SUCCESS):

/* . . point to the buffer */
       pBuf = (int *)wParam2;

/* . . loop back if talk thru is enabled */
       if (TalkOn) {
	 for (wTemp = 0; wTemp < SIZE_IO_BUFFER; wTemp++) {
	   pBuf[wTemp] = lastInputBuffer[wTemp];
	 }
       }

/* . . fill buffer with filtered samples if not talkthru but LPF is on */
       if (!TalkOn && LpfOn) {
	 for (wTemp = 0; wTemp < SIZE_IO_BUFFER; wTemp++) {
	   genSignal();
	   pBuf[wTemp] = sFilter();
	 }
       }

/* . . fill buffer with unfiltered samples if not talkthru and not LPF */        
       if (!TalkOn && !LpfOn) {
	 for (wTemp = 0; wTemp < SIZE_IO_BUFFER; wTemp++) {
	   pBuf[wTemp] = genSignal();
	 }
       }

/* . . requeue buffer */
       wTemp = dspSendIo(hAlg, OutputDevice, pBuf, SIZE_IO_BUFFER);
       if (wTemp != DSPERR_SUCCESS) {
	 SendErrCount++;
	 SendLastErr = wTemp;
	 dspSendMsg (hApp, hAlg, DSPMSGID_STRING_PRINT, sizeof (STR_OUTPUTBUFFERREQUEUEFAILED), STR_OUTPUTBUFFERREQUEUEFAILED);
       }
       break;

/* . CASE (aborted) */
     case (DSPERR_IO_ABORT):

/* . . tell the application */
       dspSendMsg (hApp, hAlg, DSPMSGID_STRING_PRINT, sizeof (STR_IOABORTOUTPUTBUFFER), STR_IOABORTOUTPUTBUFFER);
       break;

/* . CASE (overrun) */
     case (DSPERR_NO_BUFFER):

/* . . tell the application */
       dspSendMsg (hApp, hAlg, DSPMSGID_STRING_PRINT, sizeof (STR_UNDERRUNOUTPUTBUFFER), STR_UNDERRUNOUTPUTBUFFER);
       algIoOverrun++;
       break;

/* . ENDCASE */
     }
     break;

/* CASE (message received from the host) */
   case DSPCB_GETMSG:

/* . parse it if there were no errors */
     if (wParam1 == DSPERR_SUCCESS) {
       ParseMsg(wParam1, wParam2, wParam3);
       if (dspGetMsg (hAlg, (int *)wParam2, sizeof(MSG_STRUCT)) != 0) {
	 dspSendMsg (hApp, hAlg, DSPMSGID_STRING_PRINT, sizeof (STR_GETMSGREQUEUEERROR), STR_GETMSGREQUEUEERROR);
       }
     } else {
       dspSendMsg (hApp, hAlg, DSPMSGID_STRING_PRINT, sizeof (STR_GETMSGERROR), STR_GETMSGERROR);
     }
     break;

/* CASE (messages we don't expect) */
   case DSPCB_GETDATA:
   case DSPCB_SENDDATA:
   case DSPCB_TIMER:

/* . tell the application */
     dspSendMsg (hApp, hAlg, DSPMSGID_STRING_PRINT, sizeof (STR_UNEXPECTEDCALLBACK), STR_UNEXPECTEDCALLBACK);
     break;

/* CASE (messages we don't care about) */
   case DSPCB_IDLE:

/* . do nothing */
     break;

/* CASE (System Error Message) */
    case DSPCB_ERROR:
      dspSendMsg (hApp, hAlg, DSPMSGID_STRING_PRINT, sizeof (STR_SYSTEMERROR), STR_SYSTEMERROR);
      break;

/* CASEELSE */
   default:

/* . tell the application */
     dspSendMsg (hApp, hAlg, DSPMSGID_STRING_PRINT, sizeof (STR_UNKNOWNCALLBACK), STR_UNKNOWNCALLBACK);
     break;

/* ENDCASE */
   }

/* return */
}

