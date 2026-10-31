#include "thisID.h"
#include "thisCls.h"
#include <stdio.h>
#include <math.h>

extern int starting_places[][2];
extern int delete_class[];
extern int channels_selected[];
extern PMax_Frequency_Control max_frequency_control;
extern PMin_Frequency_Control min_frequency_control;
extern PFrequency_Control frequency_control;
extern PTX_Frequency_Control tx_frequency_control;
extern TFreq_Window2 *PTFreq,*PTTXFreq;
extern PSplit_Control split_control;
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

extern struct Frequency_Scan_Struct Frequency_Scan_Data;

TFREQUENCY_SCANChld::TFREQUENCY_SCANChld(AWindow *AParent,int ID,LPSTR AName,int child_number)
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
  Attr.W = 162;
  Attr.H = 269;
  id = ID;
  temp = Frequency_Scan_Data.listen_time*10;
  Frequency_Scan_Data.listen_time = temp/10;
  temp = Frequency_Scan_Data.hold_time*10;
  Frequency_Scan_Data.hold_time = temp/10;
  if (Frequency_Scan_Data.listen_time == 0)
    Frequency_Scan_Data.listen_time = 1;
  if (Frequency_Scan_Data.hold_time == 0)
    Frequency_Scan_Data.hold_time = 1;
  if (Frequency_Scan_Data.start_frequency<min_frequency_control->Get_Value())
    Frequency_Scan_Data.start_frequency = min_frequency_control->Get_Value();
  if (Frequency_Scan_Data.stop_frequency>max_frequency_control->Get_Value())
    Frequency_Scan_Data.stop_frequency = max_frequency_control->Get_Value();
  if (Frequency_Scan_Data.stop_frequency<Frequency_Scan_Data.start_frequency)
    Frequency_Scan_Data.stop_frequency = min_frequency_control->Get_Value();
  if (Frequency_Scan_Data.start_frequency>Frequency_Scan_Data.stop_frequency)
    Frequency_Scan_Data.start_frequency = Frequency_Scan_Data.stop_frequency;
  start = Frequency_Scan_Data.start_frequency;
  stop = Frequency_Scan_Data.stop_frequency;
  PStart = new Start_Control(&start,&stop);
  PStop = new Stop_Control(&start,&stop);
  PStep = new Step_Size_Control();
  PStep->Set_Value(Frequency_Scan_Data.step);
  Frequency_Scan_Data.step = PStep->Get_Value();
  if (Frequency_Scan_Data.listen_time<.1)
    Frequency_Scan_Data.listen_time = 1;
  if (Frequency_Scan_Data.hold_time<.1)
    Frequency_Scan_Data.hold_time = 1;
  sprintf(string,"%3.0lf",Frequency_Scan_Data.min_ss);
  new TStatic(this,-1,"Min SS",5,96,140,23);
  Min_dB = new AEdit(this,101,string,80,92,45,23);
  new TStatic(this,-1,"dBm",130,96,30,23);
  sprintf(string,"%3.0lf",Frequency_Scan_Data.max_ss);
  new TStatic(this,-1,"Max SS",5,121,140,23);
  Max_dB = new AEdit(this,102,string,80,117,45,23);
  new TStatic(this,-1,"dBm",130,121,30,23);
  Start_Window = new TFrequency_Window(this,103,"FREQ",PStart,0,0);
  Stop_Window = new TFrequency_Window(this,104,"FREQ",PStop,0,30);
  Step_Window = new TFrequency_Window(this,113,"FREQ",PStep,0,60);
  sprintf(string,"%4.1lf",Frequency_Scan_Data.listen_time);
  new TStatic(this,-1,"Listen Time",5,146,140,23);
  Listen_Time = new AEdit(this,105,string,90,142,30,23);
  new TStatic(this,-1,"Sec",130,146,30,23);
  Infinity = new TCheckBox(this,106,"Infinity",30,167,100,23);
  sprintf(string,"%4.1lf",Frequency_Scan_Data.hold_time);
  new TStatic(this,-1,"Hold Time",5,196,140,23);
  Hold_Time = new AEdit(this,107,string,90,192,30,23);
  new TStatic(this,-1,"Sec",130,196,30,23);
  new TButton(this,108,"<<",-1,226,35,23);
  new TButton(this,109,"<",33,226,23,23);
  new TButton(this,110,"Stop",55,226,48,23);
  new TButton(this,111,">",103,226,23,23);
  new TButton(this,112,">>",126,226,35,23);
}

TFREQUENCY_SCANChld::~TFREQUENCY_SCANChld()

{
  Attr.Y *= -1;
  if (Attr.Y>=0)
    Attr.Y = -1;
  Attr.H = Attr.W = -1;
  starting_places[Child_Number][0] = Attr.X;
  starting_places[Child_Number][1] = Attr.Y;
  delete_class[Child_Number] = 1;
}

void TFREQUENCY_SCANChld::SetupWindow()

{
  AWindow::SetupWindow();
  if (Frequency_Scan_Data.infinity == 1)
    Infinity->Check();
  else
    Infinity->Uncheck();
  Change_Min_SS();
  Change_Max_SS();
}

void TFREQUENCY_SCANChld::EvActivate(uint active,bool b,HWND c)

{
  if (active != WA_INACTIVE)
    MoveWindowToTopOfList(Child_Number);
  AWindow::EvActivate(active,b,c);
}

void TFREQUENCY_SCANChld::ReturnFocus()

{
  ::SetFocus(Parent->HWindow);
}

LPSTR TFREQUENCY_SCANChld::GetClassName()

{
  return("FREQUENCY_SCAN");
}

void TFREQUENCY_SCANChld::GetWindowClass(WNDCLASS _FAR & AWndClass)

{
  AWindow::GetWindowClass(AWndClass);
}

TFREQUENCY_SCANChld::Where_Y()

{
  return(Attr.Y);
}

void TFREQUENCY_SCANChld::Change_Min_SS()

{
  int value;

  value = Min_dB->Get_Value();
  if (value>30)
    value = 30;
  if (value<-120)
    value = -120;
  Frequency_Scan_Data.min_ss = value;
}

void TFREQUENCY_SCANChld::Change_Max_SS()

{
  int value;

  value = Max_dB->Get_Value();
  if (value>30)
    value = 30;
  if (value<-120)
    value = -120;
  Frequency_Scan_Data.max_ss = value;
}

void TFREQUENCY_SCANChld::Change_Start_Frequency()

{
  Frequency_Scan_Data.start_frequency = start;
}

void TFREQUENCY_SCANChld::Change_Stop_Frequency()

{
  Frequency_Scan_Data.stop_frequency = stop;
}

void TFREQUENCY_SCANChld::Change_Step_Size()

{
  Frequency_Scan_Data.step = PStep->Get_Value();
}

void TFREQUENCY_SCANChld::Change_Listen_Time()

{
  int temp;

  temp = Listen_Time->Get_Value_Double()*10;
  if (temp>=1)
    Frequency_Scan_Data.listen_time = temp/10;
}

void TFREQUENCY_SCANChld::Change_Infinity()

{
  if (Infinity->GetCheck() == BF_CHECKED)
    Frequency_Scan_Data.infinity = 1;
  else
    Frequency_Scan_Data.infinity = 0;
}

void TFREQUENCY_SCANChld::Change_Hold_Time()

{
  int temp;

  temp = Hold_Time->Get_Value_Double()*10;
  if (temp>=1)
    Frequency_Scan_Data.hold_time = temp/10;
}

void TFREQUENCY_SCANChld::Scan_Backward()

{
  scanning_direction = -1;
  Prev_Frequency();
}

void TFREQUENCY_SCANChld::Prev_Frequency()

{
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
  if (Get_Frequency()>stop || Get_Frequency()-Frequency_Scan_Data.step<start)
    Set_Frequency(stop);
  else
    Set_Frequency(Get_Frequency()-Frequency_Scan_Data.step);
}

void TFREQUENCY_SCANChld::Stop_Scanning()

{
  scanning_direction = 0;
}

void TFREQUENCY_SCANChld::Next_Frequency()

{
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
  if (Get_Frequency()<start || Get_Frequency()+Frequency_Scan_Data.step>stop)
    Set_Frequency(start);
  else
    Set_Frequency(Get_Frequency()+Frequency_Scan_Data.step);
}

void TFREQUENCY_SCANChld::Scan_Forward()

{
  scanning_direction = 1;
  Next_Frequency();
}

void TFREQUENCY_SCANChld::S_Meter_Report(double signal_strength)

{
  struct time now;
  long time_now,time_then;

  if (ptt_control->Get_Value()==1)
    if (scanning_direction!=0)
    {
      MessageBox("Frequency scanning stopped due to Transmit mode.","Warning",MB_OK);
      scanning_direction = 0;
    }
  if (scanning_direction!=0)
  {
    if (signal_strength>=Frequency_Scan_Data.min_ss &&
        signal_strength<=Frequency_Scan_Data.max_ss)
      this_channel = 1;
    else
      if (this_channel == 0)
        if (scanning_direction == 1)
          Next_Frequency();
        else
          Prev_Frequency();
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
      if (time_now>=time_then+Frequency_Scan_Data.hold_time*100)
      {
        if (scanning_direction == 1)
          Next_Frequency();
        else
          Prev_Frequency();
        this_channel = -1;
      }
    }
    if (Frequency_Scan_Data.infinity!=1)
    {
      gettime(&now);
      time_now = now.ti_hund+now.ti_sec*100+now.ti_min*6000+
                 now.ti_hour*36000;
      time_then = last_change.ti_hund+last_change.ti_sec*100+
                  last_change.ti_min*6000+last_change.ti_hour*36000;
      if (time_then>time_now)
        time_now+= 864000l;
      if (time_now>=time_then+Frequency_Scan_Data.listen_time*100)
        if (scanning_direction == 1)
          Next_Frequency();
        else
          Prev_Frequency();
    }
  }
}

void TFREQUENCY_SCANChld::EvMove(TPoint &point)

{
  AWindow::EvMove(point);
  starting_places[Child_Number][0] = Attr.X;
  starting_places[Child_Number][1] = Attr.Y;
}

void TFREQUENCY_SCANChld::EvChar(UINT key, UINT repeatCount,UINT flags)

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
void TFREQUENCY_SCANChld::Paint(TDC& DC, BOOL,TRect &)

{
  TMemoryDC *MDC;

  MDC = new TMemoryDC(DC);
  switch (max_labels)
  {
	 case 2  : MDC->SelectObject(*knobs[FREQUENCY_SCAN->Get_Value()*4+6]);
				  break;
    case 3  : MDC->SelectObject(*knobs[FREQUENCY_SCAN->Get_Value()*4+4]);
              break;
	 case 5  : MDC->SelectObject(*knobs[FREQUENCY_SCAN->Get_Value()*4]);
				  break;
	 case 6  : if (FREQUENCY_SCAN->Get_Value()>2)
					 MDC->SelectObject(*knobs[FREQUENCY_SCAN->Get_Value()*2+4]);
				  else
					 MDC->SelectObject(*knobs[FREQUENCY_SCAN->Get_Value()*2+2]);
				  break;
	 case 8  : if (FREQUENCY_SCAN->Get_Value()>3)
					 MDC->SelectObject(*knobs[FREQUENCY_SCAN->Get_Value()*2+2]);
              else
                MDC->SelectObject(*knobs[FREQUENCY_SCAN->Get_Value()*2]);
              break;
    case 9  : MDC->SelectObject(*knobs[FREQUENCY_SCAN->Get_Value()*2]);
              break;
    case 16 : MDC->SelectObject(*knobs[FREQUENCY_SCAN->Get_Value()]);
				  break;
    case 37 : MDC->SelectObject(*knobs[FREQUENCY_SCAN->Get_Value()*17/37]);
              break;
	 case 64 : MDC->SelectObject(*knobs[FREQUENCY_SCAN->Get_Value()*17/64]);
              break;
    case 80 : MDC->SelectObject(*knobs[FREQUENCY_SCAN->Get_Value()*17/80]);
              break;
    default : MDC->SelectObject(*knobs[FREQUENCY_SCAN->Get_Value()*4+2]);
  }
  DC.BitBlt(37,20,64,64,*MDC,0,0,SRCCOPY);
  MDC->RestoreObjects();
  last_value = FREQUENCY_SCAN->Get_Value();
  delete (MDC);
}
*/
/*
void TFREQUENCY_SCANChld::EvTimer(UINT)

{
  if (FREQUENCY_SCAN_timer_message == WM_LBUTTONDOWN)
	 EvLButtonDown(NULL,FREQUENCY_SCAN_timer_point);
  if (FREQUENCY_SCAN_timer_message == WM_RBUTTONDOWN)
	 EvRButtonDown(NULL,FREQUENCY_SCAN_timer_point);
}
*/
long TFREQUENCY_SCANChld::Get_Frequency()

{
  if (split_control->Get_Value() == 0 || split_control->Get_Value() == 2)
    return frequency_control->Get_Value();
  else
    return tx_frequency_control->Get_Value();
}

/*void TFREQUENCY_SCANChld::Set_Frequency(long value)

{
  if (split_control->Get_Value() == 0 || split_control->Get_Value() == 2)
    if (!PTFreq || delete_class[7])
      frequency_control->Set_Value(value);
    else
      PTFreq->Set_Value(value);
  else
    if (!PTTXFreq || delete_class[26])
      tx_frequency_control->Set_Value(value);
    else
      PTTXFreq->Set_Value(value);
}
*/
void TFREQUENCY_SCANChld::Set_Frequency(long value)

{
  if (split_control->Get_Value() == 0 || split_control->Get_Value() == 2)
  {
    frequency_control->Set_Value(value);
    if (PTFreq!=NULL && delete_class[7]==0)
      PTFreq->Check_For_Repaint();
  }
  else
  {
    tx_frequency_control->Set_Value(value);
    if (PTTXFreq!=NULL && delete_class[26]==0)
      PTTXFreq->Check_For_Repaint();
  }
}

DEFINE_RESPONSE_TABLE1(TFREQUENCY_SCANChld,AWindow)
  EV_WM_ACTIVATE,
//  EV_WM_TIMER,
  EV_WM_MOVE,
  EV_EN_CHANGE(101,Change_Min_SS),
  EV_EN_CHANGE(102,Change_Max_SS),
  EV_COMMAND(103,Change_Start_Frequency),
  EV_COMMAND(104,Change_Stop_Frequency),
  EV_EN_CHANGE(105,Change_Listen_Time),
  EV_COMMAND(106,Change_Infinity),
  EV_EN_CHANGE(107,Change_Hold_Time),
  EV_COMMAND(108,Scan_Backward),
  EV_COMMAND(109,Prev_Frequency),
  EV_COMMAND(110,Stop_Scanning),
  EV_COMMAND(111,Next_Frequency),
  EV_COMMAND(112,Scan_Forward),
  EV_COMMAND(113,Change_Step_Size),
END_RESPONSE_TABLE;

