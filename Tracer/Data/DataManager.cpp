#include "stdafx.h"
#include "DataManager.h"

using namespace std;

CDataManager *CDataManager::m_Instance = NULL;

CDataManager *CDataManager::NewDataManager()
{
    if ( m_Instance )
    {
        return m_Instance;
    }

    return new CDataManager;
}

void CDataManager::Externalize( CObjWriter& writer ) const
{
    writer<<m_LogLines.size();
    vector<string>::const_iterator it = m_LogLines.begin();
    while ( it != m_LogLines.end() )
    {
        writer<<*it;
        it++;
    }
}

void CDataManager::Internalize( CObjReader& reader )
{
    vector<string>::size_type count;
    reader>>count;

    for ( size_t i = 0; i < count; i++ )
    {
        string str;
        reader>>str;
        m_LogLines.push_back( str );
    }
}

void CDataManager::Trim( int aFrom, int aTo )
{  
    vector<string>::iterator it = m_LogLines.begin();
    while ( it != m_LogLines.end() )
    {
        (*it).erase( aFrom, aTo - aFrom );
    }
}



