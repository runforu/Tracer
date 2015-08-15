// ImportRulesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../Tracer.h"
#include "ImportRulesDlg.h"


// CImportRulesDlg dialog

IMPLEMENT_DYNAMIC(CImportRulesDlg, CDialog)

CImportRulesDlg::CImportRulesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImportRulesDlg::IDD, pParent)
	, m_strFilePath(_T(""))
{

}

CImportRulesDlg::~CImportRulesDlg()
{
}

void CImportRulesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text( pDX, IDC_FILE_PATH, m_strFilePath );
	DDX_Control( pDX, IDC_RULES_LIST, m_ctrlRuleList );
    DDX_Control( pDX, IDC_THREAD_EXP_LIST, m_ctrlThreadIdExpList );
}

// RuleConfig message handlers
BOOL CImportRulesDlg::OnInitDialog(void)
{	
	CDialog::OnInitDialog();

	m_ctrlRuleList.SetExtendedStyle(m_ctrlRuleList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES| LVS_EX_GRIDLINES );

	m_ctrlRuleList.InsertColumn( 0, _T("No."), LVCFMT_LEFT, 48 );
	m_ctrlRuleList.InsertColumn( 1, _T("Enter function Exp"), LVCFMT_LEFT, 160 );
	m_ctrlRuleList.InsertColumn( 2, _T("Exit Function Exp"), LVCFMT_LEFT, 160 );

	m_ctrlThreadIdExpList.SetExtendedStyle(m_ctrlRuleList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES| LVS_EX_GRIDLINES );

	m_ctrlThreadIdExpList.InsertColumn( 0, _T("No."), LVCFMT_LEFT, 48 );
	m_ctrlThreadIdExpList.InsertColumn( 1, _T("Thread ID Exp"), LVCFMT_LEFT, 320 );

	return 0;
}

BEGIN_MESSAGE_MAP(CImportRulesDlg, CDialog)
	ON_BN_CLICKED(IDC_OPEN_IMPORT_FILE, &CImportRulesDlg::OnBnClickedOpenImportFile)
END_MESSAGE_MAP()


// CImportRulesDlg message handlers
void CImportRulesDlg::OnBnClickedOpenImportFile()
{

	CFileDialog fDlg( TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Xml Files (*.xml)|*.xml", NULL, 0 );
	fDlg.DoModal();
	m_strFilePath = fDlg.GetPathName();
	// TODO: import file
    UpdateData( FALSE );
}
