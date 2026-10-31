//REGEN_FILEHEADING
//REGEN_FILEHEADING

     /********************************************************************
      *                                                                  *
      *   Source File: STATIONS.cpp                                      *
      *   Description: Dialog implementation for TEST                    *
      *   Date:        Thu Feb 18 22:28:42 1993                          *
      *                                                                  *
      ********************************************************************/

#include "thisCls.h"
//#include "swlib.h"

extern stations_reset_bands;
extern PFrequency_Control frequency_control;
extern PMODE_Control mode_control;
extern PFILTER_Control filter_control;
extern PMax_Frequency_Control max_frequency_control;
extern PMin_Frequency_Control min_frequency_control;

extern TCONTROLChld *PTMode,*PTFilter;
extern TFreq_Window2 *PTFreq;

struct single_station
{
  char name[16];
  long frequency;
  short mode;
  short band;
};

extern struct single_station stations[MAX_STATIONS];

struct station_struct
{
  TComboBoxData *Data;
};

struct station_struct station_data;

// Define TSTATIONSDlg, a TDialog constructor
TSTATIONSDlg::TSTATIONSDlg(TWindow *AParent, LPSTR AName)
               :TDialog(AParent, AName)
{
   int count;
   LBox101 = new TComboBox(this, 101,16);
}

TSTATIONSDlg::~TSTATIONSDlg()
{
   delete LBox101;
}

void TSTATIONSDlg::SetupWindow()

{
  int count;

  TDialog::SetupWindow();
  for(count=0;count<MAX_STATIONS;count++)
    if (stations[count].name[0] != 0)
      LBox101->AddString(stations[count].name);
}

void TSTATIONSDlg::RTSTATIONS102()
{
   char string[16];
   int which,count;

  LBox101->GetText(string,16);
  if (strlen(string)==0)
  {
    MessageBox("You must select a station before hitting OK.\nIf you wish to exit stations without changing one,\n  press cancel instead.","ERROR",MB_OK | MB_ICONEXCLAMATION);
    return;
  }
  which = -1;
  for(count=0;count<MAX_STATIONS;count++)
	 if (strcmp(string,stations[count].name)==0)
	 {
		which = count;
		break;
    }
  if (which>=0 && which<MAX_STATIONS)
  {
	 if (frequency_control->Set_Value(stations[which].frequency)==0)
    {
      if (stations_reset_bands)
      {
        max_frequency_control->Set_Value(max_frequency_control->Get_Max());
        min_frequency_control->Set_Value(min_frequency_control->Get_Min());
        frequency_control->Set_Value(stations[which].frequency);
        mode_control->Set_Value(stations[which].mode);
        filter_control->Set_Value(stations[which].band);
      }
      else
        if (MessageBox("Station is out of current band limit\n Would you like to reset your limits?",
                       "Warning",MB_YESNO | MB_ICONQUESTION))
        {
          max_frequency_control->Set_Value(max_frequency_control->Get_Max());
          min_frequency_control->Set_Value(min_frequency_control->Get_Min());
          frequency_control->Set_Value(stations[which].frequency);
          mode_control->Set_Value(stations[which].mode);
          filter_control->Set_Value(stations[which].band);
        }
    }
    else
    {
      mode_control->Set_Value(stations[which].mode);
      filter_control->Set_Value(stations[which].band);
    }
    if (PTFreq && PTFreq->Attr.H > 0)
      PTFreq->Redraw();
    if (PTMode && PTMode->Attr.H > 0)
      PTMode->Redraw();
    if (PTFilter && PTFilter->Attr.H > 0)
      PTFilter->Redraw();
  }
  CloseWindow(1);
}

void TSTATIONSDlg::RTSTATIONS103()

{
  CloseWindow(0);
}

void TSTATIONSDlg::RTSTATIONS104()

{
  char string[16];
  int count;

  string[0] = 0;
  TInputDialog(this,"Add Station","Name",string,sizeof(string)).Execute();
  if (string[0]!=0)
  {
    for(count=0;count<MAX_STATIONS;count++)
		if (stations[count].name[0] == 0)
		{
     	  LBox101->AddString(string);
		  strcpy(stations[count].name,string);
		         stations[count].frequency = frequency_control->Get_Value();
			      stations[count].mode = mode_control->Get_Value();
			      stations[count].band = filter_control->Get_Value();
        break;
      }
   if (count==MAX_STATIONS)
	  MessageBox("Limit of channels reached","ERROR",MB_ICONHAND|MB_OK);
  }
}

void TSTATIONSDlg::RTSTATIONS105()

{
  char string[16];
  int count;

  LBox101->GetSelString(string,15);
  for(count=0;count<MAX_STATIONS;count++)
	 if (strcmp(string,stations[count].name)==0)
      break;
  if (count!=MAX_STATIONS)
  {
	 LBox101->DeleteString(LBox101->GetSelIndex());
	 stations[count].name[0] = 0;
  }
}

DEFINE_RESPONSE_TABLE1(TSTATIONSDlg,TDialog)
  EV_COMMAND(102,RTSTATIONS102),
  EV_COMMAND(103,RTSTATIONS103),
  EV_COMMAND(104,RTSTATIONS104),
  EV_COMMAND(105,RTSTATIONS105),
END_RESPONSE_TABLE;

