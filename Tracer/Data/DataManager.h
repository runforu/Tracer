#ifndef _DATAMANAGER_H_
#define _DATAMANAGER_H_

#include "../stdafx.h"
#include <string>
#include <vector>
#include <utility>
#include <list>
#include "ObjStream.h"


class CDataManager : public CSerializable
{
    friend class CTracerDoc;
public:
    static CDataManager* NewDataManager();

    ~CDataManager(void)
    {
        m_Instance = NULL;
    }

    // From CSerializable
    virtual void Externalize( CObjWriter& writer ) const;
    virtual void Internalize( CObjReader& reader );

    // Trim unused chars, can not undo.
    void Trim( int aFrom, int aTo );

    std::string& GetLine( int aLineNum )
    {
        ASSERT( aLineNum >=0 && aLineNum <= m_LogLines.size() );
        return m_LogLines[aLineNum];
    }

    int GetLineCount()
    {
        return m_LogLines.size();
    }

    void Clear()
    {
        m_LogLines.clear();
    }

private:
    CDataManager( const CDataManager& aRawDataManager );
    CDataManager()
    {
        m_Instance = this;
    }
    
    std::vector<std::string> m_LogLines;
    static CDataManager *m_Instance;
    static int m_RefCount;
};

#endif //_DATAMANAGER_H_