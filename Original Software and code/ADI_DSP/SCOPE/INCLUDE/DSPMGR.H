/****************************************************************

   dspmgr.h

   Copyright (c) 1993 Analog Devices Inc.  All rights reserved.

****************************************************************/

#ifndef _INC_DSPMGR           // avoid multiple includes
#define _INC_DSPMGR


/********************************************* prerequisites */

#include "dspsdk.h"


/********************************************* definitions */

typedef HINSTANCE HALG;
typedef HINSTANCE HAPP;
typedef HALG FAR* LPHALG;

#define DSP_MAXSTRINGLENGTH  128          // length of strings

#define DSP_MAPPER  -1                    // generic device ID

#define DSPCB_API   (WM_USER + 0x8100)    // API callback messages starting value
#define DSPCB_SPI   (WM_USER + 0x8200)    // SPI callback messages starting value
#define DSPCB_DEBUG (WM_USER + 0x8300)    // Debug callback messages starting value



/********************************************* strings */

#define DSPSTR_PROGRAMMEMORY   "programmemory"
#define DSPSTR_DATAMEMORY      "datamemory"
#define DSPSTR_INIFILENAME     "dspmgr.ini"
#define DSPSTR_CLOCK           "clock"
#define DSPSTR_PROCESSOR       "processor"
#define DSPSTR_DELIMITERS      ", "
#define DSPSTR_FILE            "file"
#define DSPSTR_SCOPECOMPATIBLE "scopecompatible"
#define DSPSTR_YES             "yes"
#define DSPSTR_NO              "no"



/********************************************* callbacks */

                              // callback function into the app
typedef VOID (FAR PASCAL _export * DAPIFPCALLBACK) (HALG hAlg,
                                                    DWORD dwMsg,
                                                    DWORD dwInstance,
                                                    DWORD dwParam1,
                                                    DWORD dwParam2);

#define DSPCB_MSGPENDING (DSPCB_API + 0)  // callback messages
#define DSPCB_REPLY      (DSPCB_API + 1)
#define DSPCB_RESET      (DSPCB_API + 2)




/********************************************* processor types */

typedef enum {
        DSP_ADSP2100,
        DSP_ADSP2101,
        DSP_ADSP2102,
        DSP_ADSP2105,
        DSP_ADSP2115,
        DSP_ADSP2171,
        DSP_ADSP21MSP50,
        DSP_ADSP21010,
        DSP_ADSP21020,
        DSP_ADSP21060
} DSP_PROCESSOR_TYPES;


/********************************************* errors */

#define DSPERR_SUCCESS              0
#define DSPERR_BADALGFILE          -1
#define DSPERR_BADALGHANDLE        -2
#define DSPERR_BADDEVICEID         -3
#define DSPERR_BADDOWNLOAD         -4
#define DSPERR_BADPROCESSOR        -5
#define DSPERR_BADREQUESTID        -6
#define DSPERR_CANTSTARTALG        -7
#define DSPERR_DRIVERTIMEOUT       -8
#define DSPERR_INCOMPATIBLEMODE    -9
#define DSPERR_JOBTRUNCATED        -10
#define DSPERR_LOWDATAMEMORY       -11
#define DSPERR_LOWHOSTMEMORY       -12
#define DSPERR_LOWPROGRAMMEMORY    -13
#define DSPERR_MSGTRUNCATED        -14
#define DSPERR_NOCYCLES            -15
#define DSPERR_NOTSUPPORTED        -16
#define DSPERR_NOTSCOPECOMPATIBLE  -17
#define DSPERR_RESETTING           -18
#define DSPERR_SLOWCLOCK           -19
#define DSPERR_UNLOADING           -20
#define DSPERR_VXDINITERR          -21
#define DSPERR_VXDSERVICE          -22


/********************************************* structures */

typedef struct {
        DWORD   dwMID;                         // manufacturer ID
        DWORD   dwPID;                         // product ID
        VERSION vDriverVersion;                // driver version 
	char    szPname[DSP_MAXSTRINGLENGTH];  // product name
        DWORD   dwDifferentiator;              // differentiator
	DWORD   dwProcType;                    // processor type
	DWORD   dwProgMem;                     // program memory size
	DWORD   dwDataMem;                     // data memory size
	DWORD   dwClock;                       // clock speed
} DSPCAPS, FAR* LPDSPCAPS;

typedef struct {
	DWORD dwInstance;                          // client instance info
	HALG  hAlg;                                // algorithm handle
        DWORD dwDeviceID;                          // device ID
        DWORD dwScopeCompatible;                   // SCOPE compatibility flag
	char  szSectionName[DSP_MAXSTRINGLENGTH];  // section name
	DWORD dwCallback;                          // callback address
	DWORD dwFlags;                             // type of callback
} DSPJOBINFO, FAR* LPDSPJOBINFO;


/********************************************* functions */

LONG WINAPI _export dspGetData (HALG hAlg,
                                LPVOID lpData,
                                DWORD dwSize);

LONG WINAPI _export dspGetDevCaps (DWORD dwDeviceID,
                                   LPDSPCAPS lpCaps,
                                   DWORD dwSize);

LONG WINAPI _export dspGetErrorInfo (LONG lErrorCode,
                                     LPSTR lpBuffer,
                                     DWORD dwSize);

LONG WINAPI _export dspGetJobInfo (DWORD dwDeviceID,
                                   LPDWORD lpCount,
                                   LPDSPJOBINFO const lpJobs,
                                   DWORD dwJobNumEntries,
                                   DWORD dwSize);

LONG WINAPI _export dspGetMsg (HALG hAlg,
                               LPLONG lpMsgID,
                               LPVOID lpMsgBody,
                               DWORD dwSize);

LONG WINAPI _export dspGetNumDevs (void);

LONG WINAPI _export dspGetNumJobs (DWORD dwDeviceID);

LONG WINAPI _export dspLoadAlg (DWORD dwDeviceID,
                                DWORD dwInstance,
                                LPHALG lphAlg,
                                LPSTR lpAlgSection,
                                DWORD dwCallback,
                                DWORD dwFlags);

LONG WINAPI _export dspPreloadAlg (DWORD dwDeviceID,
                                   DWORD dwInstance,
                                   LPHALG lphAlg,
                                   LPSTR lpAlgSection,
                                   DWORD dwCallback,
                                   DWORD dwFlags);

LONG WINAPI _export dspReset (DWORD dwDeviceID);

LONG WINAPI _export dspSendData (HALG hAlg,
                                 LPVOID lpData,
                                 DWORD dwSize);

LONG WINAPI _export dspSendMsg (HALG hAlg,
                                LONG lMsgID,
                                LPVOID lpMsgBody,
                                DWORD dwSize);

LONG WINAPI _export dspStartAlg (HALG hAlg);

LONG WINAPI _export dspUnloadAlg (HALG hAlg);


#endif
