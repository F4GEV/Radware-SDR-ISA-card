/**********************************************************************

	dspshell.h

	Shell Api declarations

	Copyright (c) 1993 Analog Devices Inc. All rights reserved

***********************************************************************/

#ifndef _INC_DSPSHELL           /* avoid multiple includes */
#define _INC_DSPSHELL


/********************************************* prerequisites */

#include "dspsdk.h"


/********************************************* definitions */

#define LONG  long
#define ZSTR  char *
#define PSTR  char *
#define WORD  unsigned int
#define UINT  unsigned int
#define DWORD unsigned long
#define PTR   int *
#define HALG  WORD
#define HAPP  WORD
#define HDEV  WORD
#define BYTE  WORD
#define HANDLE WORD

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



/******************************************** structures */


typedef struct
{
  int wNum;
  DSPIODT types[DSPIODT_MAX];
}DSPIODTS;

#define DSPRATE_MAX 23

typedef struct
{
  int wNum;
  long rate[DSPRATE_MAX];
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

/*********************************************** user functions */

extern void dspAlgMain(WORD wCbId, WORD wArg1, WORD wArg2, WORD wArg3);



/*********************************************** shell functions */

extern UINT dspGetData(HALG hAlg, PTR pData, WORD wSize);
extern UINT dspGetIo(HALG hAlg, HDEV hDev, PTR pData, WORD wSize);
extern UINT dspGetIoDevCaps(HDEV hDev, DSPIOCAPS *pCaps, WORD wSize);
extern UINT dspGetIoDevConf(HDEV hDev, DSPIOCONF *pConf, WORD wSize);
extern UINT dspGetMsg(HALG hAlg, PTR pMsg, WORD wSize);
extern UINT dspGetNumIoDevs(void);
extern UINT dspGetSysCaps(DSPSYSCAPS *pCaps, WORD wSize);
extern UINT dspReleaseCheck(void);
extern UINT dspReleaseToShell(void);
extern UINT dspSendData(HALG hAlg, PTR pData, WORD wSize);
extern UINT dspSendIo(HALG hAlg, HDEV hDev, PTR pData, WORD wSize);
extern UINT dspSendMsg(HAPP hApp, HALG hAlg, WORD wId, WORD wSize, PTR pMsg);
extern UINT dspSetIoDevConf(HDEV hDev, DSPIOCONF *pConf, WORD wSize);
extern UINT dspStartIo(HALG hAlg, HDEV hDev);
extern UINT dspStartTimer(HALG hAlg, BYTE bScale, WORD wCount);
extern UINT dspStopIo(HALG hAlg, HDEV hDev);
extern UINT dspStopTimer(HALG hAlg);


#endif

