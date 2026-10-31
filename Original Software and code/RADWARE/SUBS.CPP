#include <thiscls.h>
struct single_station
{
  char name[16];
  long frequency;
  short mode;
  short band;
};

struct single_station stations[MAX_STATIONS];
struct single_station quick_stations[MAX_QUICK_STATIONS];
int channels_selected[MAX_QUICK_STATIONS];

extern short int delete_class[];
extern int starting_places[][2];
extern int starting_sizes[][2];
extern short int windows_on_top[];
extern int screen_max_x;
extern int screen_max_y;
extern char places_name[];
extern char stations_name[];
extern struct Band_Scan_Struct Band_Scan_Data;
extern struct Frequency_Scan_Struct Frequency_Scan_Data;
extern int Show_About;
extern TCONTROLChld *PTPTT;
extern PPTT_Control ptt_control;
extern int ptt_via_joystick;
extern int old_key;
extern int key_config;

int ptt_check_for_time = 0;

void Check_Key()

{
  char string[80];
  int new_key;
  int old_ptt;

  if (Show_About)
	 return;
  new_key = inportb(0x201);
  if (new_key!=old_key)
  {
	 if (((new_key&0x10) != (old_key&0x10)) && ptt_via_joystick)
	 {
		ptt_check_for_time = 0;
		if (!(new_key&0x10))
		{
		  if (Valid_Xmit(0))
			 if (PTPTT && !delete_class[20])
				PTPTT->Set_Value(1);
			 else
				ptt_control->Set_Value(1);
		}
		else
		  if (PTPTT && !delete_class[20])
			 PTPTT->Set_Value(0);
		  else
			 ptt_control->Set_Value(0);
	 }
	 old_ptt = ptt_control->Get_Value();
    if (key_config)
    {
      if (1-new_key/128)
	    	ptt_check_for_time = 1;
      Set_Morse(1-new_key/128);
      if (old_ptt!=ptt_control->Get_Value())
	     if (PTPTT && !delete_class[20])
		    PTPTT->Redraw();
    }
  }
  if (new_key/128 && key_config)
  {
	 old_ptt = ptt_control->Get_Value();
	 if ((new_key&0x10) || !ptt_via_joystick && ptt_check_for_time)
	 {
      if (ptt_control->Get_Value() == 1)
        Check_PTT_Time();
		if (old_ptt!=ptt_control->Get_Value())
		  if (PTPTT && !delete_class[20])
			 PTPTT->Redraw();
	 }
  }
  old_key = new_key;
}

void load_places()

{
  FILE *stream;
  int count;
  char string[80];

  stream = fopen(places_name,"r");
  if (!stream)
  {
    sprintf(string,"Unable to find file %s.\nUsing Defaults",places_name);
    MessageBox(NULL,string,"File Missing",MB_OK | MB_ICONINFORMATION);
    Default_Places();
  }
  else
  {
    for(count=0;count<NUMBER_OF_WINDOWS;count++)
    {
      fscanf(stream,"%d\n%d\n",&starting_places[count][0],&starting_places[count][1]);
      if (starting_places[count][0]>screen_max_x)
        starting_places[count][0] = screen_max_x;
      if (starting_places[count][0]<-screen_max_x)
        starting_places[count][0] = -screen_max_x;
      if (starting_places[count][1]>screen_max_y)
        starting_places[count][1] = screen_max_y;
      if (starting_places[count][1]<-screen_max_y)
        starting_places[count][1] = -screen_max_y;
    }
    if (feof_or_null(stream))
    {
      sprintf(string,"%s appears to be corrupt.\nUsing Defaults",places_name);
      MessageBox(NULL,string,"File Corrupt",MB_OK | MB_ICONINFORMATION);
      Default_Places();
    }
    else
    {
      for(count=0;count<NUMBER_OF_WINDOW_SIZES;count++)
        fscanf(stream,"%d\n%d\n",&starting_sizes[count][0],&starting_sizes[count][1]);
      for(count=0;count<NUMBER_OF_WINDOWS;count++)
      {
        fscanf(stream,"%d\n",&windows_on_top[count]);
        if (windows_on_top[count]<1)
          windows_on_top[count] = 100;
      }
    }
    fclose(stream);
  }
}

void load_stations()

{
  FILE *stream;
  int count;
  char string[80];

  stream = fopen(stations_name,"r");
  if (stream)
  {
    for(count=0;count<MAX_STATIONS && !feof(stream);count++)
    {
      fgets(stations[count].name,40,stream);
      if (stations[count].name[strlen(stations[count].name)-1]=='\n')
        stations[count].name[strlen(stations[count].name)-1] = 0;
      if (strcmp(stations[count].name,"NULL") == 0)
        stations[count].name[0] = 0;
      if (strcmp(stations[count].name,"0") == 0)
        stations[count].name[0] = 0;
		fgets(string,16,stream);
      sscanf(string,"%ld",&stations[count].frequency);
      fgets(string,16,stream);
      sscanf(string,"%d",&stations[count].mode);
      fgets(string,16,stream);
      sscanf(string,"%d",&stations[count].band);
    }
    while(count<MAX_STATIONS)
      stations[count++].name[0] = 0;
	 for(count=0;count<MAX_QUICK_STATIONS && !feof(stream);count++)
    {
      fgets(quick_stations[count].name,40,stream);
      if (quick_stations[count].name[strlen(quick_stations[count].name)-1]=='\n')
        quick_stations[count].name[strlen(quick_stations[count].name)-1] = 0;
      if (strcmp(quick_stations[count].name,"NULL") == 0)
        quick_stations[count].name[0] = 0;
      if (strcmp(quick_stations[count].name,"0") == 0)
        quick_stations[count].name[0] = 0;
      fgets(string,16,stream);
      sscanf(string,"%ld",&quick_stations[count].frequency);
      if (quick_stations[count].frequency<450000l ||
          quick_stations[count].frequency>30000000l)
        quick_stations[count].frequency = 10000000l;
      fgets(string,16,stream);
      sscanf(string,"%d",&quick_stations[count].mode);
      fgets(string,16,stream);
      sscanf(string,"%d",&quick_stations[count].band);
    }
    while(count<MAX_QUICK_STATIONS)
      quick_stations[count++].name[0] = 0;
    for(count=0;count<MAX_QUICK_STATIONS && !feof(stream);count++)
    {
      fgets(string,16,stream);
      sscanf(string,"%d",&channels_selected[count]);
    }
    while(count<MAX_QUICK_STATIONS)
      channels_selected[count++] = 0;

  }
  else
  {
    for(count = 0;count<MAX_STATIONS;count++)
      stations[count].name[0] = 0;
    for(count = 0;count<MAX_QUICK_STATIONS;count++)
      quick_stations[count].name[0] = 0;
    for(count = 0;count<MAX_QUICK_STATIONS;count++)
      channels_selected[count] = 0;
  }
  if (!feof_or_null(stream))
  {
    fgets(string,16,stream);
    sscanf(string,"%lf",&Band_Scan_Data.min_ss);
    fgets(string,16,stream);
    sscanf(string,"%lf",&Band_Scan_Data.max_ss);
    fgets(string,16,stream);
    sscanf(string,"%lf",&Band_Scan_Data.listen_time);
    fgets(string,16,stream);
    sscanf(string,"%lf",&Band_Scan_Data.hold_time);
    fgets(string,16,stream);
    sscanf(string,"%d",&Band_Scan_Data.infinity);
    fgets(string,16,stream);
    sscanf(string,"%d",&Band_Scan_Data.band);
    fgets(string,16,stream);
    sscanf(string,"%d",&Band_Scan_Data.channel);
    fgets(string,16,stream);
    sscanf(string,"%lf",&Frequency_Scan_Data.min_ss);
    fgets(string,16,stream);
    sscanf(string,"%lf",&Frequency_Scan_Data.max_ss);
    fgets(string,16,stream);
    sscanf(string,"%lf",&Frequency_Scan_Data.listen_time);
    fgets(string,16,stream);
    sscanf(string,"%lf",&Frequency_Scan_Data.hold_time);
    fgets(string,16,stream);
    sscanf(string,"%d",&Frequency_Scan_Data.infinity);
    fgets(string,16,stream);
    sscanf(string,"%ld",&Frequency_Scan_Data.start_frequency);
    fgets(string,16,stream);
    sscanf(string,"%ld",&Frequency_Scan_Data.stop_frequency);
    fgets(string,16,stream);
    sscanf(string,"%ld",&Frequency_Scan_Data.step);
  }
}

void Default_Places()

{
  starting_places[0][0] = 50;
  starting_places[0][1] = 50;
  starting_places[1][0] = 190;
  starting_places[1][1] = 50;
  starting_places[2][0] = 330;
  starting_places[2][1] = 50;
  starting_places[3][0] = 470;
  starting_places[3][1] = 50;
  starting_places[4][0] = 50;
  starting_places[4][1] = 151;
  starting_places[5][0] = 190;
  starting_places[5][1] = 151;
  starting_places[6][0] = 330;
  starting_places[6][1] = 151;
  starting_places[7][0] = 50;
  starting_places[7][1] = 252;
  starting_places[8][0] = 50;
  starting_places[8][1] = 352;
  starting_places[9][0] = 0;
  starting_places[9][1] = 0;
  starting_places[10][0] = 214;
  starting_places[10][1] = 460;
  starting_places[11][0] = 140;
  starting_places[11][1] = 441;
  starting_places[12][0] = 446;
  starting_places[12][1] = 92;
  starting_places[13][0] = 112;
  starting_places[13][1] = 210;
  starting_places[14][0] = 157;
  starting_places[14][1] = 170;
  starting_places[15][0] = 490;
  starting_places[15][1] = 84;
  starting_places[16][0] = 600;
  starting_places[16][1] = 113;
  starting_places[17][0] = 150;
  starting_places[17][1] = 200;
  starting_places[18][0] = 0;
  starting_places[18][1] = 0;
  starting_places[19][0] = 471;
  starting_places[19][1] = 151;
  starting_places[20][0] = 470;
  starting_places[20][1] = 151;
  starting_places[21][0] = 619;
  starting_places[21][1] = -268;
  starting_places[22][0] = 190;
  starting_places[22][1] = -250;
  starting_places[23][0] = 330;
  starting_places[23][1] = -250;
  starting_places[24][0] = 470;
  starting_places[24][1] = -250;
  starting_places[25][0] = 471;
  starting_places[25][1] = -253;
  starting_places[26][0] = 18;
  starting_places[26][1] = 408;
  starting_places[27][0] = 400;
  starting_places[27][1] = -300;
  starting_places[28][0] = 358;
  starting_places[28][1] = -349;
  starting_places[29][0] = 622;
  starting_places[29][1] = -53;
  starting_places[30][0] = 449;
  starting_places[30][1] = 350;
  starting_places[31][0] = 459;
  starting_places[31][1] = 360;
  starting_places[32][0] = 459;
  starting_places[32][1] = -360;
  starting_places[33][0] = 469;
  starting_places[33][1] = -370;
  starting_places[34][0] = 479;
  starting_places[34][1] = -380;
  starting_places[35][0] = 489;
  starting_places[35][1] = -390;
  starting_places[36][0] = 499;
  starting_places[36][1] = -400;
  starting_places[37][0] = 509;
  starting_places[37][1] = -410;
  starting_places[38][0] = 519;
  starting_places[38][1] = -420;
  starting_places[39][0] = 529;
  starting_places[39][1] = -430;
  starting_places[40][0] = 249;
  starting_places[40][1] = 250;
  starting_places[41][0] = 249;
  starting_places[41][1] = -350;
  starting_places[42][0] = 259;
  starting_places[42][1] = -360;
  starting_places[43][0] = 269;
  starting_places[43][1] = -370;
  starting_places[44][0] = 279;
  starting_places[44][1] = -380;
  starting_places[45][0] = 289;
  starting_places[45][1] = -390;
  starting_places[46][0] = 299;
  starting_places[46][1] = -400;
  starting_places[47][0] = 309;
  starting_places[47][1] = -410;
  starting_places[48][0] = 319;
  starting_places[48][1] = -420;
  starting_places[49][0] = 329;
  starting_places[49][1] = -430;
  starting_sizes[0][0] = 639;
  starting_sizes[0][1] = 479;
  starting_sizes[1][0] = 562;
  starting_sizes[1][1] = 169;
  starting_sizes[2][0] = 283;
  starting_sizes[2][1] = -43;
  starting_sizes[3][0] = 308;
  starting_sizes[3][1] = -123;
  starting_sizes[4][0] = 135;
  starting_sizes[4][1] = -215;
  starting_sizes[5][0] = 262;
  starting_sizes[5][1] = -248;
  starting_sizes[6][0] = 137;
  starting_sizes[6][1] = 248;
  starting_sizes[7][0] = 135;
  starting_sizes[7][1] = -244;
  starting_sizes[8][0] = 138;
  starting_sizes[8][1] = 288;
  starting_sizes[9][0] = 138;
  starting_sizes[9][1] = 296;
  starting_sizes[10][0] = 0;
  starting_sizes[10][1] = 0;
  starting_sizes[13][0] = 140;
  starting_sizes[13][1] = 101;
}

void save_stations()

{
  FILE *stream;
  int count;

  stream = fopen(stations_name,"w");
  for(count=0;count<MAX_STATIONS;count++)
    if (stations[count].name[0] == 0)
      fprintf(stream,"NULL\n0\n0\n0\n");
    else
      fprintf(stream,"%s\n%ld\n%d\n%d\n",stations[count].name,
    	      stations[count].frequency,stations[count].mode,
			stations[count].band);
  for(count=0;count<MAX_QUICK_STATIONS;count++)
    if (quick_stations[count].name[0] == 0)
      fprintf(stream,"%d\n%ld\n%d\n%d\n",count,
              quick_stations[count].frequency,quick_stations[count].mode,
	           quick_stations[count].band);
    else
      fprintf(stream,"%s\n%ld\n%d\n%d\n",quick_stations[count].name,
    	        quick_stations[count].frequency,quick_stations[count].mode,
	           quick_stations[count].band);
  for(count=0;count<MAX_QUICK_STATIONS;count++)
    fprintf(stream,"%d\n",channels_selected[count]);
  fprintf(stream,"%lf\n",Band_Scan_Data.min_ss);
  fprintf(stream,"%lf\n",Band_Scan_Data.max_ss);
  fprintf(stream,"%lf\n",Band_Scan_Data.listen_time);
  fprintf(stream,"%lf\n",Band_Scan_Data.hold_time);
  fprintf(stream,"%d\n",Band_Scan_Data.infinity);
  fprintf(stream,"%d\n",Band_Scan_Data.band);
  fprintf(stream,"%d\n",Band_Scan_Data.channel);
  fprintf(stream,"%lf\n",Frequency_Scan_Data.min_ss);
  fprintf(stream,"%lf\n",Frequency_Scan_Data.max_ss);
  fprintf(stream,"%lf\n",Frequency_Scan_Data.listen_time);
  fprintf(stream,"%lf\n",Frequency_Scan_Data.hold_time);
  fprintf(stream,"%d\n",Frequency_Scan_Data.infinity);
  fprintf(stream,"%ld\n",Frequency_Scan_Data.start_frequency);
  fprintf(stream,"%ld\n",Frequency_Scan_Data.stop_frequency);
  fprintf(stream,"%ld\n",Frequency_Scan_Data.step);
  fclose(stream);
}

long get_long(FILE *stream)

{
  long answer;

  fscanf(stream,"%ld\n",&answer);
  return answer;
}

void dly()

{
  struct time start_time,end_time;
  int count;
  char string[80];

  for(count=0;count<2;count++)
  {
    gettime(&start_time);
    do
      gettime(&end_time);
    while (start_time.ti_hund == end_time.ti_hund);
  }
}

int Keyboard(uint key, uint repeatCount, uint flags)

{
  if (key == 13)
  {
    ClipCursor(NULL);
    ShowCursor(0);
    while (ShowCursor(1)<0)
      {}
  }
//  MessageBox(NULL,"A key has been pressed",NULL,MB_OK);
}

void MoveWindowToTopOfList(int which)

{
  int current;
  int count;

  current = windows_on_top[which];
  if (current<1)
	 current = NUMBER_OF_WINDOWS;
  while (current>1)
  {
    for(count=0;count<NUMBER_OF_WINDOWS;count++)
      if (windows_on_top[count] == current-1)
        windows_on_top[count] = current;
    current--;
  }
  windows_on_top[which] = 1;
}

int feof_or_null(FILE *stream)

{
  if (!stream)
	 return 1;
  else
	 return feof(stream);
}

void PrintInt(int value)

{
  char string[80];

  sprintf(string,"%d",value);
  MessageBox(NULL,string,"TEST",MB_OK);
}

