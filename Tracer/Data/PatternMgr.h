#ifndef _PATTERNMGR_H_
#define _PATTERNMGR_H_

#include "IRegExp.h"

class IPatternCollection;
class CPatternLoader;
struct MatchResult;

class CPatternMgr
{
    enum EParseType
    {
        EPARSE_THREAD_ID,
        EPARSE_FUNCTION_NAME,
    };
public:
    CPatternMgr(void);
    ~CPatternMgr(void);
    
    IPatternCollection * ThreadPatternCollection(void);
    IPatternCollection * GuarderPatternCollection(void);
    void BuildPatternCollections( CPatternLoader & aLoader);
    bool Parse( const std::string &str, MatchResult & aResult, EParseType aType );
    static bool IsTwinId( const PatternId aId1, const PatternId aId2);
    static bool IsEnterGuarder( const PatternId aId )
    {
        return aId.m_Low == GUARDER_ENTER_PATTERN_LOW;
    }
    static bool IsExitGuarder( const PatternId aId )
    {
        return aId.m_Low == GUARDER_EXIT_PATTERN_LOW;
    }

private:
    PatternId UniqueId();

    void TwinId( PatternId &enter, PatternId & exit );

    IPatternCollection * m_pThreadPatterns;
    IPatternCollection * m_pGuarderPatterns;
    
    static unsigned short PatternIdHigh;
    static const unsigned short GUARDER_ENTER_PATTERN_LOW = 0;
    static const unsigned short GUARDER_EXIT_PATTERN_LOW = 0xffff;
    static const unsigned short THREAD_ID_LOW = 0x1010;
};

#endif // _PATTERNMGR_H_

