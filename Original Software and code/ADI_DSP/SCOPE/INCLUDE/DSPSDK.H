/****************************************************************

   dspsdk.h

   Copyright (c) 1993 Analog Devices Inc.  All rights reserved.

****************************************************************/

#ifndef _INC_DSPSDK           /* avoid multiple includes */
#define _INC_DSPSDK


/********************************************* message ids */

#define DSPMSGID_SYSTEM                 (-100)
#define DSPMSGID_DEBUG                  (-200)
#define DSPMSGID_MEM                    (-300)
#define DSPMSGID_STRING                 (-400)
#define DSPMSGID_FILE                   (-500)
#define DSPMSGID_AUDIO                  (-600)

#define DSPMSGID_MEM_GETDM              (DSPMSGID_MEM - 0)
#define DSPMSGID_MEM_GETPM              (DSPMSGID_MEM - 1)

#define DSPMSGID_STRING_PRINT           (DSPMSGID_STRING - 0)

#define DSPMSGID_FILE_FREAD             (DSPMSGID_FILE - 0)
#define DSPMSGID_FILE_FOPEN             (DSPMSGID_FILE - 1)
#define DSPMSGID_FILE_FCLOSE            (DSPMSGID_FILE - 2)
#define DSPMSGID_FILE_FWRITE            (DSPMSGID_FILE - 3)

#define DSPMSGID_AUDIO_START            (DSPMSGID_AUDIO - 0)
#define DSPMSGID_AUDIO_STOP             (DSPMSGID_AUDIO - 1)
#define DSPMSGID_AUDIO_PAUSE            (DSPMSGID_AUDIO - 2)
#define DSPMSGID_AUDIO_RESUME           (DSPMSGID_AUDIO - 3)
#define DSPMSGID_AUDIO_MUTE             (DSPMSGID_AUDIO - 4)
#define DSPMSGID_AUDIO_UNMUTE           (DSPMSGID_AUDIO - 5)
#define DSPMSGID_AUDIO_GETMODE          (DSPMSGID_AUDIO - 6)
#define DSPMSGID_AUDIO_SETMODE          (DSPMSGID_AUDIO - 7)
#define DSPMSGID_AUDIO_GETRATE          (DSPMSGID_AUDIO - 8)
#define DSPMSGID_AUDIO_SETRATE          (DSPMSGID_AUDIO - 9)
#define DSPMSGID_AUDIO_GETBASS          (DSPMSGID_AUDIO - 10)
#define DSPMSGID_AUDIO_SETBASS          (DSPMSGID_AUDIO - 11)
#define DSPMSGID_AUDIO_GETTREBLE        (DSPMSGID_AUDIO - 12)
#define DSPMSGID_AUDIO_SETTREBLE        (DSPMSGID_AUDIO - 13)
#define DSPMSGID_AUDIO_GETVOLUME        (DSPMSGID_AUDIO - 14)
#define DSPMSGID_AUDIO_SETVOLUME        (DSPMSGID_AUDIO - 15)
#define DSPMSGID_AUDIO_GETPOSITION      (DSPMSGID_AUDIO - 16)
#define DSPMSGID_AUDIO_SETPOSITION      (DSPMSGID_AUDIO - 17)
#define DSPMSGID_AUDIO_GETWORDSIZE      (DSPMSGID_AUDIO - 18)
#define DSPMSGID_AUDIO_SETWORDSIZE      (DSPMSGID_AUDIO - 19)
#define DSPMSGID_AUDIO_GETSTATUS        (DSPMSGID_AUDIO - 20)
#define DSPMSGID_AUDIO_SETSTATUS        (DSPMSGID_AUDIO - 21)
#define DSPMSGID_AUDIO_RESET            (DSPMSGID_AUDIO - 22)
#define DSPMSGID_AUDIO_PLAY             (DSPMSGID_AUDIO - 23)
#define DSPMSGID_AUDIO_RECORD           (DSPMSGID_AUDIO - 24)



/********************************************* structures */

typedef struct {          /* DSP message header */
  int to;
  int from;
  int id;
  int size;
}DSPMSGHEADER;


/***************************************** File IO Message Structures */

/******************************************* User Defined Lengths */
#ifndef DSPMSG_FILE_DATALENGTH
#define DSPMSG_FILE_DATALENGTH 200
#endif

#ifndef DSPMSG_FILE_NAMELENGTH
#define DSPMSG_FILE_NAMELENGTH 100
#endif

#ifndef DSPMSG_FILE_MODELENGTH
#define DSPMSG_FILE_MODELENGTH 10
#endif

/******************************************* Dsp to Host Request Messages */

typedef struct dspmsg_file_fopenreq {
  char FileName[DSPMSG_FILE_NAMELENGTH];
  char FileMode[DSPMSG_FILE_MODELENGTH];
} DSPMSGFOPENREQ;

typedef struct dspmsg_file_fclosereq {
  short hFile;
} DSPMSGFCLOSEREQ;

typedef struct dspmsg_file_freadreq {
  short hFile;
  short nWordsToRead;
} DSPMSGFREADREQ;

typedef struct dspmsg_file_fwritereq {
  short hFile;
  short nWordsToWrite;
  short data[DSPMSG_FILE_DATALENGTH];
} DSPMSGFWRITEREQ;


/******************************************* Host to Dsp Acknowledge Messages */

typedef struct dspmsg_file_fopenack {
#ifdef _INC_DSPSHELL
  DSPMSGHEADER hdr;
#endif
  short hFile;
} DSPMSGFOPENACK;

typedef struct dspmsg_file_fcloseack {
#ifdef _INC_DSPSHELL
  DSPMSGHEADER hdr;
#endif
  short nError;
} DSPMSGFCLOSEACK;

typedef struct dspmsg_file_freadack {
#ifdef _INC_DSPSHELL
  DSPMSGHEADER hdr;
#endif
  short hFile;
  short nWordsRead;
  short data[DSPMSG_FILE_DATALENGTH];
} DSPMSGFREADACK;

typedef struct dspmsg_file_fwriteack {
#ifdef _INC_DSPSHELL
  DSPMSGHEADER hdr;
#endif
  short hFile;
  short nWordsWritten;
} DSPMSGFWRITEACK;

/***************************************** End File IO Message Structures */




#endif
 
