#ifndef _LINEPROPERTYOBSERVER_H_
#define _LINEPROPERTYOBSERVER_H_

class CLinePropertyObserver
{
public:
    virtual void HandleLineProperty( int aLineNum ) = 0;
};

#endif  // _LINEPROPERTYOBSERVER_H_

