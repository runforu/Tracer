#ifndef _LITEARRAY_H_
#define _LITEARRAY_H_

// A template thin array 
class CLiteArrayBase
{
    CLiteArrayBase( int aEntrySize )
        :_EntrySize(aEntrySize), _Count(0),_Granularity( KDefaultLiteArrayGranularity ),_Entries( NULL ),_Allocated(0)
    {    
    }

    CLiteArrayBase( int aEntrySize, int aGranularity )
        :_EntrySize(aEntrySize), _Count(0),_Granularity(aGranularity),_Entries( NULL ),_Allocated(0)
    {
    }

    CLiteArrayBase( int aEntrySize, void *aEntries, int aCount )
        :_EntrySize(aEntrySize), _Count(aCount),_Granularity(KDefaultLiteArrayGranularity),_Entries( aEntries ),_Allocated(aCount)
    {

    }

    CLiteArrayBase( int aEntrySize, int aPreallocatedCount, int aGranularity )
        :_EntrySize(aEntrySize), _Count(0),_Granularity(aGranularity),_Entries( NULL ),_Allocated(aPreallocatedCount)
    {
        _Entries = malloc( aEntrySize*aPreallocatedCount );
    }

    void * At( int aIndex )
    {
        ASSERT( aIndex >= 0 && aIndex < _Count );
        return ( int * )_Entries + aIndex * _EntrySize;
    }
    
private:
    int Grow()
    {
        void *p = malloc( _EntrySize*( _Count + _Granularity ) );
        memcpy( p, _Entries, _EntrySize*_Count );
        free(_Entries);
        _Entries = p;
    }

private:
    int _Count;
    int _EntrySize;
    int _Granularity;
    int _Allocated;
    void * _Entries;
    static const int KDefaultLiteArrayGranularity = 8;
};

template< typename T>
class CLiteArray : private CLiteArrayBase
{
public:
    CLiteArray(void){}
    ~CLiteArray(void){}
};

#endif
