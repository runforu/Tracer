#pragma once

#include "../stdafx.h"
#include "../resource.h"

// CLicenseDlg dialog

class CLicenseDlg : public CDialog
{
	//DECLARE_DYNAMIC(CLicenseDlg)

public:
	CLicenseDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	enum { IDD = IDD_LICENSE_NOTE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
