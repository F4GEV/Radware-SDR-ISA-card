/****************************************************************

   dsp1848.h

   Copyright (c) 1993 Analog Devices Inc.  All rights reserved.

****************************************************************/

#ifndef _INC_DSP1848           /* avoid multiple includes */
#define _INC_DSP1848


/**************************************** structures */
typedef struct soundportregs
{
  int wLeftInputControl;
  int wRightInputControl;
  int wLeftAux1Control;
  int wRightAux1Control;
  int wLeftAux2Control;
  int wRightAux2Control;
  int wLeftOutputControl;
  int wRightOutputControl;
  int wClockDataFormat;
  int wInterfaceConfig;
  int wPinControl;
  int wTestInitialization;
  int wMiscellaneous;
  int wDigitalMix;
  int wUpperBaseCount;
  int wLowerBaseCount;
}SOUNDPORTREGS;

/**************************************** functions */

extern void dspUtilProgram1848 (SOUNDPORTREGS *pSpregs);
extern void dspUtilGet1848 (SOUNDPORTREGS *pSpregs);



#endif
