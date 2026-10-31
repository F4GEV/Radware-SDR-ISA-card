/****************************************************************

   fcreate.c

   Copyright (c) 1994 Analog Devices Inc.  All rights reserved.
          
****************************************************************/
#define DSPMSG_FILE_DATALENGTH 1000
#include "dspshell.h"

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

#define STR_INVALIDHANDLE        "Invalid Handle"
#define STR_INVALIDDEVICE        "Invalid Device"
#define STR_INVALIDIO            "Invalid IO Configuration"
#define STR_USERABORT            "User I/O Abort"
#define STR_NOIOBUFFER           "No I/O Buffer"
#define STR_INVALIDSHELLARGUMENT "Invalid Shell Argument"
#define STR_INVALIDSIZEARGUMENT  "Invalid Size Argument"
#define STR_SHELLBUFFERQUEUEFULL "Shell Buffer Queue Full"
#define STR_SHELLEVENTQUEUEFULL  "Shell Event Queue Full"
#define STR_DEVCAPSERROR         "Dev Caps Error"
#define STR_GETMSGERROR          "Get Msg Error"
#define STR_UNKNOWNMSG           "Algorithm received unknown message"

typedef struct msg
{
  DSPMSGHEADER hdr;
  int data[502];
}MSG_STRUCT;

MSG_STRUCT msg;
DSPMSGFWRITEREQ fwritereq;
int nReqNumber = 0;
DSPMSGFOPENREQ fopenreq;
DSPMSGFCLOSEREQ fclosereq;

int nCount = 0;
int hFile = -1;
int hAlg;
int hApp;

/********************************************************************
  Function: ErrChk
  Args:     error value
  Rets:     error value
  Info:     Function will send a error string to the host if the 
            error value is not DSPERR_SUCCESS.
 ********************************************************************/
int ErrChk(int error)
{
  switch(error)
  {
    case DSPERR_SUCCESS:
      break;
    case DSPERR_HANDLE:
      dspSendMsg (hApp, hAlg, DSPMSGID_STRING_PRINT, sizeof (STR_INVALIDHANDLE), STR_INVALIDHANDLE);
      break;
    case DSPERR_IO_DEVICE:
      dspSendMsg (hApp, hAlg, DSPMSGID_STRING_PRINT, sizeof (STR_INVALIDDEVICE), STR_INVALIDDEVICE);
      break;
    case DSPERR_IO_CONF:
      dspSendMsg (hApp, hAlg, DSPMSGID_STRING_PRINT, sizeof (STR_INVALIDIO), STR_INVALIDIO);
      break;
    case DSPERR_IO_ABORT:
      dspSendMsg (hApp, hAlg, DSPMSGID_STRING_PRINT, sizeof (STR_USERABORT), STR_USERABORT);
      break;
    case DSPERR_NO_BUFFER:
      dspSendMsg (hApp, hAlg, DSPMSGID_STRING_PRINT, sizeof (STR_NOIOBUFFER), STR_NOIOBUFFER);
      break;
    case DSPERR_ARG:
      dspSendMsg (hApp, hAlg, DSPMSGID_STRING_PRINT, sizeof (STR_INVALIDSHELLARGUMENT), STR_INVALIDSHELLARGUMENT);
      break;
    case DSPERR_SIZE:
      dspSendMsg (hApp, hAlg, DSPMSGID_STRING_PRINT, sizeof (STR_INVALIDSIZEARGUMENT), STR_INVALIDSIZEARGUMENT);
      break;
    case DSPERR_QUEUE_FULL:
      dspSendMsg (hApp, hAlg, DSPMSGID_STRING_PRINT, sizeof (STR_SHELLBUFFERQUEUEFULL), STR_SHELLBUFFERQUEUEFULL);
      break;
    case DSPERR_FULL:
      dspSendMsg (hApp, hAlg, DSPMSGID_STRING_PRINT, sizeof (STR_SHELLEVENTQUEUEFULL), STR_SHELLEVENTQUEUEFULL);
      break;
  }
  return(error);
}

/********************************************************************
  Function:  SendString
  Args:      str string pointer
  Rets:      NONE
  Info:      Function to send a string print message to the host
             application.
 ********************************************************************/
void SendString(char *str)
{
  ErrChk(dspSendMsg(hApp, hAlg, DSPMSGID_STRING_PRINT, strlen(str)+1,str));
}

/********************************************************************
  Function:  ParseMsg
  Args:      wParam1 wParam2 wParam3
  Rets:      NONE
  Info:      Function that handles all messages from the host 
             application.
 ********************************************************************/
void ParseMsg(int wParam1, int wParam2, int wParam3)
{
  int i;
  int wTemp;
  long *pLong;
  MSG_STRUCT *pMsg;
  DSPMSGFOPENACK *fopenack;
  DSPMSGFCLOSEACK *fcloseack;
  DSPMSGFWRITEACK *fwriteack;

  pMsg = (MSG_STRUCT *)wParam2;

  switch(pMsg->hdr.id)
  {
  case DSPMSGID_FILE_FOPEN:
    fopenack = (DSPMSGFOPENACK *)wParam2;
    hFile = fopenack->hFile;
    if(hFile < 0)
      SendString("File Handle Error");
    else
    {
      fwritereq.hFile = hFile;
      fwritereq.nWordsToWrite = DSPMSG_FILE_DATALENGTH;
      for(i=0; i<DSPMSG_FILE_DATALENGTH; i++)
	fwritereq.data[i]=nCount++;
      ErrChk(dspSendMsg (hApp, hAlg, DSPMSGID_FILE_FWRITE, 
			 sizeof(DSPMSGFWRITEREQ),
			 (int *)&fwritereq));
    }
    break;

  case DSPMSGID_FILE_FCLOSE:
    fcloseack = (DSPMSGFCLOSEACK *)wParam2;
    if(fcloseack->nError)
      SendString("Fclose Error");
    break;

  case DSPMSGID_FILE_FWRITE:
    fwriteack = (DSPMSGFWRITEACK *)wParam2;
    if(fwriteack->hFile == hFile)
    {
      if(fwriteack->nWordsWritten != DSPMSG_FILE_DATALENGTH)
      {
	if(fwriteack->nWordsWritten == 0)
	{
	  SendString("Write Error");
	  fclosereq.hFile = hFile;
	  hFile = -1;
	  SendString("Closing File");
	  ErrChk(dspSendMsg(hApp, hAlg, DSPMSGID_FILE_FCLOSE, 
			    sizeof(DSPMSGFCLOSEREQ),
			    (int *)&fclosereq));
	}else
	  SendString("!1000 words!");
      }else
      {
	if(hFile != -1)
	{
	  for(i=0; i<DSPMSG_FILE_DATALENGTH; i++)
	    fwritereq.data[i]=nCount++;
	  ErrChk(dspSendMsg (hApp, hAlg, DSPMSGID_FILE_FWRITE, 
			     sizeof(DSPMSGFWRITEREQ),
			     (int *)&fwritereq));
	  if(nCount >= 10000)
	  {
	    SendString("Done");
	    fclosereq.hFile = hFile;
	    fwritereq.nWordsToWrite = DSPMSG_FILE_DATALENGTH;
	    hFile = -1;
	    ErrChk(dspSendMsg(hApp, hAlg, DSPMSGID_FILE_FCLOSE, 
			      sizeof(DSPMSGFCLOSEREQ),
			      (int *)&fclosereq));
	  }
	}
      }
    }
    break;      
  }
}

/********************************************************************
  Function:  dspAlgMain
  Args:      wCbId wParam1 wParam2 wParam3
  Rets:      NONE
  Info:      Function that is called by the shell. This function 
             handles all event messages.
  ********************************************************************/
void dspAlgMain(WORD wCbId, WORD wParam1, WORD wParam2, WORD wParam3)
{
  int *pBuf;
  int wTemp;
  int i,wNumDevs;

  switch(wCbId)
  {
    case DSPCB_INIT:
      /* INIT Algorithm Message */
      /* Save my alg and the host app handles */
      hAlg = wParam1;
      hApp = wParam2;
      SendString("Filling EXAMPLE3.DAT with 0-10000");
      msg.hdr.size = 502;
      ErrChk(dspGetMsg(hAlg, (int *)&msg, sizeof(MSG_STRUCT)));
      
      strcpy(fopenreq.FileName,"example3.dat");
      strcpy(fopenreq.FileMode,"wb");
      ErrChk(dspSendMsg(hApp, hAlg, DSPMSGID_FILE_FOPEN, 
			sizeof(DSPMSGFOPENREQ),
			(int *)&fopenreq));
      break;

    case DSPCB_CLOSE:
      /* CLOSE Algorithm Message */
      if(hFile != -1)
      {
	fclosereq.hFile = hFile;
	hFile = -1;
	SendString("Closing File");
	ErrChk(dspSendMsg(hApp, hAlg, DSPMSGID_FILE_FCLOSE, 
			  sizeof(DSPMSGFCLOSEREQ),
			  (int *)&fclosereq));
      }
      break;

    case DSPCB_SENDMSG:
      break;

    case DSPCB_GETIO:
      break;

    case DSPCB_GETDATA:
      /* Give the buffer back to the IO driver where it came from */
      break;

    case DSPCB_GETMSG:
      /* Message From the Host Application */
      /* Check For an Error */
      if(wParam1 == DSPERR_SUCCESS)
      { 
        ParseMsg(wParam1, wParam2, wParam3);
        ErrChk(dspGetMsg(hAlg, (int *)wParam2, sizeof(MSG_STRUCT))); 
      }else
	ErrChk(wParam1);
      break;
  }
}
