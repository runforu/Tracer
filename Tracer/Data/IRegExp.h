#ifndef _PATTERN_H_
#define _PATTERN_H_

#include <string>
#include <vector>
#include "Interface.h"

// A twin pattern id's high 16-bit is the same and low 16-bit is not the same.
class PatternId
{
public:    
    explicit inline PatternId( unsigned short aHigh = 0, unsigned short aLow = 0 )
        : m_High( aHigh ), m_Low( aLow )
    {        
    }

    static inline PatternId Null()
    {
        return PatternId();
    }
    
    bool operator == ( const PatternId & aId )
    {
        return ( m_High == aId.m_High && m_Low == aId.m_Low );
    }

public:
    unsigned short m_High;
    unsigned short m_Low;
};

class IBackRef : public Interface
{
public:
    virtual const std::string String() const = 0;
    virtual operator bool() const = 0 ;
};

class IResult : public Interface
{
public:
    virtual const IBackRef & BackRef( int n ) = 0;
    virtual const std::vector<IBackRef *> & AllBackRef() = 0;
    virtual unsigned int BackRefCount() = 0;

private:
    // AppendBackRef is exposed only for IPattern
    virtual void AppendBackRef( IBackRef * aBackRef ) = 0;
};


class IPattern : public Interface
{
public:
    static IPattern * Pattern( const std::string & pat, PatternId aId );
    static IPattern * Pattern( const char * pat, PatternId aId );
    // aResult should be unchanged if no match.
    virtual bool Match( const char * aTarget, IResult *& aResult ) = 0;
    virtual bool Match( const std::string & aTarget, IResult *& aResult ) = 0;
    virtual PatternId GetId() const = 0;
    virtual bool IsTwin( const IPattern & aPattern ) = 0;
    virtual int GroupCount() const = 0;
};

#endif // _PATTERN_H_