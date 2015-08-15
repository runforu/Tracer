#include "StdAfx.h"
#include "ParsedDataMgr.h"
#include "IPatternCollection.h"
#include "PatternMgr.h"
using namespace regex;
using namespace std;

bool CParsedDataMgr::BuildParsedDataQueue( CPatternMgr * aPatternMgr )
{
    IPatternCollection* pGuarderPatterns = aPatternMgr->GuarderPatternCollection();
    IPatternCollection* pThreadPatterns = aPatternMgr->ThreadPatternCollection();

    if ( 0 == pGuarderPatterns->Count() )
    {
        return false;
    }

    CDataManager *dataMgr = CDataManager::NewDataManager();
    unsigned int dataCount = dataMgr->GetLineCount();
    if ( 0 == dataCount )
    {
        return false;
    }

    for ( unsigned int i = 0; i < dataCount; i++ )
    {
        std::string line =  dataMgr->GetLine( i );
        CParsedData parsedData;

        MatchResult result1;
        if ( pThreadPatterns->Match( line, result1 ) )
        {
            const IBackRef &group1 = result1.m_Result->BackRef( 1 );
            // Test the group 1 if valid
            if ( group1 )
            {
                parsedData.m_ThreadId = atoi( group1.String().c_str() );
                parsedData.m_ThreadPatternId = result1.m_PatternId;
                m_ThreadIds.push_back( parsedData.m_ThreadId );
            }
        }

        MatchResult result2;
        if ( pGuarderPatterns->Match( line, result2 ) )
        {
            const IBackRef &group1 = result2.m_Result->BackRef( 1 );
            // Test the group 1 if valid                
            if ( group1 )
            {
                parsedData.m_GuarderPatternId = result2.m_PatternId;  
                parsedData.m_FunctionName = group1.String();
                parsedData.m_MaskCode = CParsedData::EVALID_PARSED_DATA;
            }    
        }

        AppendParsedData( parsedData );
    }

    return true;
}
