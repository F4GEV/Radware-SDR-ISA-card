//REGEN_FILEHEADING
//REGEN_FILEHEADING

     /********************************************************************
      *                                                                  *
      *   Source File: SpectrID.h                                        *
      *   Description: Header file IDS for Spectrum application          *
      *   Date:        Fri Oct 02 17:42:35 1992                          *
      *                                                                  *
      ********************************************************************/

#ifndef __SPECTRUMID__H
#define __SPECTRUMID__H

//REGEN_HEADER
//REGEN_HEADER

// Defines for menu item IDs
#define   IDM_SWEEP                           101
#define   IDM_ITEM2                           102
#define   IDM_ITEM3                           103

#define MAX_PIXELS 250

struct unit_status_struct

{
  int unit,channel;
  long frequency;
};

#endif // __SPECTRUMID__H
