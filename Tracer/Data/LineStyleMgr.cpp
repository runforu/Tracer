#include "StdAfx.h"
#include "LineStyleMgr.h"
#include "LineStyle.h"
#include "DataManager.h"

using namespace std;

CLineStyleMgr::CLineStyleMgr( CDataManager *aRawDataMgr )
{
    m_RawDataMgr = aRawDataMgr;
}

CLineStyleMgr::~CLineStyleMgr(void)
{
}

void CLineStyleMgr::Externalize( CObjWriter& writer ) const
{
    writer<<m_LineStyleSet.size();
    vector<CLineStyle>::const_iterator it = m_LineStyleSet.begin();
    while ( it != m_LineStyleSet.end() )
    {
        writer<<(*it);
        it++;
    }

    writer<<m_LineStyleMap.size();
    vector<int>::const_iterator iter = m_LineStyleMap.begin();
    while ( iter != m_LineStyleMap.end() )
    {
        writer<<(*iter);
        iter++;
    }
}

void CLineStyleMgr::Internalize( CObjReader& reader )
{
    vector<CLineStyle>::size_type count;
    reader>>count;

    for ( size_t i = 0; i < count; i++ )
    {
        CLineStyle  lp;
        reader>>lp;
        m_LineStyleSet.push_back( lp );
    }

    reader>>count;
    for ( size_t i = 0; i < count; i++ )
    {
        int  index;
        reader>>index;
        m_LineStyleMap.push_back( index );
    }
}

CLineStyle* CLineStyleMgr::GetLineStyle( int aLineNum )
{
    return NULL;
}


