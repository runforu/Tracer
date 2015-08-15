#include "stdafx.h"
#include "IRegExp.h"
#include "GretaPattern.h"

IPattern * IPattern::Pattern( const std::string & pat, PatternId aId )
{
    return new CGretaPattern( pat, aId );
}

IPattern * IPattern::Pattern( const char * pat, PatternId aId )
{
    return new CGretaPattern( pat, aId );
}
