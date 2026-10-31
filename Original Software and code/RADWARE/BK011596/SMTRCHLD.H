_CLASSDEF(TSMTRChld)
// Declare TSMTRChld, a TDialog descendant
class TSMTRChld : public AWindow

{
public:
  long smeter;
  int last_values[100];
  int place;
  int filter;
  int mute_position;
  TButton *Mute_Button;
  TStatic *Digital_Readout;

  TSMTRChld(TWindow *AParent,int id, LPSTR AName,int child_number,int w,int h,
            char *icon,int Filter);
  ~TSMTRChld();
  LPSTR GetClassName();
  void GetWindowClass(WNDCLASS _FAR & AWndClass);
  void Set_Value(int value);
  int Where_Y();
  int Width();
  int Height();
  void Toggle_Mute();
  void Paint(TDC& DC, BOOL,TRect &);
  DECLARE_RESPONSE_TABLE(TSMTRChld);
};
