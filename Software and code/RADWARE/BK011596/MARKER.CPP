
//REGEN_FILEHEADING
//REGEN_FILEHEADING

#include <owl\framewin.h>
#include <owl\edit.h>
#include <owl\listbox.h>
#include <owl\combobox.h>
#include <owl\scrollba.h>
#include <owl\dialog.h>
#include <bwcc.h>
#include <stdio.h>
#include "thisCls.h"
//#include "SWLIB.h"
#include <owl\radiobut.h>
#include <owl\button.h>
#include <math.h>;

extern int starting_places[][2];
extern short int delete_class[];

// Define TMarker, a TWindow constructor
AMarker::AMarker(TWindow *AParent,int id, LPSTR AName,int child_number,TWindow *OWNER,unsigned int Spectrum[][2],
                 long Spectrum_Freq[][2],PStart_Control pStart,PStop_Control pStop,long *Start,long *Stop):AWindow(AParent, AName,child_number)

{
   int x,y;

   Child_Number = child_number;
   x = starting_places[Child_Number][0];
   y = starting_places[Child_Number][1];
   Attr.Style = WS_MINIMIZEBOX | WS_POPUP | WS_CLIPSIBLINGS | WS_VISIBLE |
                WS_CAPTION | WS_SYSMENU;
   SetCaption("Marker");
   start = Start;
   stop = Stop;
   if (x>=0)
     Attr.X = x;
   else
     Attr.X = -x;
   if (y>=0)
     Attr.Y = y;
   else
     Attr.Y = -y;
   Attr.W = 162;
   Attr.H = 141;
   Marker_Window = new TFrequency_Window(this,101,"FREQ",Marker = new Marker_Control(start,stop),0,0);
   Owner = OWNER;
   ID = id;
   this_marker = Marker->Get_Value();
   PmVolts = new WhiteStatic(this,-1," 0.000016 V",70,33,85,20,20);
   PmVolts->Attr.Style |= WS_BORDER;
   PDBM = new WhiteStatic(this,-1," -96 dbm",5,33,60,20,20);
   PDBM->Attr.Style |= WS_BORDER;
   PPeak = new TButton(this,102,"Peak",5,63,60,20,FALSE);
   PNext = new TButton(this,103,"Next Peak",70,63,85,20,FALSE);
   PStart = pStart;
   PStop = pStop;
   spectrum = Spectrum;
   spectrum_freq = Spectrum_Freq;
}

// Define TMarker destructor
AMarker::~AMarker()
{
  Attr.H = -1;
  delete Marker;
}

LPSTR AMarker::GetClassName()

{
  return("MARKERWIND");
}

void AMarker::GetWindowClass(WNDCLASS _FAR & AWndClass)

{
//  HBRUSH hbr;

  hbr = CreateSolidBrush(RGB(0,0,255));
  TWindow::GetWindowClass(AWndClass);
  AWndClass.hbrBackground = hbr;
//  DeleteObject(hbr);
}

void AMarker::Changed_Marker()

{
  HRGN Region;
  int pixel;
  char string[20];
  int true_value;
  double Volts;


  ::PostMessage(Parent->HWindow,ID,0,0);
  last_marker = this_marker;
  this_marker = Marker->Get_Value();
  pixel = (this_marker-PStart->Get_Value())*1.0*MAX_PIXELS/(PStop->Get_Value()-PStart->Get_Value());
  true_value = -90+(spectrum[pixel][1]-16)/2.4;
  Volts = pow(10,true_value/20.0);
  sprintf(string," %d dbm",true_value);
  PDBM->SetText(string);
  sprintf(string," %7.6lf V",Volts);
  PmVolts->SetText(string);

 }

void AMarker::Peak_Button()

{
  int count;
  int peak=0;

  for(count=0;count<MAX_PIXELS;count++)
    if (spectrum[count][1]>spectrum[peak][1])
      peak = count;
  Marker->Set_Value(spectrum_freq[peak][1]);
  InvalidateRgn(NULL,1);
  UpdateWindow();
  PostMessage(101,0,0);
}
 
void AMarker::Next_Button()

{
}

long AMarker::Get_Value()

{
  return Marker->Get_Value();
}

long AMarker::Get_Last()

{
  return last_marker;
}

void AMarker::set_DBM(int DBM)

{
  char string[20];
  int true_value;
  double Volts;

  true_value = -90+(DBM-16)/2.4;
  Volts = pow(10,true_value/20.0);
  sprintf(string," %d dbm",true_value);
  PDBM->SetText(string);
  sprintf(string," %7.6lf V",Volts);
  PmVolts->SetText(string);
}

DEFINE_RESPONSE_TABLE1(AMarker,TWindow)
  EV_COMMAND(101,Changed_Marker),
  EV_COMMAND(102,Peak_Button),
  EV_COMMAND(103,Next_Button),
END_RESPONSE_TABLE;

