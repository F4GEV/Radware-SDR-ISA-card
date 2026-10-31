#define TEST_IN_SIMULATION 0
#include <thiscls.h>

extern int Show_About;
extern TCONTROLChld *PTPTT;
extern PPTT_Control ptt_control;
extern int ptt_via_joystick;
extern int old_key;
extern char places_name[];
extern char settings_name[];
extern char stations_name[];
extern int screen_max_x;
extern int screen_max_y;
extern int save_on_exit;
extern short int delete_class[];
extern int Show_About;
extern TBitmap *smeter_bitmap;
extern TBitmap *am_meter_bitmap;
extern TBitmap *sheet_bitmap[2];
extern TBitmap *tune_bitmap[2];
extern TBitmap *knobs[];

void Clear_Preselectors();

class DSP1:public TApplication

{
  public:
  DSP1(char far *Name): TApplication(Name) {}
  void InitMainWindow();
  bool ProcessAppMsg(MSG &msg);
};

void DSP1::InitMainWindow()

{
  SetMainWindow(MainWindow = new TMainWindow(NULL,"RADWARE"));
}

int poll_dsp;

bool DSP1::ProcessAppMsg(MSG &msg)

{
  int message;
  char string[80];

  if (Show_About == 1)
  {
    PostMessage(MainWindow->HWindow,WM_COMMAND,183,NULL);
    Show_About = 2;
  }
  Check_Key();
  do
  {
    message = DSPGetMsg();
    if (message)
      PostMessage(MainWindow->HWindow,WM_COMMAND,message+205,NULL);
  }
  while (message!=0);
  if (msg.message==WM_CHAR)
  {
    if (msg.wParam == '`')
      BringWindowToTop(MainWindow->HWindow);
    if (msg.wParam == VK_ESCAPE)
      if (PTPTT)
      {
        PTPTT->Set_Value(ptt_control->Get_Value()?0:1);
        return 0;
      }
      else
      {
        ptt_control->Set_Value(ptt_control->Get_Value()?0:1);
        return 0;
      }
    if (msg.wParam == 4)// && msg.lParam & 0x2000)
    {
      PostMessage(MainWindow->HWindow,WM_COMMAND,502,NULL);
    }
  }
  return TApplication::ProcessAppMsg(msg);
}

int OwlMain(int argc,char *argv[])

{
  TResId *ID;
  int count;
  char string[80];
  RECT desktop;
  FILE *stream;

  GetWindowRect(GetDesktopWindow(),&desktop);
  screen_max_x = desktop.right-50;
  screen_max_y = desktop.bottom-25;
  HINSTANCE hBorLibrary;

  if (argc>1)
  {
	 strcpy(places_name,argv[1]);
	 strcat(places_name,".plc");
  }
  else
	 strcpy(places_name,"places.plc");
  if (argc>2)
  {
	 strcpy(settings_name,argv[2]);
	 strcat(settings_name,".set");
  }
  else
	 if (argc == 2)
	 {
		strcpy(settings_name,argv[1]);
		strcat(settings_name,".set");
	 }
	 else
		strcpy(settings_name,"settings.set");
  if (argc>3)
  {
	 strcpy(stations_name,argv[3]);
	 strcat(stations_name,".sta");
  }
  else
	 if (argc == 2)
	 {
		strcpy(stations_name,argv[1]);
		strcat(stations_name,".sta");
	 }
	 else
		strcpy(stations_name,"stations.sta");
  stream = fopen("RADWARE.INI","r");
  if (stream)
  {
	 fgets(string,79,stream);
	 if (strcmp(string,"SAVE ON EXIT\n")==0)
		save_on_exit = 1;
	 else
		save_on_exit = 0;
	 fclose(stream);
  }
  hBorLibrary = LoadLibrary("bwcc.dll");

  if((UINT) hBorLibrary <= 32)
     MessageBox(NULL, "Unable to load Borland Controls", "System Error", MB_OK | MB_ICONHAND);
  if (TEST_IN_SIMULATION)
	 Simulation_Mode();
  DSP1 DSP("DSP 1");
  load_places();
  load_stations();
  for(count=0;count<NUMBER_OF_WINDOWS;count++)
    delete_class[count] = 0;
  for(count=0;count<MAX_KNOBS;count++)
  {
	 sprintf(string,"KNOB%d",count+1);
	 ID = new TResId(string);
	 knobs[count] = new TBitmap(_hInstance,*ID);
	 delete ID;
  }
  ID = new TResId("SMETER2");
  smeter_bitmap = new TBitmap(_hInstance,*ID);
  delete ID;
  ID = new TResId("AM_METER");
  am_meter_bitmap = new TBitmap(_hInstance,*ID);
  delete ID;
  ID = new TResId("SHEET_UNPRESSED");
  sheet_bitmap[0] = new TBitmap(_hInstance,*ID);
  delete ID;
  ID = new TResId("SHEET_PRESSED");
  sheet_bitmap[1] = new TBitmap(_hInstance,*ID);
  delete ID;
  ID = new TResId("TUNE_UNPRESSED");
  tune_bitmap[0] = new TBitmap(_hInstance,*ID);
  delete ID;
  ID = new TResId("TUNE_PRESSED");
  tune_bitmap[1] = new TBitmap(_hInstance,*ID);
  delete ID;
  DSP.Run();
  if((UINT) hBorLibrary > 32)
    FreeLibrary(hBorLibrary);
  Clear_Preselectors();
  stream = fopen("RADWARE.INI","w");
  for(count=0;count<MAX_KNOBS;count++)
	 delete knobs[count];
  delete smeter_bitmap;
  delete am_meter_bitmap;
  delete sheet_bitmap[0];
  delete sheet_bitmap[1];
  delete tune_bitmap[0];
  delete tune_bitmap[1];
  if (stream)
  {
	 if (save_on_exit)
		fprintf(stream,"SAVE ON EXIT\n");
	 else
		fprintf(stream,"DO NOT SAVE ON EXIT\n");
	 fclose(stream);
  }
  return DSP.Status;
}


