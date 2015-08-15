
// CObjWriter
CObjWriter::CObjWriter () : m_OStream()
{
}

CObjWriter::CObjWriter ( std::ostream* os ) : m_OStream( os )
{
}

inline std::ostream* CObjWriter::Bind( std::ostream* os )
{
	m_OStream->flush();
	std::ostream* rt = m_OStream;
	m_OStream = os;
	return rt;
}

void CObjWriter::Flush()
{
	m_OStream->flush();
}

CObjWriter::~CObjWriter()
{
	//delete m_OStream;
}

// CSerializable
CObjWriter& CObjWriter::operator<<( const CSerializable& aObj )
{
	aObj.Externalize( *this );
	return *this;
}

// c string
CObjWriter& CObjWriter::operator<<( const char * const aString ) throw ( CNullPointer )
{
    if( aString == NULL )
        throw CNullPointer( 1 );

	size_t  len = strlen(aString);
	m_OStream->write( reinterpret_cast<const char*>(&len), sizeof(len) );
	m_OStream->write( aString, static_cast<std::streamsize>( len ) );
	return *this;
}

// std::string
CObjWriter& CObjWriter::operator<<( const std::string& aString )
{
	std::string::size_type  len = aString.length();
	m_OStream->write( reinterpret_cast<const char*>(&len), sizeof(len) );
    m_OStream->write( aString.c_str(), static_cast<std::streamsize>( len ) );
	return *this;
}

// boolean
CObjWriter& CObjWriter::operator<<( const bool aBoolean )
{
	m_OStream->write( reinterpret_cast<const char*>( &aBoolean ), sizeof( aBoolean ) );
	return *this;
}

// char
CObjWriter& CObjWriter::operator<<( const char aByte )
{
	m_OStream->write( reinterpret_cast<const char*>( &aByte ), sizeof( aByte ) );
	return *this;
}

// unsigned char
CObjWriter& CObjWriter::operator<<( const unsigned char aByte )
{
	m_OStream->write( reinterpret_cast<const char*>( &aByte ), sizeof( aByte ) );
	return *this;
}

// short
CObjWriter& CObjWriter::operator<<( const short aShort )
{
	m_OStream->write( reinterpret_cast<const char*>( &aShort ), sizeof( aShort ) );
	return *this;
}

// unsigned short
CObjWriter& CObjWriter::operator<<( const unsigned short aShort )
{
	m_OStream->write( reinterpret_cast<const char*>( &aShort ), sizeof( aShort ) );
	return *this;
}

// int
CObjWriter& CObjWriter::operator<<( const int aInt )
{
	m_OStream->write( reinterpret_cast<const char*>( &aInt ), sizeof( aInt ) );
	return *this;
}

// unsigned int
CObjWriter& CObjWriter::operator<<( const unsigned int aInt )
{
	m_OStream->write( reinterpret_cast<const char*>( &aInt ), sizeof( aInt ) );
	return *this;
}

// long
CObjWriter& CObjWriter::operator<<( const long aLong )
{
	m_OStream->write( reinterpret_cast<const char*>( &aLong ), sizeof( aLong ) );
	return *this;
}


// unsigned long
CObjWriter& CObjWriter::operator<<( const unsigned long aLong )
{
	m_OStream->write( reinterpret_cast<const char*>( &aLong ), sizeof( aLong ) );
	return *this;
}

// float
CObjWriter& CObjWriter::operator<<( const float aFloat )
{
	m_OStream->write( reinterpret_cast<const char*>( &aFloat ), sizeof( aFloat ) );
	return *this;
}

// double
CObjWriter& CObjWriter::operator<<( const double aDouble )
{
	m_OStream->write( reinterpret_cast<const char*>( &aDouble ), sizeof( aDouble ) );
	return *this;
}

void CObjWriter::Write( const void* const aBlock, int aLength ) throw ( CNullPointer )
{
    if( aBlock == NULL )
    {
        throw CNullPointer( 1 );
    }

	m_OStream->write( reinterpret_cast<const char*>( &aBlock ), aLength );
}




// CObjReader
CObjReader::CObjReader () : m_IStream()
{
}

CObjReader::CObjReader ( std::istream* is ) : m_IStream( is)
{
}

std::istream* CObjReader::Bind( std::istream* is )
{
	std::istream* rt = m_IStream;
	m_IStream = is;
	return rt;
}

CObjReader::~CObjReader ()
{
	//delete m_IStream;
}

// CSerializable
CObjReader& CObjReader::operator>>( CSerializable& aObj )
{
	aObj.Internalize( *this );
	return *this;
}

// string
CObjReader& CObjReader::operator>>( std::string& aString )
{
	std::streamsize  len = 0;
	m_IStream->read( reinterpret_cast<char*>( &len ), sizeof( len ) );
	aString.resize( len );
	m_IStream->read( const_cast<char *>( aString.data() ), len );
	return *this;
}

// c string
CObjReader& CObjReader::operator>>( char * aString ) throw ( CNullPointer )
{
    if ( aString == NULL )
    {
        throw CNullPointer( 1 );
    }

    size_t  len = 0;
    m_IStream->read( reinterpret_cast<char*>(&len), sizeof(len) );
	m_IStream->read( const_cast<char *>( aString ), static_cast<std::streamsize>( len ) );
	aString[len] = 0;
	return *this;
}

// boolean
CObjReader& CObjReader::operator>>( bool& aBoolean )
{
	m_IStream->read( reinterpret_cast<char*>( &aBoolean ), sizeof( aBoolean ) );
	return *this;
}

//char
CObjReader& CObjReader::operator>>( char& aByte )
{
	m_IStream->read( reinterpret_cast<char*>(&aByte), sizeof(aByte) );
	return *this;
}

//unsigned char
CObjReader& CObjReader::operator>>( unsigned char& aByte )
{
	m_IStream->read( reinterpret_cast<char*>(&aByte), sizeof(aByte) );
	return *this;
}

// short
CObjReader& CObjReader::operator>>( short& aShort )
{
	m_IStream->read( reinterpret_cast<char*>(&aShort), sizeof(aShort) );
	return *this;
}

// unsigned short
CObjReader& CObjReader::operator>>( unsigned short& aShort )
{
	m_IStream->read( reinterpret_cast<char*>(&aShort), sizeof(aShort) );
	return *this;
}

// int
CObjReader& CObjReader::operator>>( int& aInt )
{
	m_IStream->read( reinterpret_cast<char*>( &aInt ), sizeof( aInt ) );
	return *this;
}

// unsigned int
CObjReader& CObjReader::operator>>( unsigned int& aInt )
{
	m_IStream->read( reinterpret_cast<char*>( &aInt ), sizeof( aInt ) );
	return *this;
}

// long
CObjReader& CObjReader::operator>>( long& aLong )
{
	m_IStream->read( reinterpret_cast<char*>( &aLong ), sizeof( aLong ) );
	return *this;
}

// unsigned long
CObjReader& CObjReader::operator>>( unsigned long& aLong )
{
	m_IStream->read( reinterpret_cast<char*>( &aLong ), sizeof( aLong ) );
	return *this;
}

// float
CObjReader& CObjReader::operator>>( float& aFloat )
{
	m_IStream->read( reinterpret_cast<char*>( &aFloat ), sizeof( aFloat ) );
	return *this;
}

// double
CObjReader& CObjReader::operator>>( double& aDouble )
{
	m_IStream->read( reinterpret_cast<char*>( &aDouble ), sizeof( aDouble ) );
	return *this;
}

void CObjReader::Read( void* aBlock, int aLength )  throw ( CNullPointer )
{
    if ( aBlock == NULL )
    {
        throw CNullPointer( 1 );
    }

	m_IStream->read( reinterpret_cast<char*>( aBlock ), sizeof( aLength ) );
}