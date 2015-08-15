#pragma once

#include "IRegExp.h"
#include "../imports/regexpr2.h"


class CGretaBackRef : public IBackRef
{
public:
    // From Interface
    ~CGretaBackRef();
    CGretaBackRef()
    {}

    CGretaBackRef( const CGretaBackRef & br ) : m_BackRef( br.m_BackRef )
    {}

    CGretaBackRef( const regex::rpattern::backref_type & br ) : m_BackRef( br )
    {}

    // From IBackRef
    const std::string String() const;
    operator bool() const;

private:
    regex::rpattern::backref_type m_BackRef;
};

class CGretaResult : public IResult
{
public:
    // From Interface
    ~CGretaResult();

    // From IResult
    const IBackRef & BackRef( int n );
    const std::vector<IBackRef *> & AllBackRef();
    unsigned int BackRefCount();
    void AppendBackRef( IBackRef * aBackRef );

private:    
    std::vector<IBackRef *> m_AllBackRef;
};

class CGretaPattern : public IPattern, regex::rpattern
{
public:
    CGretaPattern():m_Id( PatternId::Null() )
    {
    }
    CGretaPattern( const char *pat, PatternId aId )
        : regex::rpattern ( std::string( pat ) ), m_Id( aId )
    {}

    CGretaPattern( const std::string & pat, PatternId aId )
        :regex::rpattern ( pat ), m_Id( aId )
    {}

    CGretaPattern( const CGretaPattern & pat, PatternId aId )
        : regex::rpattern ( pat ), m_Id( aId )
    {}

    // From Interface
    ~CGretaPattern(void);

    // From IPattern
    bool Match( const char * aTarget, IResult *& aResult );
    bool Match( const std::string & aTarget, IResult *& aResult );
    PatternId GetId() const;
    bool IsTwin( const IPattern & aPattern );
    int GroupCount() const;

private:
    PatternId m_Id;
};
