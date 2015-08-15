#pragma once

#include "ScintillaView.h"
#include "../Data/TracerDoc.h"

class CTracerView : public CScintillaView
{
protected: // create from serialization only
    CTracerView();
    DECLARE_DYNCREATE(CTracerView)

// Attributes
public:
    CTracerDoc* GetDocument();

    virtual void OnDraw(CDC* pDC);  // overridden to draw this view
    virtual void OnInitialUpdate();

#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    afx_msg void OnAddBookmark();
    afx_msg void OnDeleteBookmark();
    afx_msg void OnUpdateDeleteBookmark(CCmdUI* pCmdUI);
    afx_msg void OnNextBookmark();
    afx_msg void OnPrevBookmark();
    afx_msg void OnFoldMargin();
    afx_msg void OnBookmarkMargin();
    afx_msg void OnUpdateViewBookmarkMargin(CCmdUI* pCmdUI);
    afx_msg void OnViewLinenumbers();
    afx_msg void OnUpdateViewLinenumbers(CCmdUI* pCmdUI);
    afx_msg void OnUpdateAddBookmark(CCmdUI* pCmdUI);
    afx_msg void OnUpdateViewFoldMargin(CCmdUI* pCmdUI);
    afx_msg void OnUpdateLine(CCmdUI* pCmdUI);
    afx_msg void OnUpdateStyle(CCmdUI* pCmdUI);
    afx_msg void OnUpdateFold(CCmdUI* pCmdUI);
    afx_msg void OnUpdateInsert(CCmdUI* pCmdUI);
    afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
    DECLARE_MESSAGE_MAP()

    virtual void OnCharAdded(SCNotification* pSCNotification);
    virtual void OnDwellStart(SCNotification* pSCNotification);
    virtual void OnDwellEnd(SCNotification* pSCNotification);
    virtual void OnModifyAttemptRO(SCNotification* pSCNotification);

    void SetAStyle(int style, COLORREF fore, COLORREF back=RGB(0xff, 0xff, 0xff), int size=-1, const char *face=0);
    void DefineMarker(int marker, int markerType, COLORREF fore, COLORREF back);
public:
	void OnUpdate(CView* pSender , LPARAM lHint , CObject* pHint);
public:
    // // Update view
    void UpdateView( int aThreadId );
    int GetLineNum();
    void FocusLine( int focusedline );
};

#ifndef _DEBUG  // debug version in ScintillaDemoView.cpp
inline CTracerDoc* CTracerView::GetDocument()
{
    return (CTracerDoc*)m_pDocument;
}
#endif
