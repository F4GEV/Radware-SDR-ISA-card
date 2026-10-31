
#include "thisCls.h"

AButton::AButton(TWindow *AParent, int id, LPSTR AName,int x,int y,
                             int w,int h,BOOL IsDefault):
                             TButton(AParent,id,AName,x,y,w,h,IsDefault)

{
  ID = id;
}

AButton::~AButton()

{
}

void AButton::EvRButtonUp(UINT modKeys,TPoint& point)

{
  PostMessage(WM_COMMAND,ID+100,NULL);
}

DEFINE_RESPONSE_TABLE1(AButton,TButton)
  EV_WM_RBUTTONUP,
END_RESPONSE_TABLE;

