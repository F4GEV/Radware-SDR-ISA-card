_CLASSDEF(TSTATChld)

// Declare TSTATChld, a AWindow descendant
class TSTATChld : public AWindow
{
   AButton *Button[9];
   int bank;

public:
   TSTATChld(AWindow *AParent, LPSTR AName,int child_number,int Bank);
   ~TSTATChld();

   Where_Y();
   void Reply_Button(WPARAM ID);
   void Change_Button(WPARAM ID);
   void Update_Channel();
   DECLARE_RESPONSE_TABLE(TSTATChld);
};

void Recall_Channel(int which);
