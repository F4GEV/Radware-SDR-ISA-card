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

TPORTChld::TPORTChld(TWindow *AParent,int id,LPSTR Title,int child_number,int w,int h):
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
  Attr.W = w;
  Attr.H = h;
  cr = 0;
  Attr.Style = WS_POPUP | WS_VISIBLE | WS_CAPTION |WS_MINIMIZEBOX|WS_SYSMENU;
  Display = new TStatic(this,-1,"",10,10,380,180,199);
}

void TPORTChld::SetupWindow()

{
  AWindow::SetupWindow();
}

TPORTChld::~TPORTChld()

{
  Attr.Y *= -1;
  if (Attr.Y == 0)
    Attr.Y = -1;
  starting_places[Child_Number][0] = Attr.X;
  starting_places[Child_Number][1] = Attr.Y;
  delete_class[Child_Number] = 1;
}

void TPORTChld::EvActivate(uint active,bool b,HWND c)

{
  AWindow::EvActivate(active,b,c);
}

void TPORTChld::Add_Int_String(int *add)   /* add may not be longer than
                                             300 charactors  */

{
  char string[601];
  int length;
  int count;
  int position;

  Display->GetText(string,299);
  length = 0;
  while(add[length++]!=0)
    if (length>300)
      break;
  position = strlen(string);
  for(count=0;count<length;count++)
  {
    if (add[count]>0)
      string[position++] = '1';
    else
      string[position++] = '0';
    if (cr++>40)
    {
      string[position++] = '\n';
      cr = 0;
    }
  }
  string[position] = 0;
  length = strlen(string);
  if (length>=299)
    Display->SetText(string+length-298);
  else
    Display->SetText(string);
}

/*
DEFINE_RESPONSE_TABLE1(TPORTChld,AWindow)
END_RESPONSE_TABLE;
*/
