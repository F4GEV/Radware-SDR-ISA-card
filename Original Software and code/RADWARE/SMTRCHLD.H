_CLASSDEF(TSMTRChld)
// Declare TSMTRChld, a TDialog descendant
class TSMTRChld : public AWindow

{
public:
  int smeter;
  int last_values[100];
  int place;
  int filter;

  TSMTRChld(TWindow *AParent,int id, LPSTR AName,int x,int y,int w,int h,
            char *icon,int Filter);
  ~TSMTRChld();
  LPSTR GetClassName();
  void GetWindowClass(WNDCLASS _FAR & AWndClass);
  void Set_Value(int value);
  int Where_Y();
  int Width();
  int Height();
  void Paint(TDC& DC, BOOL,TRect &);
};
