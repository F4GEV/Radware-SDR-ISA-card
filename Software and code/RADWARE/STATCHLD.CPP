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

extern struct single_station quick_stations[];

extern int starting_places[][2];
extern int delete_class[];

extern TCONTROLChld *PTMode,*PTFilter;
extern TFreq_Window2 *PTFreq;
extern TBANDSCANChld *PTBandScan;

extern PFrequency_Control frequency_control;
extern PMODE_Control mode_control;
extern PFILTER_Control filter_control;
extern PMax_Frequency_Control max_frequency_control;
extern PMin_Frequency_Control min_frequency_control;

TSTATChld::TSTATChld(AWindow *AParent, LPSTR AName,int Child_Number,int Bank)
		     :AWindow(AParent, AName,Child_Number)
{
   int count;
   int x,y;

   x = starting_places[Child_Number][0];
   y = starting_places[Child_Number][1];
   if (y<0)
     y = -y;
   Attr.Style = WS_POPUP | WS_VISIBLE | WS_CAPTION |WS_MINIMIZEBOX |
                WS_SYSMENU;
   bank = Bank;
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
   Button[0] = new AButton(this,101,quick_stations[bank*9+0].name,0,0,46,28,0);
   Button[1] = new AButton(this,102,quick_stations[bank*9+1].name,46,0,47,28,0);
   Button[2] = new AButton(this,103,quick_stations[bank*9+2].name,93,0,46,28,0);
   Button[3] = new AButton(this,104,quick_stations[bank*9+3].name,0,27,46,28,0);
   Button[4] = new AButton(this,105,quick_stations[bank*9+4].name,46,27,47,28,0);
   Button[5] = new AButton(this,106,quick_stations[bank*9+5].name,93,27,46,28,0);
   Button[6] = new AButton(this,107,quick_stations[bank*9+6].name,0,54,46,28,0);
   Button[7] = new AButton(this,108,quick_stations[bank*9+7].name,46,54,47,28,0);
   Button[8] = new AButton(this,109,quick_stations[bank*9+8].name,93,54,46,28,0);
}

TSTATChld::~TSTATChld()

{
  Attr.Y *= -1;
  if (Attr.Y>=0)
    Attr.Y = -1;
}

TSTATChld::Where_Y()

{
  return(Attr.Y);
}

void TSTATChld::Reply_Button(WPARAM ID)

{
  int which;

  which = bank*9+ID-101;
  Recall_Channel(which);
}

void TSTATChld::Change_Button(WPARAM ID)

{
  int which;
  char string[30];
  int count;

  which = bank*9+ID-201;

  strcpy(string,quick_stations[which].name);
  if (TInputDialog(this,"Memory Channels","Please enter the new label for this channel",
               string,5).Execute()==IDOK)
  {
    if (string[0]!=0)
    {
      quick_stations[which].frequency = frequency_control->Get_Value();
      quick_stations[which].mode = mode_control->Get_Value();
      quick_stations[which].band = filter_control->Get_Value();
      strcpy(quick_stations[which].name,string);
      Button[ID-201]->SetCaption(quick_stations[which].name);
      if (PTBandScan!=NULL && delete_class[56] == 0)
        if (PTBandScan->PTAll_Channels!=NULL && delete_class[57] == 0)
          PTBandScan->PTAll_Channels->Update_Channel_Names();
    }
  }
}

void TSTATChld::Update_Channel()

{
  int count;

  for(count=0;count<9;count++)
    if (strcmp(Button[count]->Title,quick_stations[bank*9+count].name)!=0)
      Button[count]->SetCaption(quick_stations[bank*9+count].name);
}

void Recall_Channel(int which)

{
  long saved_freq;

  if (frequency_control->Set_Value(quick_stations[which].frequency)==0)
  {
    if (MessageBox(NULL,"Station is out of current band limit\n Would you like to reset your limits?",
                   "Warning",MB_YESNO | MB_ICONQUESTION))
    {
      max_frequency_control->Set_Value(max_frequency_control->Get_Max());
      min_frequency_control->Set_Value(min_frequency_control->Get_Min());
      frequency_control->Set_Value(quick_stations[which].frequency);
      mode_control->Set_Value(quick_stations[which].mode);
      filter_control->Set_Value(quick_stations[which].band);
    }
  }
  else
  {
    if (frequency_control->Get_Value() != quick_stations[which].frequency)
      if (Is_Frequency_Amateur(quick_stations[which].frequency))
        while (frequency_control->Get_Value() != quick_stations[which].frequency)
          frequency_control->Set_Value(quick_stations[which].frequency);
      else
      {
        max_frequency_control->Set_Value(max_frequency_control->Get_Max());
        min_frequency_control->Set_Value(min_frequency_control->Get_Min());
        frequency_control->Set_Value(quick_stations[which].frequency);
      }
    mode_control->Set_Value(quick_stations[which].mode);
    filter_control->Set_Value(quick_stations[which].band);
  }
  if (PTFreq && PTFreq->Attr.H > 0)
    PTFreq->Redraw();
  if (PTMode && PTMode->Attr.H > 0)
    PTMode->Redraw();
  if (PTFilter && PTFilter->Attr.H > 0)
    PTFilter->Redraw();
}

DEFINE_RESPONSE_TABLE1(TSTATChld,AWindow)
  EV_COMMAND_AND_ID(101,Reply_Button),
  EV_COMMAND_AND_ID(102,Reply_Button),
  EV_COMMAND_AND_ID(103,Reply_Button),
  EV_COMMAND_AND_ID(104,Reply_Button),
  EV_COMMAND_AND_ID(105,Reply_Button),
  EV_COMMAND_AND_ID(106,Reply_Button),
  EV_COMMAND_AND_ID(107,Reply_Button),
  EV_COMMAND_AND_ID(108,Reply_Button),
  EV_COMMAND_AND_ID(109,Reply_Button),
  EV_COMMAND_AND_ID(201,Change_Button),
  EV_COMMAND_AND_ID(202,Change_Button),
  EV_COMMAND_AND_ID(203,Change_Button),
  EV_COMMAND_AND_ID(204,Change_Button),
  EV_COMMAND_AND_ID(205,Change_Button),
  EV_COMMAND_AND_ID(206,Change_Button),
  EV_COMMAND_AND_ID(207,Change_Button),
  EV_COMMAND_AND_ID(208,Change_Button),
  EV_COMMAND_AND_ID(209,Change_Button),
END_RESPONSE_TABLE;

