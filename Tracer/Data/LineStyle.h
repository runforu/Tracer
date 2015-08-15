#ifndef _LINESTYLE_H_
#define _LINESTYLE_H_



// Define a line's bookmark type of the editor.
enum  BookMarkType
{
    BMT_None = 0x00,
    BMT_UserDefine = 0x01,
    BMT_Error = 0x02,
    BMT_Warning = 0x04,
};

// BookMarkerType serialization.
inline CObjWriter& operator<<( CObjWriter& writer, BookMarkType& bmt )
{
    writer.Write( reinterpret_cast<const char*>( &bmt ), sizeof( bmt ) );
    return writer;
}

inline CObjReader& operator>>( CObjReader& reader, BookMarkType& bmt )
{
    reader.Read( reinterpret_cast<char*>(&bmt), sizeof(bmt) );
    return reader;
}

// Decorate the raw data for a text line display
class CLineStyle : public CSerializable
{
public:
    CLineStyle ()
            : m_nLineNum( -1 ),
            m_yHeight( 10 ),
            m_crTextColor( 0x00000000 ),
            m_nBookMark( BMT_None ),
            m_nThreadID( 0 ),
            m_strComment(),
            m_strFaceName( "Verdana" ),
            m_nMask( 0 )
    {
    }

    void Externalize( CObjWriter& writer ) const
    {
        writer<<m_nLineNum;
        writer<<m_yHeight;
        writer<<m_crTextColor;
        writer<<m_nBookMark;
        writer<<m_nThreadID;
        writer<<m_nMask;
        writer<<m_strComment;
        writer<<m_strFaceName;
    }

    void Internalize( CObjReader& reader )
    {
        reader>>m_nLineNum;
        reader>>m_yHeight;
        reader>>m_crTextColor;
        reader>>m_nBookMark;
        reader>>m_nThreadID;
        reader>>m_nMask;
        reader>>m_strComment;
        reader>>m_strFaceName;
    }

    int m_nLineNum;
    int m_yHeight;
    int m_crTextColor;
    int m_nBookMark;
    int m_nThreadID;
    // Indicates the line's properties
    unsigned int m_nMask;
    std::string m_strComment;
    std::string m_strFaceName;
};



#endif  // _LINESTYLE_H_

