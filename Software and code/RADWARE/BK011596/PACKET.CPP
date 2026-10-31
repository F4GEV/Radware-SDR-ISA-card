#include <thiscls.h>
#include <stdio.h>
extern int starting_places[][2];
extern int delete_class[];
extern char morse_button_names[][6];
extern char morse_button_macros[][41];

extern TCONTROLChld *PTPTT;
extern PPTT_Control ptt_control;
extern char CallSign[];
extern TBitmap *sheet_bitmap[];
extern TBitmap *tune_bitmap[];
extern packet_length;

int packet_speed = 27;
int packet_tolerance = 20;
char last_8_bits(int bit);

TPACKETChld::TPACKETChld(TWindow *AParent,int id,LPSTR Title,int child_number,int w,int h):
                                 AWindow(AParent,Title,child_number)

{
  int x,y,count;

  Child_Number = child_number;
  x = starting_places[Child_Number][0];
  y = starting_places[Child_Number][1];
  if (y<0)
    y = -y;
  Attr.X = x;
  Attr.Y = y;
  Attr.W = 600; //w;
  Attr.H = 400; //h;
  cr = 0;
  Attr.Style = WS_POPUP | WS_VISIBLE | WS_CAPTION |WS_MINIMIZEBOX|WS_SYSMENU;
  Display = new TStatic(this,-1,"",10,10,580,380,399);
}

void TPACKETChld::SetupWindow()

{
  AWindow::SetupWindow();
}

TPACKETChld::~TPACKETChld()

{
  Attr.Y *= -1;
  if (Attr.Y == 0)
    Attr.Y = -1;
  starting_places[Child_Number][0] = Attr.X;
  starting_places[Child_Number][1] = Attr.Y;
  delete_class[Child_Number] = 1;
}

void TPACKETChld::EvActivate(uint active,bool b,HWND c)

{
  AWindow::EvActivate(active,b,c);
}

int last_direction = 0,last_amount = 0;

void TPACKETChld::Add_Int_String(int *add)   /* add may not be longer than
                                             300 charactors  */

{
  char string[801],temp[15];
  int length;
  int count;
  int position,temp_position;

  Display->GetText(string,399);
  sprintf(temp,"\n <%d> ",packet_length);
  strcat(string,temp);
  cr=strlen(temp);
  length = add[0]-1;
  add++;
  sprintf(temp," {%d} ",length);
  strcat(string,temp);
  cr+=strlen(temp);
  position = strlen(string);
  for (count=0;count<length;count++)
  {
    sprintf(temp,"[%d]",add[count]);
    temp_position = 0;
    while(temp[temp_position]!=0)
      string[position++] = temp[temp_position++];
    cr+=strlen(temp);
/*    if ((add[count]>=-(packet_speed+1)/2) && (add[count]<=(packet_speed+1)/2))
    {
      string[position++] = 'E';
      if (cr++>40)
      {
        string[position++] = '\n';
        cr = 0;
      }
    }
*/  if (add[count]<0)
    {
      if (last_direction<0)
        add[count]+=last_amount;
      else
      {
/*        sprintf(temp,"-(%d)",last_amount);
        temp_position = 0;
        while(temp[temp_position]!=0)
          string[position++] = temp[temp_position++];
        cr+=strlen(temp);
*/      }
      while (add[count]<-packet_tolerance)
      {
//        string[position++] = '0';
//        string[position++] = last_8_bits(0);
        add[count]+=packet_speed;
        if ((cr++)>38)
        {
          string[position++] = '\n';
          cr = 0;
        }
        else
          if (cr%10==0)
            string[position++] = ' ';
      }
      last_direction = -1;
      last_amount = add[count];
    }
    else
    {
      if (last_direction>0)
        add[count]+=last_amount;
      else
      {
/*        sprintf(temp,"+(%d)",last_amount);
        temp_position = 0;
        while(temp[temp_position]!=0)
          string[position++] = temp[temp_position++];
        cr+=strlen(temp);
*/      }
      while (add[count]>packet_tolerance)
      {
//        string[position++] = '1';
//        string[position++] = last_8_bits(1);
        add[count]-=packet_speed;
        if ((cr++)>38)
        {
          string[position++] = '\n';
          cr = 0;
        }
        else
          if (cr%10==0)
            string[position++] = ' ';
      }
      last_direction = 1;
      last_amount = add[count];
    }
    if (position>600)
      break;
/*    if (add[count]>packet_speed/4 || add[count]<-packet_speed/4)
      string[position++] = ' ';
*/
  }
  string[position] = 0;
  length = strlen(string);
  if (length>=399)
    Display->SetText(string+length-398);
  else
    Display->SetText(string);
}

int bits=0;

char last_8_bits(int bit)

{
  bits = ((bits*2)+bit)%256;
  return bits;
}

/*
DEFINE_RESPONSE_TABLE1(TPACKETChld,AWindow)
END_RESPONSE_TABLE;
*/
