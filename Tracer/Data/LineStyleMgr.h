#ifndef _CLINESTYLEMGR_H_
#define _CLINESTYLEMGR_H_

#include <vector>
#include "ObjStream.h"

class CLineStyle;
class CDataManager;

// Decorate the raw data for a text line
class CLineStyleMgr:public CSerializable
{
public:
    CLineStyleMgr( CDataManager * aRawDataMgr );
    
    ~CLineStyleMgr(void);
    
    // From CSerializable
    void Externalize( CObjWriter& writer ) const;
    
    void Internalize( CObjReader& reader ) ;

    CLineStyle* GetLineStyle( int aLineNum );
    
private:    
    CDataManager *m_RawDataMgr;
    std::vector<CLineStyle> m_LineStyleSet;
    std::vector<int> m_LineStyleMap;
};


#endif //_CLINESTYLEMGR_H_

