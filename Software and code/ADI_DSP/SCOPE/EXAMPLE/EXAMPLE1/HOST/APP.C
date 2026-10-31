/***********************************************************************
//
//     APP.C
//
//        Copyright (c) 1993-94 Analog Devices, Inc.
//
//**********************************************************************
//
//        Module:     app.c
//        Purpose:    host module of the APP.EXE demo program
//
************************************************************************/

#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>
#include <io.h>

#include "dspmgr.h"

#include "dlgs.h"
#include "app.h"

/*********************************************** plot stuff */

#define ID_TIMER 1
#define TIMER_RATE 50

#define PLOT_DATA_SIZE 200         /* size of data in plot messages */
int PlotArray[PLOT_DATA_SIZE];
int PlotHold[PLOT_DATA_SIZE];

HPEN hPenGrey,hPenClear,hPenDraw;

/********************************************** load algorithm dialog stuff */

#define MAX_FILENAME_LENGTH     500
char SectionNames[MAX_FILENAME_LENGTH];


/********************************************** window position stuff */

#define STATIC_TOP 45
#define STATIC_SIZE 21

int cxChar,cyChar;
int wMainWinXMax;
int wMainWinYMax;


/********************************************* initialized globals */

long SampleRate = 8000;
int SampleRateEnum = FREQ_8K;
int WordSize = 16;
int Mode = MONO;

int Sine1Freq = 1000;
int Sine1Volume = 10000;
int Sine2Freq = 2000;
int Sine2Volume = 7000;
int Sine3Freq = 3000;
int Sine3Volume = 5000;
int WhichSine;

int ChirpMinFreq = 1000;
int ChirpMaxFreq = 2000;
int ChirpSpeed = 10000;
int ChirpVolumeVar = 15000;

int CcittVolume = 20000;

int Running = 0;

BOOL Persistance = FALSE;
BOOL ShowMessages = FALSE;
BOOL ShowPlot = FALSE;
BOOL PausePlot = FALSE;
BOOL Echo = FALSE;
BOOL Frequency = TRUE;
BOOL Linear = TRUE;
BOOL Lpf = FALSE;
BOOL Hann = FALSE;

/******************************************** uninitialized globals */

HANDLE hHotKeys;
FARPROC lpfnCallBack;
char szString[128];       /* variable to load resource strings         */
char szAppName[20];       /* class name for the window                 */
HALG hMyAlg;              /* Algorithm handle for api calls            */
LPHALG lphMyAlg;          /* pointer for dspLoadAlg call               */
                      

/******************************************* button stuff */

BUTTON_STRUCT button[]=            /* the actual button structure */
{
  { BS_PUSHBUTTON, "Sine1*","Sine1 ", FALSE },
  { BS_PUSHBUTTON, "Sine2*","Sine2 ", FALSE },
  { BS_PUSHBUTTON, "Sine3*","Sine3 ", FALSE },
  { BS_PUSHBUTTON, "Chirp*","Chirp ", FALSE },
  { BS_PUSHBUTTON, "Ccitt*","Ccitt ", FALSE },
  { BS_PUSHBUTTON, "Talk*", "Talk",   FALSE }
};


/******************************************* window handles */

HWND hInst;
HWND hWndMain;

#define NUM (sizeof button / sizeof button[0])
HWND hWndButton[NUM];

HWND hWndPlotCap;
HWND hWndPlot;
HWND hWndMessageCap;
HWND hWndPrintCap;
HWND hWndMessages;
HWND hWndPrints;

/******************************************* interrupt stuff */

int iInterruptsDisabled = 0;             // interrupts disabled semaphore


//****************************************** message stuff

#define MAXDSPMSGSIZE   512
#define MAXDSPMSGS      10

typedef struct _DSPMSG FAR* LPDSPMSG;
typedef struct _DSPMSG {
        LONG     lMsgID;
        DWORD    dwSize;
        LONG     lRequestID;
        LPDSPMSG lpNext;
        WORD     wBody [MAXDSPMSGSIZE/2];
} DSPMSG;

DSPMSG DSPMsgStack[MAXDSPMSGS];
LPDSPMSG lpFreeMsgQueue;
LPDSPMSG lpInboundMsgQueue;
LPDSPMSG lpOutboundMsgQueue;


/******************************************* function prototypes */

int PASCAL WinMain(HANDLE hInstance, HANDLE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow);
LONG FAR PASCAL WndProc(HWND hWnd, WORD Message, WORD wParam, LONG lParam);
BOOL FAR PASCAL SINEMsgProc(HWND hWndDlg, WORD Message, WORD wParam, LONG lParam);
BOOL FAR PASCAL CHIRPMsgProc(HWND hWndDlg, WORD Message, WORD wParam, LONG lParam);
BOOL FAR PASCAL CCITTMsgProc(HWND hWndDlg, WORD Message, WORD wParam, LONG lParam);
BOOL FAR PASCAL CONFIGMsgProc(HWND hWndDlg, WORD Message, WORD wParam, LONG lParam);
int nCwRegisterClasses(void);
VOID CwUnRegisterClasses(void);
VOID CALLBACK CallbackProc (HALG hAlg, DWORD dwMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2 );
VOID clearButtonsAndMenus(void);
VOID swapWindows(void);
VOID SetSine(int wButton);
LPDSPMSG AllocateMsg (void);
void FreeMsg (LPDSPMSG lpMsg);
void QueueMsg (LPDSPMSG *lpQueue, LPDSPMSG lpMsg);
void DequeueMsg (LPDSPMSG *lpQueue, LPDSPMSG lpMsg);
LPDSPMSG FindMsgGivenlRequestID (LPDSPMSG *lpQueue, LONG lRequestID);
void ProcessInboundReply (LPDSPMSG lpMsg);
void ProcessOutboundReply (LPDSPMSG lpMsg);
void DisableInterrupts (void);
void EnableInterrupts (void);
VOID PlotIt(void);
VOID WritePrint(LPSTR lpString);
VOID showError(LPSTR lpString);
VOID cwCenter(HWND hWnd, int top);
BOOL GetAnAlgName (LPSTR lpString);
BOOL FAR PASCAL LOADDSPMsgProc(HWND hWndDlg, WORD Message, WORD wParam, LONG lParam);
UINT WINAPI GetPrivateProfileSections( LPSTR lpOutBuf, UINT cbMax, LPSTR lpszFile);
LPSTR NEAR PASCAL FindEndOfLine( LPSTR lpLine, LPSTR lpEOF);





/**********************************************************************
//
// int    WinMain
//
// Description:
//            Main procedure
//
// Parameters:
//            HANDLE    hInstance;       handle for this instance                    
//            HANDLE    hPrevInstance;   handle for possible previous instances      
//            LPSTR     lpszCmdLine;     long pointer to exec command line           
//            int       nCmdShow;        Show code for main window display           
//
// Return Value:
//            int       Termination code to Windows
//
**********************************************************************/
#pragma        argsused

int PASCAL WinMain(HANDLE hInstance, HANDLE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow) {

  MSG        msg;                   /* MSG structure to store your messages */
  int        nRc;                   /* return value from Register Classes   */

// set the app name and save the instance
   strcpy(szAppName, "NOISE");
   hInst = hInstance;

// register all window classes
   if(!hPrevInstance) {
     if ((nRc = nCwRegisterClasses()) == -1) {
       LoadString(hInst, IDS_ERR_REGISTER_CLASS, szString, sizeof(szString));
       MessageBox(NULL, szString, NULL, MB_ICONSTOP);
       return nRc;
     }
   }

// create the main window
   hWndMain = CreateWindow(szAppName,               /* Window class name            */
                           "ADI SCOPE Demo Program",/* Window's title               */        
                           WS_CLIPCHILDREN |        /* don't draw in child windows areas */
                           WS_OVERLAPPEDWINDOW,
			   CW_USEDEFAULT, 0,        /* Use default X, Y            */
			   CW_USEDEFAULT, 0,        /* Use default X, Y            */
			   NULL,                    /* Parent window's handle      */
			   NULL,                    /* Default to Class Menu       */
			   hInst,                   /* Instance of window          */
			   NULL);                   /* Create struct for WM_CREATE */
   if(hWndMain == NULL) {
     LoadString(hInst, IDS_ERR_CREATE_WINDOW, szString, sizeof(szString));
     MessageBox(NULL, szString, NULL, MB_ICONSTOP);
     return IDS_ERR_CREATE_WINDOW;
   }

// load the keyboard accelerators
   hHotKeys = LoadAccelerators(hInst, "QuickKeys");

// allocate a timer for plot updates
   while (!SetTimer (hWndMain, ID_TIMER, TIMER_RATE, NULL)) {
     if (IDCANCEL == MessageBox (hWndMain, "Can't allocate a timer", szAppName, MB_ICONEXCLAMATION | MB_RETRYCANCEL)) {
       return (FALSE);
     }
   }

// put up the window
   ShowWindow(hWndMain, nCmdShow);

// process messages
   while (GetMessage (&msg, NULL, 0, 0)) {
     TranslateMessage (&msg);
     DispatchMessage (&msg);
   }

// do clean up before exiting from the application
   CwUnRegisterClasses();

// return
   return (msg.wParam);
}


/*********************************************************************
//
// LONG WndProc            *** Main Window Procedure ***
//
// Description:
//            This procedure provides service routines for the Windows events      
//            (messages) that Windows sends to the window, as well as the 
//            user initiated events (messages) that are generated when the 
//            user selectsthe action bar and pulldown menu controls or the 
//            corresponding keyboard accelerators.
//            
// Parameters:
//            HWND hWnd      - Handle of windows
//            WORD Message   - Message 
//            WORD wParam    - First message parameter
//            LONG lParam    - Second message parameter
//
// Return Value:
//            nothing
//
**********************************************************************/

LONG FAR PASCAL WndProc(HWND hWnd, WORD Message, WORD wParam, LONG lParam) {

  HDC        hDC;                /* handle for the display device */
  PAINTSTRUCT ps;                /* holds PAINT information       */  
  LONG       nRdsp=0;            /* return code for dsp mgr calls */
  DWORD dw;
  static WORD tabStop;
  char buffer[DSP_MAXSTRINGLENGTH];
  TEXTMETRIC tm;
  int i;
  LPDSPMSG lpMsg;

// CASEOF (message type)
   switch (Message) {

// CASE (timer message)
   case WM_TIMER:

// . get more plot data if plotting
     if (ShowPlot) {
       if ((lpMsg = AllocateMsg ()) == NULL) {
         WritePrint ("*** ERROR: Can't allocate a message buffer\n");
         return (0);
       }
       lpMsg->lMsgID = msg_PLOT_SEND;
       lpMsg->dwSize = 0;
       QueueMsg (&lpOutboundMsgQueue, lpMsg);
       lpMsg->lRequestID = dspSendMsg (hMyAlg, lpMsg->lMsgID, (LPVOID) lpMsg->wBody, lpMsg->dwSize);
       if (lpMsg->lRequestID > DSPERR_SUCCESS) {
       } else {
         ProcessOutboundReply (lpMsg);
       }
     }
     return (0);

// CASE (window commands)
   case WM_COMMAND:

// . CASEOF (specific command type)
     switch (wParam) {

// . CASE (SIN wave generation buttons)
     case BUTTONID_SINE1:
     case BUTTONID_SINE2:
     case BUTTONID_SINE3:

// . . compute index into button array
       i = wParam - BUTTONID_SINE1;

// . . toggle the button
       if(button[i].state) {
         button[i].state = FALSE;
       } else {
         button[i].state = TRUE;
       }

// . . check/uncheck the config pulldown
       switch(i) {
       case 0:
         CheckMenuItem(GetMenu(hWndMain), IDM_SINE1, (button[i].state ? MF_CHECKED:MF_UNCHECKED));
         break;
       case 1:
	 CheckMenuItem(GetMenu(hWndMain), IDM_SINE2, (button[i].state ? MF_CHECKED:MF_UNCHECKED));
         break;
       case 2:
         CheckMenuItem(GetMenu(hWndMain), IDM_SINE3, (button[i].state ? MF_CHECKED:MF_UNCHECKED));
         break;
       }

// . . process it
       SetSine(i);
       break;

// . CASE (CHIRP button)
     case BUTTONID_CHIRP:

// . . toggle the button and update menu stuff
       if (button[NOISE_CHIRP].state) {
         button[NOISE_CHIRP].state = FALSE;
         SendMessage(hWndButton[NOISE_CHIRP], WM_SETTEXT, 0, (DWORD)button[NOISE_CHIRP].offText);
         CheckMenuItem(GetMenu(hWndMain), IDM_CHIRP, MF_UNCHECKED);
       } else {
         button[NOISE_CHIRP].state = TRUE;
         SendMessage(hWndButton[NOISE_CHIRP], WM_SETTEXT, 0, (DWORD)button[NOISE_CHIRP].onText);
         CheckMenuItem(GetMenu(hWndMain), IDM_CHIRP, MF_CHECKED);
       }

// . . repaint the button
       InvalidateRect(hWndButton[NOISE_CHIRP], NULL, TRUE);

// . . prepare a message to tell the algorithm about the chirp parameters
       if ((lpMsg = AllocateMsg ()) == NULL) {
         WritePrint ("*** ERROR: Can't allocate a message buffer\n");
         break;
       }
       lpMsg->lMsgID = msg_SET_NOISE;
       lpMsg->dwSize = 12;
       lpMsg->wBody[0] = NOISE_CHIRP; 
       if (button[NOISE_CHIRP].state) {
 	 lpMsg->wBody[1] = ChirpVolumeVar;
       } else {
 	 lpMsg->wBody[1] = 0;
       }
       lpMsg->wBody[2] = ChirpMinFreq;
       lpMsg->wBody[3] = ChirpMaxFreq;
       lpMsg->wBody[4] = ChirpSpeed;
       lpMsg->wBody[5] = 0;

// . . send it
       QueueMsg (&lpOutboundMsgQueue, lpMsg);
       lpMsg->lRequestID = dspSendMsg (hMyAlg, lpMsg->lMsgID, (LPVOID) lpMsg->wBody, lpMsg->dwSize);
       if (lpMsg->lRequestID > DSPERR_SUCCESS) {
         break;
       } else {
         ProcessOutboundReply (lpMsg);
       }
       break;

// . CASE (CCITT button)
     case BUTTONID_CCITT:

// . . toggle the button
       if(button[NOISE_CCITT].state) {
 	 button[NOISE_CCITT].state = FALSE;
 	 SendMessage(hWndButton[NOISE_CCITT], WM_SETTEXT, 0, (DWORD)button[NOISE_CCITT].offText);
         CheckMenuItem(GetMenu(hWndMain), IDM_CCITT, MF_UNCHECKED);
       } else {
         button[NOISE_CCITT].state = TRUE;
         SendMessage(hWndButton[NOISE_CCITT], WM_SETTEXT, 0, (DWORD)button[NOISE_CCITT].onText);
         CheckMenuItem(GetMenu(hWndMain), IDM_CCITT, MF_CHECKED);
       }

// . . prepare a message to tell the algorithm about the CCITT parameters
       if ((lpMsg = AllocateMsg ()) == NULL) {
         WritePrint ("*** ERROR: Can't allocate a message buffer\n");
         break;
       }
       lpMsg->lMsgID = msg_SET_NOISE;
       lpMsg->dwSize = 4;
       lpMsg->wBody[0] = NOISE_CCITT; 
       if (button[NOISE_CCITT].state) {
 	 lpMsg->wBody[1] = CcittVolume;
       } else {
 	 lpMsg->wBody[1] = 0;
       }

// . . send it
       QueueMsg (&lpOutboundMsgQueue, lpMsg);
       lpMsg->lRequestID = dspSendMsg (hMyAlg, lpMsg->lMsgID, (LPVOID) lpMsg->wBody, lpMsg->dwSize);
       if (lpMsg->lRequestID > DSPERR_SUCCESS) {
         break;
       } else {
         ProcessOutboundReply (lpMsg);
       }
       break;

// . CASE (TALK button)
     case BUTTONID_TALK:

// . . toggle the button
       if(button[NOISE_TALK].state) {
         button[NOISE_TALK].state = FALSE;
	 SendMessage(hWndButton[NOISE_TALK], WM_SETTEXT, 0, (DWORD)button[NOISE_TALK].offText);
       } else {
         button[NOISE_TALK].state = TRUE;
         SendMessage(hWndButton[NOISE_TALK], WM_SETTEXT, 0, (DWORD)button[NOISE_TALK].onText);
       }

// . . prepare a message to tell the algorithm about the talk state
       if ((lpMsg = AllocateMsg ()) == NULL) {
         WritePrint ("*** ERROR: Can't allocate a message buffer\n");
         break;
       }
       lpMsg->lMsgID = msg_TALK_TOGGLE;
       lpMsg->dwSize = 0;

// . . send it
       QueueMsg (&lpOutboundMsgQueue, lpMsg);
       lpMsg->lRequestID = dspSendMsg (hMyAlg, lpMsg->lMsgID, (LPVOID) lpMsg->wBody, lpMsg->dwSize);
       if (lpMsg->lRequestID > DSPERR_SUCCESS) {
         break;
       } else {
         ProcessOutboundReply (lpMsg);
       }
       break;


//. CASE (FILE button)
    case IDM_FILE:

// . . BEGIN
       {    
        char lpsLoadFile[MAX_FILENAME_LENGTH];
        BOOL i;
	
// . . . ask the user for info on what he wants to load
         i = GetAnAlgName((LPSTR) &lpsLoadFile);

// . . . IF (user gave us an algorithm)
         if (i) {

// . . . . Unload the current algorithm
           nRdsp=dspUnloadAlg( hMyAlg );
           if (nRdsp < 0) {    
             dspGetErrorInfo (nRdsp, (LPSTR) &buffer, DSP_MAXSTRINGLENGTH);
	     strcat (buffer, "\n");
	     WritePrint(buffer);
	     break;
	   }

// . . . . reset all buttons, menus, etc.
           clearButtonsAndMenus();

// . . . . make a new callback instance
           FreeProcInstance(lpfnCallBack);
           lpfnCallBack = MakeProcInstance((FARPROC) CallbackProc, hInst );

// . . . . load the algorithm
           if( (nRdsp = dspLoadAlg( DSP_MAPPER, (DWORD) hWnd, (LPHALG) &hMyAlg, lpsLoadFile, (DWORD) lpfnCallBack, CALLBACK_FUNCTION) ) == 0) {
             Running = TRUE;
	   } else {
	     dspGetErrorInfo (nRdsp, (LPSTR) &buffer, DSP_MAXSTRINGLENGTH);
	     strcat (buffer, "\n");
	     MessageBox(hWnd, buffer, "ERROR", MB_OK);
	   }

// . . . ENDIF
         }

// . . END
       }
       break; 
      

// . CASE (Quit)
     case IDM_QUIT:

// . . process it
       PostMessage(hWnd, WM_CLOSE, 0, 0L);
       break;
      

// . CASE (SINE Configuration)
     case IDM_SINE1:
     case IDM_SINE2:
     case IDM_SINE3:

// . . BEGIN
       {
          FARPROC lpfnSINEMsgProc;

// . . . get the info via the dialog box
         WhichSine = wParam - IDM_SINE1;
	 lpfnSINEMsgProc = MakeProcInstance((FARPROC)SINEMsgProc, hInst);
	 DialogBox(hInst, MAKEINTRESOURCE(DLG_SINE), hWnd, lpfnSINEMsgProc);
	 FreeProcInstance(lpfnSINEMsgProc);

// . . END
       }
       break;


// . CASE (Chirp config)
     case IDM_CHIRP:

// . . BEGIN
       {
         FARPROC lpfnCHIRPMsgProc;
	
// . . . get the info via the dialog
         lpfnCHIRPMsgProc = MakeProcInstance((FARPROC)CHIRPMsgProc, hInst);
         DialogBox(hInst, MAKEINTRESOURCE(DLG_CHIRP), hWnd, lpfnCHIRPMsgProc);
         FreeProcInstance(lpfnCHIRPMsgProc);
	
// . . END
       }
       break;
      

// . CASE (CCITT config)
    case IDM_CCITT:

// . . BEGIN
       {
         FARPROC lpfnCCITTMsgProc;

// . . . get the info via the dialog
         lpfnCCITTMsgProc = MakeProcInstance((FARPROC)CCITTMsgProc, hInst);
         DialogBox(hInst, MAKEINTRESOURCE(DLG_CCITT), hWnd, lpfnCCITTMsgProc);
         FreeProcInstance(lpfnCCITTMsgProc);
	
// . . END
       }
       break;


// . CASE (1848 config)
     case IDM_1848_CONFIG:

// . . BEGIN
       {
         FARPROC lpfnCONFIGMsgProc;
	
// . . . IF (user gives us new info)
         lpfnCONFIGMsgProc = MakeProcInstance((FARPROC)CONFIGMsgProc, hInst);
         if(DialogBox(hInst, MAKEINTRESOURCE(DLG_1848), hWnd, lpfnCONFIGMsgProc)) {

// . . . . prepare a message to tell the algorithm about the rate info
           if ((lpMsg = AllocateMsg ()) == NULL) {
             WritePrint ("*** ERROR: Can't allocate a message buffer\n");
             break;
           }
           lpMsg->lMsgID = DSPMSGID_AUDIO_SETRATE;
           lpMsg->dwSize = 4;
           lpMsg->wBody[0] = LOWORD (SampleRate); 
           lpMsg->wBody[1] = HIWORD (SampleRate);

// . . . . send it
           QueueMsg (&lpOutboundMsgQueue, lpMsg);
           lpMsg->lRequestID = dspSendMsg (hMyAlg, lpMsg->lMsgID, (LPVOID) lpMsg->wBody, lpMsg->dwSize);
           if (lpMsg->lRequestID > DSPERR_SUCCESS) {
           } else {
             ProcessOutboundReply (lpMsg);
           }

// . . . . prepare a message to tell the algorithm about the new mode info
           if ((lpMsg = AllocateMsg ()) == NULL) {
             WritePrint ("*** ERROR: Can't allocate a message buffer\n");
             break;
           }
           lpMsg->lMsgID = DSPMSGID_AUDIO_SETMODE;
           lpMsg->dwSize = 2;
           lpMsg->wBody[0] = Mode; 

// . . . . send it
           QueueMsg (&lpOutboundMsgQueue, lpMsg);
           lpMsg->lRequestID = dspSendMsg (hMyAlg, lpMsg->lMsgID, (LPVOID) lpMsg->wBody, lpMsg->dwSize);
           if (lpMsg->lRequestID > DSPERR_SUCCESS) {
           } else {
             ProcessOutboundReply (lpMsg);
           }

// . . . ENDIF
         }
         FreeProcInstance(lpfnCONFIGMsgProc);

// . . END
       }
       break;

// . CASE (Echo toggle)
     case IDM_ECHO:

// . . toggle the check
       if(Echo) {
         Echo = FALSE;
         CheckMenuItem(GetMenu(hWndMain), IDM_ECHO, MF_UNCHECKED);
       } else {
         Echo = TRUE;
         CheckMenuItem(GetMenu(hWndMain), IDM_ECHO, MF_CHECKED);
       }

// . . prepare a message to tell the algorithm about the echo
       if ((lpMsg = AllocateMsg ()) == NULL) {
         WritePrint ("*** ERROR: Can't allocate a message buffer\n");
         break;
       }
       lpMsg->lMsgID = msg_ECHO_TOGGLE;
       lpMsg->dwSize = 0;

// . . send it
       QueueMsg (&lpOutboundMsgQueue, lpMsg);
       lpMsg->lRequestID = dspSendMsg (hMyAlg, lpMsg->lMsgID, (LPVOID) lpMsg->wBody, lpMsg->dwSize);
       if (lpMsg->lRequestID > DSPERR_SUCCESS) {
         break;
       } else {
         ProcessOutboundReply (lpMsg);
       }
       break;

// . CASE (1848 register dump)
     case IDM_1848_DUMP:

// . . prepare a message to tell the algorithm to provide us the 1848 info
       if ((lpMsg = AllocateMsg ()) == NULL) {
         WritePrint ("*** ERROR: Can't allocate a message buffer\n");
         break;
       }
       lpMsg->lMsgID = msg_GET_1848;
       lpMsg->dwSize = 0;

// . . send it
       QueueMsg (&lpOutboundMsgQueue, lpMsg);
       lpMsg->lRequestID = dspSendMsg (hMyAlg, lpMsg->lMsgID, (LPVOID) lpMsg->wBody, lpMsg->dwSize);
       if (lpMsg->lRequestID > DSPERR_SUCCESS) {
         break;
       } else {
         ProcessOutboundReply (lpMsg);
       }
       break;


// . CASE (Plotting toggle)
     case IDM_PLOT:

// . . prepare a message to tell the algorithm to start plotting
       if ((lpMsg = AllocateMsg ()) == NULL) {
         WritePrint ("*** ERROR: Can't allocate a message buffer\n");
         break;
       }
       lpMsg->lMsgID = msg_PLOT_TOGGLE;
       lpMsg->dwSize = 0;

// . . send it
       QueueMsg (&lpOutboundMsgQueue, lpMsg);
       lpMsg->lRequestID = dspSendMsg (hMyAlg, lpMsg->lMsgID, (LPVOID) lpMsg->wBody, lpMsg->dwSize);
       if (lpMsg->lRequestID > DSPERR_SUCCESS) {
       } else {
         ProcessOutboundReply (lpMsg);
       }

// . . Adjust state of all plot options according to new settings
       if(ShowPlot) {
         ShowPlot = FALSE;
         EnableMenuItem(GetMenu(hWndMain), IDM_TIME,      MF_GRAYED);
         EnableMenuItem(GetMenu(hWndMain), IDM_FREQUENCY, MF_GRAYED);
         EnableMenuItem(GetMenu(hWndMain), IDM_LOG,       MF_GRAYED);
         EnableMenuItem(GetMenu(hWndMain), IDM_LINEAR,    MF_GRAYED);
         EnableMenuItem(GetMenu(hWndMain), IDM_PERSIST,   MF_GRAYED);
         EnableMenuItem(GetMenu(hWndMain), IDM_CLEAR,     MF_GRAYED);
         CheckMenuItem(GetMenu(hWndMain),   IDM_PLOT,      MF_UNCHECKED);
       } else {
         ShowPlot = TRUE;
         EnableMenuItem(GetMenu(hWndMain), IDM_TIME,      MF_ENABLED);
         EnableMenuItem(GetMenu(hWndMain), IDM_FREQUENCY, MF_ENABLED);
         EnableMenuItem(GetMenu(hWndMain), IDM_LOG,       MF_ENABLED);
         EnableMenuItem(GetMenu(hWndMain), IDM_LINEAR,    MF_ENABLED);
         EnableMenuItem(GetMenu(hWndMain), IDM_PERSIST,   MF_ENABLED);
         EnableMenuItem(GetMenu(hWndMain), IDM_CLEAR,     MF_ENABLED);
         CheckMenuItem(GetMenu(hWndMain),   IDM_PLOT,      MF_CHECKED);
       }
       swapWindows();
       break;

// . CASE (TIME toggle)
     case IDM_TIME:

// . . IF (plotting)
       if(ShowPlot) {

// . . . IF (showing frequency)
         if (Frequency) {

// . . . . prepare a message to tell the algorithm to switch modes
           if ((lpMsg = AllocateMsg ()) == NULL) {
             WritePrint ("*** ERROR: Can't allocate a message buffer\n");
             break;
           }
           lpMsg->lMsgID = msg_FREQ_TOGGLE;
           lpMsg->dwSize = 0;

// . . . . send it
           QueueMsg (&lpOutboundMsgQueue, lpMsg);
           lpMsg->lRequestID = dspSendMsg (hMyAlg, lpMsg->lMsgID, (LPVOID) lpMsg->wBody, lpMsg->dwSize);
           if (lpMsg->lRequestID > DSPERR_SUCCESS) {
             break;
           } else {
             ProcessOutboundReply (lpMsg);
           }

// . . . . toggle the menu status
           Frequency = FALSE;
	   CheckMenuItem(GetMenu(hWndMain), IDM_TIME, MF_CHECKED);
	   CheckMenuItem(GetMenu(hWndMain), IDM_FREQUENCY, MF_UNCHECKED);
	   InvalidateRect(hWndPlot,NULL,TRUE);

// . . . ENDIF
         }

// . . ENDIF
       }
       break;

// . CASE (Frequency toggle)
     case IDM_FREQUENCY:
      
// . . IF (plotting)
       if(ShowPlot) {

// . . . IF (not showing frequency)
         if (!Frequency) {

// . . . . prepare a message to tell the algorithm about the frequency toggle
           if ((lpMsg = AllocateMsg ()) == NULL) {
             WritePrint ("*** ERROR: Can't allocate a message buffer\n");
             break;
           }
           lpMsg->lMsgID = msg_FREQ_TOGGLE;
           lpMsg->dwSize = 0;

// . . . . send it
           QueueMsg (&lpOutboundMsgQueue, lpMsg);
           lpMsg->lRequestID = dspSendMsg (hMyAlg, lpMsg->lMsgID, (LPVOID) lpMsg->wBody, lpMsg->dwSize);
           if (lpMsg->lRequestID > DSPERR_SUCCESS) {
           } else {
             ProcessOutboundReply (lpMsg);
           }

// . . . . toggle the menu status
           Frequency = TRUE;
	   CheckMenuItem(GetMenu(hWndMain), IDM_FREQUENCY, MF_CHECKED);
	   CheckMenuItem(GetMenu(hWndMain), IDM_TIME, MF_UNCHECKED);
	   InvalidateRect(hWndPlot,NULL,TRUE);

// . . . ENDIF
         }

// . . ENDIF
       }
       break;


// . CASE (log toggle)
     case IDM_LOG:

// . . IF (plotting)
       if(ShowPlot) {

// . . . IF (plotting linear)
         if (Linear) {

// . . . . prepare a message to tell the algorithm about the linear toggle
           if ((lpMsg = AllocateMsg ()) == NULL) {
             WritePrint ("*** ERROR: Can't allocate a message buffer\n");
             break;
           }
           lpMsg->lMsgID = msg_LOG_TOGGLE;
           lpMsg->dwSize = 0;

// . . . . send it
           QueueMsg (&lpOutboundMsgQueue, lpMsg);
           lpMsg->lRequestID = dspSendMsg (hMyAlg, lpMsg->lMsgID, (LPVOID) lpMsg->wBody, lpMsg->dwSize);
           if (lpMsg->lRequestID > DSPERR_SUCCESS) {
           } else {
             ProcessOutboundReply (lpMsg);
           }

// . . . . toggle the menu status
           Linear = FALSE;
	   CheckMenuItem(GetMenu(hWndMain), IDM_LOG, MF_CHECKED);
	   CheckMenuItem(GetMenu(hWndMain), IDM_LINEAR, MF_UNCHECKED);

// . . . ENDIF
         }

// . . ENDIF
       }
       break;


 // . CASE (linear toggle)
      case IDM_LINEAR:

 // . . IF (plotting)
        if(ShowPlot) {

 // . . . IF (not plotting linear)
          if (!Linear) {

// . . . . prepare a message to tell the algorithm about the log toggle
           if ((lpMsg = AllocateMsg ()) == NULL) {
             WritePrint ("*** ERROR: Can't allocate a message buffer\n");
             break;
           }
           lpMsg->lMsgID = msg_LOG_TOGGLE;
           lpMsg->dwSize = 0;

// . . . . send it
           QueueMsg (&lpOutboundMsgQueue, lpMsg);
           lpMsg->lRequestID = dspSendMsg (hMyAlg, lpMsg->lMsgID, (LPVOID) lpMsg->wBody, lpMsg->dwSize);
           if (lpMsg->lRequestID > DSPERR_SUCCESS) {
           } else {
             ProcessOutboundReply (lpMsg);
           }

// . . . . toggle the menu status
           Linear = TRUE;
	   CheckMenuItem(GetMenu(hWndMain), IDM_LINEAR, MF_CHECKED);
	   CheckMenuItem(GetMenu(hWndMain), IDM_LOG, MF_UNCHECKED);

// . . . ENDIF
         }

// . . ENDIF
       }
       break;


// . CASE (Persist toggle)
     case IDM_PERSIST:

// . . IF (plotting)
       if(ShowPlot) {

// . . . toggle the mode
         if(Persistance) {
	   Persistance = FALSE;
	   CheckMenuItem(GetMenu(hWndMain), IDM_PERSIST, MF_UNCHECKED);
	 } else {
	   Persistance = TRUE;
	   CheckMenuItem(GetMenu(hWndMain), IDM_PERSIST, MF_CHECKED);
	 }
	 InvalidateRect(hWndPlot,NULL,TRUE);

// . . ENDIF
       }
       break;

// . CASE (clear the plot window)
     case IDM_CLEAR:

// . . clear it
       if(ShowPlot) {
         InvalidateRect(hWndPlot,NULL,TRUE);
       }
       break;

// . CASE (LPF toggle) 
     case IDM_LPF:

// . . prepare a message to tell the algorithm to toggle the filter
       if ((lpMsg = AllocateMsg ()) == NULL) {
         WritePrint ("*** ERROR: Can't allocate a message buffer\n");
         break;
       }
       lpMsg->lMsgID = msg_LPF_TOGGLE;
       lpMsg->dwSize = 0;

// . . send it
       QueueMsg (&lpOutboundMsgQueue, lpMsg);
       lpMsg->lRequestID = dspSendMsg (hMyAlg, lpMsg->lMsgID, (LPVOID) lpMsg->wBody, lpMsg->dwSize);
       if (lpMsg->lRequestID > DSPERR_SUCCESS) {
         break;
       } else {
         ProcessOutboundReply (lpMsg);
       }

// . . update the menus
       if(Lpf) {
         Lpf = FALSE;
         CheckMenuItem(GetMenu(hWndMain), IDM_LPF, MF_UNCHECKED);
       } else {
         Lpf = TRUE;
         CheckMenuItem(GetMenu(hWndMain), IDM_LPF, MF_CHECKED);
       }
       break;
      
// . CASE (hanning filter toggle)
     case IDM_HANN:
      
// . . prepare a message to tell the algorithm to toggle the filter
       if ((lpMsg = AllocateMsg ()) == NULL) {
         WritePrint ("*** ERROR: Can't allocate a message buffer\n");
         break;
       }
       lpMsg->lMsgID = msg_HANN_TOGGLE;
       lpMsg->dwSize = 0;

// . . send it
       QueueMsg (&lpOutboundMsgQueue, lpMsg);
       lpMsg->lRequestID = dspSendMsg (hMyAlg, lpMsg->lMsgID, (LPVOID) lpMsg->wBody, lpMsg->dwSize);
       if (lpMsg->lRequestID > DSPERR_SUCCESS) {
         break;
       } else {
         ProcessOutboundReply (lpMsg);
       }

// . . update the menus
       if(Hann) {
         Hann = FALSE;
         CheckMenuItem(GetMenu(hWndMain), IDM_HANN, MF_UNCHECKED);
       } else {
         Hann = TRUE;
         CheckMenuItem(GetMenu(hWndMain), IDM_HANN, MF_CHECKED);
       }
       break;
      
// . CASEELSE
     default:

// . . let windows process it
       return DefWindowProc(hWnd, Message, wParam, lParam);

// . ENDCASE
     }
     break;

// CASE (the DSP has been reset)
   case WM_DSPCB_RESET:

// . alert the user cause there's nothing we can do
     MessageBox(hWndMain, "The DSP has been reset. Please reload the algorithm.", "Alert", MB_ICONSTOP);
     break;

// CASE (reply to a message being sent to or received from the DSP)
   case WM_DSPCB_REPLY:

// . process outbound message replies
     if ((lpMsg = FindMsgGivenlRequestID (&lpOutboundMsgQueue, lParam)) != NULL) {
       ProcessOutboundReply (lpMsg);
       break;
     }

// . process inbound message replies
     if ((lpMsg = FindMsgGivenlRequestID (&lpInboundMsgQueue, lParam)) != NULL) {
       ProcessInboundReply (lpMsg);
       break;
     }
     break;

// CASE (create window)
   case WM_CREATE:

// . BEGIN
     {
       int xmax;
       int ymax;
       int winsize;
      
// . . initialize the message lists
       for (i = 0; i < MAXDSPMSGS; i++) {
         DSPMsgStack[i].lpNext = &DSPMsgStack[i + 1];
       }
       DSPMsgStack[MAXDSPMSGS - 1].lpNext = NULL;
       lpFreeMsgQueue = DSPMsgStack;
       lpInboundMsgQueue = NULL;
       lpOutboundMsgQueue = NULL;

// . . get text info
       hDC = GetDC(hWnd);
       dw = GetWindowExt(hDC);
       xmax = LOWORD(dw);
       ymax = HIWORD(dw);
       SelectObject(hDC, GetStockObject(SYSTEM_FIXED_FONT));
       GetTextMetrics(hDC, &tm);
       cxChar = tm.tmAveCharWidth;
       cyChar = tm.tmHeight + tm.tmExternalLeading;
       ReleaseDC(hWnd, hDC);

// . . draw the buttons
       for(i=0; i<NUM; i++) {
         button[i].state = FALSE;
         hWndButton[i] = CreateWindow("button", button[i].offText,
                                      WS_CHILD | WS_VISIBLE | button[i].style,
                                      cxChar*i*9+5, cyChar,
		        cxChar*8, cyChar+6,
		        hWnd, BUTTONID_SINE1 + i,
		        ((LPCREATESTRUCT) lParam)->hInstance, NULL);
       }

// . . setup the main windows
       winsize = (ymax-STATIC_TOP-(STATIC_SIZE*2))/2 - 10;
       hWndPlotCap = CreateWindow("static", "Plot", WS_CHILD |
                                  WS_VISIBLE |
		    WS_BORDER | SS_CENTER,
		    xmax/2,STATIC_TOP,
		    xmax/2-5,STATIC_SIZE,
		    hWnd, 1,
		    ((LPCREATESTRUCT) lParam)->hInstance, NULL);
       hWndPlot = CreateWindow("static", "", WS_CHILD |
                    WS_VISIBLE | WS_BORDER |
	         SS_CENTER,
	         xmax/2,STATIC_TOP+STATIC_SIZE-1,
	         xmax/2-5,ymax-STATIC_TOP-STATIC_SIZE-10-1,
	         hWnd, 1,
	         ((LPCREATESTRUCT) lParam)->hInstance, NULL);
       hWndMessageCap = CreateWindow("static", "Messages", WS_CHILD |
                                 WS_VISIBLE |
		       WS_BORDER | SS_CENTER,
		       5,STATIC_TOP+STATIC_SIZE+winsize+10,
		       (xmax/2)-10,STATIC_SIZE,
		       hWnd, 1,
	       ((LPCREATESTRUCT) lParam)->hInstance, NULL);
       hWndMessages = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE |
                               WS_HSCROLL | WS_VSCROLL  | WS_BORDER |
		     ES_LEFT | ES_MULTILINE | ES_AUTOHSCROLL |
		     ES_AUTOVSCROLL,
		     5,STATIC_TOP+winsize+STATIC_SIZE*2+10-1,
		     (xmax/2)-10,winsize,
		     hWnd, 1,
		     ((LPCREATESTRUCT) lParam)->hInstance, NULL);
       hWndPrintCap = CreateWindow("static", "DSP Prints", WS_CHILD |
		     WS_VISIBLE | WS_BORDER |
		     SS_CENTER,
		     5,STATIC_TOP,
		     (xmax/2)-10,STATIC_SIZE,
		     hWnd, 1,
		     ((LPCREATESTRUCT) lParam)->hInstance, NULL);
       hWndPrints = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE |
		   WS_HSCROLL | WS_VSCROLL  | WS_BORDER |
		   ES_LEFT | ES_MULTILINE | ES_AUTOHSCROLL |
		   ES_AUTOVSCROLL,
		   5,STATIC_TOP+STATIC_SIZE-1,
		   (xmax/2)-10,winsize,
		   hWnd, 1,
		   ((LPCREATESTRUCT) lParam)->hInstance, NULL);
       tabStop = 4;
       SendMessage(hWndMessages, EM_SETTABSTOPS, 1, (DWORD)&tabStop);
       InvalidateRect(hWnd, NULL, TRUE);

// . . thunk the callback
       lpfnCallBack = MakeProcInstance((FARPROC) CallbackProc, hInst );
      
// . . load the algorithm
       if ( dspGetNumDevs() ) {
         nRdsp = dspLoadAlg (0, (DWORD) hWnd, &hMyAlg, "alg", (DWORD) lpfnCallBack, CALLBACK_FUNCTION );
         if (nRdsp < 0) {    
           dspGetErrorInfo (nRdsp, (LPSTR) &buffer, DSP_MAXSTRINGLENGTH);
           strcat (buffer, "\n");
           WritePrint(buffer);
         }
         Running = TRUE;
       } else {
         sprintf(buffer, "\ndspGetNumDevs () says there are no devices");
         WritePrint(buffer);
       }

// . . create the pens
       hPenClear = CreatePen(PS_SOLID, 0, GetSysColor(COLOR_WINDOW));
       hPenDraw  = CreatePen(PS_SOLID, 0, GetSysColor(COLOR_WINDOWTEXT));
       hPenGrey  = CreatePen(PS_SOLID, 0, GetSysColor(COLOR_GRAYTEXT));
      
// . END
     }
     break;

// CASE (resize)
   case WM_SIZE:

// . BEGIN               
     {
       int xmax;
       int ymax;
       int winsize;
       int prntsize;
      
// . . figure out the new coordinates
       xmax = LOWORD(lParam);
       ymax = HIWORD(lParam);
       wMainWinXMax = xmax;
       wMainWinYMax = ymax;
      
// . . resize (watch out for plotting)
       if(!ShowPlot) {
        winsize = (ymax-STATIC_TOP-(STATIC_SIZE*2))/2 - 10;
        MoveWindow(hWndPlotCap, xmax/2,ymax, xmax-10, STATIC_SIZE, TRUE);
        MoveWindow(hWndPlot, xmax/2,ymax, xmax-10, ymax-STATIC_SIZE-STATIC_TOP-10-1, TRUE);
        MoveWindow(hWndMessageCap, 5,STATIC_TOP, xmax-10, STATIC_SIZE, TRUE);
        MoveWindow(hWndMessages, 5,STATIC_TOP+STATIC_SIZE-1, xmax-10, winsize, TRUE);
        MoveWindow(hWndPrintCap, 5, STATIC_TOP+STATIC_SIZE+winsize+10, xmax-10, STATIC_SIZE, TRUE);
        MoveWindow(hWndPrints, 5, STATIC_TOP+STATIC_SIZE*2+winsize+10-1, xmax-10, winsize, TRUE);
      } else {
        winsize = (ymax-STATIC_TOP)*2/3;
        prntsize = ymax-STATIC_TOP-winsize;
        MoveWindow(hWndPlotCap, 5,STATIC_TOP, xmax-10, STATIC_SIZE, TRUE);
        MoveWindow(hWndPlot, 5,STATIC_TOP+STATIC_SIZE-1, xmax-10, winsize-STATIC_SIZE-5, TRUE);
        MoveWindow(hWndMessageCap, 5,ymax, (xmax/2)-10, STATIC_SIZE, TRUE);
        MoveWindow(hWndMessages, 5,ymax, (xmax/2)-10, winsize, TRUE);
        MoveWindow(hWndPrintCap, 5, STATIC_TOP+winsize, xmax-10, STATIC_SIZE, TRUE);
        MoveWindow(hWndPrints, 5, STATIC_TOP+STATIC_SIZE+winsize-1, xmax-10, prntsize-STATIC_SIZE, TRUE);
      }

// . END
     }
     break;

// CASE (paint message)
   case WM_PAINT:        /* Obtain a handle to the device context
			   BeginPaint will sends WM_ERASEBKGND if appropriate  */

// . do it
     memset(&ps, 0x00, sizeof(PAINTSTRUCT));
     hDC = BeginPaint(hWnd, &ps);
     SetBkMode(hDC, TRANSPARENT);
     EndPaint(hWnd, &ps);
     break;

// CASE (closing down)
   case WM_CLOSE:  

// . unload our algorithm
     dspUnloadAlg( hMyAlg );

// . destroy all windows
     DeleteObject(hPenClear);
     DeleteObject(hPenGrey);
     DeleteObject(hPenDraw);
     DestroyWindow(hWnd);
     if (hWnd == hWndMain) {
       PostQuitMessage(0);
     }
     break;

// CASEELSE
   default:

// . let windows process it
     return DefWindowProc(hWnd, Message, wParam, lParam);

// ENDCASE
   }

// return
   return (0L);
}



/*********************************************************************
//
// BOOL     SINEMsgProc
//
// Description:        Sine dialog window procedure
//            This procedure is associated with the Sine dialog box. 
//            It provides the service routines for the events (messages)  
//            that occur because the end user operates one of the 
//            dialog box's buttons, entry fields, or controls.
//            
// Parameters:
//            HWND     hWndDlg       - Handle of the dialog box
//            WORD     Message       - Message ID coming from the dialog box
//            WORD     wParam        - First message dependent parameter
//            LONG     lParam        - Second message dependent parameter
//
// Return Value:
//            BOOL     Termination status
//
**********************************************************************/

BOOL FAR PASCAL SINEMsgProc(HWND hWndDlg, WORD Message, WORD wParam, LONG lParam) {

   HWND hWndScroll;
   BOOL junk;
   int tempSineFreq;
   int tempSineVolume;
  
// CASEOF (message)
   switch(Message) {

// CASE (initialization)
   case WM_INITDIALOG:

// . center the dialog
     cwCenter(hWndDlg, 0);

// . initialize variables
     switch(WhichSine) {
     case 0:
       tempSineVolume = Sine1Volume;
       tempSineFreq = Sine1Freq;
       break;
     case 1:
       tempSineVolume = Sine2Volume;
       tempSineFreq = Sine2Freq;
       break;
     case 2:
       tempSineVolume = Sine3Volume;
       tempSineFreq = Sine3Freq;
       break;
     default:
       break;
     }

// . set the values
     SetDlgItemInt(hWndDlg, Freq, tempSineFreq, FALSE);
     hWndScroll = GetDlgItem(hWndDlg, Volume);
     SetScrollRange(hWndScroll, SB_CTL, 0, 32767, TRUE);
     SetScrollPos(hWndScroll, SB_CTL, tempSineVolume, TRUE);
     InvalidateRect(hWndDlg, NULL, TRUE);
     break;
    
// CASE (closing)
   case WM_CLOSE:

// . default to cancel
     PostMessage(hWndDlg, WM_COMMAND, IDCANCEL, 0L);
     break; 

// CASE (scrolling)
   case WM_HSCROLL:

// . adjust volume accordingly
     junk = TRUE;
     switch(wParam) {
     case SB_PAGEDOWN:
       tempSineVolume -= 20;
       break;
     case SB_PAGEUP:
       tempSineVolume += 20;
       break;
     case SB_LINEDOWN:
       tempSineVolume--;
       break;
     case SB_LINEUP:
       tempSineVolume++;
       break;
     case SB_TOP:
       tempSineVolume = 32767;
       break;
     case SB_BOTTOM:
       tempSineVolume = 0;
       break;
     case SB_THUMBTRACK:
     case SB_THUMBPOSITION:
       tempSineVolume = LOWORD(lParam);
       break;
     default:
       junk = FALSE;
       break;
     }
     if(junk) {
       hWndScroll = GetDlgItem(hWndDlg, Volume);
       SetScrollPos(hWndScroll, SB_CTL, tempSineVolume, TRUE);
       InvalidateRect(hWndScroll, NULL, TRUE);
     }
     break;
    

// CASE (a command message)
   case WM_COMMAND:

// . CASEOF (specific command)
     switch(wParam) {

// . CASE (okay)
     case IDOK:

// . . update values
       hWndScroll = GetDlgItem(hWndDlg, Volume);
       tempSineVolume = GetScrollPos(hWndScroll, SB_CTL);
       tempSineFreq = GetDlgItemInt(hWndDlg, Freq, &junk, FALSE);
       switch(WhichSine) {
       case 0:
         Sine1Freq = tempSineFreq;
	 Sine1Volume = tempSineVolume;
	 break;
       case 1:
         Sine2Freq = tempSineFreq;
	 Sine2Volume = tempSineVolume;
	 break;
       case 2:
	 Sine3Freq = tempSineFreq;
	 Sine3Volume = tempSineVolume;
	 break;
       default:
	 break;
       }
       if(button[WhichSine].state) {
         SetSine(WhichSine);
       }
       EndDialog(hWndDlg, TRUE);
       break;

// . CASE (cancel)
     case IDCANCEL:

// . . do nothing but close down
       EndDialog(hWndDlg, FALSE);
       break;

// . ENDCASE
     }
     break;                                 

// CASEELSE
   default:

// . let windows handle it
     return FALSE;

// ENDCASE
   }

// return
   return TRUE;
}


/* *********************************************************************

 BOOL     CHIRPMsgProc

 Description:        Chirp dialog window procedure
            This procedure is associated with the Chirp dialog box. 
            It provides the service routines for the events (messages) 
            that occur because the end user operates one of the 
            dialog box's buttons, entry fields, or controls.
            
 Parameters:
            HWND     hWndDlg       - Handle of the dialog box
            WORD     Message       - Message ID coming from the dialog box
            WORD     wParam        - First message dependent parameter
            LONG     lParam        - Second message dependent parameter

 Return Value:
            BOOL     Termination status

 ********************************************************************* */

BOOL FAR PASCAL CHIRPMsgProc(HWND hWndDlg, WORD Message, WORD wParam, LONG lParam) {

  int tempMinFreq,tempMaxFreq,tempChirpVolume,tempChirpSpeed;
  BOOL junk;
  HWND hWndScroll;
  LPDSPMSG lpMsg;

// CASEOF (message)
   switch(Message) {

// CASE (initialization)
   case WM_INITDIALOG:

// . center the dialog
     cwCenter(hWndDlg, 0);

// . initialize variables
     tempMinFreq = ChirpMinFreq;
     tempMaxFreq = ChirpMaxFreq;
     tempChirpSpeed = ChirpSpeed;
     tempChirpVolume = 32767 - ChirpVolumeVar;
    
// . set the values
     SetDlgItemInt(hWndDlg, MinFreq, tempMinFreq, FALSE);
     SetDlgItemInt(hWndDlg, MaxFreq, tempMaxFreq, FALSE);
     SetDlgItemInt(hWndDlg, Speed, tempChirpSpeed, FALSE);
     hWndScroll = GetDlgItem(hWndDlg, ChirpVolume);
     SetScrollRange(hWndScroll, SB_CTL, 0, 32767, TRUE);
     SetScrollPos(hWndScroll, SB_CTL, tempChirpVolume, TRUE);
     InvalidateRect(hWndDlg, NULL, TRUE);
     break;
    
// CASE (scroll message)
   case WM_VSCROLL:

// . set volume accordingly
     junk = TRUE;
     switch(wParam) {
     case SB_PAGEDOWN:
       tempChirpVolume -= 20;
       break;
     case SB_PAGEUP:
       tempChirpVolume += 20;
       break;
     case SB_LINEDOWN:
       tempChirpVolume--;
       break;
     case SB_LINEUP:
       tempChirpVolume++;
       break;
     case SB_TOP:
       tempChirpVolume = 32767;
       break;
     case SB_BOTTOM:
       tempChirpVolume = 0;
       break;
     case SB_THUMBPOSITION:
       tempChirpVolume = LOWORD(lParam);
       break;
     default:
       junk = FALSE;
       break;
     }
     if(junk) {
       hWndScroll = GetDlgItem(hWndDlg, ChirpVolume);
       SetScrollPos(hWndScroll, SB_CTL, tempChirpVolume, TRUE);
       InvalidateRect(hWndScroll, NULL, TRUE);
     }
     break;

// CASE (closing down)
   case WM_CLOSE:

// . use cancel for the default
     PostMessage(hWndDlg, WM_COMMAND, IDCANCEL, 0L);
     break;                                      

// CASE (windows command)
   case WM_COMMAND:

// . CASEOF (specific command)
     switch(wParam) {

// . CASE (okay)
     case IDOK:

// . . Set the values
       hWndScroll = GetDlgItem(hWndDlg, ChirpVolume);
       tempChirpVolume = GetScrollPos(hWndScroll, SB_CTL);
       tempMinFreq = GetDlgItemInt(hWndDlg, MinFreq, &junk, FALSE);
       tempMaxFreq = GetDlgItemInt(hWndDlg, MaxFreq, &junk, FALSE);
       tempChirpSpeed = GetDlgItemInt(hWndDlg, Speed, &junk, FALSE);
       ChirpVolumeVar = 32767 - tempChirpVolume;
       ChirpSpeed = tempChirpSpeed;
       ChirpMinFreq = tempMinFreq;
       ChirpMaxFreq = tempMaxFreq;

// . . tell the algorithm about the new settings if chirp is enabled
       if (button[NOISE_CHIRP].state) {
         if ((lpMsg = AllocateMsg ()) == NULL) {
           WritePrint ("*** ERROR: Can't allocate a message buffer\n");
           break;
         }
         lpMsg->lMsgID = msg_SET_NOISE;
         lpMsg->dwSize = 12;
         lpMsg->wBody[0] = NOISE_CHIRP; 
//         lpMsg->wBody[1] = (32767 - tempChirpVolume);
         lpMsg->wBody[1] = ChirpVolumeVar;
         lpMsg->wBody[2] = ChirpMinFreq;
         lpMsg->wBody[3] = ChirpMaxFreq;
         lpMsg->wBody[4] = ChirpSpeed;
         lpMsg->wBody[5] = 0;
         QueueMsg (&lpOutboundMsgQueue, lpMsg);
         lpMsg->lRequestID = dspSendMsg (hMyAlg, lpMsg->lMsgID, (LPVOID) lpMsg->wBody, lpMsg->dwSize);
         if (lpMsg->lRequestID > DSPERR_SUCCESS) {
         } else {
           ProcessOutboundReply (lpMsg);
         }
       }

// . . we be done
       EndDialog(hWndDlg, TRUE);
       break;

// . CASE (cancel)
     case IDCANCEL:

// . . do nothing
       EndDialog(hWndDlg, FALSE);
       break;

// . ENDCASE
     }
     break;

// CASEELSE
   default:

// . let windows handle it
     return FALSE;

// ENDCASE
   }
   return TRUE;
}


/*********************************************************************
//
// BOOL     CCITTMsgProc
//
// Description:        Ccitt dialog window procedure
//            This procedure is associated with the Ccitt dialog box.
//            It provides the service routines for the events (messages) 
//            that occur because the end user operates one of the 
//            dialog box's buttons, entry fields, or controls.
//            
// Parameters:
//            HWND     hWndDlg       - Handle of the dialog box
//            WORD     Message       - Message ID coming from the dialog box
//            WORD     wParam        - First message dependent parameter
//            LONG     lParam        - Second message dependent parameter
//
// Return Value:
//            BOOL     Termination status
//
**********************************************************************/

BOOL FAR PASCAL CCITTMsgProc(HWND hWndDlg, WORD Message, WORD wParam, LONG lParam) {

   BOOL junk;
   WORD tempVolume;
   HWND hWndScroll;
   LPDSPMSG lpMsg;

// CASEOF (message type)
   switch(Message) {

// CASE (initialization)
   case WM_INITDIALOG:

// . center dialog
     cwCenter(hWndDlg, 0);

// . initialize variables
     tempVolume = CcittVolume;
     hWndScroll = GetDlgItem(hWndDlg, Volume);
     SetScrollRange(hWndScroll, SB_CTL, 0, 32767, TRUE);
     SetScrollPos(hWndScroll, SB_CTL, tempVolume, TRUE);
     InvalidateRect(hWndDlg, NULL, TRUE);
     break;
    
// CASE (close)
   case WM_CLOSE:

// . default to cancel               
     PostMessage(hWndDlg, WM_COMMAND, IDCANCEL, 0L);
     break;                                      

// CASE (scroll)
   case WM_HSCROLL:

// . set volume accordingly
     junk = TRUE;
     switch(wParam) {
     case SB_PAGEDOWN:
       tempVolume = 2000;
       hWndScroll = GetDlgItem(hWndDlg, Volume);
       tempVolume += GetScrollPos(hWndScroll, SB_CTL);
       SetScrollPos(hWndScroll, SB_CTL, tempVolume, TRUE);
       break;
     case SB_PAGEUP:
       tempVolume = -2000;
       hWndScroll = GetDlgItem(hWndDlg, Volume);
       tempVolume += GetScrollPos(hWndScroll, SB_CTL);
       SetScrollPos(hWndScroll, SB_CTL, tempVolume, TRUE);
       break;
     case SB_LINEDOWN:
       tempVolume = 100;
       hWndScroll = GetDlgItem(hWndDlg, Volume);
       tempVolume += GetScrollPos(hWndScroll, SB_CTL);
       SetScrollPos(hWndScroll, SB_CTL, tempVolume, TRUE);
       break;
     case SB_LINEUP:
       tempVolume = -100;
       hWndScroll = GetDlgItem(hWndDlg, Volume);
       tempVolume += GetScrollPos(hWndScroll, SB_CTL);
       SetScrollPos(hWndScroll, SB_CTL, tempVolume, TRUE);
       break;
     case SB_TOP:
       tempVolume = 32767;
       hWndScroll = GetDlgItem(hWndDlg, Volume);
       SetScrollPos(hWndScroll, SB_CTL, tempVolume, TRUE);
       break;
     case SB_BOTTOM:
       tempVolume = 0;
       hWndScroll = GetDlgItem(hWndDlg, Volume);
       SetScrollPos(hWndScroll, SB_CTL, tempVolume, TRUE);
       break;
     case SB_THUMBTRACK:
     case SB_THUMBPOSITION:
       tempVolume = LOWORD(lParam);
       break;
     default:
       junk = FALSE;
       break;
     }
     if(junk) {
       hWndScroll = GetDlgItem(hWndDlg, Volume);
       SetScrollPos(hWndScroll, SB_CTL, tempVolume, TRUE);
       InvalidateRect(hWndScroll, NULL, TRUE);
     }
     break;

// CASE (windows command)
   case WM_COMMAND:

// . CASEOF (specific command)
     switch(wParam) {

// . CASE (okay)
     case IDOK:

// . . get the info
       hWndScroll = GetDlgItem(hWndDlg, Volume);
       CcittVolume = GetScrollPos(hWndScroll, SB_CTL);
      
// . . tell the algorithm about the new settings if CCITT is enabled
       if (button[NOISE_CCITT].state) {
         if ((lpMsg = AllocateMsg ()) == NULL) {
           WritePrint ("*** ERROR: Can't allocate a message buffer\n");
           break;
         }
         lpMsg->lMsgID = msg_SET_NOISE;
         lpMsg->dwSize = 4;
         lpMsg->wBody[0] = NOISE_CCITT; 
         lpMsg->wBody[1] = CcittVolume;
         QueueMsg (&lpOutboundMsgQueue, lpMsg);
         lpMsg->lRequestID = dspSendMsg (hMyAlg, lpMsg->lMsgID, (LPVOID) lpMsg->wBody, lpMsg->dwSize);
         if (lpMsg->lRequestID > DSPERR_SUCCESS) {
         } else {
           ProcessOutboundReply (lpMsg);
         }
       }

// . . we be done
       EndDialog(hWndDlg, TRUE);
       break;

// . CASE (cancel)
     case IDCANCEL:

// . . do nothing
       EndDialog(hWndDlg, FALSE);
       break;

// . ENDCASE
     }
     break;

// CASEELSE
   default:

// . let windows process it
     return FALSE;

// ENDCASE
   }

// return
   return TRUE;
}




/*********************************************************************
//
// BOOL     CONFIGMsgProc
//
// Description:        1848 Configuration dialog window procedure
//            This procedure is associated with the 1848 Config dialog box.
//            It provides the service routines for the events (messages)  
//            that occur because the end user operates one of the 
//            dialog box's buttons, entry fields, or controls.
//            
// Parameters:
//            HWND     hWndDlg       - Handle of the dialog box
//            WORD     Message       - Message ID coming from the dialog box
//            WORD     wParam        - First message dependent parameter
//            LONG     lParam        - Second message dependent parameter
//
// Return Value:
//            BOOL     Termination status
//
**********************************************************************/
#pragma        argsused

BOOL FAR PASCAL CONFIGMsgProc(HWND hWndDlg, WORD Message, WORD wParam, LONG lParam) {

  HWND hWndButton;
  static int tmpMode;
  static long tmpSampleRateLong;
  static int tmpSampleRate;
  
// CASEOF (message)
   switch(Message) {

// CASE (initialization)
   case WM_INITDIALOG:

// . center the dialog
     cwCenter(hWndDlg, 0);

// . initialize values
     tmpSampleRate = SampleRateEnum;
     tmpSampleRateLong = SampleRate;
     tmpMode = Mode;
     if(Mode == STEREO) {
       hWndButton = GetDlgItem(hWndDlg, MODE_STEREO);
     } else {
       hWndButton = GetDlgItem(hWndDlg, MODE_MONO);
     }
     SendMessage(hWndButton, BM_SETCHECK, 1, 0L);
     switch(SampleRateEnum) {
     case FREQ_8K:
       hWndButton = GetDlgItem(hWndDlg, SR_8K);
       break;
     case FREQ_16K:
       hWndButton = GetDlgItem(hWndDlg, SR_16K);
       break;
     case FREQ_27_42857K:
       hWndButton = GetDlgItem(hWndDlg, SR_27K);
       break;
     case FREQ_32K:
       hWndButton = GetDlgItem(hWndDlg, SR_32K);
       break;
     case FREQ_48K:
       hWndButton = GetDlgItem(hWndDlg, SR_48K);
       break;
     case FREQ_9_6K:
       hWndButton = GetDlgItem(hWndDlg, SR_9_6K);
       break;
     case FREQ_5_5125K:
       hWndButton = GetDlgItem(hWndDlg, SR_5K);
       break;
     case FREQ_11_025K:
       hWndButton = GetDlgItem(hWndDlg, SR_11K);
       break;
     case FREQ_18_9K:
       hWndButton = GetDlgItem(hWndDlg, SR_18K);
       break;
     case FREQ_22_05K:
       hWndButton = GetDlgItem(hWndDlg, SR_22K);
       break;
     case FREQ_37_8K:
       hWndButton = GetDlgItem(hWndDlg, SR_37K);
       break;
     case FREQ_44_1K:
       hWndButton = GetDlgItem(hWndDlg, SR_44K);
       break;
     case FREQ_33_075K:
       hWndButton = GetDlgItem(hWndDlg, SR_33K);
       break;
     case FREQ_6_615K:
       hWndButton = GetDlgItem(hWndDlg, SR_6K);
       break;
     }
     SendMessage(hWndButton, BM_SETCHECK, 1, 0L);
     break;

// CASE (close)
   case WM_CLOSE:

// . same as cancel
     PostMessage(hWndDlg, WM_COMMAND, IDCANCEL, 0L);
     break;

// CASE (windows command)
   case WM_COMMAND:

// . CASEOF (specific command)
     switch(wParam) {

// . CASE (any pushbutton)
     case MODE_STEREO: /* Radiobutton text: "Stereo" */
       tmpMode = STEREO;
       break;
     case MODE_MONO: /* Radiobutton text: "Mono" */
       tmpMode = MONO;
       break;
     case SR_5K: /* Radiobutton text: "5.51K" */
       tmpSampleRateLong = 5512L;
       tmpSampleRate = FREQ_5_5125K;
       break;
     case SR_6K: /* Radiobutton text: "6.61K"  */
       tmpSampleRateLong = 6615L;
       tmpSampleRate = FREQ_6_615K;
       break;
     case SR_8K: /* Radiobutton text: "8K" */
       tmpSampleRateLong = 8000L;
       tmpSampleRate = FREQ_8K;
       break;
     case SR_9_6K: /* Radiobutton text: "9.6K" */
       tmpSampleRateLong = 9600L;
       tmpSampleRate = FREQ_9_6K;
       break;
     case SR_11K: /* Radiobutton text: "11.02K" */
       tmpSampleRateLong = 11025L;
       tmpSampleRate = FREQ_11_025K;
       break;
     case SR_16K: /* Radiobutton text: "16K"  */
       tmpSampleRateLong = 16000L;
       tmpSampleRate = FREQ_16K;
       break;
     case SR_18K: /* Radiobutton text: "18.9K" */
       tmpSampleRateLong = 18900L;
       tmpSampleRate = FREQ_18_9K;
       break;
     case SR_22K: /* Radiobutton text: "22.05K"  */
       tmpSampleRateLong = 22050L;
       tmpSampleRate = FREQ_22_05K;
       break;
     case SR_27K: /* Radiobutton text: "27.42K" */
       tmpSampleRateLong = 27428L;
       tmpSampleRate = FREQ_27_42857K;
       break;
     case SR_32K: /* Radiobutton text: "32K" */
       tmpSampleRateLong = 32000L;
       tmpSampleRate = FREQ_32K;
       break;
    case SR_33K: /* Radiobutton text: "33.07K" */
       tmpSampleRateLong = 33075L;
       tmpSampleRate = FREQ_33_075K;
       break;
     case SR_37K: /* Radiobutton text: "37.8K"  */
       tmpSampleRateLong = 37800L;
       tmpSampleRate = FREQ_37_8K;
       break;
     case SR_44K: /* Radiobutton text: "44.1K" */
       tmpSampleRateLong = 44100L;
       tmpSampleRate = FREQ_44_1K;
       break;
     case SR_48K: /* Radiobutton text: "48K" */
       tmpSampleRateLong = 48000L;
       tmpSampleRate = FREQ_48K;
       break;

// . CASE (okay)        
     case IDOK:

// . . save the values
       SampleRate = tmpSampleRateLong;
       SampleRateEnum = tmpSampleRate;
       Mode = tmpMode;

// . . we be done
       EndDialog(hWndDlg, TRUE);
       break;

// . CASE (cancel)
     case IDCANCEL:

// . . do nothing
       EndDialog(hWndDlg, FALSE);
       break;

// . ENDCASE
     }
     break;
    
// CASEELSE
   default:

// . let windows process it
     return FALSE;

// ENDCASE
   }

// return
   return TRUE;
}


/*********************************************************************
// int        nCwRegisterClasses
//
// Description:        
//            The following function registers all the classes of all the 
//             windows associated with this application. The function returns 
//             an error code if unsuccessful, otherwise it returns 0.
//
// Parameters:
//            None
//
// Return Value:
//            int        - Termination status
//
**********************************************************************/

int nCwRegisterClasses(void) {

   WNDCLASS   wndclass;             

// initialize the structure
   memset(&wndclass, 0x00, sizeof(WNDCLASS));

// populate the structure
   wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_BYTEALIGNWINDOW;
   wndclass.lpfnWndProc = WndProc;
   wndclass.cbClsExtra = 0;
   wndclass.cbWndExtra = 0;
   wndclass.hInstance = hInst;
   wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
   wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
   wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
   wndclass.lpszMenuName = szAppName;       /* Menu Name is App Name  */
   wndclass.lpszClassName = szAppName;      /* Class Name is App Name */

// register it
   if(!RegisterClass(&wndclass)) {
     return (-1);
   }

// return
   return(0);
}



/*********************************************************************
//
//        VOID CwUnRegisterClasses
//
//     Description:
//                Deletes any refrences to windows resources created for 
//                this application, frees memory, deletes instance, handles 
//                and does clean up prior to exiting the window
//         
//     Parameters:
//                nothing
//
//     Return Value:
//                  nothing
// 
**********************************************************************/

VOID CwUnRegisterClasses(void) {

// unregister
   UnregisterClass(szAppName, hInst);
}


/*********************************************************************
//
//     VOID CallbackProc
//
//     Description:
//                Callback function for the DSP Manager... processes pending
//                messages from the algorithm, etc.
//             
//     Parameters:
//                hAlg        - Algorithm handle
//                dwMsg       - Callback message 
//                dwInstance  - Client application instance information 
//                dwParam1    - A message dependent paramater 
//                dwParam2    - A message dependent parameter
//
//     Return Value:
//                nothing
// 
**********************************************************************/

VOID CALLBACK CallbackProc (HALG hAlg, DWORD dwMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2 ) {

   LPDSPMSG lpMsg;

// CASEOF (message)
   switch ( dwMsg ) {

// CASE (message pending)
   case DSPCB_MSGPENDING:

// . get a buffer for the message
     if ((lpMsg = AllocateMsg ()) == NULL) {
       WritePrint ("*** ERROR: Can't allocate a message buffer for a pending message\n");
       break;
     }
     QueueMsg (&lpInboundMsgQueue, lpMsg);

// . save the id and length in the message at any rate
     lpMsg->lMsgID = (LONG)dwParam1;
     lpMsg->dwSize = (LONG)dwParam2;

// . tell the DSP API where to store the message
     lpMsg->lRequestID = dspGetMsg (hMyAlg, &lpMsg->lMsgID, (LPVOID) lpMsg->wBody, sizeof(lpMsg->wBody));
     if (lpMsg->lRequestID > DSPERR_SUCCESS) {
     } else {
       PostMessage((HWND) dwInstance, (UINT) WM_DSPCB_REPLY, (WPARAM) ((int)dwParam2), (LPARAM) dwParam1);
     }
     break;

// CASE (reply)
   case DSPCB_REPLY:

// . post to the app
     PostMessage((HWND) dwInstance, (UINT) WM_DSPCB_REPLY, (WPARAM) ((int)dwParam2), (LPARAM) dwParam1);
     break;

// CASE (reset)
   case DSPCB_RESET:

// . not much we can do here but tell our message processor
     PostMessage((HWND) dwInstance, (UINT) WM_DSPCB_RESET, (WPARAM) NULL, (LPARAM) NULL);
     break;

// ENDCASE
   }

// return
}

/*********************************************************************
//
//     VOID clearButtonsAndMenus
//
//     Description:
//                Called only from within the IDM_FILE case of WndProc (a new
//                algorithm has been loaded). This simply cleans up all of the
//                check marks placed in menus and buttons, and disables all of
//                the plotting-related menu items (resetting to initial state) 
//         
//     Parameters:
//                nothing
//
//     Return Value:
//                nothing
// 
**********************************************************************/

VOID clearButtonsAndMenus(void) {

  int i;

// reset all buttons
   for(i=0; i<NUM; i++) {
     button[i].state = FALSE;
     SendMessage(hWndButton[i], WM_SETTEXT, 0, (DWORD)button[i].offText);
     switch(i) {
     case NOISE_SINE1:
       CheckMenuItem(GetMenu(hWndMain), IDM_SINE1, 
 		    (button[i].state ? MF_CHECKED:MF_UNCHECKED));
       break;
     case NOISE_SINE2:
       CheckMenuItem(GetMenu(hWndMain), IDM_SINE2, 
 		    (button[i].state ? MF_CHECKED:MF_UNCHECKED));
       break;
     case NOISE_SINE3:
       CheckMenuItem(GetMenu(hWndMain), IDM_SINE3, 
 		    (button[i].state ? MF_CHECKED:MF_UNCHECKED));
       break;
     case NOISE_CHIRP:
       CheckMenuItem(GetMenu(hWndMain), IDM_CHIRP, MF_UNCHECKED);
       break;
       
     case NOISE_CCITT:
       CheckMenuItem(GetMenu(hWndMain), IDM_CHIRP, MF_UNCHECKED);
       break;
     }
   }

// reset all modes
   Persistance = FALSE;
   ShowMessages = FALSE;
   Echo = FALSE;
   Frequency = TRUE;
   Linear = TRUE;
   PausePlot= FALSE;
   Lpf = FALSE;
   Hann = FALSE;

// stop plotting
   if (ShowPlot == TRUE) {
     ShowPlot = FALSE;
     swapWindows();
   }
  
// default some 1848 values
   SampleRate = 8000L;
   SampleRateEnum = FREQ_8K;          

// gray out all our buttons
   EnableMenuItem(GetMenu(hWndMain), IDM_TIME,      MF_GRAYED);
   EnableMenuItem(GetMenu(hWndMain), IDM_FREQUENCY, MF_GRAYED);
   EnableMenuItem(GetMenu(hWndMain), IDM_LOG,       MF_GRAYED);
   EnableMenuItem(GetMenu(hWndMain), IDM_LINEAR,    MF_GRAYED);
   EnableMenuItem(GetMenu(hWndMain), IDM_PERSIST,   MF_GRAYED);
   EnableMenuItem(GetMenu(hWndMain), IDM_CLEAR,     MF_GRAYED);
   CheckMenuItem(GetMenu(hWndMain),  IDM_PLOT,      MF_UNCHECKED);
   CheckMenuItem(GetMenu(hWndMain),  IDM_ECHO,      MF_UNCHECKED);
   CheckMenuItem(GetMenu(hWndMain),  IDM_LPF,       MF_UNCHECKED);
   CheckMenuItem(GetMenu(hWndMain),  IDM_HANN,      MF_UNCHECKED);    

// return
}

/*********************************************************************
//
//     VOID swapWindows
//
//     Description:
//                Swaps to and from the plotting mode, resizing the windows
//                appropriately. Called when IDM_PLOT is generated (via
//                WndProc) and when Load DSP is selected. 
//
//                Note: the actual plotting routine (PlotIt) is in
//                APP_UTIL.C
//
//     Parameters:
//                nothing
//
//     Return Value:
//                nothing
// 
**********************************************************************/

VOID swapWindows(void) {

  int xmax;
  int ymax;
  int winsize;
  int prntsize;

// calculate sizes
   xmax = wMainWinXMax;
   ymax = wMainWinYMax;
  
// swap the windows
   if(!ShowPlot) {
     winsize = (ymax-STATIC_TOP-(STATIC_SIZE*2))/2 - 10;
     MoveWindow(hWndPlotCap, xmax/2, ymax, xmax/2-5, ymax, TRUE);
     MoveWindow(hWndPlot, xmax/2,ymax, xmax/2-5, ymax, TRUE);
     MoveWindow(hWndMessageCap, 5,STATIC_TOP, xmax-10, STATIC_SIZE, TRUE);
     MoveWindow(hWndMessages, 5,STATIC_TOP+STATIC_SIZE-1, xmax-10, winsize, TRUE);
     MoveWindow(hWndPrintCap, 5, STATIC_TOP+STATIC_SIZE+winsize+10, xmax-10, STATIC_SIZE, TRUE);
     MoveWindow(hWndPrints, 5, STATIC_TOP+STATIC_SIZE*2+winsize+10-1, xmax-10, winsize, TRUE);
   } else {
     winsize = (ymax-STATIC_TOP)*2/3;
     prntsize = ymax-STATIC_TOP-winsize;
     MoveWindow(hWndPlotCap, 5,STATIC_TOP, xmax-10, STATIC_SIZE, TRUE);
     MoveWindow(hWndPlot, 5,STATIC_TOP+STATIC_SIZE-1, xmax-10, winsize-STATIC_SIZE-10, TRUE);
     MoveWindow(hWndMessageCap, 5,ymax, (xmax/2)-10, STATIC_SIZE, TRUE);
     MoveWindow(hWndMessages, 5,ymax, (xmax/2)-10, winsize, TRUE);
     MoveWindow(hWndPrintCap, 5, STATIC_TOP+winsize-5, xmax-10, STATIC_SIZE, TRUE);
     MoveWindow(hWndPrints, 5, STATIC_TOP+STATIC_SIZE+winsize-6, xmax-10, prntsize-STATIC_SIZE-5, TRUE);
   }

// return
}

/*********************************************************************
//
//     VOID SetSine
// 
//     Description:
//             Responds to the pushing of one of the Sine 1-3 buttons
//             (via WndProc)
//        
//     Parameters:
//             int        - wButton     Button number
// 
//     Return Value:
//             nothing
// 
**********************************************************************/

VOID SetSine(int wButton) {

  int tempVolume;
  int tempFreq;
  LPDSPMSG lpMsg;

// prepare a message to tell the algorithm about the sine parameters
   if ((lpMsg = AllocateMsg ()) == NULL) {
     WritePrint ("*** ERROR: Can't allocate a message buffer\n");
     return;
   }
   lpMsg->lMsgID = msg_SET_NOISE;
   lpMsg->dwSize = 6;
   lpMsg->wBody[0] = wButton; 

#ifdef xyzzy
// toggle the button
   if(button[wButton].state) {
     button[wButton].state = FALSE;
   } else {
     button[wButton].state = TRUE;
   }
#endif

// get the values
   switch(wButton) {
   case NOISE_SINE1:
     tempVolume = Sine1Volume;
     tempFreq = Sine1Freq;
     break;
   case NOISE_SINE2:
     tempVolume = Sine2Volume;
     tempFreq = Sine2Freq;
     break;
   case NOISE_SINE3:
     tempVolume = Sine3Volume;
     tempFreq = Sine3Freq;
     break;
   }
   if(button[wButton].state) {
     lpMsg->wBody[1] = tempVolume;       /* Gain  */
     SendMessage(hWndButton[wButton], WM_SETTEXT, 0, (DWORD)button[wButton].onText);
   } else {
     lpMsg->wBody[1] = 0;                /* Gain  */
     SendMessage(hWndButton[wButton], WM_SETTEXT, 0, (DWORD)button[wButton].offText);
   }
   lpMsg->wBody[2] = tempFreq;            /* Frequency */

// send the values to the algorithm
   QueueMsg (&lpOutboundMsgQueue, lpMsg);
   lpMsg->lRequestID = dspSendMsg (hMyAlg, lpMsg->lMsgID, (LPVOID) lpMsg->wBody, lpMsg->dwSize);
   if (lpMsg->lRequestID > DSPERR_SUCCESS) {
   } else {
     ProcessOutboundReply (lpMsg);
   }

// repaint it
   InvalidateRect(hWndButton[wButton], NULL, TRUE);

// return
 }
 

/*************************************************************

Name:        AllocateMsg

Inputs:      nothing

Outputs:     LPDSPMSG

Description: Allocates a DSPMSG from the free list
*************************************************************/

LPDSPMSG AllocateMsg (void) {

   LPDSPMSG lpMsg;

// get one
   DisableInterrupts ();
   if ((lpMsg = lpFreeMsgQueue) != NULL) {
     DequeueMsg (&lpFreeMsgQueue, lpMsg);
   }
   EnableInterrupts ();

// return
   return (lpMsg);
}


     
/*************************************************************

Name:        FreeMsg

Inputs:      LPDSPMSG

Outputs:     none

Description: Puts a DSPMSG back in the free list
*************************************************************/

void FreeMsg (LPDSPMSG lpMsg) {

// put it back
   DisableInterrupts ();
   QueueMsg (&lpFreeMsgQueue, lpMsg);
   EnableInterrupts ();

// return
}


/*************************************************************

Name:        QueueMsg

Inputs:      list and msg to insert

Outputs:     none

Description: inserts the given msg into the given list
*************************************************************/

void QueueMsg (LPDSPMSG *lpQueue, LPDSPMSG lpMsg) {

   LPDSPMSG lpLastMsg;

// put it in
   DisableInterrupts ();

// new way
   if (*lpQueue == NULL) {
     lpMsg->lpNext = *lpQueue;
     *lpQueue = lpMsg;
   } else {
     for (lpLastMsg = *lpQueue; lpLastMsg->lpNext != NULL; lpLastMsg = lpLastMsg->lpNext) ;
     lpLastMsg->lpNext = lpMsg;
     lpMsg->lpNext = NULL;
   }

// old way
//   lpMsg->lpNext = *lpQueue;
//   *lpQueue = lpMsg;
   EnableInterrupts ();

// return
}


/*************************************************************

Name:        DequeueMsg

Inputs:      list and MSG to remove

Outputs:     none

Description: Removes the given DSPMSG from the given list
*************************************************************/

void DequeueMsg (LPDSPMSG *lpQueue, LPDSPMSG lpMsg) {

   LPDSPMSG lpPrevious;

// take it out
   DisableInterrupts ();
   if (*lpQueue == lpMsg) {
     *lpQueue = (*lpQueue)->lpNext;
   } else {
     for (lpPrevious = *lpQueue;
          lpPrevious;
          lpPrevious = lpPrevious->lpNext) {
       if (lpPrevious->lpNext == lpMsg) {
         lpPrevious->lpNext = lpMsg->lpNext;
         break;
       }
     }                                                          
   }
   EnableInterrupts ();

// return
}



/*************************************************************

Name:        FindMsgGivenlRequestID 

Inputs:      list and MSG to remove

Outputs:     pointer to the message

Description: Finds the message with the given request ID in the given list
*************************************************************/

LPDSPMSG FindMsgGivenlRequestID (LPDSPMSG *lpQueue, LONG lRequestID) {

   LPDSPMSG lpMsg;

// find it
   for (lpMsg = *lpQueue; lpMsg; lpMsg = lpMsg->lpNext) {
     if (lpMsg->lRequestID == lRequestID) {
       return (lpMsg);
     }
   }

// return
   return (NULL);
}



/*************************************************************

Name:        ProcessInboundReply 

Inputs:      message to process

Outputs:     none

Description: processes replies to inbound messages
*************************************************************/

void ProcessInboundReply (LPDSPMSG lpMsg) {

   char szBuffer[DSP_MAXSTRINGLENGTH];
   int i;
   LONG lTmp;
   LPWORD lpWord;

// watch for errors
   if (lpMsg->lRequestID < DSPERR_SUCCESS) {
     dspGetErrorInfo (lpMsg->lRequestID, (LPSTR) &szBuffer, DSP_MAXSTRINGLENGTH);
     strcat (szBuffer, "\n");
     WritePrint (szBuffer);
     return;
   }

// CASEOF (message ID) *** note we're guaranteed success at this point
   switch (lpMsg->lMsgID) {

// CASE (print message)
   case DSPMSGID_STRING_PRINT:

// . print it out
     WritePrint ((LPSTR)lpMsg->wBody);
     strcpy (szBuffer, "\n");
     WritePrint (szBuffer);
     break;

// CASE (plot message)
   case msg_PLOT_DATA:

// . unpack the data
     for (i = 0; i < PLOT_DATA_SIZE; i++) {
       PlotArray[i] = lpMsg->wBody[i];
     }

// . plot it
     PlotIt ();
     break;

// CASE (echoed data)
   case msg_ECHO_DATA:

// . print out the data
     WritePrint ("Echoed message received:\n");
     if (lpMsg->dwSize == 0) {
       WritePrint ("   message has body of size zero\n");
     }
     for (i = 0, lpWord = lpMsg->wBody; i < lpMsg->dwSize; i += 2, lpWord++) {
       sprintf (szBuffer, "   data[%d] = 0x%x\n", i, *lpWord);
       WritePrint (szBuffer);
     }
     WritePrint ("\n");
     break;

// CASE (1848 register dump)
   case msg_1848_REG_DUMP:

// . print out the info
     WritePrint("1848 Configuration:\n");
     sprintf (szBuffer, "  mode = %d\n", lpMsg->wBody[0]);
     WritePrint (szBuffer);
     sprintf (szBuffer, "  data type = %d\n", lpMsg->wBody[1]);
     WritePrint (szBuffer);
     lTmp = lpMsg->wBody[2];
     lTmp <<= 16;
     lTmp += lpMsg->wBody[3]; 
     sprintf (szBuffer, "  sample rate = %ld\n", lTmp);
     WritePrint (szBuffer);
     sprintf (szBuffer, "  bits per sample = %d\n", lpMsg->wBody[4]);
     WritePrint (szBuffer);
     WritePrint ("\n");
     break;

// CASEELSE
   default:

// . what kind of message is this?
     sprintf (szBuffer, "Unknown inbound message, lMsgID = %ld, dwSize = %ld\n", lpMsg->lMsgID, lpMsg->dwSize);
     WritePrint (szBuffer);
     WritePrint ("\n");
     break;

// ENDCASE
   }

// remove it from the outbound queue and return it to the free list
   DequeueMsg (&lpInboundMsgQueue, lpMsg);
   FreeMsg (lpMsg);

// return
}



/*************************************************************

Name:        ProcessOutboundReply 

Inputs:      message to process

Outputs:     none

Description: processes outbound replies to messages
*************************************************************/

void ProcessOutboundReply (LPDSPMSG lpMsg) {

   char szBuffer[DSP_MAXSTRINGLENGTH];

// watch for errors
   if (lpMsg->lRequestID < DSPERR_SUCCESS) {
     dspGetErrorInfo (lpMsg->lRequestID, (LPSTR) &szBuffer, DSP_MAXSTRINGLENGTH);
     strcat (szBuffer, "\n");
     WritePrint (szBuffer);
     return;
   }

// CASEOF (message ID) *** note we're guaranteed success at this point
   switch (lpMsg->lMsgID) {

// CASE (messages we don't have to do anything for)
   case (DSPMSGID_AUDIO_SETMODE):
   case (DSPMSGID_AUDIO_SETRATE):
   case (msg_FREQ_TOGGLE):
   case (msg_LOG_TOGGLE):
   case (msg_ECHO_TOGGLE):
   case (msg_GET_1848):
   case (msg_HANN_TOGGLE):
   case (msg_LPF_TOGGLE):
   case (msg_PLOT_SEND):
   case (msg_PLOT_TOGGLE):
   case (msg_SET_NOISE):
   case (msg_TALK_TOGGLE):
     break;

// CASEELSE
   default:

// . what kind of message is this?
     sprintf (szBuffer, "Unknown outbound message, lMsgID = %ld, dwSize = %ld\n", lpMsg->lMsgID, lpMsg->dwSize);
     WritePrint (szBuffer);
     break;

// ENDCASE
   }

// remove it from the outbound queue and return it to the free list
   DequeueMsg (&lpOutboundMsgQueue, lpMsg);
   FreeMsg (lpMsg);

// return
}



/*************************************************************

Name:        DisableInterrupts

Description: Disable interrupts and increment the semaphore

*************************************************************/

void DisableInterrupts (void) {

// disable interrupts
   _disable ();

// increment the semaphore
   iInterruptsDisabled++;

// return
}


/*************************************************************

Name:        EnableInterrupts

Description: Enables interrupts if the semaphore is clear

*************************************************************/

void EnableInterrupts (void) {

// disable interrupts (I know this looks wierd, but trust me it works)
   _disable ();

// enable interrupts if the semaphore is clear
   if (iInterruptsDisabled) {
     iInterruptsDisabled--;
   }
   if (iInterruptsDisabled == 0) {
     _enable ();
   }

// return
}



/********************************************************************
//
//     VOID  PlotIt
//
//     Description:
//            Produces all of the output to the Plots window
//         
//     Parameters:
//            nothing
//
//     Return Value:
//            nothing
// 
**********************************************************************/

VOID PlotIt(void) {

   HDC hDC;
   int i;
   RECT rect;
   int ySize;

// figure out the size of where we'll be plotting
   hDC = GetDC(hWndPlot);
   GetClientRect(hWndPlot, &rect);
   ySize = rect.bottom-10;

// set the display device for the mode we want it
   SetMapMode(hDC, MM_ANISOTROPIC);
   SetWindowExt(hDC, 133, 32767);

// configure for a logical range
   if (!Frequency) {
     SetViewportExt(hDC, rect.right, -ySize/2);
   } else {
     SetViewportExt(hDC, rect.right, -ySize);
   }

// move the origin 0,0 to the center of the y axis and 5 pixels from the left side of x axis 
   if (!Frequency) {
     SetViewportOrg(hDC, 5, ySize/2+5);
   } else {
     SetViewportOrg(hDC, 5, ySize+5);
   }

// check for persistance and set our pen accordingly
   if(Persistance) { 
     SelectObject(hDC, hPenGrey);
   } else {
     SelectObject(hDC, hPenClear);
   }

// do what we have to do to the last plot drawn
   MoveTo(hDC, 0, PlotHold[0]);
   for(i=0; i<128; i++) {
     LineTo(hDC, i, PlotHold[i]);
   }

// get ready to draw this samples plot
   SelectObject(hDC, hPenDraw);
   MoveTo(hDC,0,0);
   LineTo(hDC,128,0);
  
// draw the plot for this sample of data
   MoveTo(hDC, 0, PlotArray[0]);
   for(i=0; i<128; i++) {
     LineTo(hDC, i, PlotArray[i]);
     PlotHold[i]=PlotArray[i];
   }

// free the device
   ReleaseDC(hWndPlot, hDC);

// return
}



/*********************************************************************
//
//    VOID WritePrint
//
//    Description:
//            Display the given character string in the DSP Prints window
//         
//    Parameters:
//            char    *lpString    - long pointer to a character string
//
//    Return Value:
//            nothing
// 
**********************************************************************/

VOID WritePrint(LPSTR lpString) {

// send the string to the message handler for the window   **** should not be done char by char
   while(*lpString != 0) {
     SendMessage(hWndPrints, WM_CHAR, (WORD)*lpString++, 0L);
   }

// return
}


/*********************************************************************
//
//    VOID showError
//
//    Description:
//            Display the given error message in the DSP Prints window
//         
//     Parameters:
//            char    *lpString    - long pointer to a character string
//
//     Return Value:
//            nothing
// 
**********************************************************************/

VOID showError(LPSTR lpString) {

  LPSTR lpStr = "\nERROR: ";

// **** again, what's this char by char BS

// send error header to prints window message processor
   while(*lpStr != 0) {
     SendMessage(hWndPrints, WM_CHAR, (WORD)*lpStr++, 0L);
   }

// send the actual error string to the message processor
   while(*lpString != 0) {
     SendMessage(hWndPrints, WM_CHAR, (WORD)*lpString++, 0L);
   }

// return
}



/*********************************************************************
//
//     VOID cwCenter
//
//     Description:
//                Centers a window based on the client area of its parent
//         
//     Parameters:
//                hWnd    - handle to the window to be centered
//                top
//
//     Return Value:
//                nothing
// 
**********************************************************************/

VOID cwCenter(HWND hWnd, int top) {

  POINT      pt;
  RECT       swp;
  RECT       rParent;
  int        iwidth;
  int        iheight;

// get the rectangles for the parent and the child 
   GetWindowRect(hWnd, &swp);
   GetClientRect(hWndMain, &rParent);
  
// calculate the height and width for MoveWindow
   iwidth = swp.right - swp.left;
   iheight = swp.bottom - swp.top;
  
// find the center point and convert to screen coordinates 
   pt.x = (rParent.right - rParent.left) / 2;
   pt.y = (rParent.bottom - rParent.top) / 2;
   ClientToScreen(hWndMain, &pt);
  
// calculate the new x, y starting point 
   pt.x = pt.x - (iwidth / 2);
   pt.y = pt.y - (iheight / 2);
          
// adjust the window position, up or down 
   if(top) {
     pt.y = pt.y + top;
   }

// move the window 
   MoveWindow(hWnd, pt.x, pt.y, iwidth, iheight, FALSE);

// return
}






/*******************************************************************
//
//   VOID GetAnAlgName
//
//   Description:
//           Parses the DSPMGR.INI file to determine all of the
//           various algorithms that can currently be loaded,
//           then opens a dialog box to ask the user which particular
//           algorithm they want to load.
//
//   Parameters:
//           char *lpString        -pointer to a buffer in which the
//                                  .INI section name will be stored
//
//   Return Value:
//           TRUE if it thinks it succeeded, FALSE otherwise
//
//
**********************************************************************/

BOOL GetAnAlgName (LPSTR lpString) {

  char *szFileName="DSPMGR.INI"; // the .INI file we want to look in
    FARPROC lpfnLOADDSPMsgProc;
  BOOL i;

  GetPrivateProfileSections( (LPSTR) SectionNames, MAX_FILENAME_LENGTH,
			    (LPSTR) szFileName);

  lpfnLOADDSPMsgProc = MakeProcInstance((FARPROC)LOADDSPMsgProc, hInst);
  i = DialogBox(hInst, MAKEINTRESOURCE(DLG_LOADALG), hWndMain,
		lpfnLOADDSPMsgProc);
  FreeProcInstance(lpfnLOADDSPMsgProc);
  
  if (i) {
    strcpy( lpString, SectionNames );
    return TRUE;
  } else {
    return FALSE;
  }

}

/*********************************************************************
//
// BOOL     LOADDSPMsgProc
//
// Description:        "Load DSP"  dialog window procedure
//
// Parameters:
//            HWND     hWndDlg       - Handle of the dialog box
//            WORD     Message       - Message ID coming from the dialog box
//            WORD     wParam        - First message dependent parameter
//            LONG     lParam        - Second message dependent parameter
//
// Return Value:
//            BOOL     Termination status
//
**********************************************************************/

BOOL FAR PASCAL LOADDSPMsgProc(HWND hWndDlg, WORD Message, WORD wParam, LONG lParam) {

  LRESULT n;
  LPSTR spTemp;
  HWND hwndList;

  switch(Message) {
  case WM_INITDIALOG:
    cwCenter(hWndDlg, 0);
    hwndList = GetDlgItem(hWndDlg, 101);
    
    /* add file names to the list box in the dialog */
    spTemp=SectionNames;
    while(*spTemp) {
      SendMessage(hwndList, LB_ADDSTRING, 0, (LONG) (LPSTR) spTemp);
      
      while(*spTemp) {        /* seek to next null */
	spTemp++;
      }
	
      spTemp++;             /* take one more step... is this null? */
    }                       /* remember, the end is a double null */
      
    InvalidateRect(hWndDlg, NULL, TRUE);
    break;     /* End of WM_INITDIALOG */

  case WM_CLOSE:
      /* Closing the Dialog behaves the same as Cancel */               
    PostMessage(hWndDlg, WM_COMMAND, IDCANCEL, 0L);
    break;     /* End of WM_CLOSE */

  case WM_COMMAND:
    switch(wParam) {
    case 101:      // Listbox ID... used to catch the double click
      if ( HIWORD(lParam) == LBN_DBLCLK ) {
	PostMessage(hWndDlg, WM_COMMAND, IDOK, 0L);
      }
      break;

    case IDOK:
      hwndList = GetDlgItem(hWndDlg, 101);
      n = SendMessage (hwndList, LB_GETCURSEL, 0, 0L);
      if (n != LB_ERR) {
	SendMessage (hwndList, LB_GETTEXT, (WORD)n,
		     (LONG) (LPSTR) SectionNames);
      }
      EndDialog(hWndDlg, TRUE);
      break;
      
    case IDCANCEL:
      /* Ignore data values entered into the controls
       * and dismiss the dialog window returning FALSE */       
      EndDialog(hWndDlg, FALSE);
      break;
    }
    break;    /* End of WM_COMMAND */
    
  default:
    return FALSE;
  }
  return TRUE;
}     /* ---- End of LOADDSPMsgProc ---- */



/***********************************************************************
//
// Function: GetPrivateProfileSections
//
// Purpose: This function enumerates all of the profile sections in
//          the given profile.
//
// Parameters:
//    lpOutBuf - buffer holds returned section headers
//    cbMax - maximum size of buffer
//    lpszFile - INI file name
//
// Returns:
//    0 on error
//    TRUE otherwise
//
// Comments: Section headers in the returned buffer are separated by NULLs
//           and terminate in a double NULL
//           Requires FindEndOfLine helper function
//
// Copied from the INIHEADR sample in the Microsoft Software/Data Libary
//
*************************************************************************/

UINT WINAPI GetPrivateProfileSections( LPSTR lpOutBuf, UINT cbMax, LPSTR lpszFile ) {

   HFILE          hFile ;
   OFSTRUCT       OF ;

   LONG           lBytes ;       /* number of bytes read from file */
   LPSTR          lpFileBuf ;
   LPSTR          lpCur ;        /* Pointers into the file buffer */
   LPSTR          lpEOL ;
   LPSTR          lpEOF ;
   LPSTR          lpLine ;
   BOOL           fInSec ;

   LPSTR          lpOutBufCur ;  /* pointers into the output buffer */
   UINT           cbCurSec ;
   UINT           wFileSize ;

   char           TempBuffer[80] ;    // used for section name
   LPSTR          lpTempBuffer ;

   if ((HFILE)-1 == (hFile = OpenFile( lpszFile, &OF, OF_READ | OF_SHARE_DENY_NONE ))) {
     // Could not open ini file lpszFile
       return 0 ;
   }

   /*
    * Read the entire file into memory.
    */

   wFileSize = (UINT)filelength( hFile ) ;       

   if (!(lpFileBuf = GlobalAllocPtr( GHND, wFileSize + 1 ))) {
     // Could not allocate global mem for sections (wFileSize+1 bytes)
     _lclose( hFile ) ;
     return 0 ;
   }

   if (-1 != (lBytes = _lread( hFile, lpFileBuf, wFileSize ))) {
     /* Read lBytes bytes into buffer (wFileSize+1 bytes in size) */

     /* Empty file check */
     if (lBytes < 3) {
       _lclose( hFile ) ;
       GlobalFreePtr( lpFileBuf ) ;
       return 0 ;
     }

     /*
      * Go through file with lpLine always pointing to the
      * beginning of a line and lpEOL always pointing to the
      * last character of a line.
      *
      * lpCur is our current pointer within the line.
      */
     lpLine = lpFileBuf ;
     lpEOF = lpFileBuf + (int)lBytes ;

     lpOutBufCur = lpOutBuf;

     lpOutBuf[0] = '\0' ;
     lpOutBuf[1] = '\0' ;
     
     while ((lpEOL = FindEndOfLine( lpLine, lpEOF )) != NULL) {

       /*
	* Scan the current line.  If we find a ';' the line is
	* a comment.  If we find a '[' then we are beginning
	* a section.
	*/
       for (lpCur = lpLine, fInSec = FALSE, cbCurSec = 0, lpTempBuffer=TempBuffer ; lpCur < lpEOL ; lpCur++) {
	 if (fInSec) {
	   /*
	    * We are in a section
	    */
	   if (*lpCur == ']') {
	     break ;
	   }

	   *lpTempBuffer++ = *lpCur;
	   cbCurSec++ ;
	 } else {
	   if (*lpCur == '[') {
	     fInSec = TRUE ;
	   }

	   if ((lpCur == lpLine) && !(strncmp(lpCur,"file=",5))) {
	     /*
	      * At the start of a line and the first 5 characters
	      * are 'file='... ie a valid algorithm name is in
	      * the buffer. (used to filter out non-alg sections) 
	      */
	     strcpy(lpOutBufCur, TempBuffer);
	     while ( *lpOutBufCur++);        //move past the '\0'
	   }

	   if (*lpCur == ';') {
	     /*
	      * This line is a comment.  Break out of the for loop,
	      * and go on to the next line.
	      */
	     break ;
	   }

	   if (*lpCur == '=') {
	     /*
	      * This line is key value line.
	      */
	     break ;
	   }

	 }
       }

       if (cbCurSec) {
	 *lpTempBuffer++ = '\0' ;
       }


       lpLine = lpEOL ;

     }

     /*
      * Put the second NULL on
      */
     *lpOutBufCur++ = '\0' ;
   }

   _lclose( hFile ) ;

   GlobalFreePtr( lpFileBuf ) ;

   return TRUE ;

 } /* GetPrivateProfileSections()  */


/***********************************************************************
//
// Function: FindEndOfLine
//
// Purpose: This function scans the line lpLine returning a pointer to
//          the last character on the line.
//
// Parameters:
//   lpLine - pointer that will point to end of line
//   lpEOF  - pointer to end of file
//
//
// Returns:  NULL when lpLine == lpEOF
//           pointer to last character on line when successful
//
// Comments:
//
// Copied from the INIHEADR sample in the Microsoft Software/Data Libary
//
/***********************************************************************/

LPSTR NEAR PASCAL FindEndOfLine( LPSTR lpLine, LPSTR lpEOF ) {

   register char  c ;

   while (lpLine <= lpEOF) {
     c = *lpLine ;

     if (c == '\n' || c == '\r' || c == '\f' || c == '\x1A') {
       /*
	* Scan until you get to a non '\n', '\r', '\f', or '\x1A'
	* character and return that pointer.
	*/
       while( lpLine <= lpEOF ) {
	 if (c == '\n' || c == '\r' || c == '\f' || c == '\x1A') {
	   lpLine++ ;
	 } else {
	   return lpLine ;
	 }

	 c = *lpLine ;

       }

     }
     lpLine++ ;
   }

   if (lpLine >= lpEOF) {
      return NULL ;
    }

   return lpLine ;

 } /* FindEndOfLine()  */



