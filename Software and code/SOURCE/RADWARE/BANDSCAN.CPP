#include "thisID.h"
#include "thisCls.h"
#include <stdio.h>
#include <math.h>

extern int starting_places[][2];
extern int delete_class[];
extern int channels_selected[];
extern PPTT_Control ptt_control;
extern TCONTROLChld *PTPTT;

struct single_station
{
  char name[16];
  long frequency;
  short mode;
  short band;
};

extern struct single_station quick_stations[];

extern struct Band_Scan_Struct Band_Scan_Data;

TBANDSCANChld::TBANDSCANChld(AWindow *AParent,int ID,LPSTR AName,int child_number)
		     :AWindow(AParent,AName,child_number)

{
  int x,y,temp;
  char string[20]="HI";

  PTAll_Channels = NULL;
  Child_Number = child_number;
  x = starting_places[Child_Number][0];
  y = starting_places[Child_Number][1];
  Attr.Style = WS_POPUP | WS_VISIBLE | WS_CAPTION |WS_MINIMIZEBOX|WS_SYSMENU;
  Attr.X = x;
  scanning_direction = 0;
  if (y<0)
    Attr.Y = -y;
  else
    Attr.Y = y;
  Attr.W = 220;
  Attr.H = 229;
  id = ID;
  temp = Band_Scan_Data.listen_time*10;
  Band_Scan_Data.listen_time = temp/10;
  temp = Band_Scan_Data.hold_time*10;
  Band_Scan_Data.hold_time = temp/10;
  if (Band_Scan_Data.listen_time == 0)
    Band_Scan_Data.listen_time = 1;
  if (Band_Scan_Data.hold_time == 0)
    Band_Scan_Data.hold_time = 1;
  sprintf(string,"%3.0lf",Band_Scan_Data.min_ss);
  new TStatic(this,-1,"Min Signal Strength",5,6,140,23);
  Min_dB = new AEdit(this,101,string,145,2,42,23);
  new TStatic(this,-1,"dBm",188,6,30,23);
  sprintf(string,"%3.0lf",Band_Scan_Data.max_ss);
  new TStatic(this,-1,"Max Signal Strength",5,31,140,23);
  Max_dB = new AEdit(this,102,string,145,27,42,23);
  new TStatic(this,-1,"dBm",188,31,30,23);
  if (Band_Scan_Data.band<0 || Band_Scan_Data.band>8)
    Band_Scan_Data.band = 0;
  if (Band_Scan_Data.channel<0 || Band_Scan_Data.channel>8)
    Band_Scan_Data.channel = 0;
  if (Band_Scan_Data.listen_time<.1)
    Band_Scan_Data.listen_time = 1;
  if (Band_Scan_Data.hold_time<.1)
    Band_Scan_Data.hold_time = 1;
  sprintf(string,"%d",Band_Scan_Data.band+1);
  new TStatic(this,-1,"Current Band",5,56,140,23);
  Band = new AEdit(this,103,string,145,52,30,23);
  sprintf(string,quick_stations[Band_Scan_Data.band*9+
                                Band_Scan_Data.channel].name);
  new TStatic(this,-1,"Current Channel",5,81,140,23);
  Channel = new AEdit(this,104,string,145,77,45,23);
  sprintf(string,"%4.1lf",Band_Scan_Data.listen_time);
  new TStatic(this,-1,"Listen Time",5,106,140,23);
  Listen_Time = new AEdit(this,105,string,145,102,30,23);
  new TStatic(this,-1,"Sec",185,106,30,23);
  Infinity = new TCheckBox(this,106,"Infinity",30,127,100,23);
  sprintf(string,"%4.1lf",Band_Scan_Data.hold_time);
  new TStatic(this,-1,"Hold Time",5,156,140,23);
  Hold_Time = new AEdit(this,107,string,145,152,30,23);
  new TStatic(this,-1,"Sec",185,156,30,23);
  new TButton(this,108,"<<",-1,186,35,23);
  new TButton(this,109,"<",33,186,23,23);
  new TButton(this,110,"Stop",55,186,40,23);
  new TButton(this,111,">",94,186,23,23);
  new TButton(this,112,">>",116,186,35,23);
  new TButton(this,113,"Channels",150,186,69,23);
}

TBANDSCANChld::~TBANDSCANChld()

{
  Attr.Y *= -1;
  if (Attr.Y>=0)
    Attr.Y = -1;
  Attr.H = Attr.W = -1;
  starting_places[Child_Number][0] = Attr.X;
  starting_places[Child_Number][1] = Attr.Y;
  delete_class[Child_Number] = 1;
}

void TBANDSCANChld::SetupWindow()

{
  AWindow::SetupWindow();
  if (Band_Scan_Data.infinity == 1)
    Infinity->Check();
  else
    Infinity->Uncheck();
  Change_Min_SS();
  Change_Max_SS();
}

void TBANDSCANChld::EvActivate(uint active,bool b,HWND c)

{
  if (active != WA_INACTIVE)
    MoveWindowToTopOfList(Child_Number);
  AWindow::EvActivate(active,b,c);
}

void TBANDSCANChld::ReturnFocus()

{
  ::SetFocus(Parent->HWindow);
}

LPSTR TBANDSCANChld::GetClassName()

{
  return("BANDSCAN");
}

void TBANDSCANChld::GetWindowClass(WNDCLASS _FAR & AWndClass)

{
  AWindow::GetWindowClass(AWndClass);
}

TBANDSCANChld::Where_Y()

{
  return(Attr.Y);
}

void TBANDSCANChld::Change_Min_SS()

{
  int value;

  value = Min_dB->Get_Value();
  if (value>30)
    value = 30;
  if (value<-120)
    value = -120;
  Band_Scan_Data.min_ss = value;
}

void TBANDSCANChld::Change_Max_SS()

{
  int value;

  value = Max_dB->Get_Value();
  if (value>30)
    value = 30;
  if (value<-120)
    value = -120;
  Band_Scan_Data.max_ss = value;
}

void TBANDSCANChld::Change_Band()

{
  char string[20];

  if (Band->Get_Value()>=1 && Band->Get_Value()<=9)
  {
    Band_Scan_Data.band = Band->Get_Value()-1;
    sprintf(string,quick_stations[Band_Scan_Data.band*9+
            Band_Scan_Data.channel].name);
    Channel->SetText(string);
  }
}

void TBANDSCANChld::Change_Channel()

{
  char string[20];
  int count;

  Channel->GetText(string,19);
  for(count=0;count<9;count++)
    if (strcmp(string,quick_stations[Band_Scan_Data.band*9+count].name)==0)
      Band_Scan_Data.channel = count;
}

void TBANDSCANChld::Change_Listen_Time()

{
  int temp;

  temp = Listen_Time->Get_Value_Double()*10;
  if (temp>=1)
    Band_Scan_Data.listen_time = temp/10;
}

void TBANDSCANChld::Change_Infinity()

{
  if (Infinity->GetCheck() == BF_CHECKED)
    Band_Scan_Data.infinity = 1;
  else
    Band_Scan_Data.infinity = 0;
}

void TBANDSCANChld::Change_Hold_Time()

{
  int temp;

  temp = Hold_Time->Get_Value_Double()*10;
  if (temp>=1)
    Band_Scan_Data.hold_time = temp/10;
}

void TBANDSCANChld::Scan_Backward()

{
  scanning_direction = -1;
  Prev_Channel();
}

void TBANDSCANChld::Prev_Channel()

{
  int count,error = 1;

  if (ptt_control->Get_Value())
    if (MessageBox("You may not scan while in transmit mode.\nDo you wish to switch to recieve?",
               "Error",MB_YESNO)==IDYES)
      if (!PTPTT || delete_class[20])
        ptt_control->Set_Value(0);
      else
        PTPTT->Set_Value(0);
    else
    {
      scanning_direction = 0;
      return;
    }
  for(count = 0;count<81;count++)
    if (channels_selected[count] == 1)
    {
      error = 0;
      break;
    }
  if (error)
  {
    MessageBox("There are no selected channels","Error",MB_ICONSTOP | MB_OK);
    return;
  }
  count = Band_Scan_Data.band*9+Band_Scan_Data.channel;
  do
  {
    count--;
    if (count<0)
      count = 81;
  }
  while (channels_selected[count] != 1);
  Band_Scan_Data.band = count/9;
  Band_Scan_Data.channel = count%9;
  Band->Set_Value(Band_Scan_Data.band+1);
  Channel->SetText(quick_stations[count].name);
  gettime(&last_change);
  this_channel = -1;
  Recall_Channel(count);
}

void TBANDSCANChld::Stop_Scanning()

{
  scanning_direction = 0;
}

void TBANDSCANChld::Next_Channel()

{
  int count,error = 1;

  if (ptt_control->Get_Value())
    if (MessageBox("You may not scan while in transmit mode.\nDo you wish to switch to recieve?",
               "Error",MB_YESNO)==IDYES)
      if (!PTPTT || delete_class[20])
        ptt_control->Set_Value(0);
      else
        PTPTT->Set_Value(0);
    else
    {
      scanning_direction = 0;
      return;
    }
  for(count = 0;count<81;count++)
    if (channels_selected[count] == 1)
    {
      error = 0;
      break;
    }
  if (error)
  {
    MessageBox("There are no selected channels","Error",MB_ICONSTOP | MB_OK);
    return;
  }
  count = Band_Scan_Data.band*9+Band_Scan_Data.channel;
  do
  {
    count++;
    if (count>=81)
      count = 0;
  }
  while (channels_selected[count] != 1);
  Band_Scan_Data.band = count/9;
  Band_Scan_Data.channel = count%9;
  Band->Set_Value(Band_Scan_Data.band+1);
  Channel->SetText(quick_stations[count].name);
  gettime(&last_change);
  this_channel = -1;
  Recall_Channel(count);
}

void TBANDSCANChld::Scan_Forward()

{
  scanning_direction = 1;
  Next_Channel();
}

void TBANDSCANChld::Open_Channels()

{
  if (!PTAll_Channels || delete_class[57])
  {
    PTAll_Channels = new TALL_CHANNELSChld(this,-1,"All Channels",57);
    PTAll_Channels->Create();
    delete_class[57] = 0;
  }
  else
    PTAll_Channels->Show(SW_SHOWNORMAL);
  ::BringWindowToTop(PTAll_Channels->HWindow);
}


void TBANDSCANChld::S_Meter_Report(double signal_strength)

{
  struct time now;
  long time_now,time_then;

  if (ptt_control->Get_Value()==1)
    if (scanning_direction!=0)
    {
      MessageBox("Memory scanning stopped due to Transmit mode.","Warning",MB_OK);
      scanning_direction = 0;
    }
  if (scanning_direction!=0)
  {
    if (signal_strength>=Band_Scan_Data.min_ss &&
        signal_strength<=Band_Scan_Data.max_ss)
      this_channel = 1;
    else
      if (this_channel == 0)
        if (scanning_direction == 1)
          Next_Channel();
        else
          Prev_Channel();
      else
        if (this_channel == -1)
          this_channel = 0;
        else
          if (this_channel == 1)
          {
            this_channel = 2;
            gettime(&Hold_Start);
          }
    if (this_channel == 2)
    {
      gettime(&now);
      time_now = now.ti_hund+now.ti_sec*100+now.ti_min*6000+
                 now.ti_hour*36000;
      time_then = Hold_Start.ti_hund+Hold_Start.ti_sec*100+
                  Hold_Start.ti_min*6000+Hold_Start.ti_hour*36000;
      if (time_then>time_now)
        time_now+= 864000l;
      if (time_now>=time_then+Band_Scan_Data.hold_time*100)
      {
        if (scanning_direction == 1)
          Next_Channel();
        else
          Prev_Channel();
        this_channel = -1;
      }
    }
    if (Band_Scan_Data.infinity!=1)
    {
      gettime(&now);
      time_now = now.ti_hund+now.ti_sec*100+now.ti_min*6000+
                 now.ti_hour*36000;
      time_then = last_change.ti_hund+last_change.ti_sec*100+
                  last_change.ti_min*6000+last_change.ti_hour*36000;
      if (time_then>time_now)
        time_now+= 864000l;
      if (time_now>=time_then+Band_Scan_Data.listen_time*100)
        if (scanning_direction == 1)
          Next_Channel();
        else
          Prev_Channel();
    }
  }
}

void TBANDSCANChld::EvMove(TPoint &point)

{
  AWindow::EvMove(point);
  starting_places[Child_Number][0] = Attr.X;
  starting_places[Child_Number][1] = Attr.Y;
}

void TBANDSCANChld::EvChar(UINT key, UINT repeatCount,UINT flags)

{
  if (key == 13)
  {
    ClipCursor(NULL);
    ShowCursor(0);
    Attr.H = -1;
    while (ShowCursor(1)<0)
      {}
  }
}
/*
void TBANDSCANChld::Paint(TDC& DC, BOOL,TRect &)

{
  TMemoryDC *MDC;

  MDC = new TMemoryDC(DC);
  switch (max_labels)
  {
	 case 2  : MDC->SelectObject(*knobs[BANDSCAN->Get_Value()*4+6]);
				  break;
    case 3  : MDC->SelectObject(*knobs[BANDSCAN->Get_Value()*4+4]);
              break;
	 case 5  : MDC->SelectObject(*knobs[BANDSCAN->Get_Value()*4]);
				  break;
	 case 6  : if (BANDSCAN->Get_Value()>2)
					 MDC->SelectObject(*knobs[BANDSCAN->Get_Value()*2+4]);
				  else
					 MDC->SelectObject(*knobs[BANDSCAN->Get_Value()*2+2]);
				  break;
	 case 8  : if (BANDSCAN->Get_Value()>3)
					 MDC->SelectObject(*knobs[BANDSCAN->Get_Value()*2+2]);
              else
                MDC->SelectObject(*knobs[BANDSCAN->Get_Value()*2]);
              break;
    case 9  : MDC->SelectObject(*knobs[BANDSCAN->Get_Value()*2]);
              break;
    case 16 : MDC->SelectObject(*knobs[BANDSCAN->Get_Value()]);
				  break;
    case 37 : MDC->SelectObject(*knobs[BANDSCAN->Get_Value()*17/37]);
              break;
	 case 64 : MDC->SelectObject(*knobs[BANDSCAN->Get_Value()*17/64]);
              break;
    case 80 : MDC->SelectObject(*knobs[BANDSCAN->Get_Value()*17/80]);
              break;
    default : MDC->SelectObject(*knobs[BANDSCAN->Get_Value()*4+2]);
  }
  DC.BitBlt(37,20,64,64,*MDC,0,0,SRCCOPY);
  MDC->RestoreObjects();
  last_value = BANDSCAN->Get_Value();
  delete (MDC);
}
*/
/*
void TBANDSCANChld::EvTimer(UINT)

{
  if (BANDSCAN_timer_message == WM_LBUTTONDOWN)
	 EvLButtonDown(NULL,BANDSCAN_timer_point);
  if (BANDSCAN_timer_message == WM_RBUTTONDOWN)
	 EvRButtonDown(NULL,BANDSCAN_timer_point);
}
*/
DEFINE_RESPONSE_TABLE1(TBANDSCANChld,AWindow)
  EV_WM_ACTIVATE,
//  EV_WM_TIMER,
  EV_WM_MOVE,
  EV_EN_CHANGE(101,Change_Min_SS),
  EV_EN_CHANGE(102,Change_Max_SS),
  EV_EN_CHANGE(103,Change_Band),
  EV_EN_CHANGE(104,Change_Channel),
  EV_EN_CHANGE(105,Change_Listen_Time),
  EV_COMMAND(106,Change_Infinity),
  EV_EN_CHANGE(107,Change_Hold_Time),
  EV_COMMAND(108,Scan_Backward),
  EV_COMMAND(109,Prev_Channel),
  EV_COMMAND(110,Stop_Scanning),
  EV_COMMAND(111,Next_Channel),
  EV_COMMAND(112,Scan_Forward),
  EV_COMMAND(113,Open_Channels),
END_RESPONSE_TABLE;

