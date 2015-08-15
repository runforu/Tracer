// TracerDoc.h : interface of the CTracerDoc class
//

#ifndef _TRACERDOC_H_
#define _TRACERDOC_H_

#include <vector>

class CDataManager;
class CLineStyleMgr;
class CPatternMgr;
class CParsedDataMgr;
class CCallStack;
class CPatternLoader;

class CTracerDoc : public CDocument
{
protected: // create from serialization only
    CTracerDoc();  
    DECLARE_DYNCREATE(CTracerDoc)
    DECLARE_MESSAGE_MAP()

public:
    virtual BOOL OnNewDocument();

    virtual ~CTracerDoc();

    CDataManager * CTracerDoc::GetRawDataMgr()
    {
        return m_RawDataMgr;
    }   

    CLineStyleMgr * CTracerDoc::GetLinePropertyMgr()
    {
        return m_LineStyleMgr;
    }   

    BOOL OnOpenDocument( LPCTSTR lpszPathName );
    
    BOOL OnSaveDocument( LPCTSTR lpszPathName );

    bool IsTrcFile( LPCTSTR lpszPathName );
    const CCallStack * BuildCallStack( int threadId = -1 );

    void LoadPatterns( CPatternLoader & aLoader );
    const std::vector<int> & ThreadIds();

    LPCTSTR GetFunctionName( int aIndex );

private:
    BOOL HandleRawDataChanged();
    BOOL HandlePatternChanged();
    BOOL IsDocumentChanged();
    void ClearCallStacks();

private:
    CDataManager *m_RawDataMgr;
    
    CLineStyleMgr *m_LineStyleMgr;
    
    // Builds the line elements
    int ParseThreadId(void);
    CPatternMgr * m_pPatternMgr;

    CParsedDataMgr * m_pParsedDataMgr;
    std::vector<CCallStack *> m_CallStacks;

    // Flag to raw data changing.
    bool m_bRawDataDirty;
    // Flag to pattern changing.
    bool m_bPatternsChanged;
};

#endif //_TRACERDOC_H_
