/*  Project logbook
    
    Copyright © 1994. All Rights Reserved.

    SUBSYSTEM:    logbook.exe Application
    FILE:         logbkapp.cpp
    AUTHOR:       


    OVERVIEW
    ========
    Source file for implementation of logbookApp (TApplication).      
*/


#include <owl\owlpch.h>
#pragma hdrstop

#include <owl\vbxctl.h>

#include <dir.h>

#include "logbkapp.h"
#include "lgbkabtd.h"                        // Definition of about dialog.       


// Drag / Drop support:
TFileDrop::TFileDrop (char* fileName, TPoint& p, BOOL inClient, TModule*)
{
    char    exePath[MAXPATH];
    
    exePath[0] = 0;
    FileName = strcpy(new char[strlen(fileName) + 1], fileName);
    Point = p;
    InClientArea = inClient;
}

TFileDrop::~TFileDrop ()
{
    delete FileName;
}

const char *TFileDrop::WhoAmI ()
{
  return FileName;
}


//{{logbookApp Implementation}}

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
DEFINE_RESPONSE_TABLE1(logbookApp, TApplication)
//{{logbookAppRSP_TBL_BEGIN}}
    EV_OWLVIEW(dnCreate, EvNewView),
    EV_OWLVIEW(dnClose,  EvCloseView),
    EV_COMMAND(CM_HELPABOUT, CmHelpAbout),
    EV_COMMAND(CM_FILEPRINT, CmFilePrint),
    EV_COMMAND(CM_FILEPRINTERSETUP, CmFilePrintSetup),
    EV_COMMAND(CM_FILEPRINTPREVIEW, CmFilePrintPreview),
    EV_COMMAND_ENABLE(CM_FILEPRINT, CmPrintEnable),
    EV_COMMAND_ENABLE(CM_FILEPRINTERSETUP, CmPrintEnable),
    EV_COMMAND_ENABLE(CM_FILEPRINTPREVIEW, CmPrintEnable),
    EV_WM_DROPFILES,
    EV_WM_WININICHANGE,
//{{logbookAppRSP_TBL_END}}
END_RESPONSE_TABLE;


//////////////////////////////////////////////////////////
// logbookApp
// =====
//
logbookApp::logbookApp () : TApplication("RADWARE Logbook")
{

    Printer = 0;
    Printing = 0;

    DocManager = new TDocManager(dmSDI | dmMenu);

    // INSERT>> Your constructor code here.

}


logbookApp::~logbookApp ()
{
    if (Printer)
        delete Printer;

    // INSERT>> Your destructor code here.

}


void logbookApp::SetupSpeedBar (TDecoratedFrame *frame)
{ 
    //
    // Create default toolbar New and associate toolbar buttons with commands.
    //   
    TControlBar* cb = new TControlBar(frame);
    cb->Insert(*new TButtonGadget(CM_FILENEW, CM_FILENEW));
    cb->Insert(*new TButtonGadget(CM_FILEOPEN, CM_FILEOPEN));
    cb->Insert(*new TButtonGadget(CM_FILESAVE, CM_FILESAVE));
    cb->Insert(*new TSeparatorGadget(6));
    cb->Insert(*new TButtonGadget(CM_EDITCUT, CM_EDITCUT));
    cb->Insert(*new TButtonGadget(CM_EDITCOPY, CM_EDITCOPY));
    cb->Insert(*new TButtonGadget(CM_EDITPASTE, CM_EDITPASTE));
    cb->Insert(*new TSeparatorGadget(6));
    cb->Insert(*new TButtonGadget(CM_EDITUNDO, CM_EDITUNDO));
    cb->Insert(*new TSeparatorGadget(6));
    cb->Insert(*new TButtonGadget(CM_EDITFIND, CM_EDITFIND));
    cb->Insert(*new TButtonGadget(CM_EDITFINDNEXT, CM_EDITFINDNEXT));
    cb->Insert(*new TSeparatorGadget(6));
    cb->Insert(*new TButtonGadget(CM_FILEPRINT, CM_FILEPRINT));
    cb->Insert(*new TButtonGadget(CM_FILEPRINTPREVIEW, CM_FILEPRINTPREVIEW));

    // Add fly-over help hints.
    cb->SetHintMode(TGadgetWindow::EnterHints);

    frame->Insert(*cb, TDecoratedFrame::Top);
}


//////////////////////////////////////////////////////////
// logbookApp
// =====
// Application intialization.
//
void logbookApp::InitMainWindow ()
{
    SDIDecFrame *frame = new SDIDecFrame(0, GetName(), 0, TRUE);

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

    SetupSpeedBar(frame);

    TStatusBar *sb = new TStatusBar(frame, TGadget::Recessed,
                                    TStatusBar::CapsLock        |
                                    TStatusBar::NumLock         |
                                    TStatusBar::ScrollLock      |
                                    TStatusBar::Overtype);
    frame->Insert(*sb, TDecoratedFrame::Bottom);
  
    SetMainWindow(frame);

}


//////////////////////////////////////////////////////////
// logbookApp
// =====
// Response Table handlers:
//
void logbookApp::EvNewView (TView& view)
{
    GetMainWindow()->SetClientWindow(view.GetWindow());
    if (!view.IsOK())
        GetMainWindow()->SetClientWindow(0);
}


void logbookApp::EvCloseView (TView&)
{
    GetMainWindow()->SetClientWindow(0);
    GetMainWindow()->SetCaption("RADWARE Logbook");
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


//
// Paint routine for Window, Printer, and PrintPreview for an TEdit client.
//
void SDIDecFrame::Paint (TDC& dc, BOOL, TRect& rect)
{
    logbookApp *theApp = TYPESAFE_DOWNCAST(GetApplication(), logbookApp);
    if (theApp) {
        // Only paint if we're printing and we have something to paint, otherwise do nothing.
        if (theApp->Printing && theApp->Printer && !rect.IsEmpty()) {
            // Use pageSize to get the size of the window to render into.  For a Window it's the client area,
            // for a printer it's the printer DC dimensions and for print preview it's the layout window.
            TSize   pageSize(rect.right - rect.left, rect.bottom - rect.top);

            HFONT   hFont = (HFONT)GetClientWindow()->GetWindowFont();
            TFont   font("Arial", -12);
            if (hFont == 0)
              dc.SelectObject(font);
            else
              dc.SelectObject(TFont(hFont));
    
            TEXTMETRIC  tm;
            int fHeight = (dc.GetTextMetrics(tm) == TRUE) ? tm.tmHeight + tm.tmExternalLeading : 10;
    
            // How many lines of this font can we fit on a page.
            int linesPerPage = MulDiv(pageSize.cy, 1, fHeight);
            if (linesPerPage) {    
                TPrintDialog::TData &printerData = theApp->Printer->GetSetup();

                int maxPg = 1;

                // Get the client class window (this is the contents we're going to print).
                TEdit *clientEditWindow = 0;
                TListBox *clientListWindow = 0;

                clientEditWindow = TYPESAFE_DOWNCAST(GetClientWindow(), TEdit);
                if (clientEditWindow)
                    maxPg = ((clientEditWindow->GetNumLines() / linesPerPage) + 1.0);
                else {
                    clientListWindow = TYPESAFE_DOWNCAST(GetClientWindow(), TListBox);
                    if (clientListWindow)
                        maxPg = ((clientListWindow->GetCount() / linesPerPage) + 1.0);
                }

                // Compute the number of pages to print.
                printerData.MinPage = 1;
                printerData.MaxPage = maxPg;
   
                // Do the text stuff:
                int     fromPage = printerData.FromPage == -1 ? 1 : printerData.FromPage;
                int     toPage = printerData.ToPage == -1 ? 1 : printerData.ToPage;
                char    buffer[255];
                int     currentPage = fromPage;
    
                while (currentPage <= toPage) {
                    int startLine = (currentPage - 1) * linesPerPage;
                    int lineIdx = 0;
                    while (lineIdx < linesPerPage) {
                        // If the string is no longer valid then there's nothing more to display.
                        if (clientEditWindow) {
                            if (!clientEditWindow->GetLine(buffer, sizeof(buffer), startLine + lineIdx))
                                break;
                        }
                        if (clientListWindow) {
                            if (clientListWindow->GetString(buffer, startLine + lineIdx) < 0)
                                break;
                        }
                        dc.TabbedTextOut(TPoint(0, lineIdx * fHeight), buffer, lstrlen(buffer), 0, NULL, 0);
                        lineIdx++;
                    }
                    currentPage++;
                }
            }
        }
    }
}


//////////////////////////////////////////////////////////
// logbookApp
// ==========
// Menu File Print command
void logbookApp::CmFilePrint ()
{
    //
    // Create Printer object if not already created.
    // 
    if (!Printer)
        Printer = new TPrinter;

    //
    // Create Printout window and set characteristics.
    //
    APXPrintOut printout(Printer, "Title", GetMainWindow());
    printout.SetBanding(TRUE);

    Printing++;

    //
    // Bring up the Print dialog and print the document.
    //
    Printer->Print(GetMainWindow(), printout, TRUE);

    Printing--;
}


//////////////////////////////////////////////////////////
// logbookApp
// ==========
// Menu File Print Setup command
void logbookApp::CmFilePrintSetup ()
{
    if (!Printer)
        Printer = new TPrinter;

    //
    // Bring up the Print Setup dialog.
    //
    Printer->Setup(GetMainWindow());
}


//////////////////////////////////////////////////////////
// logbookApp
// ==========
// Menu File Print Preview command
void logbookApp::CmFilePrintPreview ()
{
    SDIDecFrame *sdiFrame = TYPESAFE_DOWNCAST(GetMainWindow(), SDIDecFrame); 
    if (sdiFrame) {
        if (Printer)
            Printer = new TPrinter;

        Printing++;

        PreviewWindow *prevW = new PreviewWindow(sdiFrame, Printer, sdiFrame, "Print Preview", new TLayoutWindow(0));
        prevW->Create();

        BeginModal(GetMainWindow());

        Printing--;

        // Now that printing is off we can invalidate because the edit window to repaint.
        GetMainWindow()->SetRedraw(TRUE);
        GetMainWindow()->Invalidate();

        // We must destroy the preview window explicitly.  Otherwise, the window will not be destroyed until
        // it's parent the MainWindow is destroyed.
        prevW->Destroy();
        delete prevW;
    }
}


//////////////////////////////////////////////////////////
// logbookApp
// ==========
// Menu enabler used by Print, Print Setup and Print Preview.
void logbookApp::CmPrintEnable (TCommandEnabler &tce)
{
    // If we have a Printer already created just test if all is okay.
    // Otherwise create a Printer object and make sure the printer
    // really exists and then delete the Printer object.
    if (!Printer) {
        Printer = new TPrinter;
        
        tce.Enable(Printer->GetSetup().Error == 0);
    } else
        tce.Enable(Printer->GetSetup().Error == 0);
}


//////////////////////////////////////////////////////////
// logbookApp
// ===========
// Menu Help About logbook.exe command
void logbookApp::CmHelpAbout ()
{
    //
    // Show the modal dialog.
    //
    logbookAboutDlg(MainWindow).Execute();
}


void logbookApp::InitInstance ()
{
    TApplication::InitInstance();

    // Accept files via drag/drop in the frame window.
    GetMainWindow()->DragAcceptFiles(TRUE);
}


void logbookApp::EvDropFiles (TDropInfo drop)
{
	// Tell DragQueryFile the file interested in (0) and the length of your buffer.
	int     fileLength = drop.DragQueryFileNameLen(0) + 1;
	char    *fileName = new char[fileLength];

	drop.DragQueryFile(0, fileName, fileLength);

	// Open the file that was dropped.
	AddFile(fileName);
    // Release the memory allocated for this handle with DragFinish.
    drop.DragFinish();
}


void logbookApp::AddFile (const char *FileName)
{
    TDocTemplate* tpl = GetDocManager()->MatchTemplate(FileName);
    if (tpl)
      tpl->CreateDoc(FileName);
}


void logbookApp::EvWinIniChange (char far* section)
{
    if (lstrcmp(section, "windows") == 0) {
        // If the device changed in the WIN.INI file then the printer
        // might have changed.  If we have a TPrinter (Printer) then
        // check and make sure it's identical to the current device
        // entry in WIN.INI.
        if (Printer) {
            char printDBuffer[255];
            LPSTR printDevice = printDBuffer;
            LPSTR devName = 0;
            LPSTR driverName = 0;
            LPSTR outputName = 0;
        
            if (::GetProfileString("windows", "device", "", printDevice, sizeof(printDevice))) {
                // The string which should come back is something like:
                //
                //      HP LaserJet III,hppcl5a,LPT1:
                //
                // Where the format is:
                //
                //      devName,driverName,outputName
                //
                devName = printDevice;
                while (*printDevice) {
                    if (*printDevice == ',') {
                        *printDevice++ = 0;
                        if (!driverName)
                            driverName = printDevice;
                        else
                            outputName = printDevice;
                    } else
                        printDevice = AnsiNext(printDevice);
                }

                if ((Printer->GetSetup().Error != 0)                                ||
                    (lstrcmp(devName, Printer->GetSetup().GetDeviceName()) != 0)    ||
                    (lstrcmp(driverName, Printer->GetSetup().GetDriverName()) != 0) ||
                    (lstrcmp(outputName, Printer->GetSetup().GetOutputName()) != 0)) {
                
                    // New printer installed so get the new printer device now.
                    delete Printer;
                    Printer = new TPrinter;
                }
            } else {
                // No printer installed (GetProfileString failed).
                delete Printer;
                Printer = new TPrinter;
            }
        }
    }
}


int OwlMain (int , char* [])
{
    TBIVbxLibrary   vbxSupport;                 // This application has VBX controls.
    logbookApp     App;
    int             result;

    result = App.Run();

    return result;
}
