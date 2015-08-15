// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "../Tracer.h"

#include "MainFrm.h"
#include "RuleConfig.h"
#include "ImportRulesDlg.h"
#include "../Data/PatternLoader.h"
#include "../Data/TracerDoc.h"
#include "../Data/CallStack.h"
#include "TracerView.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CAutoHideFrame)

BEGIN_MESSAGE_MAP(CMainFrame, CAutoHideFrame)
    ON_WM_CREATE()
    ON_WM_DESTROY()
    ON_COMMAND(ID_VIEW_TREE_BAR, &CMainFrame::OnViewTreeBar)
    ON_UPDATE_COMMAND_UI(ID_VIEW_TREE_BAR, &CMainFrame::OnUpdateViewTreeBar)
    ON_COMMAND(ID_VIEW_REPORT_BAR, &CMainFrame::OnViewOutputBar)
    ON_UPDATE_COMMAND_UI(ID_VIEW_REPORT_BAR, &CMainFrame::OnUpdateViewOutputBar)
    ON_COMMAND(ID_RULES_CONFIG, &CMainFrame::OnRulesConfig)
    ON_COMMAND(ID_RULES_IMPORT, &CMainFrame::OnRulesImport)
    ON_COMMAND(ID_RULES_RUN, &CMainFrame::OnRulesRun)
    ON_NOTIFY_RANGE(NM_CLICK, TREECTRL_BEGIN_ID, TREECTRL_END_ID, &CMainFrame::OnTreeClicked)
END_MESSAGE_MAP()

static UINT indicators[] =
{
    ID_SEPARATOR,           // status line indicator
    ID_INDICATOR_CAPS,
    ID_INDICATOR_NUM,
    ID_INDICATOR_SCRL,
};


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
: m_ActiveThreadId(-1)
{
    // TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
    for ( int pos = 0; pos < m_ThreadTrees.size(); pos++ )
    {
        delete m_ThreadTrees[pos].m_Tree;
        m_ThreadTrees[pos].m_Tree = NULL;
    }
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
                               | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
            !m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
    {
        TRACE0("Failed to create toolbar\n");
        return -1;      // fail to create
    }

    if (!m_wndStatusBar.Create(this) ||
            !m_wndStatusBar.SetIndicators(indicators,
                                          sizeof(indicators)/sizeof(UINT)))
    {
        TRACE0("Failed to create status bar\n");
        return -1;      // fail to create
    }

    // TODO: Delete these three lines if you don't want the toolbar to be dockable
    //m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
    //DockControlBar(&m_wndToolBar);
    EnableDocking(CBRS_ALIGN_ANY);

    return CreateDockingBar();
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    if ( !CFrameWnd::PreCreateWindow(cs) )
        return FALSE;
    // TODO: Modify the Window class or styles here by modifying
    //  the CREATESTRUCT cs

    return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
    CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
    CFrameWnd::Dump(dc);
}

#endif //_DEBUG


int CMainFrame::CreateDockingBar(void)
{
    // Create left dock bar
    if ( !m_wndTreeDockPageBar.Create( _T( "DockPageBar" ), this, CSize( 200, 200 ), TRUE, 123 ) )
    {
        TRACE0( "Failed to create Tree DockPageBar.\n" );
        return -1;
    }

    m_wndTreeDockPageBar.SetBarStyle( m_wndTreeDockPageBar.GetBarStyle() |
                                      CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC );
    m_wndTreeDockPageBar.EnableDocking (CBRS_ALIGN_ANY);

    // Create output dock bar
    if (!m_wndOutputDockPageBar.Create(_T("DockPageBar"), this, CSize(200,200),TRUE,1234))
    {
        TRACE0("Failed to create Output DockPageBar.\n");
        return -1;
    }

    m_wndOutputDockPageBar.SetBarStyle(m_wndOutputDockPageBar.GetBarStyle() |
                                       CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
    m_wndOutputDockPageBar.EnableDocking (CBRS_ALIGN_ANY);

    EnableDocking( CBRS_ALIGN_ANY );
    DockControlBar( &m_wndTreeDockPageBar, AFX_IDW_DOCKBAR_LEFT );
    DockControlBar( &m_wndOutputDockPageBar, AFX_IDW_DOCKBAR_BOTTOM );

    if ( !m_wndEdit.Create( WS_CHILD|WS_VISIBLE| ES_MULTILINE | ES_WANTRETURN | ES_AUTOVSCROLL,
                           CRect( 0, 0, 0, 0 ), this, 123 ) )
    {
        TRACE0("Failed to create edit control.\n");
        return -1;
    }
    m_wndOutputDockPageBar.AddPage( &m_wndEdit,"Output", IDI_OUTPUT_ICON );

    ShowControlBar( &m_wndOutputDockPageBar, FALSE,  FALSE );

    return 0;
}

// CMainFrame message handlers

void CMainFrame::OnViewTreeBar()
{
    ShowControlBar( &m_wndTreeDockPageBar, !m_wndTreeDockPageBar.IsVisible(), FALSE );
}

void CMainFrame::OnUpdateViewTreeBar(CCmdUI *pCmdUI)
{
    pCmdUI->SetCheck( m_wndTreeDockPageBar.IsVisible() );
}

void CMainFrame::OnViewOutputBar()
{
    ShowControlBar( &m_wndOutputDockPageBar, !m_wndOutputDockPageBar.IsVisible(), FALSE );
}

void CMainFrame::OnUpdateViewOutputBar(CCmdUI *pCmdUI)
{
    pCmdUI->SetCheck( m_wndOutputDockPageBar.IsVisible() );
}

void CMainFrame::OnRulesConfig()
{
    CRuleConfigDlg dlg;
    INT_PTR nResult = dlg.DoModal();
    switch( nResult )
    {
    case IDOK:
        // Retrieves the dialog data.
        {
            CTracerDoc* pDoc = ((CTracerDoc*)GetActiveDocument());
            CPatternLoader patternLoader;
            for ( int i = 0; i < dlg.GetRulesCount(); i++ )
            {
                CRouteGuarder rule;
                dlg.GetGuarder( i, rule);
                patternLoader.AppendGuarder( rule );
            }

            for ( int i = 0; i < dlg.GetThreadIdExpCount(); i++ )
            {
                CThreadIdExp exp;
                dlg.GetThreadIdExp( i, exp);
                patternLoader.AppendThreadIdExp( exp );
            }   
            pDoc->LoadPatterns( patternLoader );
        }
        break;
    default:
        break;
    }
}

void CMainFrame::OnRulesImport()
{
    CImportRulesDlg dlg;
    dlg.DoModal();
}

void CMainFrame::OnRulesRun()
{
    CTracerDoc* pDoc = static_cast<CTracerDoc*>( GetActiveDocument() );
    const std::vector<int> & threadIds = pDoc->ThreadIds();

    for ( int i = 0; i < threadIds.size(); i++ )
    {
        CreateTree( threadIds[i] );
    }
    CTracerView *pView = static_cast<CTracerView *>( GetActiveView() );
    pView->UpdateView( m_ActiveThreadId ); 
}

void CMainFrame::OnTreeClicked( UINT id, NMHDR * pNMHDR, LRESULT * result )
{
    NM_TREEVIEW* pTreeView = reinterpret_cast<NM_TREEVIEW*>( pNMHDR );
    CTreeCtrl *pTree = m_ThreadTrees[id - TREECTRL_BEGIN_ID].m_Tree; 
    ASSERT(pTreeView->hdr.hwndFrom == pTree->m_hWnd );
    HTREEITEM hItem = pTree->HitTest(pTreeView->ptDrag);

    if (hItem==NULL)
    {
        return;
    }
    LinePair*lp = reinterpret_cast<LinePair*>(pTree->GetItemData(hItem));
    if (!lp)
    {
        return;
    }
    CTracerView *pView = static_cast<CTracerView *>( GetActiveView() );

    if (lp->first ==-1)
    {
        pView->FocusLine( lp->second );
    }
    else if (lp->second ==-1)
    {
        pView->FocusLine( lp->first );
    }
    else if ( pView->GetLineNum() == lp->first )
    {
        pView->FocusLine( lp->second );
    }
    else
    {
        pView->FocusLine( lp->first );
    }
}

bool CMainFrame::CreateTree( int aThreadId )
{
    CTracerDoc* pDoc = static_cast<CTracerDoc*>( GetActiveDocument() );
    const CCallStack * pStack = pDoc->BuildCallStack( aThreadId );
    CTreeCtrl *pTree = NULL;

    int pos; 
    for ( pos = 0; pos < m_ThreadTrees.size(); pos++ )
    {
        if ( aThreadId == m_ThreadTrees[pos].m_ThreadId )
        {
            break;
        }
    }

    if ( pos == m_ThreadTrees.size() )
    {    
        pTree = new CTreeCtrl;
        CThreadTreeInfo info( aThreadId, pTree );
        m_ThreadTrees.push_back( info );    
   
        // Create "Thread X" tree.
        if (!pTree->Create( WS_CHILD|WS_VISIBLE | TVS_HASLINES
            | TVS_HASBUTTONS | TVS_LINESATROOT,
            CRect( 0, 0, 0, 0 ), this, TREECTRL_BEGIN_ID + pos) )
        {
            TRACE0("Failed to create tree control.\n");
            return false;
        }
        char szBuf[32];
        if ( aThreadId == -1 )
        {
            sprintf_s( szBuf, 32, "All threads");
        }
        else
        {
            sprintf_s( szBuf, 32, "Thread: 0x%8X");
        }
        m_wndTreeDockPageBar.AddPage( pTree, szBuf, IDI_OUTPUT_ICON );    
    }
    else
    {
        pTree = m_ThreadTrees[pos].m_Tree;
        pTree->DeleteAllItems();
    } 

    // Create root tree item.
    HTREEITEM hItem = pTree->InsertItem( "root" );
    pTree->SetItemData( hItem, NULL ); 
    
    for ( int i = 0; i < pStack->Count(); i++ )
    {
        const CStackItem * pItem = pStack->StackItem( i );
        if ( CStackItem::IsNullStackItem( *pItem ) )
        {
            continue;
        }

        // No Exit
        if ( pItem->m_LinePair  && -1 == pItem->m_LinePair->second )
        {
            HTREEITEM tmp = pTree->InsertItem( pDoc->GetFunctionName( i ), hItem );
            pTree->SetItemData( tmp, reinterpret_cast<DWORD_PTR>( pItem->m_LinePair ) ); 
            continue;
        }

        // No enter
        if ( pItem->m_LinePair && -1 == pItem->m_LinePair->first )
        {
            HTREEITEM tmp = pTree->InsertItem( pDoc->GetFunctionName( i ), hItem );
            pTree->SetItemData( tmp, reinterpret_cast<DWORD_PTR>( pItem->m_LinePair ) ); 
            continue;
        }

        // Here high not equal low
        int high = pItem->m_nLevel >> 16;
        int low = pItem->m_nLevel & 0xFFFF;
        if ( high > low )
        {            
            while ( 1 )
            {
                LinePair* lines = reinterpret_cast<LinePair*>( pTree->GetItemData( hItem ) );
                if ( lines && ( -1 == lines->first || -1 == lines->second ) )
                {
                    hItem = pTree->GetParentItem( hItem );
                    continue;
                }

                hItem = pTree->InsertItem( pDoc->GetFunctionName( i ), hItem );
                pTree->SetItemData( hItem, reinterpret_cast<DWORD_PTR>( pItem->m_LinePair ) ); 
                break;
            }
        }
        else
        {            
            LinePair* lines = reinterpret_cast<LinePair*>( pTree->GetItemData( hItem ) );
            while( lines )
            {
                if ( lines == pItem->m_LinePair)
                {
                    hItem = pTree->GetParentItem( hItem );
                    break;
                }                
                hItem = pTree->GetParentItem( hItem );
                lines = reinterpret_cast<LinePair*>( pTree->GetItemData( hItem ) );
            }
        }
    }

    return true;
}

void CMainFrame::OnDestroy()
{
    CAutoHideFrame::OnDestroy();
}
