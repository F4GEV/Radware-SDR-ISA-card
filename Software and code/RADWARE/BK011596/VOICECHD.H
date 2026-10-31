
struct voice_memory
{
  char label[20];
  char filename[80];
  int record;
  int play;
};

void play_voice_memory_start(int which);
void play_voice_memory();

_CLASSDEF(TVoiceChld)

// Declare TVoiceChld, a AWindow descendant
class TVoiceChld : public AWindow
{
   AButton *Button[9];
   int bank;
   int Child_Number;

public:
   TVoiceChld(AWindow *AParent, LPSTR AName,int Bank,int child_number);
   ~TVoiceChld();

   Where_Y();
   void Reply_Button(WPARAM ID);
   void Change_Button(WPARAM ID);
   void UpdateAllButtons();
   void CleanupWindow();
   void EvActivate(uint,bool,HWND);
   DECLARE_RESPONSE_TABLE(TVoiceChld);
};
