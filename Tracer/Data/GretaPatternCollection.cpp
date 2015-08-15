#include "StdAfx.h"
#include "GretaPatternCollection.h"
#include "GretaPattern.h"

using namespace std;

CGretaPatternCollection::~CGretaPatternCollection(void)
{
    Clear();
}

bool CGretaPatternCollection::Match( const std::string &str, MatchResult & aResult )
{
    int count = m_Patterns.size();
    if ( 0 == count )
    {
        return false;
    }

    IResult *pResult = NULL;
    for ( int i = 0; i < count; i++ )
    {
        if ( m_Patterns[i]->Match( str, pResult ) )
        {
            aResult.m_PatternId = m_Patterns[i]->GetId();
            // IResult is delete by MatchResult.
            aResult.m_Result = pResult;
            return true;
        }
    }
    delete pResult;
    return false;
}

void CGretaPatternCollection::Append( IPattern* aPattern )
{
    m_Patterns.push_back( aPattern );
}

void CGretaPatternCollection::Remove( int aIndex )
{
    ASSERT( aIndex < 0 || aIndex >= m_Patterns.size() );
    IPattern *p = m_Patterns[aIndex];
    m_Patterns[aIndex] = m_Patterns.back();
    m_Patterns.pop_back();
}

const IPattern * CGretaPatternCollection::GetPattern( int aIndex )
{
    return m_Patterns[aIndex];
}

void CGretaPatternCollection::Clear()
{
    std::vector<IPattern *>::iterator iter = m_Patterns.begin();
    while ( iter != m_Patterns.end() )
    {
        delete (*iter++);
    }
    m_Patterns.clear();
}

unsigned int CGretaPatternCollection::Count() const
{
    return static_cast<unsigned int>( m_Patterns.size() );
}

const IPattern * CGretaPatternCollection::FindPattern( PatternId aId )
{
    std::vector<IPattern *>::iterator iter = m_Patterns.begin();
    while ( iter != m_Patterns.end() )
    {
        if ( (*iter)->GetId() == aId )
        {
            return *iter;
        }
    }
}
