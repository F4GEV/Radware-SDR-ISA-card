#include <stdio.h>
#include "dsp1lib.h"
//#include "dspmgr.h"
long pascal _export dspGetNumDevs(void);

static int AGC = 0,filter = 0,mute = 0, volume = 0;


long GetNumDevs(void)

{
  return dspGetNumDevs();
}

