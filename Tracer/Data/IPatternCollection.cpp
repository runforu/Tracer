#include "stdafx.h"
#include "IPatternCollection.h"
#include "GretaPatternCollection.h"

IPatternCollection * IPatternCollection::PatternCollection()
{
    return new CGretaPatternCollection();
}