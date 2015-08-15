#include "stdafx.h"
#include "../Tracer.h"
#include "TracerView.h"
#include "../Data/DataManager.h"
#include "../Data/CallStack.h"

#include <vector>
#include <string>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/* XPM */
static char *bookmarkBluegem[] =
{
    /* width height num_colors chars_per_pixel */
    "    15    15      64            1",
    /* colors */
    "  c none",
    ". c #0c0630",
    "# c #8c8a8c",
    "a c #244a84",
    "b c #545254",
    "c c #cccecc",
    "d c #949594",
    "e c #346ab4",
    "f c #242644",
    "g c #3c3e3c",
    "h c #6ca6fc",
    "i c #143789",
    "j c #204990",
    "k c #5c8dec",
    "l c #707070",
    "m c #3c82dc",
    "n c #345db4",
    "o c #619df7",
    "p c #acacac",
    "q c #346ad4",
    "r c #1c3264",
    "s c #174091",
    "t c #5482df",
    "u c #4470c4",
    "v c #2450a0",
    "w c #14162c",
    "x c #5c94f6",
    "y c #b7b8b7",
    "z c #646464",
    "A c #3c68b8",
    "B c #7cb8fc",
    "C c #7c7a7c",
    "D c #3462b9",
    "E c #7c7eac",
    "F c #44464c",
    "G c #a4a4a4",
    "H c #24224c",
    "I c #282668",
    "J c #5c5a8c",
    "K c #7c8ebc",
    "L c #dcd7e4",
    "M c #141244",
    "N c #1c2e5c",
    "O c #24327c",
    "P c #4472cc",
    "Q c #6ca2fc",
    "R c #74b2fc",
    "S c #24367c",
    "T c #b4b2c4",
    "U c #403e58",
    "V c #4c7fd6",
    "W c #24428c",
    "X c #747284",
    "Y c #142e7c",
    "Z c #64a2fc",
    "0 c #3c72dc",
    "1 c #bcbebc",
    "2 c #6c6a6c",
    "3 c #848284",
    "4 c #2c5098",
    "5 c #1c1a1c",
    "6 c #243250",
    "7 c #7cbefc",
    "8 c #d4d2d4",
    /* pixels */
    "    yCbgbCy    ",
    "   #zGGyGGz#   ",
    "  #zXTLLLTXz#  ",
    " p5UJEKKKEJU5p ",
    " lfISa444aSIfl ",
    " wIYij444jsYIw ",
    " .OsvnAAAnvsO. ",
    " MWvDuVVVPDvWM ",
    " HsDPVkxxtPDsH ",
    " UiAtxohZxtuiU ",
    " pNnkQRBRhkDNp ",
    " 1FrqoR7Bo0rF1 ",
    " 8GC6aemea6CG8 ",
    "  cG3l2z2l3Gc  ",
    "    1GdddG1    "
};

#define  NB_FOLDER_STATE 7
// Folder types
const int g_MarkersArray[][NB_FOLDER_STATE] =
{
    {SC_MARKNUM_FOLDEROPEN, SC_MARKNUM_FOLDER, SC_MARKNUM_FOLDERSUB, SC_MARKNUM_FOLDERTAIL, SC_MARKNUM_FOLDEREND,        SC_MARKNUM_FOLDEROPENMID,     SC_MARKNUM_FOLDERMIDTAIL},
    {SC_MARK_MINUS,         SC_MARK_PLUS,      SC_MARK_EMPTY,        SC_MARK_EMPTY,         SC_MARK_EMPTY,               SC_MARK_EMPTY,                SC_MARK_EMPTY},
    {SC_MARK_ARROWDOWN,     SC_MARK_ARROW,     SC_MARK_EMPTY,        SC_MARK_EMPTY,         SC_MARK_EMPTY,               SC_MARK_EMPTY,                SC_MARK_EMPTY},
    {SC_MARK_CIRCLEMINUS,   SC_MARK_CIRCLEPLUS,SC_MARK_VLINE,        SC_MARK_LCORNERCURVE,  SC_MARK_CIRCLEPLUSCONNECTED, SC_MARK_CIRCLEMINUSCONNECTED, SC_MARK_TCORNERCURVE},
    {SC_MARK_BOXMINUS,      SC_MARK_BOXPLUS,   SC_MARK_VLINE,        SC_MARK_LCORNER,       SC_MARK_BOXPLUSCONNECTED,    SC_MARK_BOXMINUSCONNECTED,    SC_MARK_TCORNER}
};

const TCHAR cppKeyWords[] =
    _T("and and_eq asm auto bitand bitor bool break ")
    _T("case catch char class compl const const_cast continue ")
    _T("default delete do double dynamic_cast else enum explicit export extern false float for ")
    _T("friend goto if inline int long mutable namespace new not not_eq ")
    _T("operator or or_eq private protected public ")
    _T("register reinterpret_cast return short signed sizeof static static_cast struct switch ")
    _T("template this throw true try typedef typeid typename union unsigned using ")
    _T("virtual void volatile wchar_t while xor xor_eq ");


IMPLEMENT_DYNCREATE(CTracerView, CScintillaView)

BEGIN_MESSAGE_MAP(CTracerView, CScintillaView)
    ON_COMMAND(ID_BM_ADD_BOOKMARK, OnAddBookmark)
    ON_UPDATE_COMMAND_UI(ID_BM_ADD_BOOKMARK, OnUpdateAddBookmark)

    ON_COMMAND(ID_BM_DELETE_BOOKMARK, OnDeleteBookmark)
    ON_UPDATE_COMMAND_UI(ID_BM_DELETE_BOOKMARK, OnUpdateDeleteBookmark)

    ON_COMMAND(ID_BM_NEXT_BOOKMARK, OnNextBookmark)
    ON_COMMAND(ID_BM_PREV_BOOKMARK, OnPrevBookmark)

    ON_COMMAND(ID_VIEW_BOOKMARK_MARGIN, OnBookmarkMargin)
    ON_UPDATE_COMMAND_UI(ID_VIEW_BOOKMARK_MARGIN, OnUpdateViewBookmarkMargin)

    ON_COMMAND(ID_VIEW_LINENUMBERS, OnViewLinenumbers)
    ON_UPDATE_COMMAND_UI(ID_VIEW_LINENUMBERS, OnUpdateViewLinenumbers)

    ON_COMMAND(ID_VIEW_FOLD_MARGIN, OnFoldMargin)
    ON_UPDATE_COMMAND_UI(ID_VIEW_FOLD_MARGIN, OnUpdateViewFoldMargin)

    ON_UPDATE_COMMAND_UI(ID_INDICATOR_LINE, OnUpdateLine)
    ON_UPDATE_COMMAND_UI(ID_INDICATOR_STYLE, OnUpdateStyle)
    ON_UPDATE_COMMAND_UI(ID_INDICATOR_FOLD, OnUpdateFold)
    ON_UPDATE_COMMAND_UI(ID_INDICATOR_OVR, OnUpdateInsert)
    ON_WM_ACTIVATE()
END_MESSAGE_MAP()


CTracerView::CTracerView()
{
    LoadMarginSettings();
}

void CTracerView::OnDraw(CDC* /*pDC*/)
{
    CTracerDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
}

#ifdef _DEBUG
void CTracerView::AssertValid() const
{
    CScintillaView::AssertValid();
}

void CTracerView::Dump(CDumpContext& dc) const
{
    CScintillaView::Dump(dc);
}

CTracerDoc* CTracerView::GetDocument() // non-debug version is inline
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTracerDoc)));
    return static_cast<CTracerDoc*>(m_pDocument);
}
#endif //_DEBUG

void CTracerView::SetAStyle(int style, COLORREF fore, COLORREF back, int size, const char* face)
{
    CScintillaCtrl& rCtrl = GetCtrl();

    rCtrl.StyleSetFore(style, fore);
    rCtrl.StyleSetBack(style, back);
    if (size >= 1)
        rCtrl.StyleSetSize(style, size);
    if (face)
        rCtrl.StyleSetFont(style, face);
}

void CTracerView::DefineMarker(int marker, int markerType, COLORREF fore, COLORREF back)
{
    CScintillaCtrl& rCtrl = GetCtrl();

    rCtrl.MarkerDefine(marker, markerType);
    rCtrl.MarkerSetFore(marker, fore);
    rCtrl.MarkerSetBack(marker, back);
}

void CTracerView::OnInitialUpdate()
{
    //Let the base class do its thing
    CScintillaView::OnInitialUpdate();

    CScintillaCtrl& rCtrl = GetCtrl();

    // Set cpp lexer
    //rCtrl.SetLexer(SCLEX_CPP);
    //rCtrl.SetKeyWords(0, cppKeyWords);

    //Setup styles and set font
    SetAStyle(STYLE_DEFAULT, RGB(0, 0, 0), RGB(0xff, 0xff, 0xff), 10, "Verdana");
    rCtrl.StyleClearAll();

    SetAStyle(SCE_C_DEFAULT, RGB(0, 0, 0));
    SetAStyle(SCE_C_COMMENT, RGB(0, 0x80, 0));
    SetAStyle(SCE_C_COMMENTLINE, RGB(0, 0x80, 0));
    SetAStyle(SCE_C_COMMENTDOC, RGB(0, 0x80, 0));
    SetAStyle(SCE_C_COMMENTLINEDOC, RGB(0, 0x80, 0));
    SetAStyle(SCE_C_COMMENTDOCKEYWORD, RGB(0, 0x80, 0));
    SetAStyle(SCE_C_COMMENTDOCKEYWORDERROR, RGB(0, 0x80, 0));
    SetAStyle(SCE_C_NUMBER, RGB(0, 0x80, 0x80));
    SetAStyle(SCE_C_WORD, RGB(0, 0, 0x80));
    rCtrl.StyleSetBold(SCE_C_WORD, 1);
    SetAStyle(SCE_C_STRING, RGB(0x80, 0, 0x80));
    SetAStyle(SCE_C_IDENTIFIER, RGB(0, 0, 0));
    SetAStyle(SCE_C_PREPROCESSOR, RGB(0x80, 0, 0));
    SetAStyle(SCE_C_OPERATOR, RGB(0x80, 0x80, 0));

    //Setup folding
    // set the folder margin width to 16
    rCtrl.SetMarginWidthN(2, 16);   // set the folder margin width to 16
    rCtrl.SetFoldFlags(SC_FOLDFLAG_LINEAFTER_CONTRACTED, TRUE); //draw line if not expanded
    rCtrl.MarkerSetFore(0, RGB(0x80, 0, 0x80));
    rCtrl.MarkerSetBack(0, RGB(0x80, 0, 0x80));
    rCtrl.SetMarginSensitiveN(2, TRUE);
    rCtrl.SetMarginTypeN(2, SC_MARGIN_SYMBOL);
    rCtrl.SetMarginMaskN(2, SC_MASK_FOLDERS);
    rCtrl.SetProperty(_T("fold"), _T("1"));
    rCtrl.SetProperty(_T("fold.compact"), _T("0"));

    // Add bookmark icons
    rCtrl.MarkerDefinePixmap(0, (char*)bookmarkBluegem); // set marker's icon

    rCtrl.SetSelBack(true, RGB(128,128,196));

    //Setup markers
    rCtrl.SetMarginWidthN(1, 16); // set the marker margin width to 16


    for ( int i = 0; i < NB_FOLDER_STATE; i++ )
    {
        DefineMarker( g_MarkersArray[FOLDER_TYPE][i],
                      g_MarkersArray[FOLDER_STYLE_BOX][i],
                      RGB(0xff, 0xff, 0xff),
                      RGB(0, 0, 0));
    }

    //Setup call tips
    rCtrl.SetMouseDwellTime(1000);

    // Setup wrap mode
    rCtrl.SetWrapMode( SC_WRAP_WORD );

    rCtrl.StyleSetFore( 1, RGB(0,0,196));
    rCtrl.StyleSetBold( 1, 1);

    rCtrl.StyleSetFore( 2, RGB(0,128,128));
    rCtrl.StyleSetBold( 2, 1);

    rCtrl.StyleSetFore( 3, RGB(128,0,64));
    rCtrl.StyleSetBold( 3, 1);

    rCtrl.StyleSetFore( 4, RGB(196,0,128));
    rCtrl.StyleSetBold( 4, 1);

#ifdef _DEBUG
    AfxDump(this);
#endif
}

void CTracerView::OnAddBookmark()
{
    CScintillaCtrl& rCtrl = GetCtrl();
    int nPos = rCtrl.GetCurrentPos();
    int nLine = rCtrl.LineFromPosition(nPos);
    int x = rCtrl.MarkerAdd(nLine, 0);
}

void CTracerView::OnUpdateAddBookmark(CCmdUI* pCmdUI)
{
    CScintillaCtrl& rCtrl = GetCtrl();
    int nPos = rCtrl.GetCurrentPos();
    int nLine = rCtrl.LineFromPosition(nPos);
    int nBits = rCtrl.MarkerGet(nLine);
    pCmdUI->Enable((nBits & 0x1) == 0);
}

void CTracerView::OnDeleteBookmark()
{
    CScintillaCtrl& rCtrl = GetCtrl();
    int nPos = rCtrl.GetCurrentPos();
    int nLine = rCtrl.LineFromPosition(nPos);
    rCtrl.MarkerDelete(nLine, 0);
}

void CTracerView::OnUpdateDeleteBookmark(CCmdUI* pCmdUI)
{
    CScintillaCtrl& rCtrl = GetCtrl();
    int nPos = rCtrl.GetCurrentPos();
    int nLine = rCtrl.LineFromPosition(nPos);
    int nBits = rCtrl.MarkerGet(nLine);
    pCmdUI->Enable(nBits & 0x1);
}

void CTracerView::OnNextBookmark()
{
    CScintillaCtrl& rCtrl = GetCtrl();
    int nPos = rCtrl.GetCurrentPos();
    int nLine = rCtrl.LineFromPosition(nPos);
    int nFoundLine = rCtrl.MarkerNext(nLine + 1, 0x1);
    if (nFoundLine >= 0)
        rCtrl.GotoLine(nFoundLine);
    else
        MessageBeep(MB_ICONHAND);
}

void CTracerView::OnPrevBookmark()
{
    CScintillaCtrl& rCtrl = GetCtrl();
    int nPos = rCtrl.GetCurrentPos();
    int nLine = rCtrl.LineFromPosition(nPos);
    int nFoundLine = rCtrl.MarkerPrevious(nLine - 1, 0x1);
    if (nFoundLine >= 0)
        rCtrl.GotoLine(nFoundLine);
    else
        MessageBeep(MB_ICONHAND);
}

void CTracerView::OnFoldMargin()
{
    CScintillaCtrl& rCtrl = GetCtrl();
    int nMarginWidth = rCtrl.GetMarginWidthN(2);
    if (nMarginWidth)
        rCtrl.SetMarginWidthN(2, 0);
    else
        rCtrl.SetMarginWidthN(2, 16);
}


void CTracerView::OnUpdateViewFoldMargin(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(GetCtrl().GetMarginWidthN(2) != 0);
}

void CTracerView::OnBookmarkMargin()
{
    CScintillaCtrl& rCtrl = GetCtrl();
    int nMarginWidth = rCtrl.GetMarginWidthN(1);
    if (nMarginWidth)
        rCtrl.SetMarginWidthN(1, 0);
    else
        rCtrl.SetMarginWidthN(1, 16);
}

void CTracerView::OnUpdateViewBookmarkMargin(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(GetCtrl().GetMarginWidthN(1) != 0);
}

void CTracerView::OnViewLinenumbers()
{
    CScintillaCtrl& rCtrl = GetCtrl();
    int nMarginWidth = rCtrl.GetMarginWidthN(0);
    if (nMarginWidth)
        rCtrl.SetMarginWidthN(0, 0);
    else
        rCtrl.SetMarginWidthN(0, 32);
}

void CTracerView::OnUpdateViewLinenumbers(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(GetCtrl().GetMarginWidthN(0) != 0);
}

void CTracerView::OnUpdateInsert(CCmdUI* pCmdUI)
{
    CString sText;
    if (GetCtrl().GetOvertype())
        sText.LoadString(ID_INDICATOR_OVR);
    pCmdUI->SetText(sText);
}

void CTracerView::OnUpdateStyle(CCmdUI* pCmdUI)
{
    CScintillaCtrl& rCtrl = GetCtrl();
    int nPos = rCtrl.GetCurrentPos();
    int nStyle = rCtrl.GetStyleAt(nPos);
    CString sLine;
    sLine.Format(IDS_STYLE_INDICATOR, nStyle);
    pCmdUI->SetText(sLine);
}

void CTracerView::OnUpdateFold(CCmdUI* pCmdUI)
{
    CScintillaCtrl& rCtrl = GetCtrl();
    int nPos = rCtrl.GetCurrentPos();
    int nLine = rCtrl.LineFromPosition(nPos);
    int nLevel = rCtrl.GetFoldLevel(nLine) & SC_FOLDLEVELNUMBERMASK;
    nLevel -= 1024;

    CString sLine;
    sLine.Format(IDS_FOLD_INDICATOR, nLevel);
    pCmdUI->SetText(sLine);
}

void CTracerView::OnUpdateLine(CCmdUI* pCmdUI)
{
    CScintillaCtrl& rCtrl = GetCtrl();
    int nPos = rCtrl.GetCurrentPos();
    int nLine = rCtrl.LineFromPosition(nPos);
    int nColumn = rCtrl.GetColumn(nPos);

    CString sLine;
    sLine.Format(IDS_LINE_INDICATOR, nLine, nColumn, nPos);
    pCmdUI->SetText(sLine);
}

//Some simple examples of implementing auto completion
void CTracerView::OnCharAdded(SCNotification* /*pSCNotification*/)
{
    CScintillaCtrl& rCtrl = GetCtrl();

    //Get the previous 13 characters and if it is "scintilla is " case insensitive
    //then display a list which allows "very cool", "easy" or the "way cool!!"
    int nStartSel = rCtrl.GetSelectionStart();
    int nEndSel = rCtrl.GetSelectionEnd();
    if ((nStartSel == nEndSel) && (nStartSel >= 13))
    {
        TextRange tr;
        tr.chrg.cpMin = nStartSel - 13;
        tr.chrg.cpMax = nEndSel;
        char sText[14];
        tr.lpstrText = sText;
        rCtrl.GetTextRange(&tr);

        //Display the auto completion list
        if (_strcmpi(sText, "scintilla is ") == 0)
        {
            //Display the auto completion list
            rCtrl.AutoCShow(0, _T("very cool\neasy to use\nway cool!!"));
        }
    }

    //As another example, get the previous 2 characters and if it is "res" case sensitive
    //then display a list which allows "resize", "restart" and "restore"
    if ((nStartSel == nEndSel) && (nStartSel >= 3))
    {
        TextRange tr;
        tr.chrg.cpMin = nStartSel - 3;
        tr.chrg.cpMax = nEndSel;
        char sText[4];
        tr.lpstrText = sText;
        rCtrl.GetTextRange(&tr);

        //Display the auto completion list
        if (strcmp(sText, "res") == 0)
        {
            //Display the auto completion list
            rCtrl.AutoCShow(3, _T("resize\nrestart\nrestore"));
        }
    }
}

//A simple example of call tips
void CTracerView::OnDwellStart(SCNotification* pSCNotification)
{
    CScintillaCtrl& rCtrl = GetCtrl();

    //Only display the call tip if the scintilla window has focus
    CWnd* pFocusWnd = GetFocus();
    if (pFocusWnd && (pFocusWnd->GetSafeHwnd() == rCtrl.GetSafeHwnd()))
    {
        //Get the previous 7 characters and next 7 characters arround
        //the current dwell position and if it is "author " case insensitive
        //then display "PJ Naughter" as a call tip
        TextRange tr;
        tr.chrg.cpMin = max(0, pSCNotification->position - 7);
        tr.chrg.cpMax = min(pSCNotification->position + 7, rCtrl.GetLength());
        char sText[15];
        memset(sText, 0, sizeof(sText));
        tr.lpstrText = sText;
        rCtrl.GetTextRange(&tr);

        //Display the call tip
#if (_MSC_VER >= 1400)
        _strupr_s(sText, sizeof(sText));
#else
        _strupr(sText);
#endif
        if (strstr(sText, "AUTHOR "))
            rCtrl.CallTipShow(pSCNotification->position, _T("PJ Naughter"));
    }
}

void CTracerView::OnDwellEnd(SCNotification* /*pSCNotification*/)
{
    CScintillaCtrl& rCtrl = GetCtrl();

    //Cancel any outstanding call tip
    if (rCtrl.CallTipActive())
        rCtrl.CallTipCancel();
}

void CTracerView::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
    //Let the base class do its thing
    CScintillaView::OnActivate(nState, pWndOther, bMinimized);

    CScintillaCtrl& rCtrl = GetCtrl();

    //Cancel any outstanding call tip
    if (nState == WA_INACTIVE && rCtrl.CallTipActive())
        rCtrl.CallTipCancel();
}

void CTracerView::OnModifyAttemptRO(SCNotification* /*pSCNotification*/)
{
    if (AfxMessageBox(IDP_ALLOW_MODIFY_READONLY_FILE, MB_YESNO) == IDYES)
        GetCtrl().SetReadOnly(FALSE);
}

void CTracerView::OnUpdate(CView* pSender , LPARAM lHint , CObject* pHint)
{
    CTracerDoc *pDoc = static_cast<CTracerDoc *>(GetDocument());
    CDataManager * rawData = pDoc->GetRawDataMgr();

    CScintillaCtrl& rCtrl = GetCtrl();
    rCtrl.ClearAll();

    for ( int i = 0; i < rawData->GetLineCount(); i++ )
    {
        string str = rawData->GetLine( i );
        rCtrl.AppendText(  static_cast<int>( str.length() ), str.c_str() );
    }
}

// Update view
void CTracerView::UpdateView( int aThreadId )
{
    CTracerDoc *pDoc = static_cast<CTracerDoc *>(GetDocument());
    CDataManager * rawData = pDoc->GetRawDataMgr();
    const CCallStack * pStack = pDoc->BuildCallStack( aThreadId );

    CScintillaCtrl& rCtrl = GetCtrl();
    rCtrl.ClearAll();

    for ( int i = 0; i < rawData->GetLineCount(); i++ )
    {
        string str = rawData->GetLine( i );
        rCtrl.AppendText(  static_cast<int>( str.length() ), str.c_str() );
    }

    int padding = 0;
    int level = 0;
    for ( int i = 0; i < pStack->Count(); i++ )
    {
        const CStackItem *pItem = pStack->StackItem( i );
        string str = rawData->GetLine( i );
        LPCTSTR name = pDoc->GetFunctionName(i);
        int pos = str.find(name) + rCtrl.PositionFromLine(i);
        rCtrl.StartStyling( pos, 31 );

        if ( pItem->m_nLevel != 0 )
        {
            level = pItem->m_nLevel;
        }

        int high = level >> 16;
        int low = level & 0xFFFF;
        if ( high > low )
        {
            level |= SC_FOLDLEVELHEADERFLAG;

            // High light function or method.
            rCtrl.SetStyling( strlen(name), 1 ); 
        }
        else if ( high < low )
        {
            level |= SC_FOLDLEVELWHITEFLAG;

            // High light function or method.
            rCtrl.SetStyling( strlen(name), 2 ); 
        }
        else
        {
            // High light function or method.
            if ( pItem->m_LinePair &&pItem->m_LinePair->first == -1 )
            {
                rCtrl.SetStyling( strlen(name), 3 ); 
            }
            if ( pItem->m_LinePair &&pItem->m_LinePair->second == -1 )
            {
                rCtrl.SetStyling( strlen(name), 4 ); 
            }            

            rCtrl.MarkerAdd( i, 0 );
        }
        
        if ( level != 0 )
            rCtrl.SetFoldLevel( i, level );
    }
}

int CTracerView::GetLineNum()
{
    CScintillaCtrl& rCtrl = GetCtrl();
    int line = rCtrl.LineFromPosition(rCtrl.GetCurrentPos());
    return line;
}

void CTracerView::FocusLine( int focusedline )
{
    CScintillaCtrl& rCtrl = GetCtrl();
    rCtrl.GotoLine(focusedline);
    rCtrl.SetSel(rCtrl.PositionFromLine(focusedline), rCtrl.GetLineEndPosition(focusedline));
}

