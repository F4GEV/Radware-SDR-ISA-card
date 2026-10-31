#include <thiscls.h>
#define SENT_BUFFER_LENGTH 500

extern int closing;

extern int starting_places[][2];
extern int delete_class[];

TSENT_MORSEChld::TSENT_MORSEChld(TWindow *AParent,LPSTR Title,int child_number,int w,int h):
                                 AWindow(AParent,Title,child_number)

{
  int x,y;

  Child_Number = child_number;
  x = starting_places[child_number][0];
  y = starting_places[child_number][1];
  if (y<0)
    y = -y;
  Attr.X = x;
  Attr.Y = y;
  Attr.W = w;
  Attr.H = h;
  Attr.Style = WS_POPUP | WS_VISIBLE | WS_CAPTION |WS_MINIMIZEBOX|WS_SYSMENU;
  sent_data = new TStatic(this,-1,"",3,3,w,h,SENT_BUFFER_LENGTH);
  closing = 0;
}

TSENT_MORSEChld::~TSENT_MORSEChld()

{
  Attr.Y *= -1;
  if (Attr.Y>=0)
    Attr.Y = -1;
  closing = 1;
  starting_places[Child_Number][0] = Attr.X;
  starting_places[Child_Number][1] = Attr.Y;
  delete_class[Child_Number] = 1;
}

void TSENT_MORSEChld::EvActivate(uint active,bool b,HWND c)

{
  if (active != WA_INACTIVE)
    MoveWindowToTopOfList(Child_Number);
  AWindow::EvActivate(active,b,c);
//  PostMessage(WM_COMMAND,201,NULL);
}

/*BOOL TSENT_MORSEChld::CanClose()

{
  if (closing)
    return AWindow::CanClose();
  else
  {
    Show(SW_HIDE);
    return FALSE;
  }
}
*/
void TSENT_MORSEChld::Add_Char(char c)

{
  char string[SENT_BUFFER_LENGTH+2];
  int length;

  sent_data->GetText(string,SENT_BUFFER_LENGTH);
  length = strlen(string);
  string[length] = c;
  string[length+1] = 0;
  if (length>=SENT_BUFFER_LENGTH)
    sent_data->SetText(string+1);
  else
    sent_data->SetText(string);
}


