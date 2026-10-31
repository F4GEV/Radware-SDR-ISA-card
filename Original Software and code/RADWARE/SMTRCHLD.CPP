#include "thisCls.h"
#include <stdio.h>
#include <math.h>

#define MAX_PLACES 25

//REGEN_VARIABLES
extern TBitmap *smeter_bitmap;

int get_channel_status(struct channel_status_struct *status);
//REGEN_VARIABLES

TSMTRChld::TSMTRChld(TWindow *AParent, int id,LPSTR AName,int x,int y,int w,
                     int h,char *icon,int Filter)
		     :AWindow(AParent, AName)
{
  int count;
  char message[3];

  Attr.Style = WS_POPUP | WS_VISIBLE | WS_CAPTION |WS_MINIMIZEBOX | WS_SYSMENU;
  Attr.X = x;
  if (y<0)
    Attr.Y = -y;
  else
    Attr.Y = y;
  Attr.W = 140;
  Attr.H = 101;
  smeter = 0;
  filter = Filter;
  for(count=0;count<MAX_PLACES;count++)
    last_values[count]=1;
  place = 0;
  message[0] = 1;
  message[1] = 0;
  if (filter)     /* SMeter always has filter on */
    DSPSendMsg(msg_SMETER,message,0);
}

LPSTR TSMTRChld::GetClassName()

{
  return("S METER WINDOW");
}

void TSMTRChld::GetWindowClass(WNDCLASS _FAR & AWndClass)

{
  TWindow::GetWindowClass(AWndClass);
  AWndClass.hIcon = LoadIcon(_hInstance,"SMETER");
}

TSMTRChld::~TSMTRChld()

{
  char message[3];

  Attr.Y *= -1;
  if (Attr.Y>=0)
    Attr.Y = -1;
  message[0] = 0;
  message[1] = 0;
  if (filter == 1)
    DSPSendMsg(msg_SMETER,message,0);
}

TSMTRChld::Where_Y()

{
  return(Attr.Y);
}

int TSMTRChld::Width()

{
  return Attr.W;
}

int TSMTRChld::Height()

{
  return Attr.H;
}

void TSMTRChld::Set_Value(int value)

{
  int count;

  if (filter)
  {
    last_values[place++] = value;
    if (place>=MAX_PLACES)
      place = 0;
    for(count = 0;count<MAX_PLACES;count++)
      smeter+=last_values[count];
    smeter/=MAX_PLACES;
  }
  else
    smeter = value;
  InvalidateRgn(NULL,0);
  UpdateWindow();
}

void TSMTRChld::Paint(TDC& DC, BOOL,TRect &)

{
  int x,y;
  TMemoryDC *MDC;

  MDC = new TMemoryDC(DC);
  MDC->SelectObject(*smeter_bitmap);
  DC.BitBlt(5,-10,130,95,*MDC,0,0,SRCCOPY);
  if (smeter<0)
    smeter+=256;
  DC.MoveTo(70,150);
  x = 70+150*cos(2-smeter/245.0);
  y = 150-150*sin(2-smeter/245.0);
  DC.LineTo(x,y);
  MDC->RestoreObjects();
  delete (MDC);
}


