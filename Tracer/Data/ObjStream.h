#ifndef __OBJSTREAM_H_
#define __OBJSTREAM_H_

#include <fstream>
#include <iostream>

#pragma warning( disable : 4290 )

class CObjWriter;
class CObjReader;

class CNullPointer
{
private:
    std::string _Descriptor;
    int _ExpNum;
public:
    CNullPointer( int aExpNum = 0, std::string& aDes = std::string() ) 
        : _ExpNum ( aExpNum ), _Descriptor( aDes )
    {    
    }
    CNullPointer( const CNullPointer & aNullPointerExp )
    {
        _Descriptor = aNullPointerExp._Descriptor;
        _ExpNum = aNullPointerExp._ExpNum;
    }
};

class CSerializable
{
public:
    virtual void Externalize( CObjWriter& writer ) const = 0;
    virtual void Internalize( CObjReader& reader ) = 0;
};

/***********************************************************
* This class serialize a object to a binded stream.
***********************************************************/
class CObjWriter
{
public:
	inline CObjWriter ();

	inline explicit CObjWriter ( std::ostream* os ) ;

	inline ~CObjWriter ();

	inline std::ostream*  Bind( std::ostream* os );

	inline void Flush();

	inline CObjWriter& operator<<( const CSerializable& aObj );

	inline CObjWriter& operator<<( const char* const aString )  throw ( CNullPointer );

	inline CObjWriter& operator<<( const std::string& aString );

	inline void Write( const void* const aBlock, int aLength ) throw ( CNullPointer );
 
	inline CObjWriter& operator<<( const bool aBoolean );	
	
	inline CObjWriter& operator<<( const char aByte );
	inline CObjWriter& operator<<( const unsigned char aByte );	

	inline CObjWriter& operator<<( const short aShort );
	inline CObjWriter& operator<<( const unsigned short aShort );

	inline CObjWriter& operator<<( const int aInt );
	inline CObjWriter& operator<<( const unsigned int aInt );

	inline CObjWriter& operator<<( const long aLong );
	inline CObjWriter& operator<<( const unsigned long aLong );

	inline CObjWriter& operator<<( const long long aLongLong );
	inline CObjWriter& operator<<( const unsigned long long aLongLong );	


	inline CObjWriter& operator<<( const float aFloat );
	inline CObjWriter& operator<<( const double aDouble );

private:
	std::ostream* m_OStream;
};


class CObjReader 
{
public:
	inline CObjReader ();

	inline explicit CObjReader ( std::istream* is );

	inline ~CObjReader ();

	inline std::istream* Bind( std::istream* is );

	inline CObjReader& operator>>( CSerializable& aObj );

	inline CObjReader& operator>>( char* aString ) throw ( CNullPointer );

	inline CObjReader& operator>>( std::string& aString );

	inline CObjReader& operator>>( bool& aBoolean );
	
	inline CObjReader& operator>>( char& aByte );
	inline CObjReader& operator>>( unsigned char& aByte );	

	inline CObjReader& operator>>( short& aShort );
	inline CObjReader& operator>>( unsigned short& aShort );

	inline CObjReader& operator>>( int& aInt );
	inline CObjReader& operator>>( unsigned int& aInt );

	inline CObjReader& operator>>( long& aLong );
	inline CObjReader& operator>>( unsigned long& aLong );

	inline CObjReader& operator>>( long long& aLongLong );
	inline CObjReader& operator>>( unsigned long long& aLongLong );	

	inline CObjReader& operator>>( float& aFloat );
	inline CObjReader& operator>>( double& aDouble );

	inline void Read( void* aBlock, int aLength ) throw ( CNullPointer );

private:
	std::istream* m_IStream;
};


#include "ObjStream.inl"

#endif // __OBJSTREAM_H_