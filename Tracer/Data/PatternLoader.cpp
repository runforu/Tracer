#include "StdAfx.h"
#include "PatternLoader.h"


void CPatternLoader::AppendGuarder( CRouteGuarder &rule )
{
    std::string::size_type pos = rule.m_FuncExp.first.find( FUNC_NAME_MACRO );
    if ( pos != -1 )
    {
        size_t len = strlen( FUNC_NAME_MACRO );
        rule.m_FuncExp.first.replace( pos, len, FUNC_NAME_REGULAR_EXP);
        rule.m_FuncExp.second.replace( pos, len, FUNC_NAME_REGULAR_EXP);
    }
    m_Rules.push_back( rule );
}

void CPatternLoader::RemoveGuarder( int index )
{
    for ( unsigned int i = index+1; i < m_Rules.size(); i++)
    {
        m_Rules[i - 1] = m_Rules[i];
    }
    m_Rules.pop_back();
}

void CPatternLoader::RemoveAll()
{
    m_Rules.clear();
    m_ThreadIDExp.clear();
}

void CPatternLoader::AppendThreadIdExp( CThreadIdExp& exp )
{
    m_ThreadIDExp.push_back( exp );
}

const CThreadIdExp& CPatternLoader::GetThreadIdExp( int index )
{
    return m_ThreadIDExp[index];
}

void CPatternLoader::RemoveThreadIdExp( int index )
{
    for ( unsigned int i = index+1; i < m_ThreadIDExp.size(); i++)
    {
        m_ThreadIDExp[i - 1] = m_ThreadIDExp[i];
    }
    m_ThreadIDExp.pop_back();
}

