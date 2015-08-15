// Tracer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Tracer.h"

#include "./View/MainFrm.h"
#include "./Data/TracerDoc.h"
#include "./View/TracerView.h"
#include "./View/LicenseDlg.h"
#include "./View/AboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTracerApp

BEGIN_MESSAGE_MAP(CTracerApp, CWinApp)
    ON_COMMAND(ID_APP_ABOUT, &CTracerApp::OnAppAbout)
    // Standard file based document commands
    ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
    ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
    // Standard print setup command
    ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
	ON_COMMAND(ID_APP_LICENSE, &CTracerApp::OnAppLicense)
	ON_COMMAND(ID_RULES_IMPORT, &CTracerApp::OnRulesImport)
	ON_COMMAND(ID_RULES_EXPORT, &CTracerApp::OnRulesExport)
END_MESSAGE_MAP()


// CTracerApp construction

CTracerApp::CTracerApp()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}


// The one and only CTracerApp object

CTracerApp theApp;


// CTracerApp initialization

BOOL CTracerApp::InitInstance()
{
    //Load the scintilla dll
#ifdef _DEBUG
    m_hSciDLL = LoadLibrary(_T("SciEdD.dll"));
#else
    m_hSciDLL = LoadLibrary(_T("SciEd.dll"));
#endif

    if (m_hSciDLL == NULL)
    {
        AfxMessageBox(_T("Scintilla DLL is not installed, Please download the SciTE editor and copy the SciLexer.dll into this application's directory"));
        return FALSE;
    }

    // InitCommonControlsEx() is required on Windows XP if an application
    // manifest specifies use of ComCtl32.dll version 6 or later to enable
    // visual styles.  Otherwise, any window creation will fail.
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    // Set this to include all the common control classes you want to use
    // in your application.
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();

    // Initialize OLE libraries
    if (!AfxOleInit())
    {
        AfxMessageBox(IDP_OLE_INIT_FAILED);
        return FALSE;
    }
    AfxEnableControlContainer();
    // Standard initialization
    // If you are not using these features and wish to reduce the size
    // of your final executable, you should remove from the following
    // the specific initialization routines you do not need
    // Change the registry key under which our settings are stored
    // TODO: You should modify this string to be something appropriate
    // such as the name of your company or organization
    SetRegistryKey(_T("Local AppWizard-Generated Applications"));
    LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)
    // Register the application's document templates.  Document templates
    //  serve as the connection between documents, frame windows and views
    CSingleDocTemplate* pDocTemplate;
    pDocTemplate = new CSingleDocTemplate(
        IDR_MAINFRAME,
        RUNTIME_CLASS(CTracerDoc),
        RUNTIME_CLASS(CMainFrame),       // main SDI frame window
        RUNTIME_CLASS(CTracerView));
    if (!pDocTemplate)
        return FALSE;
    AddDocTemplate(pDocTemplate);


    // Enable DDE Execute open
    EnableShellOpen();
    RegisterShellFileTypes(TRUE);

    // Parse command line for standard shell commands, DDE, file open
    CCommandLineInfo cmdInfo;
    ParseCommandLine(cmdInfo);


    // Dispatch commands specified on the command line.  Will return FALSE if
    // app was launched with /RegServer, /Register, /Unregserver or /Unregister.
    if (!ProcessShellCommand(cmdInfo))
        return FALSE;

    // The one and only window has been initialized, so show and update it
    m_pMainWnd->ShowWindow(SW_SHOW);
    m_pMainWnd->UpdateWindow();
    // call DragAcceptFiles only if there's a suffix
    // In an SDI app, this should occur after ProcessShellCommand
    // Enable drag/drop open
    m_pMainWnd->DragAcceptFiles();
    return TRUE;
}


// App command to run the dialog
void CTracerApp::OnAppAbout()
{
    CAboutDlg aboutDlg;
    aboutDlg.DoModal();
}


// // exit application instance


int CTracerApp::ExitInstance(void)
{
    //Free up the Scintilla DLL
    if ( m_hSciDLL )
        FreeLibrary( m_hSciDLL );

    return CWinApp::ExitInstance();
}

void CTracerApp::OnAppLicense()
{
	// TODO: Add your command handler code here
	CLicenseDlg licenseDlg;
	licenseDlg.DoModal();
}

void CTracerApp::OnRulesImport()
{
	// TODO: Add your command handler code here
}

void CTracerApp::OnRulesExport()
{
	// TODO: Add your command handler code here
}
