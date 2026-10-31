/****************************************************************

   dspmgr.h

   Copyright (c) 1993 Analog Devices Inc.  All rights reserved.

****************************************************************/

#ifndef _INC_DSPMGR           // avoid multiple includes
#define _INC_DSPMGR


/********************************************* definitions */

typedef HINSTANCE HALG;
typedef HALG FAR* LPHALG;

#define DSP_MAXSTRINGLENGTH  128
#define DSP_MAPPER  -1


/********************************************* processors */

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
#define DSPERR_BADAPPINSTANCE      -3
#define DSPERR_BADDEVICEID         -4
#define DSPERR_BADPROCESSOR        -5
#define DSPERR_JOBTRUNCATED        -6
#define DSPERR_LOWDATAMEMORY       -7
#define DSPERR_LOWPROGRAMMEMORY    -8
#define DSPERR_MSGTRUNCATED        -9
#define DSPERR_NOCYCLES            -10
#define DSPERR_NOMSG               -11
#define DSPERR_RESETTING           -12
#define DSPERR_SLOWCLOCK           -13
#define DSPERR_UNLOADING           -14
#define DSPERR_NOTSUPPORTED        -15
#define DSPERR_NOTSCOPECOMPATIBLE  -16

// NOT DOCUMENTED IN API SPEC
#define DSPERR_LOWHOSTMEMORY       -17
#define DSPERR_BADDOWNLOAD         -18
#define DSPERR_BADREQUESTID        -19
#define DSPERR_DRIVERTIMEOUT       -20


/********************************************* structures */

typedef struct {
        DWORD   dwMID;                         // manufacturer ID
        DWORD   dwPID;                         // product ID
        VERSION vDriverVersion;                // driver version 
	char    szPname[DSP_MAXSTRINGLENGTH];  // product name
	DWORD   dwProcType;                    // processor type
	DWORD   dwProgMem;                     // program memory size
	DWORD   dwDataMem;                     // data memory size
	DWORD   dwClock;                       // clock speed
} DSPCAPS, FAR* LPDSPCAPS;



#endif
