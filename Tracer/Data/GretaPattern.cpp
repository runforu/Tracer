#include "StdAfx.h"
#include "GretaPattern.h"
#include <string>
using namespace regex;

CGretaBackRef::~CGretaBackRef()
{
}

const std::string CGretaBackRef::String() const
{
    return m_BackRef.str();
}

CGretaBackRef::operator bool() const 
{
    return m_BackRef;
}

CGretaResult::~CGretaResult()
{
    std::vector<IBackRef *>::iterator iter = m_AllBackRef.begin();
    while ( iter != m_AllBackRef.end() )
    {
        delete *iter;
        ++iter;
    }
}

const IBackRef & CGretaResult::BackRef( int n )
{
    return *m_AllBackRef[n];
}

const std::vector<IBackRef *> & CGretaResult::AllBackRef()
{
    return m_AllBackRef;
}

unsigned int CGretaResult::BackRefCount()
{
    return m_AllBackRef.size();
}

void CGretaResult::AppendBackRef( IBackRef * aBackRef )
{
    return m_AllBackRef.push_back( aBackRef );
}


// From Interface
CGretaPattern::~CGretaPattern()
{
}

// From IPattern
bool CGretaPattern::Match( const char * aTarget, IResult *& aResult )
{
    return this->Match( std::string(aTarget), aResult );
}

bool CGretaPattern::Match( const std::string & aTarget, IResult *& aResult )
{
    match_results  results;
    CGretaResult *pResults = new CGretaResult();
    rpattern::backref_type br = this->match( aTarget, results );

    if ( br )
    {
        rpattern::backref_vector brVector = results.all_backrefs();
        rpattern::backref_vector::iterator iter = brVector.begin();
        while ( iter != brVector.end() )
        {
            if ( *iter ) // Use operator bool_type() cast, it is a must
            {
                pResults->AppendBackRef( new CGretaBackRef ( *iter ) );
            }
            ++iter;
        }
        aResult = pResults;
    }
    else
    {
        delete pResults;
    }

    return br;
}

PatternId CGretaPattern::GetId() const
{
    return m_Id;
}

bool CGretaPattern::IsTwin( const IPattern & aPattern )
{
    PatternId id = aPattern.GetId();
    return ( m_Id.m_High == id.m_High ) 
        && ( m_Id.m_Low != id.m_Low );
}

int CGretaPattern::GroupCount() const
{
    return this->cgroups();
}