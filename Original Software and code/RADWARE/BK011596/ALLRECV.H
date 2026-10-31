#ifndef __TALLRECIEVE__H
#define __TALLRECIEVE__H

_CLASSDEF(TAllRecieve);

class TAllRecieve:public AWindow

{
  int Child_Number;

  public:
  TFrequency_Window *PVFO_A;

  TAllRecieve(AWindow *AParent, LPSTR AName,PFrequency_Control vfo_a,
                  int child_number);
  ~TAllRecieve();
  DECLARE_RESPONSE_TABLE(TAllRecieve);
};

#endif 

