// RuleConfig.cpp : implementation file
//

#include "stdafx.h"
#include "../Tracer.h"
#include "RuleConfig.h"

char buf[1024];

// RuleConfig dialog

IMPLEMENT_DYNAMIC(CRuleConfigDlg, CDialog)

CRuleConfigDlg::CRuleConfigDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CRuleConfigDlg::IDD, pParent)
    , m_strEnterFunctionExp(_T(""))
    , m_ctrlRuleList()
    , m_strExitFunctionExp(_T(""))
    , m_nRuleCount(0)
    , m_strThreadIdExp(_T(""))
{
}

CRuleConfigDlg::~CRuleConfigDlg()
{
    m_ctrlRuleList.DestroyWindow();
    m_ctrlThreadIdExpList.DestroyWindow();
}

void CRuleConfigDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text( pDX, IDC_EDIT_NEW_THREAD_EXP, m_strThreadIdExp );
    DDX_Text( pDX, IDC_EDIT_ENTER_FUNCTION_EXP, m_strEnterFunctionExp );
    DDX_Text( pDX, IDC_EDIT_EXIT_FUNCTION_EXP, m_strExitFunctionExp );
    DDX_Control( pDX, IDC_RULES_LIST,  m_ctrlRuleList);
    DDX_Control( pDX, IDC_THREAD_EXP_LIST,  m_ctrlThreadIdExpList);
}


BEGIN_MESSAGE_MAP(CRuleConfigDlg, CDialog)
    ON_BN_CLICKED(IDC_ADD_NEW_RULE, &CRuleConfigDlg::OnBnClickedAddNewRule)
    ON_BN_CLICKED(IDC_REMOVE_RULES, &CRuleConfigDlg::OnBnClickedRemoveRule)
    ON_BN_CLICKED(IDC_ADD_THREAD_EXP, &CRuleConfigDlg::OnBnClickedAddThreadExp)
    ON_BN_CLICKED(IDC_REMOVE_THREAD_EXP, &CRuleConfigDlg::OnBnClickedRemoveThreadExp)
    ON_NOTIFY_EX( TTN_NEEDTEXT, 0, UpdateTipText )
END_MESSAGE_MAP()


// RuleConfig message handlers
BOOL CRuleConfigDlg::OnInitDialog(void)
{   
    CDialog::OnInitDialog();

    EnableToolTips(TRUE); 

    m_ctrlRuleList.SetExtendedStyle(m_ctrlRuleList.GetExtendedStyle() 
        | LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES | LVS_EX_GRIDLINES );

    m_ctrlThreadIdExpList.SetExtendedStyle(m_ctrlThreadIdExpList.GetExtendedStyle() 
        | LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES | LVS_EX_GRIDLINES );

    m_ctrlRuleList.InsertColumn( 0, _T("No."), LVCFMT_LEFT, 48 );
    m_ctrlRuleList.InsertColumn( 1, _T("Enter function Exp"), LVCFMT_LEFT, 160 );
    m_ctrlRuleList.InsertColumn( 2, _T("Exit Function Exp"), LVCFMT_LEFT, 160 );

    m_ctrlThreadIdExpList.SetExtendedStyle(m_ctrlThreadIdExpList.GetExtendedStyle() 
        | LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES | LVS_EX_GRIDLINES );

    m_ctrlThreadIdExpList.InsertColumn( 0, _T("No."), LVCFMT_LEFT, 48 );
    m_ctrlThreadIdExpList.InsertColumn( 1, _T("Thread ID Exp"), LVCFMT_LEFT, 160 );

    // TODO: Read xml file and import current settings.
    
    char buf1[8];
    sprintf_s( buf1, 8, "%.4d", 1 );
    int nRow = m_ctrlRuleList.InsertItem( 1, buf1 );
    m_ctrlRuleList.SetItemText( nRow, 1, "-->%FUNC_NAME" );
    m_ctrlRuleList.SetItemText( nRow, 2, "<--%FUNC_NAME" );
    m_ctrlRuleList.SetCheck( nRow );

    return 0;
}

void CRuleConfigDlg::OnBnClickedAddNewRule()
{
    UpdateData();

    if ( m_strEnterFunctionExp.GetLength() == 0
        || m_strExitFunctionExp.GetLength() == 0 )
    {
        return;
    }

    int num = m_ctrlRuleList.GetItemCount();
    char buf[8];
    sprintf_s( buf, 8, "%.4d", num+1 );
    int nRow = m_ctrlRuleList.InsertItem( num+1, buf );
    m_ctrlRuleList.SetItemText( nRow, 1, m_strEnterFunctionExp );
    m_ctrlRuleList.SetItemText( nRow, 2, m_strExitFunctionExp );
    m_ctrlRuleList.SetCheck( nRow );
}

void CRuleConfigDlg::OnBnClickedRemoveRule()
{
    UINT uSelectedCount = m_ctrlRuleList.GetSelectedCount();
    int nItem;
    // Update all of the selected items.
    if (uSelectedCount > 0)
    {
        for ( int i=0; i < m_ctrlRuleList.GetItemCount(); i++)
        {
            nItem = m_ctrlRuleList.GetNextItem( -1, LVNI_SELECTED );
            m_ctrlRuleList.DeleteItem(nItem); 
        }
        for ( int i=0; i < m_ctrlRuleList.GetItemCount(); i++ )
        {
            char buf[8];
            sprintf_s( buf, 8, "%.4d", i+1 );
            m_ctrlRuleList.SetItemText( i, 0, buf );
        }
    }
}

void CRuleConfigDlg::OnOK(void)
{
    m_ctrlRuleList.SetParent( NULL );
    m_ctrlThreadIdExpList.SetParent( NULL );
    m_nRuleCount = m_ctrlRuleList.GetItemCount();
    m_nThreadIdExpCount = m_ctrlThreadIdExpList.GetItemCount();
    CDialog::OnOK();
}

void CRuleConfigDlg::OnCancel(void)
{
    m_nRuleCount = 0;
    CDialog::OnCancel();
}


BOOL CRuleConfigDlg::PreTranslateMessage(MSG* pMsg)
{
    //m_ctrlToolTip.RelayEvent( pMsg );
    return CDialog::PreTranslateMessage( pMsg );
}


void CRuleConfigDlg::OnBnClickedAddThreadExp()
{
    UpdateData();

    if ( m_strThreadIdExp.GetLength() == 0 )
    {
        return;
    }

    int num = m_ctrlThreadIdExpList.GetItemCount();
    char buf[8];
    sprintf_s( buf, 8, "%.4d", num+1 );
    int nRow = m_ctrlThreadIdExpList.InsertItem( num+1, buf );
    m_ctrlThreadIdExpList.SetItemText( nRow, 1, m_strThreadIdExp );
}

void CRuleConfigDlg::OnBnClickedRemoveThreadExp()
{
    UINT uSelectedCount = m_ctrlThreadIdExpList.GetSelectedCount();
    int nItem;
    // Update all of the selected items.
    if (uSelectedCount > 0)
    {
        for ( int i=0; i < m_ctrlThreadIdExpList.GetItemCount(); i++)
        {
            nItem = m_ctrlThreadIdExpList.GetNextItem( -1, LVNI_SELECTED );
            m_ctrlThreadIdExpList.DeleteItem(nItem); 
        }
        for ( int i=0; i < m_ctrlThreadIdExpList.GetItemCount(); i++ )
        {
            char buf[8];
            sprintf_s( buf, 8, "%.4d", i+1 );
            m_ctrlThreadIdExpList.SetItemText( i, 0, buf );
        }
    }
}

BOOL CRuleConfigDlg::UpdateTipText(UINT id, NMHDR * pTTTStruct, LRESULT* pResult)
{
    CToolTipCtrl* pToolTip = AfxGetModuleState()->m_thread.GetDataNA()->m_pToolTip;
    pToolTip->SetMargin( CRect(2,0,2,0) );
    pToolTip->SetMaxTipWidth( 400 );    
    pToolTip->SetTipTextColor( RGB( 0, 80, 255 ) );
        
    TOOLTIPTEXT *pTTT = ( TOOLTIPTEXT * )pTTTStruct;
    UINT nID =pTTTStruct->idFrom;

    if (pTTT->uFlags & TTF_IDISHWND)
    {
        nID = ::GetDlgCtrlID( (HWND)nID );
        switch ( nID )
        {
        case IDC_EDIT_ENTER_FUNCTION_EXP:
        case IDC_EDIT_EXIT_FUNCTION_EXP:
            {
                TCHAR buf[256];
                _tcscpy_s( buf, 256, FUNC_NAME_EDITOR_TIPS );
                _tcscat_s( buf, 256, FUNC_NAME_EXP_EXAMPLE );
                sprintf_s( m_TipsBuffer, TIPS_BUFFER_SIZE, buf, FUNC_NAME_MACRO, FUNC_NAME_MACRO, FUNC_NAME_MACRO );
                pTTT->lpszText = m_TipsBuffer;

            }
            return TRUE;
        default:
            return FALSE;
        }
    }

    return FALSE;
}