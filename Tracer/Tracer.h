// Tracer.h : main header file for the Tracer application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CTracerApp:
// See Tracer.cpp for the implementation of this class
//

class CTracerApp : public CWinApp
{
public:
	CTracerApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
public:
    // // exit application instance
    int ExitInstance(void);

    // scintilla dll
    HINSTANCE m_hSciDLL;

public:
	afx_msg void OnAppLicense();
public:
	afx_msg void OnRulesImport();
public:
	afx_msg void OnRulesExport();
};

extern CTracerApp theApp;