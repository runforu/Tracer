#pragma once
#include "afxcmn.h"


// CImportRulesDlg dialog

class CImportRulesDlg : public CDialog
{
	// Dialog Data
	enum { IDD = IDD_IMPORT_RULES };

	DECLARE_DYNAMIC(CImportRulesDlg)

public:
	CImportRulesDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CImportRulesDlg();

	afx_msg void OnBnClickedOpenImportFile();
	BOOL OnInitDialog(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	// import rules file path
	CString m_strFilePath;  
    CListCtrl m_ctrlThreadIdExpList;
	CListCtrl m_ctrlRuleList;
};
