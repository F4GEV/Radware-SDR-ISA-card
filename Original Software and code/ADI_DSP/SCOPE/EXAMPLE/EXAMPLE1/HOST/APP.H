/**********************************************************************
//
//    APP.H
//
//    Copyright (c) 1993-94 Analog Devices, Inc.
//
//----------------------------------------------------------------------
//
//    Module:     app.h
//    Purpose:    header file for APP.C
//
***********************************************************************/




/************************************ algorithm/application messages */

typedef enum                       /* messages IDs sent to the dsp */
{
  msg_SET_NOISE=500,
  msg_ECHO_TOGGLE,     // 501
  msg_PLOT_TOGGLE,     // 502
  msg_GET_1848,        // 503
  msg_TALK_TOGGLE,     // 504
  msg_LOG_TOGGLE,      // 505
  msg_FREQ_TOGGLE,     // 506
  msg_LPF_TOGGLE,      // 507
  msg_HANN_TOGGLE,     // 508
  msg_PLOT_SEND        // 509
}MYMSGS;


#define msg_PLOT_DATA     500      /* messages IDs dsp sends up */
#define msg_1848_REG_DUMP 503
#define msg_ECHO_DATA     1001


/************************************ callback posted messages */

#define WM_DSPCB_REPLY  0x7fff
#define WM_DSPCB_RESET  0x7ffe

/************************************ 1848 stuff */

#define STEREO                    0x01
#define MONO                      0x00

typedef enum                       /* list of supported sample rates */
{
  FREQ_8K,
  FREQ_16K,
  FREQ_27_42857K,
  FREQ_32K,
  FREQ_48K,
  FREQ_9_6K,
  FREQ_5_5125K,
  FREQ_11_025K,
  FREQ_18_9K,
  FREQ_22_05K,
  FREQ_37_8K,
  FREQ_44_1K,
  FREQ_33_075K,
  FREQ_6_615K
}SAMPLE_RATES;  


char *RegNames[] =                      /* 1848 Register names, used for the */
{                                       /* register dump operation           */
"Input Controls(L)",
"Input Controls(R)",
"AuxIn#1 Ctrls(L)",
"AuxIn#1 Ctrls(R)",
"AuxIn#2 Ctrls(L)",
"AuxIn#2 Ctrls(R)",
"Output Control(L)",
"Output Control(R)",
"Data Format Ctrl",
"Interface Config",
"Pin Control",
"Test and Init",
"Miscellaneous",
"Digital Mix Ctrl",
"DMA Base Count(H)",
"DMA Base Count(L)"
};



/************************************* button stuff */

typedef struct buttonStruct
{
  long style;
  char *onText;
  char *offText;
  BOOL state;
}BUTTON_STRUCT;

#define BUTTONID_SINE1   700               // button IDs
#define BUTTONID_SINE2   701
#define BUTTONID_SINE3   702
#define BUTTONID_CHIRP   703
#define BUTTONID_CCITT   704
#define BUTTONID_TALK    705

typedef enum                     
{
  NOISE_SINE1,
  NOISE_SINE2,
  NOISE_SINE3,
  NOISE_CHIRP,
  NOISE_CCITT,
  NOISE_TALK
}NOISES;

