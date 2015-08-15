#ifndef _GRETAPATTERNCOLLECTION_H_
#define _GRETAPATTERNCOLLECTION_H_

#include <string>
#include <vector>
#include <utility>
#include "IPatternCollection.h"
#include "IRegExp.h"

class CGretaPatternCollection : public IPatternCollection
{
public:
    ~CGretaPatternCollection(void);

    bool Match( const std::string &str, MatchResult & aResult );

    void Append( IPattern* aPattern );

    void Remove( int aIndex );

    const IPattern * GetPattern( int aIndex );

    void Clear();

    unsigned int Count() const;

    CGretaPatternCollection()
    {
    }

    const IPattern * FindPattern( PatternId aId );

private:
    CGretaPatternCollection( const CGretaPatternCollection& );    
    std::vector<IPattern *> m_Patterns;      
};

#endif //_GRETAPATTERNCOLLECTION_H_