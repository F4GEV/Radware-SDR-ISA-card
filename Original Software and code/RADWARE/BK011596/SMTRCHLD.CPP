#include "thisCls.h"
#include <stdio.h>
#include <math.h>

#define MAX_PLACES 25

//REGEN_VARIABLES
extern int starting_places[][2];
extern int delete_class[];
extern int am_offset_active;

extern TBitmap *smeter_bitmap;
extern TBitmap *am_meter_bitmap;
extern PIF_GAIN_Control if_gain_control;
extern PMIXER_GAIN_Control mixer_gain_control;
extern PMODE_Control mode_control;
extern PAM_Frequency_Offset_Control am_offset_control;

int get_channel_status(struct channel_status_struct *status);
//REGEN_VARIABLES

TSMTRChld::TSMTRChld(TWindow *AParent, int id,LPSTR AName,int child_number,int w,
                     int h,char *icon,int Filter)
		     :AWindow(AParent, AName,child_number)
{
  int count;
  char message[3];
  int x,y;

  x = starting_places[child_number][0];
  y = starting_places[child_number][1];
  if (y<0)
    y = -y;
  Attr.Style = WS_POPUP | WS_VISIBLE | WS_CAPTION |WS_MINIMIZEBOX | WS_SYSMENU;
  Attr.X = x;
  if (y<0)
    Attr.Y = -y;
  else
    Attr.Y = y;
  Attr.W = 140;
  Attr.H = 101;
  filter = Filter;
  place = 0;
  message[0] = 1;
  message[1] = 0;
  mute_position = 0;
  if (filter)     /* SMeter always has filter on */
  {
    Digital_Readout = new TStatic(this,-1,"        0 dBm",0,61,138,20);
    DSPSendMsg(msg_SMETER,message,0);
    smeter = 50;
  }
  else
  {
    Mute_Button = new AButton(this,101,"ON",0,61,138,20,FALSE);
    smeter = 125;
  }
  for(count=0;count<MAX_PLACES;count++)
    last_values[count]=smeter;
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
  if (filter)
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
  char string[80];

  if (filter)
  {
/*    if (value>40)
    {
      value += (45-if_gain_control->Get_Value()*3);
      value += (20-mixer_gain_control->Get_Value()*10);
    }
*/
    if (value>-19)
      value = -19;
    last_values[place++] = value;
    smeter = 0;
    if (place>=MAX_PLACES)
      place = 0;
    for(count = 0;count<MAX_PLACES;count++)
      smeter+=last_values[count];
    smeter/=MAX_PLACES;
    smeter = value;
    sprintf(string,"          %d dBm",smeter);
    Digital_Readout->SetText(string);
  }
  else
    smeter = value;
  InvalidateRgn(NULL,0);
  UpdateWindow();
}

void TSMTRChld::Toggle_Mute()

{
  if (mute_position)
  {
    mute_position = 0;
    am_offset_active = 1;
    Mute_Button->SetCaption("ON");
  }
  else
  {
    mute_position = 1;
    am_offset_active = 0;
    Mute_Button->SetCaption("HOLD");
  }
  Redraw();
}

void TSMTRChld::Paint(TDC& DC, BOOL,TRect &)

{
  int x,y;
  TMemoryDC *MDC;

  MDC = new TMemoryDC(DC);
  if (filter)
    MDC->SelectObject(*smeter_bitmap);
  else
    MDC->SelectObject(*am_meter_bitmap);
  if (filter)
    DC.BitBlt(0,0,140,95,*MDC,0,0,SRCCOPY);
  else
    DC.BitBlt(0,-10,140,71,*MDC,0,0,SRCCOPY);
  if (filter || mode_control->Get_Value() == 0) /*if showing AM OFFSET,
                                                  only do so in AM */
  {
    if (smeter<0)
      smeter+=256;
    DC.MoveTo(70,150);
    if (filter)
    {
      x = 70+150*cos(3.30-smeter/110.0);
      y = 150-150*sin(3.30-smeter/110.0);
    }
    else
    {
      x = 70+150*cos(2.20-smeter/200.0);
      y = 150-150*sin(2.20-smeter/200.0);
    }
//    if (!filter)
      DC.MoveTo((x+70)/2,60);
    DC.LineTo(x,y);
  }
  MDC->RestoreObjects();
  delete (MDC);
}

DEFINE_RESPONSE_TABLE1(TSMTRChld,AWindow)
  EV_COMMAND(101,Toggle_Mute),
END_RESPONSE_TABLE;

