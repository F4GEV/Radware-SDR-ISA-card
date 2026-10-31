#include "thisCls.h"
#include <owl\inputdia.h>
#include <stdio.h>
#include <math.h>

//REGEN_VARIABLES
struct single_station
{
  char name[16];
  long frequency;
  short mode;
  short band;
};

extern int starting_places[][2];
extern short int delete_class[];

extern struct single_station quick_stations[];

extern long bands[][2];
extern TCONTROLChld *PTMode,*PTFilter;
extern TFreq_Window2 *PTFreq;
extern TSpectrum *PTSpectrum;

extern PFrequency_Control frequency_control;
extern PMODE_Control mode_control;
extern PFILTER_Control filter_control;
extern PMax_Frequency_Control max_frequency_control;
extern PMin_Frequency_Control min_frequency_control;
extern TFreq_Window2 *PTFreq,*PTTXFreq,*PTMaxFreq,*PTMinFreq;

TBANDChld::TBANDChld(AWindow *AParent, LPSTR AName,int child_number)
		     :AWindow(AParent, AName, child_number)
{
   int count;
   int x,y;

   Child_Number = child_number;
   x = starting_places[Child_Number][0];
   y = starting_places[Child_Number][1];
   if (y<0)
     y = -y;
   Attr.Style = WS_POPUP | WS_VISIBLE | WS_CAPTION |WS_MINIMIZEBOX |
                WS_SYSMENU;
   if (x>=0)
     Attr.X = x;
   else
     Attr.X = -x;
   if (y>=0)
     Attr.Y = y;
   else
     Attr.Y = -y;
   Attr.W = 140;
   Attr.H = 101;
   Button[0] = new AButton(this,114,"160",0,0,46,28,0);
   Button[1] = new AButton(this,115,"80",46,0,47,28,0);
   Button[2] = new AButton(this,116,"40",93,0,46,28,0);
   Button[3] = new AButton(this,117,"30",0,27,46,28,0);
   Button[4] = new AButton(this,118,"20",46,27,47,28,0);
   Button[5] = new AButton(this,119,"17",93,27,46,28,0);
   Button[6] = new AButton(this,120,"15",0,54,46,28,0);
   Button[7] = new AButton(this,121,"12",46,54,47,28,0);
   Button[8] = new AButton(this,122,"10",93,54,46,28,0);
}

TBANDChld::~TBANDChld()

{
  Attr.Y *= -1;
  if (Attr.Y>=0)
    Attr.Y = -1;
  starting_places[Child_Number][0] = Attr.X;
  starting_places[Child_Number][1] = Attr.Y;
  delete_class[Child_Number] = 1;
}

void TBANDChld::EvActivate(uint active,bool b,HWND c)

{
  if (active != WA_INACTIVE)
    MoveWindowToTopOfList(Child_Number);
  AWindow::EvActivate(active,b,c);
}

TBANDChld::Where_Y()

{
  return(Attr.Y);
}

void TBANDChld::Reply_Button(WPARAM ID)

{
  int wParam;

  wParam = ID;
  if (PTSpectrum==NULL || delete_class[12]) //PTSpectrum->Attr.H<0)
  {
    if (ID == 161) /* CB IS OUT OF PLACE */
      ID = 201;
    if (PTMinFreq && PTMinFreq->Attr.H >= 0)
      PTMinFreq->Set_Value(bands[ID-113][0]);
    else
      min_frequency_control->Set_Value(bands[ID-113][0]);
    if (PTMaxFreq && PTMaxFreq->Attr.H >= 0)
      PTMaxFreq->Set_Value(bands[ID-113][1]);
    else
      max_frequency_control->Set_Value(bands[ID-113][1]);
    if (PTFreq && PTFreq->Attr.H >= 0)
      PTFreq->Redraw_Frequency();
    if (PTTXFreq && PTTXFreq->Attr.H >= 0)
      PTTXFreq->Redraw_Frequency();
    return;
  }
  MessageBox("You may not change the band while "
             "Spectrum Analyzer is running","Error",MB_ICONHAND | MB_OK);
}

DEFINE_RESPONSE_TABLE1(TBANDChld,AWindow)
  EV_COMMAND_AND_ID(114,Reply_Button),
  EV_COMMAND_AND_ID(115,Reply_Button),
  EV_COMMAND_AND_ID(116,Reply_Button),
  EV_COMMAND_AND_ID(117,Reply_Button),
  EV_COMMAND_AND_ID(118,Reply_Button),
  EV_COMMAND_AND_ID(119,Reply_Button),
  EV_COMMAND_AND_ID(120,Reply_Button),
  EV_COMMAND_AND_ID(121,Reply_Button),
  EV_COMMAND_AND_ID(122,Reply_Button),
END_RESPONSE_TABLE;

