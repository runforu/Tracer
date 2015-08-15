// MainFrm.h : interface of the CMainFrame class
//


#pragma once
#include "afxext.h"
#include "afxcmn.h"
#include "scbarcf.h"
#include "DockPageBar.h"
#include "AutoHideBar.h"
#include <vector>


#define TREECTRL_BEGIN_ID 200
#define TREECTRL_END_ID 300

class CThreadTreeInfo
{
public:
    CThreadTreeInfo( int aId = 0, CTreeCtrl* aTreeCtrl = NULL )
        : m_ThreadId( aId ), m_Tree( aTreeCtrl )
    {    
    }

public:
    int m_ThreadId;
    CTreeCtrl* m_Tree;
};



class CMainFrame : public CAutoHideFrame
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

private:
	afx_msg void OnViewTreeBar();
	afx_msg void OnUpdateViewTreeBar(CCmdUI *pCmdUI);
	int CreateDockingBar(void);
	afx_msg void OnViewOutputBar();
	afx_msg void OnUpdateViewOutputBar(CCmdUI *pCmdUI);
	afx_msg void OnRulesConfig();
	afx_msg void OnRulesImport();
    afx_msg void OnRulesRun();
    afx_msg void OnTreeClicked( UINT id, NMHDR * pNMHDR, LRESULT * result );

private:
    bool CreateTree( int aThreadId );

private:
	CEdit	m_wndEdit;
    std::vector<CThreadTreeInfo> m_ThreadTrees;
	CDockPageBar m_wndTreeDockPageBar;
	CDockPageBar m_wndOutputDockPageBar;
    afx_msg void OnDestroy(void);
private:
    // The Active thread tree's Id
    int m_ActiveThreadId;
};


