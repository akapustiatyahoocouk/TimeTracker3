//
//  Tt3.XML/Dom.hpp
//  
//  Tt3.XML DOM definitions
//
//////////
BEGIN_TT3_NAMESPACE

//  Supported XML specification versions.
enum class XmlVersion
{
    Unknown,
    V_1_0,
    V_1_1,
    Default = V_1_0
};

//  Provides utility services used by XML subsystem.
class TT3_XML_PUBLIC CXml
{
    UTILITY_CLASS(CXml)

    //////////
    //  Operations
public:
    //  Checks if the specified string is a valid XML identifier.
    //
    //  Valid XML identifiers can contain letters, digits and/or special characters
    //  '.' (dot), '-' (hyphen), '_' (underscore) and ':' (colon) and must start with
    //  a letter, underscore or colon.
    static bool                 IsValidName(const CString & name);

    //  Checks if an XML identifier is allowed to start with `c`.
    static bool                 IsValidNameStart(TCHAR c);

    //  Checks if an XML identifier is allowed to include `c`.
    static bool                 IsValidNameChar(TCHAR c);

    //  Returns the argument with special XML characters encoded as entities.
    static CString              EncodeXmlEntities(const CString & s);

    //  Returns the argument with special XML character entities decoded.
    static CString              DecodeXmlEntities(const CString & s);

    //////////
    //  Implementation
private:
    class _EntityRegistry
    {
        DECLARE_SINGLETON(_EntityRegistry)

        //////////
        //  Operations
    public:
        bool                    FindEntityForChar(TCHAR c, CString & entity) const;
        bool                    FindCharForEntity(const CString & entity, TCHAR & c) const;

        //////////
        //  Implementation
    private:
        CDictionary<CString, TCHAR> m_MapEntitiesToChars;
        CDictionary<TCHAR, CString> m_MapCharsToEntities;

        //  Helpers
        void                    _MapEntity(const CString & entity, TCHAR c);
    };
};

//  A common base class for all nodes of a XML DOM document tree.
class TT3_XML_PUBLIC CXmlNode
{
    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CXmlNode)

    //////////
    //  Construction/destruction
public:
    CXmlNode() = default;
    virtual ~CXmlNode() noexcept = default;

    //////////
    //  Operations
public:
    //  Returns the XML DOM document to which this XML node belongs.
    virtual CXmlDocument *      GetDocument() const = 0;

    //  Returns the immediate parent of this XML DOM node in an XML DOM 
    // document tree; returns NULL if there's no parent.
    virtual CXmlNode *          GetParentNode() const = 0;

    //  Returns the ordered list of all immediate children of this node in a XML
    //  DOM document tree.
    virtual CXmlNodeList        GetChildNodes() const = 0;
};

//  An XML DOM element (XML tag).
class TT3_XML_PUBLIC CXmlElement : public CXmlNode
{
    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CXmlElement)

    friend class CXmlDocument;
    friend class CXmlAttribute;
    friend class CXmlText;
    friend class CXmlComment;
    friend class CXmlProcessingInstruction;

    //////////
    //  Construction (internal only)/destruction
private:
    explicit CXmlElement(CXmlDocument * pDocument);
public:
    virtual ~CXmlElement() noexcept;

    //////////
    //  CXmlNode
public:
    virtual CXmlDocument *      GetDocument() const override;
    virtual CXmlNode *          GetParentNode() const override;
    virtual CXmlNodeList        GetChildNodes() const override;

    //////////
    //  Operations
public:
    //  Returns the immediate parent element of this XML DOM element
    //  or NULL if this is the root element.
    CXmlElement *               GetParentElement() const;

    //  Returns the list of all attributes of this XML DOM element.
    CXmlAttributeList           GetAttributes() const;

    //  Returns the ordered list of all inner nodes of this XML DOM element.
    //
    //  The "inner nodes" of an XML DOM element represent the XML fragment
    //  that appears between the opening and closing tag of the XML element.
    CXmlNodeList                GetInnerNodes() const;

    //  Returns an ordered list of all XML DOM elements which are immediate
    //  children of this XML DOM element.
    CXmlElementList             GetChildElements() const;

    //  Returns an ordered list of all XML DOM elements which are immediate
    //  children of this XML DOM element which have the specified `agName`.
    //
    //  Note, that the namespace of the child element is ignored by search
    //  even if declared.
    CXmlElementList             GetChildElements(const CString & tagName) const;

    //  Returns/sets the namespace URI of this XML DOM element's tag.
    CString                     GetNamespaceUri() const;
    void                        SetNamespaceUri(const CString & namespaceUri);

    //  Returns/sets the tag name of this XML DOM element.
    CString                     GetTagName() const;
    void                        SetTagName(const CString & tagName);

    //  Finds an attribute of this XMl DOM element by name.
    //  Returns NULL if not found.
    CXmlAttribute *             FindAttribute(const CString & name) const;
    CXmlAttribute *             FindAttribute(const CString & namespaceUri, const CString & name) const;

    //  Returns the value of an attribute of this XML DOM element with the
    //  specified name.
    CString                     GetAttributeValue(const CString & name, const CString & defaultValue) const;

    //  Returns the value of an attribute of this XML DOM element with the
    //  specified namespace URI and name.
    CString                     GetAttributeValue(const CString & namespaceUri, const CString & name, const CString & defaultValue) const;

    //  Adds the specified XMl DOM attribute to this XML DOM element.
    //
    //  The `xmlAttribute` must belong to (i.e. be created by) the same XML DOM document
    //  as this XMl DOM element, and it must not already be assigned to a XML DOM element
    //  or an XML DOM processing instruction.
    //  Throws XmlDomException* if an attribute already exists.
    void                        AddAttribute(CXmlAttribute * pXmlAttribute) throws(XmlDomException*);

    //  Creates or updates an attribute of this XML DOM element.
    void                        SetAttribute(const CString & name, const CString & value);
    void                        SetAttribute(const CString & namespaceUri, const CString & name, const CString & value);

    //  Adds the specified XML DOM element to this XML DOM element as a child.
    //
    //  The `xmlElement` must belong to (i.e. be created by) the same XMl DOM document
    //  as this XML DOM element, and it must not already have a parent.
    //
    //  The `xmlElement` is added at the end of the list of this element's children.
    void                        AddElement(CXmlElement * pXmlElement);

    //  Adds the specified XML DOM text to this XML DOM element as a child.
    //
    //  The `xmlText` must belong to (i.e. be created by) the same XMl DOM document
    //  as this XML DOM element, and it must not already have a parent.
    //
    //  The `xmlText` is added at the end of the list of this element's children.
    void                        AddText(CXmlText * pXmlText);

    //  Adds the specified XML DOM comment to this XML DOM element as a child.
    //
    //  The `xmlComment` must belong to (i.e. be created by) the same XMl DOM document
    //  as this XML DOM element, and it must not already have a parent.
    //
    //  The `xmlComment` is added at the end of the list of this element's children.
    void                        AddComment(CXmlComment * pXmlComment);

    //  Adds the specified XML DOM processing instruction to this XML DOM element as a child.
    //
    //  The `xmlProcessingInstruction` must belong to (i.e. be created by) the same XMl
    //  DOM document as this XML DOM element, and it must not already have a parent.
    //
    //  The `xmlProcessingInstruction` is added at the end of the list of this
    //  element's children.
    void                        AddProcessingInstruction(CXmlProcessingInstruction * pXmlProcessingInstruction);

    //////////
    //  Implementation
private:
    CXmlDocument *              m_pDocument;      //  never NULL
    CXmlNode *                  m_pParentNode;    //  NULL if this is a standalone element
    CXmlAttributeList           m_Attributes;    //  ...in order of appearance
    CXmlNodeList                m_InnerNodes;    //  ...nested within this node
    CString                     m_NamespaceUri;  //  empty if not used
    CString                     m_TagName;       //  element's tag, without namespace prefix
};

//  An XML DOM attribute (of an XML element or XML processing instruction).
class TT3_XML_PUBLIC CXmlAttribute : public CXmlNode
{
    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CXmlAttribute)

    friend class CXmlDocument;
    friend class CXmlElement;
    friend class CXmlProcessingInstruction;

    //////////
    //  Construction (internal only)/destruction
private:
    explicit CXmlAttribute(CXmlDocument * pDocument);
public:
    virtual ~CXmlAttribute() noexcept;

    //////////
    //  CXmlNode
public:
    virtual CXmlDocument *      GetDocument() const override;
    virtual CXmlNode *          GetParentNode() const override;
    virtual CXmlNodeList        GetChildNodes() const override;

    //////////
    //  Operations
public:
    //  Returns the XML DOM element to which this XML DOM attribute belongs
    //  or NULL if it has no parent, or its parent is an XML DOM processing instruction.
    CXmlElement *               GetParentElement() const;

    //  Returns the XML DOM processing instruction to which this XML DOM attribute belongs
    //  or NULL if it has no parent, or its parent is an XML DOM element.
    CXmlProcessingInstruction * GetParentProcessingInstruction() const;

    //  Returns/sets the namespace URI of this XML DOM element.
    //  Throws CXmlDomException* if setting the name will lead to name
    //  conflict with another attribute under the same parent.
    CString                     GetNamespaceUri() const;
    void                        SetNamespaceUri(const CString & namespaceUri) throws(CXmlDomException*);

    //  Returns/sets the name of this XML DOM element.
    //  Throws CXmlDomException* if setting the name will lead to name
    //  conflict with another attribute under the same parent.
    CString                     GetName() const;
    void                        SetName(const CString & name) throws(CXmlDomException*);

    //  Returns/sets the value of this XML DOM attribute.
    CString                     GetValue() const;
    void                        SetValue(const CString & value);

    //////////
    //  Implementation
private:
    CXmlDocument *              m_pDocument;     //  never NULL
    CXmlNode *                  m_pParentNode;   //  NULL, or XmlElement or XmlProcessingInstruction
    CString                     m_NamespaceUri;  //  empty if not used
    CString                     m_Name;          //  ...without the namespace prefix
    CString                     m_Value;
};

//  An XML DOM text (a text context within XML).
class TT3_XML_PUBLIC CXmlText : public CXmlNode
{
    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CXmlText)

    friend class CXmlElement;
    friend class CXmlDocument;

    //////////
    //  Construction (internal only)/destruction
private:
    explicit CXmlText(CXmlDocument * pDocument);
public:
    virtual ~CXmlText() noexcept;

    //////////
    //  CXmlNode
public:
    virtual CXmlDocument *      GetDocument() const override;
    virtual CXmlNode *          GetParentNode() const override;
    virtual CXmlNodeList        GetChildNodes() const override;

    //////////
    //  Operations
public:
    //  Returns the parent XML DOM element of this XML DOM text or NULL
    //  if it has no parent.
    CXmlElement *               GetParentElement() const;

    //  Returns/sets the textual content of this XMl DOM text.
    CString                     GetText() const;
    void                        SetText(const CString & text);

    //////////
    //  Implementation
private:
    CXmlDocument *              m_pDocument;  //  never NULL
    CXmlElement *               m_pPrentElement; //  NULL is none
    CString                     m_Text;
};

//  An XML DOM comment (a comment context within XML).
class TT3_XML_PUBLIC CXmlComment : public CXmlNode
{
    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CXmlComment)

    friend class CXmlElement;
    friend class CXmlDocument;

    //////////
    //  Construction (internal only)/destruction
private:
    explicit CXmlComment(CXmlDocument * pDocument);
public:
    virtual ~CXmlComment() noexcept;

    //////////
    //  XmlNode
public:
    virtual CXmlDocument *      GetDocument() const override;
    virtual CXmlNode *          GetParentNode() const override;
    virtual CXmlNodeList        GetChildNodes() const override;

    //////////
    //  Operations
public:
    //  Returns the parent XML DOM element of this XML DOM comment 
    //  or NULL if it has no parent.
    CXmlElement *               GetParentElement() const;

    //  Returns/sets the textual content of this XMl DOM comment.
    CString                     GetText() const;
    void                        SetText(const CString & text);

    //////////
    //  Implementation
private:
    CXmlDocument *              m_pDocument;  //  never NULL
    CXmlElement *               m_pParentElement; //  NULL is none
    CString                     m_Text;
};

//  An XML DOM declaration (XML declaration).
class TT3_XML_PUBLIC CXmlDeclaration : public CXmlNode
{
    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CXmlDeclaration)

    friend class CXmlDocument;

    //////////
    //  Construction (internal only)/destruction
private:
    explicit CXmlDeclaration(CXmlDocument * pDocument);
public:
    virtual ~CXmlDeclaration() noexcept;

    //////////
    //  XmlNode
public:
    virtual CXmlDocument *      GetDocument() const override;
    virtual CXmlNode *          GetParentNode() const override;
    virtual CXmlNodeList        GetChildNodes() const override;

    //////////
    //  Operations
public:
    //  Returns/sets the XML version.
    XmlVersion                  GetVersion() const;
    void                        SetVersion(XmlVersion version);

    //  Returns/sets the XML encoding of the serialised XML document.
    CCharacterSet *             GetEncoding() const;
    void                        SetEncoding(CCharacterSet * pEncoding);

    //  Returns/sets the standalone flag of the containing XML document.
    bool                        IsStandalone() const;
    void                        SetStandalone(bool standalone);

    //////////
    //  Implementation
private:
    CXmlDocument *              m_pDocument;          //  never NULL
    CXmlDocument *              m_pParentDocument;    //  NULL if this is a standalone XML declaration
    XmlVersion                  m_Version;
    CCharacterSet *             m_pEncoding;
    bool                        m_Standalone;
};

//  An XML DOM processing instruction (XML processing instruction).
class TT3_XML_PUBLIC CXmlProcessingInstruction : public CXmlNode
{
    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CXmlProcessingInstruction)

    friend class CXmlElement;
    friend class CXmlAttribute;
    friend class CXmlDocument;

    //////////
    //  Construction (internal only)/destruction
private:
    explicit CXmlProcessingInstruction(CXmlDocument * pDocument);
public:
    virtual ~CXmlProcessingInstruction() noexcept;

    //////////
    //  CXmlNode
public:
    virtual CXmlDocument *      GetDocument() const override;
    virtual CXmlNode *          GetParentNode() const override;
    virtual CXmlNodeList        GetChildNodes() const override;

    //////////
    //  Operations
public:
    //  Returns the immediate parent element of this XML DOM element 
    //  or NULL if it has no parent.
    CXmlElement *               GetParentElement() const;

    //  Returns the list of all attributes of this XML DOM processing instruction.
    CXmlAttributeList           GetAttributes() const;

    //  Returns/sets the target of this XML DOM processing instruction.
    CString                     GetTarget() const;
    void                        SetTarget(const CString & target) throws (CXmlDomException*);

    //  Finds an attribute of this XMl DOM processing instruction by name,
    //  returns NULL if not found.
    CXmlAttribute *             FindAttribute(const CString & name) const;
    CXmlAttribute *             FindAttribute(const CString & namespaceUri, const CString & name) const;

    //  Adds the specified XMl DOM attribute to this XML DOM processing instruction.
    //  Throws XmlDomException* if an attribute already exists.
    void                        AddAttribute(CXmlAttribute * xmlAttribute) throws(CXmlDomException*);

    //  Creates or updates an attribute of this XML DOM processing instruction.
    void                        SetAttribute(const CString & name, const CString & value);
    void                        SetAttribute(const CString & namespaceUri, const CString & name, const CString & value);

    //////////
    //  Implementation
private:
    CXmlDocument *              m_pDocument;     //  never NULL
    CXmlElement *               m_pParentElement;//  NULL if this is a standalone PI
    CXmlAttributeList           m_Attributes;    //  ...in order of appearance
    CString                     m_Target;
};

//  An XML DOM representation of an XML document.
class TT3_XML_PUBLIC CXmlDocument : public CXmlNode
{
    friend class CXmlElement;
    friend class CXmlAttribute;
    friend class CXmlText;
    friend class CXmlComment;
    friend class CXmlDeclaration;
    friend class CXmlProcessingInstruction;

    //////////
    //  Construction/destruction/assignment
public:
    CXmlDocument();
    CXmlDocument(const CXmlDocument & src); //  deep clone
    virtual ~CXmlDocument() noexcept;

    CXmlDocument &              operator = (const CXmlDocument & src);  //  deep clone

    //////////
    //  CXmlNode
public:
    virtual CXmlDocument *      GetDocument() const override;
    virtual CXmlNode *          GetParentNode() const override;
    virtual CXmlNodeList        GetChildNodes() const override;

    //////////
    //  Operations
public:
    //  Returns/sets the XML declaration of this XML DOM document.
    CXmlDeclaration *           GetXmlDeclaration() const;
    void                        SetXmlDeclaration(CXmlDeclaration * pXmlDeclaration);

    //  Returns/sets the root element of this XML DOM document.
    CXmlElement *               GetRootElement() const;
    void                        SetRootElement(CXmlElement * pRootElement);

    //  Creates a new XML DOM XMLdeclaration that can be used in this XML DOM document.
    CXmlDeclaration *           CreateXmlDeclaration();
    CXmlDeclaration *           CreateXmlDeclaration(XmlVersion version, CCharacterSet * pEncoding, bool standalone);

    //  Creates a new, initially empty, XML DOM element with the specified
    //  tag name (and namespace URI) that can be used in this XML DOM document.
    //
    //  The newly created XML DOM element initially has no parent node.
    CXmlElement *               CreateElement(const CString & tagName);
    CXmlElement *               CreateElement(const CString & namespaceUri, const CString & tagName);

    //  Creates a new XML DOM attribute with the specified name and value
    //  (and namespace URI) that can be used in this XML DOM document.
    //
    //  The newly created XML DOM attribute initially has no parent node.
    CXmlAttribute *             CreateAttribute(const CString & name, const CString & value);
    CXmlAttribute *             CreateAttribute(const CString & namespaceUri, const CString & name, const CString & value);

    //  Creates a new XML DOM text node that can be used in this XML DOM document.
    //
    //  The newly created XML DOM text node initially has no parent node.
    CXmlText *                  CreateText(const CString & text);

    //  Creates a new XML DOM comment that can be used in this XML DOM document.
    //
    //  The newly created XML DOM comment initially has no parent node.
    CXmlComment *               CreateComment(const CString & text);

    //  Creates a new XML DOM processing instruction that can be used in
    //  this XML DOM document.
    //
    //  The newly created XML DOM processing instruction initially has no
    //  parent node.
    CXmlProcessingInstruction * CreateProcessingInstruction(const CString & target);

    //  Saves this XML DOM document as formatted XML to the specified stream/file.
    void                        Save(ITextOutput & stream) throws (CXmlDomException*, CIoException*);
    void                        Save(const CString& xmlFileName) throws(CXmlDomException*, CIoException*);

    //  Loads this XML DOM document from the specified `stream`/file.
    //  Any previous document content is lost.
    void                        Load(ITextInput & stream) throws(CXmlDomException*, CIoException*);
    void                        Load(const CString & xmlFileName) throws(CXmlDomException*, CIoException*);

    //////////
    //  Implementation
private:
    CXmlDeclaration *           m_pXmlDeclaration;
    CXmlElement *               m_pRootElement;

    CSet<CXmlNode*>             m_liveNodes;    //  all of them that refer to this XmlDocument instance

    CDictionary<size_t, CString>    m_MapDepthsToMarginStrings;

    typedef CDictionary<CString, CString> _NamespaceMap;  //  namespace URI -> namespace prefix

    //  Helpers
    void                        _Save(ITextOutput & stream, CXmlElement * pXmlElement, size_t depth, _NamespaceMap & namespaceMap) throws(CXmlDomException*, CIoException*);
    void                        _Save(ITextOutput & stream, CXmlText * pXmlText, size_t depth) throws(CXmlDomException*, CIoException*);
    void                        _Save(ITextOutput & stream, CXmlComment * pXmlComment, size_t depth) throws(CXmlDomException*, CIoException*);
    void                        _Save(ITextOutput & stream, CXmlProcessingInstruction * pXmlProcessingInstruction, size_t depth, _NamespaceMap & namespaceMap);
    static CString              _CreateAttributesString(const CXmlAttributeList & attributes, _NamespaceMap & namespaceMap);
    static CString              _NormalizeText(const CString & text);
    CString                     _GetMarginForDepth(size_t depth);

    CXmlDeclaration *           _CloneXmlDeclarationForMyself(CXmlDeclaration * pXmlDeclaration);
    CXmlElement *               _CloneXmlElementForMyself(CXmlElement * pXmlElement);
    CXmlText *                  _CloneXmlTextForMyself(CXmlText * pXmlText);
    CXmlComment *               _CloneXmlCommentForMyself(CXmlComment * pXmlComment);
    CXmlProcessingInstruction * _CloneXmlProcessingInstructionForMyself(CXmlProcessingInstruction * pXmlProcessingInstruction);
};

//  An exception thrown when an XML DOM service fails.
class TT3_XML_PUBLIC CXmlDomException : public CCheckedException
{
    //////////
    //  Construction/destruction/assignment
public:
    explicit CXmlDomException(const CString & errorMessage) : CCheckedException(errorMessage) {}
};

END_TT3_NAMESPACE
//  End of Tt3.XML/Dom.hpp
