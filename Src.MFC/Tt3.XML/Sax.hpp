//
//  Tt3.XML/Sax.hpp
//  
//  Tt3.XML SAX definitions
//
//////////
BEGIN_TT3_NAMESPACE

//  The agent notified about various events by a SAX parser during the parsing
//  of an XML stream.
class TT3_XML_PUBLIC IXmlContentHandler
{
    //////////
    //  Types
public:
    //  Encapsulates name and value of a single attribute of an XML element or
    //  processing instruction.
    struct Attribute
    {
        CString             namespaceUri;   //  The namespace URI of the attribute, empty string if none.
        CString             name;   //  The name of the attribute.
        CString             value;//  The value of the attribute.

        bool                operator == (const Attribute & op2) const
        {
            return namespaceUri == op2.namespaceUri &&
                   name == op2.name &&
                   value == op2.value;
        }
    };
    using CAttributeList = CVector<Attribute>;

    //////////
    //  This is an interface
public:
    virtual ~IXmlContentHandler() = default;

    //////////
    //  Operations
public:
    //  Called by XML SAX parser when a beginning of an XML document is recognized.
    virtual void            BeginDocument(XmlVersion version, CharacterSet * encoding, bool standalone) = 0;

    //  Called by XML SAX parser when an end of an XML document is recognized.
    virtual void            EndDocument() = 0;

    //  Called by XML SAX parser when a beginning of an XML element is recognized.
    virtual void            BeginElement(const CString & namespaceUri, const CString & tagName, const CAttributeList & attributes) = 0;

    //  Called by XML SAX parser when an end of an XML element is recognized.
    virtual void            EndElement(const CString & namespaceUri, const CString & tagName) = 0;

    //  Called by XML SAX parser when a text block is recognized.
    virtual void            Text(const CString & text) = 0;

    //  Called by XML SAX parser when a comment is recognized.
    virtual void            Comment(const CString & text) = 0;

    //  Called by XML SAX parser when a processing instruction is recognized.
    virtual void            ProcessingInstruction(const CString & target, const CAttributeList & attributes) = 0;
};

//  The agent that can parse an XML stream, reporting XML parse events to an associated handler.
class TT3_XML_PUBLIC CXmlReader
{
    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CXmlReader)

    //////////
    //  Construction/destruction
public:
    explicit CXmlReader(IXmlContentHandler * contentHandler);
    virtual ~CXmlReader();

    //////////
    //  Operations
public:
    //  Parses the specified XML stream, reporting parse events to associated content handler.
    void                    Parse(ITextInput & stream);

    //  Parses the specified XML file, reporting parse events to associated content handler.
    void                    Parse(const CString & xmlFileName);

    //////////
    //  Implementation
private:
    IXmlContentHandler *    m_pContentHandler;  //  never NULL
    ITextInput *            m_pStream;

    CVector<TCHAR>          m_PrereadCharacters;
    CStack<CString>         m_OpenXmlElementTags;

    //  Helpers
    typedef CXmlContentHandler::Attribute       _Attribute;
    typedef CXmlContentHandler::CAttributeList  _AttributeList;

    typedef CDictionary<CString, CString>       _NamespaceMap;   //  prefix -> namespace URI

    static CCharacterSet *  _DetermineEncoding(const CString & xmlFileName);
    TCHAR                   _Lookahead(size_t distance);
    void                    _SkipCharacter();
    void                    _SkipCharacter(Char c);
    void                    _SkipSpaces();
    void                    _ParseXmlStream();
    void                    _ParseXmlDeclaration(XmlVersion & version, CharacterSet *& encoding, bool & standalone);
    void                    _ParseXmlElement(_NamespaceMap & namespaceMap);
    void                    _ParseComment();
    void                    _ParseProcessingInstruction(_NamespaceMap & namespaceMap);
    _AttributeList          _ParseAttributes(_NamespaceMap & namespaceMap);
    String                  _ParseIdentifier();
    static void             _ResolveNamespaceUri(const _NamespaceMap & namespaceMap, CString & name, CString & namespaceUri);
};

//  An exception thrown when an XML SAX service fails.
class TT3_XML_PUBLIC CXmlSaxException : public CCheckedException
{
    //////////
    //  Construction/destruction/assignment
public:
    explicit CXmlSaxException(const CString & errorMessage) : CCheckedException(errorMessage) {}
};

END_TT3_NAMESPACE
//  End of Tt3.XML/Sax.hpp
