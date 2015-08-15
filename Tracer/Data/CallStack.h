#ifndef _CALLSTACK_H_
#define _CALLSTACK_H_

#include <vector>

class CParsedDataMgr;

typedef  std::pair<int, int> LinePair;

struct CStackItem
{
    static bool IsNullStackItem( const CStackItem & aItem )
    {
        return aItem.m_nLevel == 0 && aItem.m_LinePair == NULL;
    }

    CStackItem( int level = 0, LinePair *aLinePair = NULL ):m_nLevel( level ), m_LinePair( aLinePair ) 
    {
    }
    int m_nLevel;
    LinePair * m_LinePair;
};

class CCallStack
{
public:
    CCallStack(int aThreadIdFilter = -1 ) : m_nThreadId( aThreadIdFilter )
    {
    }

    ~CCallStack();

    void BuildCallStack( CParsedDataMgr *pParsedDataMgr );

    int ThreadIdFilter()
    {
        return m_nThreadId;
    }

    const CStackItem * StackItem( int aIndex ) const
    {
        return &m_Stack[aIndex];
    }

    unsigned int Count() const
    {
        return m_Stack.size();
    }

#ifdef _DEBUG
    void DumpElements() 
    {
        std::vector<CStackItem>::iterator iter = m_Stack.begin();
        while ( iter != m_Stack.end() )
        {        
            CStackItem* pItem = &(*iter);
            ++iter;
        }
    }
#endif

private:
    const int m_nThreadId;
    std::vector<CStackItem> m_Stack;
};

#endif //_CALLSTACK_H_