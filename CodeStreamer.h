#ifndef CCodeStreamer_H
#define CCodeStreamer_H


class CStream
{
public:
    class CEndLine
    {
    };
    static CEndLine endl;

    CStream()
    {
        m_isNewLine = false;
        m_indentationLevel = 0;
        m_indentationSize = 4;
    }
    void Append(
        const char* item
        )
    {
        Flush();
        Stream() << item;
    }
    void Append( const std::string& item )
    {
        Flush();
        Stream() << item;
    }
    virtual std::ostream&
        Stream(
            ) = 0;
    
    void Flush()
    {
        if( m_isNewLine )
        {
            for( unsigned int i = 0; i < m_indentationLevel * m_indentationSize; i++ )
            {
                Stream() << " ";
            }
        }
        m_isNewLine = false;
    }
    void
        EndLine(
            )
    {
        Stream() << "\n";
        m_isNewLine = true;
    }
    
    
    unsigned int 
        m_indentationSize;

    unsigned int 
        m_indentationLevel;            
        
    bool
        m_isNewLine;
};


template<class TColl, class TStream>
class CCollectionStreamer
{
public:
    CCollectionStreamer(const TColl& coll, const std::string& sep):m_coll(coll), m_sep(sep)
    {
    }

    void
        ToStream(
            TStream& stream
            )const
    {
        
        for( auto it = m_coll.begin(); it != m_coll.end(); it++ )
        {
            if( it != m_coll.begin() )
            {
                stream << m_sep;
            }
            stream << *it;
        }

    }
    
    const std::string m_sep;
    TColl m_coll;                 
};

template<class TColl, class TStream>
TStream& operator<<(TStream& stream, const CCollectionStreamer<TColl, TStream>& collStreamer )
{
    collStreamer.ToStream(stream);
    return stream;
}

template<class TColl, class TStream>
CCollectionStreamer<TColl, TStream> StreamCollection(TStream& stream, const TColl& coll, const char* sep )
{
    return CCollectionStreamer<TColl, TStream>(coll, sep);
}

template <class TActual>
class CStreamT : public CStream
{
public:
    
    template<typename... Args>
        CStreamT(Args&&... args): m_actualStream(args...) 
        {
        }

    TActual&
        Stream(
            ) override
    {
        return m_actualStream;
    }
     

    TActual
        m_actualStream;
};
//--------------------------------------------------------------------------------------------------
CStream& operator<<( CStream& stream, const char* item );
//--------------------------------------------------------------------------------------------------
CStream& operator<<( CStream& stream, const std::string& item );
//--------------------------------------------------------------------------------------------------
CStream& operator<<( CStream& stream, const CStream::CEndLine& item );
//--------------------------------------------------------------------------------------------------


class CCodeStreamer
{
public:
    CCodeStreamer(
        CStream& stream
        );
    

    std::ostream&
        CoreStream(
            )
    {
        return Stream().Stream();
    }
    CStream&
        Stream();

    void 
        BeginStruct(
            const std::string& name
            );
   
    void 
        EndStruct(
            );

    CStream& 
        m_stream;

};


class CIndenter
{
public:
   CIndenter(
        CStream& stream,
        unsigned int units):
        m_units(units),
        m_stream(stream)
    {
        m_stream.m_indentationLevel += m_units;
    }
    ~CIndenter()
    {
        m_stream.m_indentationLevel -= m_units;
    }
private:
    unsigned int 
        m_units;
    
    CStream& 
        m_stream;
};


//--------------------------------------------------------------------------------------------------
CCodeStreamer& operator<<( CCodeStreamer& stream, const char* item );
//--------------------------------------------------------------------------------------------------
CCodeStreamer& operator<<( CCodeStreamer& stream, const std::string& item );
//--------------------------------------------------------------------------------------------------
CCodeStreamer& operator<<( CCodeStreamer& stream, const CStream::CEndLine& item );
//--------------------------------------------------------------------------------------------------

#endif

