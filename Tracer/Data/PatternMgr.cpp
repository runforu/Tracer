#include "StdAfx.h"
#include "PatternMgr.h"
#include "IPatternCollection.h"
#include "PatternLoader.h"

unsigned short CPatternMgr::PatternIdHigh = 1;


CPatternMgr::CPatternMgr(void)
{
    m_pThreadPatterns = IPatternCollection::PatternCollection();
    m_pGuarderPatterns = IPatternCollection::PatternCollection();
}

CPatternMgr::~CPatternMgr(void)
{
    delete m_pThreadPatterns;
    delete m_pGuarderPatterns;
}

IPatternCollection * CPatternMgr::ThreadPatternCollection(void)
{
    return m_pThreadPatterns;
}

IPatternCollection * CPatternMgr::GuarderPatternCollection(void)
{
    return m_pGuarderPatterns;
}

void CPatternMgr::BuildPatternCollections( CPatternLoader & aLoader)
{
    unsigned int guarderCount = aLoader.FuncGuarderCount();
    unsigned int threadExpCount = aLoader.ThreadIdExpCount();

    for ( unsigned int i = 0; i < threadExpCount; i++)
    { 
        CThreadIdExp threadIdExp = aLoader.GetThreadIdExp( i );
        if ( threadIdExp.IsEnabled() )
        {
            PatternId id = UniqueId();
            IPattern * pPat = IPattern::Pattern( threadIdExp.GetThreadIdExp(), id );
            m_pThreadPatterns->Append( pPat );
        }
    } 

    for ( unsigned int j = 0; j < guarderCount; j++)
    {
        CRouteGuarder rule = aLoader.GetGuarder( j );
        if ( rule.IsEnabled() )
        {
            const FunctionGuarder &fe = rule.GetValue();
            PatternId enter, exit;
            TwinId( enter, exit );
            IPattern * pEnter = IPattern::Pattern( fe.first, enter );
            IPattern * pExit = IPattern::Pattern( fe.second, exit );
            m_pGuarderPatterns->Append( pEnter );
            m_pGuarderPatterns->Append( pExit );
        }
    }
}

PatternId CPatternMgr::UniqueId()
{
    return PatternId::PatternId( PatternIdHigh++, THREAD_ID_LOW );
}

void CPatternMgr::TwinId( PatternId &enter, PatternId & exit )
{
    enter.m_High = PatternIdHigh++;
    enter.m_Low = GUARDER_ENTER_PATTERN_LOW;
    exit.m_High = enter.m_High;
    exit.m_Low = GUARDER_EXIT_PATTERN_LOW;
}

bool CPatternMgr::Parse( const std::string &str, MatchResult & aResult, EParseType aType )
{
    if ( EPARSE_FUNCTION_NAME == aType )
    {
        return m_pGuarderPatterns->Match( str, aResult );
    }
    else
    {
        return m_pThreadPatterns->Match( str, aResult );
    }
}

bool CPatternMgr::IsTwinId( const PatternId aId1, const PatternId aId2)
{
    return ( ( aId1.m_High == aId2.m_High ) && ( aId1.m_Low != aId2.m_Low ) );        
}

