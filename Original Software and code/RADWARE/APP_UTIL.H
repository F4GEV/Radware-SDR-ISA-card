/***************************************************************************
//
//      APP_UTIL.H
//
//      Copyright (c) 1993-94 Analog Devices, Inc.
//
//-------------------------------------------------------------------------
//
//      Module:     app_util.h
//      Purpose:    Header file for APP_UTIL.C
//
/**************************************************************************/

#include <stdio.h>             /* mostly for sprintf */
#include <ctype.h>             /* used for isupper, tolower */
#include "appmain.h"
#include "dspapi.h"            /* this has most of the #defines in it */


#define WAIT1 60000
#define WAIT2 1024

char *RegNames[16] =                    /* 1848 Register names, used for the */ 
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

int LastMsg = -1;
int BaseAddress = 0x220;
char DebugBuffer[200];

extern int Running;

extern HWND hWndMain;

extern int MessageId;
extern int MessageLen;                           
extern int MessageBuffer[MESSAGE_BUFFER_LEN];    

extern int MessageInId;
extern int MessageInLen;                         /* note: buffer contents
                                                  * length in BYTES*/
extern int MessageInBuffer[MESSAGE_BUFFER_LEN];  /* Currently defined as the
                                                  * size of the plot data. */
extern int MessageOutId;                         /* Otherwise, would be */
extern int MessageOutLen;                        /* DSP_MAXSTRINGLENGTH */

extern int MessageOutBuffer[MESSAGE_BUFFER_LEN];

int LastMessageId;

extern BOOL Persistance;
extern BOOL ShowMessages;
extern BOOL ShowPlot;
extern BOOL Echo;
extern BOOL Frequency;
extern BOOL Linear;

int PlotArray[200];
int PlotHold[200];

extern HWND hWndPlotCap;
extern HWND hWndPlot;
extern HWND hWndMessageCap;
extern HWND hWndPrintCap;
extern HWND hWndMessages;
extern HWND hWndPrints;

HPEN hPenGrey,hPenClear,hPenDraw;

/*-------------------------- END OF APP_UTIL.H ---------------------------*/