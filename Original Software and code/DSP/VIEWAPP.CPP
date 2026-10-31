/*  Project view
    Comer Communications, Inc.
    Copyright © 1994. All Rights Reserved.

    SUBSYSTEM:    view.exe Application
    FILE:         viewapp.cpp
    AUTHOR:       Alan Comer


    OVERVIEW
    ========
    Source file for implementation of viewApp (TApplication).      
*/


#include <owl\owlpch.h>
#pragma hdrstop


#include "viewapp.h"
#include "vwabtdlg.h"                        // Definition of about dialog.       


//{{viewApp Implementation}}

//{{DOC_VIEW}}
DEFINE_DOC_TEMPLATE_CLASS(TFileDocument, TEditView, DocType1);
//{{DOC_VIEW_END}}

//{{DOC_MANAGER}}
DocType1 __dvt1("All Files (*.*)", "*.*", 0, "TXT", dtAutoDelete | dtUpdateDir);
//{{DOC_MANAGER_END}}

//
// Build a response table for all messages/commands handled
// by the application.
//
DEFINE_RESPONSE_TABLE1(viewApp, TApplication)
//{{viewAppRSP_TBL_BEGIN}}
    EV_OWLVIEW(dnCreate, EvNewView),
    EV_OWLVIEW(dnClose,  EvCloseView),
    EV_COMMAND(CM_HELPABOUT, CmHelpAbout),
//{{viewAppRSP_TBL_END}}
END_RESPONSE_TABLE;


//////////////////////////////////////////////////////////
// viewApp
// =====
//
viewApp::viewApp () : TApplication("view")
{

    DocManager = new TDocManager(dmSDI | dmMenu);

    // INSERT>> Your constructor code here.

}


viewApp::~viewApp ()
{
    // INSERT>> Your destructor code here.

}


//////////////////////////////////////////////////////////
// viewApp
// =====
// Application intialization.
//
void viewApp::InitMainWindow ()
{
    SDIDecFrame *frame = new SDIDecFrame(0, GetName(), 0, FALSE);

    nCmdShow = nCmdShow != SW_SHOWMINIMIZED ? SW_SHOWNORMAL : nCmdShow;

    //
    // Assign ICON w/ this application.
    //
    frame->SetIcon(this, IDI_SDIAPPLICATION);

    //
    // Menu associated with window and accelerator table associated with table.
    //
    frame->AssignMenu(SDI_MENU);
    
    //
    // Associate with the accelerator table.
    //
    frame->Attr.AccelTable = SDI_MENU;

  
    SetMainWindow(frame);

}


//////////////////////////////////////////////////////////
// viewApp
// =====
// Response Table handlers:
//
void viewApp::EvNewView (TView& view)
{
    GetMainWindow()->SetClientWindow(view.GetWindow());
    if (!view.IsOK())
        GetMainWindow()->SetClientWindow(0);
}


void viewApp::EvCloseView (TView&)
{
    GetMainWindow()->SetClientWindow(0);
    GetMainWindow()->SetCaption("view");
}


//{{SDIDecFrame Implementation}}


SDIDecFrame::SDIDecFrame (TWindow *parent, const char far *title, TWindow *clientWnd, BOOL trackMenuSelection, TModule *module)
    : TDecoratedFrame(parent, title, clientWnd, trackMenuSelection, module)
{
    // INSERT>> Your constructor code here.

}


SDIDecFrame::~SDIDecFrame ()
{
    // INSERT>> Your destructor code here.

}


//////////////////////////////////////////////////////////
// viewApp
// ===========
// Menu Help About view.exe command
void viewApp::CmHelpAbout ()
{
    //
    // Show the modal dialog.
    //
    viewAboutDlg(MainWindow).Execute();
}


int OwlMain (int , char* [])
{
    viewApp     App;
    int             result;

    result = App.Run();

    return result;
}
