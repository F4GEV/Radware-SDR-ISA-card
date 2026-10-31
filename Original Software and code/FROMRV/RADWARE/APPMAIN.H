/**********************************************************************
//
//    APPMAIN.H
//
//    Copyright (c) 1993-94 Analog Devices, Inc.
//
//----------------------------------------------------------------------
//
//    Module:     appmain.h
//    Purpose:    central header file for all modules of APP.EXE
//
***********************************************************************/

#include <windows.h>
#include <mmsystem.h>
#include <string.h>
#include <f:\scope\include\dspshmgr.h>
#include "f:\scope\include\dspapi.h"                /* this has most of the #defines in it */


#define PLOT_HANDSHAKING           /* This dictates how plot information
                                      is transferred from the DSP to
                                      the host... is it just dumped
                                      as quickly as the DSP can go or
                                      does the DSP wait for the host
                                      to ask for it. IF DEFINED HERE, IT
                                      MUST ALSO BE DEFINED IN ALG.C !!! */

#define USING_CALLBACKS            /* Specifies that the DSP Manager is
                                      notifying us of new messages from
                                      the algorithm via the CallbackProc
                                      function, instead of having to
                                      poll dspGetMsg. Only needed here. */

#define MAX_FILENAME_LENGTH     500
#define MESSAGE_BUFFER_LEN      1000
extern HALG hMyAlg;

extern BOOL PausePlot;

typedef enum                       /* various messages I send to the dsp */
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


/*
 *     defines
 */

#define WM_MYMSG        0x7FFF

#define DLG_INIT        550
#define DLG_BASE        500
#define DLG_SINE        400
#define DLG_CHIRP       300
#define DLG_CCITT       200
#define DLG_1848        600
#define DLG_DUMP        700
#define DLG_VARIABLE    800
#define DLG_LOADALG     900

#define IDM_FILE                  1100
#define IDS_FILTERSTRING          1101
#define IDM_CONFIG                1000
#define IDM_SINE1                 1001
#define IDM_SINE2                 1002
#define IDM_SINE3                 1003
#define IDM_CHIRP                 1004
#define IDM_CCITT                 1005
#define IDM_1848_CONFIG           1006
#define IDM_1848_DUMP             1007
#define IDM_DUMP                  2000
#define IDM_DUMP_INC              2001
#define IDM_DUMP_DEC              2002
#define IDM_VAR                   4000
#define IDM_MSG                   5000
#define IDM_ECHO                  6000
#define IDM_PLOT                  7000
#define IDM_WATERFALL             7001
#define IDM_TIME                  7002
#define IDM_FREQUENCY             7003
#define IDM_LOG                   7004
#define IDM_LINEAR                7005
#define IDM_CLEAR                 7006
#define IDM_PERSIST               7007
#define IDM_QUIT                  8000
#define IDM_LPF                   9000
#define IDM_HANN                  9001

#define DSPMSG_PENDING            3000
#define DSPMSG_RECEIVED           3001
#define DSPMSG_RESET              3002
#define DSPMSG_ERROR              3003

#define IDS_ERR_REGISTER_CLASS    1
#define IDS_ERR_CREATE_WINDOW     2

#define WRN_TITLE "Warning"
#define ERR_TITLE "ERROR"
#define STRING_BUF_SIZE           75
                              
#define STEREO                    0x10
#define MONO                      0x00

/*
 *        Prototypes
 */

LONG FAR PASCAL WndProc(HWND, WORD, WORD, LONG);
BOOL FAR PASCAL SINEMsgProc(HWND, WORD, WORD, LONG);
BOOL FAR PASCAL CHIRPMsgProc(HWND, WORD, WORD, LONG);
BOOL FAR PASCAL CCITTMsgProc(HWND, WORD, WORD, LONG);
BOOL FAR PASCAL DUMPMsgProc(HWND, WORD, WORD, LONG);
BOOL FAR PASCAL VARMsgProc(HWND, WORD, WORD, LONG);
BOOL FAR PASCAL FILEMsgProc(HWND, WORD, WORD, LONG);
BOOL FAR PASCAL ADDRMsgProc(HWND, WORD, WORD, LONG);
BOOL FAR PASCAL CONFIGMsgProc(HWND, WORD, WORD, LONG);

int nCwRegisterClasses(void);
void CwUnRegisterClasses(void);
void swapWindows( void );
void cwCenter(HWND, int);
int getVarAddress(char *lpString, char *lpFileName);
void showError(char *lpString);
void WriteMessage(char *lpString);
void WritePrint(char *lpString);
void SetSine(int wButton);
void PlotIt(void);
void clearButtonsAndMenus(void);
void ParseMsg(DWORD dwMsgID);

/* ------------------------- END OF APPMAIN.H ----------------------------*/