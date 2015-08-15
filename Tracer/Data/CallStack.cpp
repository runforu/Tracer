#include "StdAfx.h"
#include "CallStack.h"
#include "ParsedDataMgr.h"
#include "PatternMgr.h"

using namespace std;

#define LEVEL_BASE 1024

CCallStack::~CCallStack()
{
    std::vector<CStackItem>::iterator iter = m_Stack.begin();
    while ( iter != m_Stack.end() )
    {        
        CStackItem* pItem = &(*iter);
        if ( pItem->m_LinePair )
        { 
            //Note: the enter and exit guarder share same line pair.
            if ( -1 != pItem->m_LinePair->first && -1 != pItem->m_LinePair->second )
            {
                m_Stack[ pItem->m_LinePair->second ].m_LinePair = NULL;
            }
            delete pItem->m_LinePair; 
        }

        ++iter;
    }
}

void CCallStack::BuildCallStack( CParsedDataMgr *pParsedDataMgr )
{
    int level = LEVEL_BASE;
    vector<int> stack;

    int cnt = pParsedDataMgr->GetParsedDataCount();
    m_Stack.assign( cnt, CStackItem() );

    for ( int line =0; line < cnt; line++ )
    {
        CParsedData parsedData = pParsedDataMgr->GetParsedData( line );
        if ( CParsedData::EVALID_PARSED_DATA == parsedData.m_MaskCode )
        {
            if ( CPatternMgr::IsEnterGuarder( parsedData.m_GuarderPatternId ) )
            {
                stack.push_back( line );
                level++;
            }
            else
            {
                int count = stack.size();
                int cursor = count - 1;     
                for( ; cursor >= 0; cursor-- )
                {
                    int tmpLine = stack[cursor];
                    CParsedData top = pParsedDataMgr->GetParsedData( tmpLine );
                    if ( top.m_FunctionName == parsedData.m_FunctionName
                        && top.m_ThreadId == parsedData.m_ThreadId 
                        && CPatternMgr::IsTwinId( top.m_GuarderPatternId, parsedData.m_GuarderPatternId ) )
                    {
                        // No exit guarder matches the enter guarder.
                        // Handle the none-matched enter guarder.
                        for ( int m = count - 1; m > cursor; m-- ) 
                        {
                            CStackItem &item = m_Stack[ stack[m] ];
                            item.m_LinePair = new LinePair( stack[m], -1 ); 
                            item.m_nLevel = level | level << 16;
                        }
                        // The enter guarder matches the exit guarder
                        CStackItem &item1 = m_Stack[ tmpLine ];
                        item1.m_nLevel = ( level-1 ) | level << 16;                        
                        CStackItem &item2 = m_Stack[line];
                        item2.m_nLevel = level | ( level-1 ) << 16 ;                           
                        item2.m_LinePair = item1.m_LinePair = new LinePair( tmpLine, line ); 
                        --level;
                        stack.erase( stack.begin() + cursor, stack.end() );
                        break;
                    }
                } // for( ; cursor >= 0; cursor-- )

                // No enter guarder matches the exit guarder.
                if ( cursor == -1 )
                {
                    // Badly, the exit guarder is stray, 
                    // Some log information in the head missed.
                    CStackItem &item = m_Stack[line];
                    item.m_LinePair = new LinePair( -1, line );  
                    item.m_nLevel = level | level << 16; ; 
                }
            } 
        } // if ( parsedData.m_PatternId & PARSED_DATA_VALID_MASK )
    } // for ( int line =0; line < cnt; line++ )
#ifdef _DEBUG
    DumpElements();
#endif
}