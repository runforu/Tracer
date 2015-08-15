#ifndef _RULECONFIG_H_
#define  _RULECONFIG_H_

#include "afxcmn.h"
#include "../Data/PatternLoader.h"
#include <vector>
#include "afxwin.h"

#define TIPS_BUFFER_SIZE 1024

// RuleConfig dialog

class CRuleConfigDlg : public CDialog
{
    // Dialog Data
    enum { IDD = IDD_RULES_CONFIE };

    DECLARE_DYNAMIC(CRuleConfigDlg)

public:
    CRuleConfigDlg(CWnd* pParent = NULL);   // standard constructor
    virtual ~CRuleConfigDlg();

    BOOL OnInitDialog(void);

    afx_msg void OnBnClickedAddNewRule();
    afx_msg void OnBnClickedRemoveRule();
    afx_msg void OnBnClickedAddThreadExp();
    afx_msg void OnBnClickedRemoveThreadExp();
    BOOL UpdateTipText(UINT id, NMHDR * pTTTStruct, LRESULT* pResult);
    void OnOK(void);
    void OnCancel(void);

    // Retrieve the count of rules.
    int GetRulesCount(void)
    {
        return m_nRuleCount = m_ctrlRuleList.GetItemCount();
    }

    int GetGuarder(int aIndex, CRouteGuarder& rule)
    {
        if ( aIndex < 0 || aIndex >= m_nRuleCount )
        {
            return ERR_NOT_FOUND;
        }

        LVITEM item;
        char szBuf[512];
        char szText[512];
        item.iItem = aIndex;
        item.cchTextMax = 1024;
        item.mask = LVIF_TEXT;
        item.pszText = szBuf;       
        item.iSubItem = 1;
        m_ctrlRuleList.GetItem( &item);
        item.iSubItem = 2;
        item.pszText = szText;
        m_ctrlRuleList.GetItem( &item);
        rule.SetValue( szBuf, szText );
        rule.Enable( m_ctrlRuleList.GetCheck( aIndex ) );        
        return ERR_NONE;
    }


    // Retrieve the count of rules.
    int GetThreadIdExpCount(void)
    {
        return m_nThreadIdExpCount = m_ctrlThreadIdExpList.GetItemCount();
    }

    int GetThreadIdExp(int aIndex, CThreadIdExp& exp)
    {
        if ( aIndex < 0 || aIndex >= m_nThreadIdExpCount )
        {
            return ERR_NOT_FOUND;
        }

        LVITEM item;
        char szBuf[512];
        item.iItem = aIndex;
        item.cchTextMax = 1024;
        item.mask = LVIF_TEXT;
        item.pszText = szBuf;       
        item.iSubItem = 1;
        m_ctrlThreadIdExpList.GetItem( &item);
        exp.SetThreadIdExp( item.pszText );
        exp.Enable( m_ctrlThreadIdExpList.GetCheck( aIndex ) );
        return ERR_NONE;
    }


protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP()

private:
    // List of current rules.
    CListCtrl m_ctrlRuleList;

    // List of current thread id exp.
    CListCtrl m_ctrlThreadIdExpList;

    // Thread expression.
    CString m_strThreadIdExp;

    // Enter function expression.
    CString m_strEnterFunctionExp;

    // Exit function expression.
    CString m_strExitFunctionExp;

    // Count of rules
    int m_nRuleCount;
    int m_nThreadIdExpCount;

    TCHAR m_TipsBuffer[TIPS_BUFFER_SIZE];

    //CToolTipCtrl m_ctrlToolTip;

    BOOL PreTranslateMessage(MSG* pMsg);    
};

#endif  //_RULECONFIG_H_
