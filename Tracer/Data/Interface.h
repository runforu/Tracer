#ifndef _INTERFACE_H_
#define _INTERFACE_H_


// The base abstract class of other abstract class.
// Make sure that delete on right class.
class Interface
{
public:
    virtual ~Interface(void) = 0
    {
    }
};
#endif // _INTERFACE_H_