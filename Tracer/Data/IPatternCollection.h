#ifndef _IPATTERNCOLLECTION_H_
#define _IPATTERNCOLLECTION_H_

#include <string>
#include "Interface.h"
#include "IRegExp.h"

struct MatchResult
{
    // Which pattern is used for the result. 
    PatternId m_PatternId;
    IResult * m_Result;

    MatchResult() : m_PatternId( PatternId::Null() ), m_Result( NULL )
    {
    }

    // The m_Result is created by new and brought out through MatchResult.
    ~MatchResult()
    {
        delete m_Result;
    }
};

class IPatternCollection : public Interface
{
public:
    static IPatternCollection * PatternCollection();
    virtual bool Match( const std::string &str, MatchResult & aResult ) = 0;
    virtual void Append( IPattern* aPattern ) = 0;
    virtual void Remove( int aIndex ) = 0;
    virtual const IPattern * GetPattern( int aIndex ) = 0;
    virtual const IPattern * FindPattern( PatternId aId ) = 0;
    virtual void Clear() = 0;
    virtual unsigned int Count() const = 0;
};

#endif // _IPATTERNCOLLECTION_H_