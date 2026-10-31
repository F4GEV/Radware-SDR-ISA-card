#if !defined(__viewapp_h)              // Sentry, use file only if it's not already included.
#define __viewapp_h

/*  Project view
    Comer Communications, Inc.
    Copyright © 1994. All Rights Reserved.

    SUBSYSTEM:    view.exe Application
    FILE:         viewapp.h
    AUTHOR:       Alan Comer


    OVERVIEW
    ========
    Class definition for viewApp (TApplication).      
*/


#include <owl\owlpch.h>
#pragma hdrstop

#include <owl\editview.h>
#include <owl\listview.h>
#include <owl\docmanag.h>
#include <owl\filedoc.h>

#include "viewapp.rh"            // Definition of all resources.


//
// FrameWindow must be derived to override Paint for Preview and Print.
//
//{{TDecoratedFrame = SDIDecFrame}}
class SDIDecFrame : public TDecoratedFrame {
public:
    SDIDecFrame (TWindow *parent, const char far *title, TWindow *clientWnd, BOOL trackMenuSelection = FALSE, TModule *module = 0);
    ~SDIDecFrame ();
};    //{{SDIDecFrame}}


//{{TApplication = viewApp}}
class viewApp : public TApplication {
private:

public:
    viewApp ();
    virtual ~viewApp ();

//{{viewAppVIRTUAL_BEGIN}}
public:
    virtual void InitMainWindow();
//{{viewAppVIRTUAL_END}}

//{{viewAppRSP_TBL_BEGIN}}
protected:
    void EvNewView (TView& view);
    void EvCloseView (TView& view);
    void CmHelpAbout ();
//{{viewAppRSP_TBL_END}}
  DECLARE_RESPONSE_TABLE(viewApp);
};    //{{viewApp}}


#endif                                      // __viewapp_h sentry.
