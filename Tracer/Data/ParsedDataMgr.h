#ifndef _PARSEDDATAMGR_H_
#define _PARSEDDATAMGR_H_

#include <string>
#include <vector>
#include "IRegExp.h"
#include "DataManager.h"
#include "../imports/regexpr2.h"

class CPatternMgr;

struct CParsedData
{
    enum 
    {
        EVALID_PARSED_DATA = 0x01,
    };
    
    CParsedData( unsigned int aMask = 0 )
        : m_ThreadId(-1), m_MaskCode( aMask )
    {
    }

    int m_ThreadId;
    std::string m_FunctionName;
    PatternId m_ThreadPatternId;
    PatternId m_GuarderPatternId;
    unsigned int m_MaskCode;
};

class CParsedDataMgr
{
public:
    CParsedDataMgr() 
    {
        m_ThreadIds.push_back( -1 );
    }

    ~CParsedDataMgr()
    {
    }

    //void RemoveParsedDate( int index );

    void RemoveAll()
    {
        m_ParsedDataQueue.clear();
    }

    const CParsedData & GetParsedData( int aIndex )
    {
        return m_ParsedDataQueue[aIndex];
    }
    
    void AppendParsedData( CParsedData& aParsedData )
    {
        m_ParsedDataQueue.push_back( aParsedData );
    }
    
    int GetParsedDataCount()
    {
        return m_ParsedDataQueue.size();
    }
       
    const std::vector<int> & GetThreadIds()
    {
        return m_ThreadIds;
    }

    bool BuildParsedDataQueue( CPatternMgr * aPatternMgr );

private:
    std::vector<int> m_ThreadIds;
    std::vector<CParsedData> m_ParsedDataQueue;
};

#endif // _PARSEDDATAMGR_H_