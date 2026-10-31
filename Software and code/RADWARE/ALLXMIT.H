#ifndef __TALLTRANSMIT__H
#define __TALLTRANSMIT__H

_CLASSDEF(TAllTransmit);

class TAllTransmit:public AWindow

{
  int Child_Number;

  public:
  PFrequency_Control VFO_A;
  TFrequency_Window *PVFO_A;

  TAllTransmit(AWindow *AParent, LPSTR AName,PFrequency_Control vfo_a,
                  int child_number);
  ~TAllTransmit();
  DECLARE_RESPONSE_TABLE(TAllTransmit);
};
#endif 

