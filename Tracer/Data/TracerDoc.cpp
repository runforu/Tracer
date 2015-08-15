// TracerDoc.cpp : implementation of the CTracerDoc class
//

#include "stdafx.h"
#include "../Tracer.h"

#include "TracerDoc.h"
#include "ObjStream.h"
#include "ParsedDataMgr.h"
#include "DataManager.h"
#include "PatternLoader.h"
#include <regexpr2.h>
#include <vector>
#include <string>
#include "CallStack.h"
#include "PatternMgr.h"

using namespace std;
using namespace regex;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// "TRAC"
#define  TRACER_FILE_ID 0x43415254

#define  TRACER_VERSION  1

using namespace std;
struct FILE_HEADER_ID
{
    int m_nVersion;
    int m_nId;
};

// CTracerDoc

IMPLEMENT_DYNCREATE(CTracerDoc, CDocument)

BEGIN_MESSAGE_MAP(CTracerDoc, CDocument)
END_MESSAGE_MAP()

// CTracerDoc construction/destruction
CTracerDoc::CTracerDoc(): m_bRawDataDirty( false ), m_bPatternsChanged( false )
{
    m_RawDataMgr = CDataManager::NewDataManager();
    m_pPatternMgr = new CPatternMgr();
    m_pParsedDataMgr = new CParsedDataMgr();
}

CTracerDoc::~CTracerDoc()
{
    delete m_RawDataMgr;
    delete m_pPatternMgr;
    delete m_pParsedDataMgr;
    ClearCallStacks();
}

BOOL CTracerDoc::OnNewDocument()
{
    if ( !CDocument::OnNewDocument() )
        return FALSE;

    //  TODO: add reinitialization code here
    // (SDI documents will reuse this document)

    return TRUE;
}

// CTracerDoc commands
BOOL CTracerDoc::OnOpenDocument( LPCTSTR lpszPathName )
{
    // CDocument::OnOpenDocument( lpszPathName );    
    ifstream ifile( lpszPathName,  ios::in | ios::binary );
    if( !ifile )
    {
        return false;
    }
    
    CObjReader or( &ifile ); 
    m_RawDataMgr->Clear();
    
    if ( IsTrcFile( lpszPathName ) )
    {
        if ( !ifile )
        {
            return false;   
        }

        FILE_HEADER_ID fileheader;
        or>> fileheader.m_nVersion >>fileheader.m_nId;

        m_RawDataMgr->Internalize( or );
    }
    else
    {
        string str;
        while ( getline( ifile, str ) )
        {
            m_RawDataMgr->m_LogLines.push_back( str );
        }
        CDocument::SetModifiedFlag( TRUE );        
    }
    
    if ( !m_RawDataMgr->m_LogLines.empty() )
    {
        UpdateAllViews( NULL );
    }
    or.Bind( NULL );
    ifile.close();    
    
    HandleRawDataChanged();

    return true;
}

BOOL CTracerDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
    ofstream ofile( lpszPathName,  ios::out | ios::binary  );

    if ( !ofile )
    {
        return false;   
    }

    CObjWriter ow( &ofile );
    ow<<TRACER_VERSION<<TRACER_FILE_ID;

    m_RawDataMgr->Externalize( ow );

    ow.Flush();   
    ow.Bind( NULL );
    ofile.close();
    return true;
}


bool CTracerDoc::IsTrcFile( LPCTSTR lpszPathName )
{
    bool rt = false;

    ifstream ifile( lpszPathName,  ios::in | ios::binary );

    if ( !ifile )
    {
        return false;   
    }

    CObjReader or( &ifile );
    FILE_HEADER_ID fileheader = { 0, 0 };
    or >> fileheader.m_nVersion >>fileheader.m_nId;
    if ( TRACER_FILE_ID == fileheader.m_nId )
    {
        rt = true;
    }
    or.Bind( NULL );
    ifile.close();
    return rt;
}

const CCallStack * CTracerDoc::BuildCallStack( int threadId )
{    
    vector<CCallStack *>::iterator iter = m_CallStacks.begin();
    while ( iter != m_CallStacks.end() )
    {
        if ( threadId == (*iter)->ThreadIdFilter() )
        {
            return *iter;
        }
        ++iter;
    }

    CCallStack * pCallStack = new CCallStack( threadId );
    m_CallStacks.push_back( pCallStack );

    pCallStack->BuildCallStack( m_pParsedDataMgr );

    return pCallStack;
}

void CTracerDoc::LoadPatterns( CPatternLoader & aLoader )
{
    m_pPatternMgr->BuildPatternCollections( aLoader );
    HandlePatternChanged();
}

const std::vector<int> & CTracerDoc::ThreadIds()
{
    return m_pParsedDataMgr->GetThreadIds();
}

BOOL CTracerDoc::HandleRawDataChanged()
{
    ClearCallStacks();
    bool rt = m_pParsedDataMgr->BuildParsedDataQueue( m_pPatternMgr );
    //UpdateUi();
    return rt;
}

BOOL CTracerDoc::HandlePatternChanged()
{
    ClearCallStacks();
    bool rt = m_pParsedDataMgr->BuildParsedDataQueue( m_pPatternMgr );
    //UpdateUi();
    return rt;
}

void CTracerDoc::ClearCallStacks()
{
    vector<CCallStack *>::iterator iter = m_CallStacks.begin();
    while ( iter != m_CallStacks.end() )
    {
        delete (*iter); 
        ++iter;
    }
    m_CallStacks.clear();
}


BOOL CTracerDoc::IsDocumentChanged()
{  
    return FALSE;
}


LPCTSTR CTracerDoc::GetFunctionName( int aIndex )
{  
    return m_pParsedDataMgr->GetParsedData( aIndex ).m_FunctionName.c_str();
}

