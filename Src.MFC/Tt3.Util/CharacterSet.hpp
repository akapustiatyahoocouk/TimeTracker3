//
//  Tt3.Util/CharacterSet.hpp
//  
//  Tt3 character set support framework
//
//////////
BEGIN_TT3_NAMESPACE

//  A generic "module descriptor"character set" - a set of rules for
//  two-way conversions between sequences of bytes and sequences of
//  characters
class TT3_UTIL_PUBLIC CCharacterSet;
using CCharacterSetList = CVector<CCharacterSet*>;

class TT3_UTIL_PUBLIC CCharacterSet
{
    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CCharacterSet)

    //////////
    //  Types
public:
    //  TODO document
    class TT3_UTIL_PUBLIC Transcoder
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Transcoder)

        //////////
        //  Construction/destruction
    public:
        Transcoder() = default;
        virtual ~Transcoder() = default;

        //////////
        //  Operations
    public:
        //  Returns the character set implemented by this transcoder
        virtual CCharacterSet*  GetCharacterSet() const = 0;

        //  Resets this transcoder to its initial state
        virtual void        Reset() = 0;
    };

    //  TODO document
    class TT3_UTIL_PUBLIC Encoder : public Transcoder
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Encoder)

        //////////
        //  Construction/destruction
    public:
        Encoder() = default;

        //////////
        //  Operations
    public:
        //  Returns the number of bytes required to encode the specified
        //  character; returns -1 if the character cannot be encoded.
        virtual int         GetEncodingLength(TCHAR c) = 0;

        //  Encodes the sequence of characters, sending the resulting
        //  bytes to the specified "destination".
        //  Returns the total number of characters consumed by encoding.
        virtual size_t      Encode(const TCHAR* pChars, size_t numChars, IDataDestination<BYTE> & destination) = 0;
        virtual size_t      Encode(const CString& chars, size_t numChars, IDataDestination<BYTE>& destination)
        {
            return Encode((LPCTSTR)chars, size_t(chars.GetLength()), destination);
        }
    };

    //  TODO document
    class TT3_UTIL_PUBLIC Decoder : public Transcoder
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Decoder)

        //////////
        //  Construction/destruction
    public:
        Decoder() = default;

        //////////
        //  Operations
    public:
        //  If a valid character encoding occurs at the beginning of the
        //  data area that starts at pBytes and is numBytes bytes long, returns
        //  the number of bytes that make up that encoding; else returns -1.
        virtual int         GetEncodingLength(const BYTE* pBytes, size_t numBytes) = 0;

        //  Decodes as many bytes as possible from the beginning of the
        //  data area that starts at pBytes and is numBytes bytes long, sending
        //  the decoded characters to the specified "destination".
        //  Returns the number of bytes consumed, which can be 0 if
        //  no characters could be decoded at all.
        virtual size_t      Decode(const BYTE* pBytes, size_t numBytes, IDataDestination<TCHAR>& destination) = 0;
    };

    //////////
    //  Construction/destruction
public:
    CCharacterSet() = default;
    virtual ~CCharacterSet() = default;

    //////////
    //  Operations
public:
    //  Returns the canonical (ISO) name of this character set.
    virtual CString         GetCanonicalName() const = 0;

    //  Returns the preferred (MIME) name of this character set.
    virtual CString         GetPreferredName() const = 0;

    //  Returns the list of all names of this character set.
    virtual CVector<CString>GetAllNames() const = 0;

    //  Returns the length of the longest encoding under the rules 
    //  of this character set.
    virtual size_t          GetMaximumEncodingLength() const = 0;

    //  Creates a new encoder/decoder for this character set.
    //  It is the caller's responsibility to delete it when no longer needed.
    virtual Encoder*        CreateEncoder() = 0;
    virtual Decoder*        CreateDecoder() = 0;

    //  Returns the "default" character that is 1) guaranteed to be
    //  encodable with this character set and 2) is used to replace
    //  unsupported characters or encodings.
    virtual TCHAR           GetDefaultCharacter() const { return '?'; }

    //////////
    //  Registry
public:
    //  The read-only view of all registered character sets
    class AllCollection : public CReadOnlyCollection<CCharacterSet*>
    {
        friend class CCharacterSet;

        //////////
        //  Construction/destruction
    private:
        AllCollection() = default;

        //////////
        //  CReadOnlyCollection<CCharacterSet*>
    public:
        using T = CCharacterSet*;
        virtual size_t      GetSize() const override;
        virtual bool        Contains(const T& value) const override;
        virtual CIterator<T>GetElements() const override;
    };
    static AllCollection    All;

    //  Returns the "system" character set
    static CCharacterSet*   GetSystem();

    //  Finds a character set by name (ignoring character case); 
    //  returns NULL if not found.
    static CCharacterSet*   Find(const char* pName);
    static CCharacterSet*   Find(const CString& name);

    //////////
    //  Implementation
private:
    class _Registry
    {
        DECLARE_SINGLETON(_Registry)

        friend class CCharacterSet;

        //////////
        //  Operations
    public:
        void                RegisterCharacterSet(CCharacterSet* pCharacterSet);
        CCharacterSet*      FindCharacterSet(const CString& name);
        CCharacterSetList   GetAllCharacterSets();

        //////////
        //  Implementation
    private:
        CDictionary<CString, CCharacterSet*> m_CharacterSetsMap;   //  keys are all-lowercase
        CCharacterSetList   m_CharacterSetsList;
    };
};

//  The ISO-8859-1 character set
class TT3_UTIL_PUBLIC CIso88591CharacterSet : public CCharacterSet
{
    DECLARE_SINGLETON(CIso88591CharacterSet)

    //////////
    //  CCharacterSet
public:
    virtual CString         GetCanonicalName() const override { return m_CanonicalName; }
    virtual CString         GetPreferredName() const override { return m_PreferredName; }
    virtual CVector<CString>GetAllNames() const override { return m_AllNames; }
    virtual size_t          GetMaximumEncodingLength() const override { return 1; }
    virtual Encoder*        CreateEncoder() override { return new _Encoder(); }
    virtual Decoder*        CreateDecoder() override { return new _Decoder(); }

    //////////
    //  Implementation
private:
    CString                 m_CanonicalName;
    CString                 m_PreferredName;
    CVector<CString>        m_AllNames;

    class _Encoder : public Encoder
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(_Encoder)

        //////////
        //  Construction/destruction
    public:
        _Encoder() = default;

        //////////
        //  Transcoder
    public:
        virtual CCharacterSet* GetCharacterSet() const override { return CIso88591CharacterSet::GetInstance(); }
        virtual void        Reset() override {}

        //////////
        //  Encoder
    public:
        virtual int         GetEncodingLength(TCHAR c) override;
        virtual size_t      Encode(const TCHAR* pChars, size_t numChars, IDataDestination<BYTE>& destination) override;
    };

    class _Decoder : public Decoder
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(_Decoder)

        //////////
        //  Construction/destruction
    public:
        _Decoder() = default;

        //////////
            //  Transcoder
    public:
        virtual CCharacterSet* GetCharacterSet() const override { return CIso88591CharacterSet::GetInstance(); }
        virtual void        Reset() override {}

        //////////
        //  Decoder
    public:
        virtual int         GetEncodingLength(const BYTE* pBytes, size_t numBytes) override;
        virtual size_t      Decode(const BYTE* pBytes, size_t numBytes, IDataDestination<TCHAR>& destination) override;
    };
};

//  The UTF-8 character set
class TT3_UTIL_PUBLIC CUtf8CharacterSet : public CCharacterSet
{
    DECLARE_SINGLETON(CUtf8CharacterSet)

    //////////
    //  CCharacterSet
public:
    virtual CString         GetCanonicalName() const override { return m_CanonicalName;  }
    virtual CString         GetPreferredName() const override { return m_PreferredName; }
    virtual CVector<CString>GetAllNames() const override { return m_AllNames; }
    virtual size_t          GetMaximumEncodingLength() const override { return 3; } //  sizeof(TCHAR) == 2
    virtual Encoder*        CreateEncoder() override { return new _Encoder(); }
    virtual Decoder*        CreateDecoder() override { return new _Decoder(); }

    //////////
    //  Implementation
private:
    CString                 m_CanonicalName;
    CString                 m_PreferredName;
    CVector<CString>        m_AllNames;

    class _Encoder : public Encoder
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(_Encoder)

        //////////
        //  Construction/destruction
    public:
        _Encoder() = default;

        //////////
        //  Transcoder
    public:
        virtual CCharacterSet*  GetCharacterSet() const override { return CUtf8CharacterSet::GetInstance(); }
        virtual void        Reset() override {}

        //////////
        //  Encoder
    public:
        virtual int         GetEncodingLength(TCHAR c) override;
        virtual size_t      Encode(const TCHAR* pChars, size_t numChars, IDataDestination<BYTE>& destination) override;
    };

    class _Decoder : public Decoder
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(_Decoder)

        //////////
        //  Construction/destruction
    public:
        _Decoder() = default;

        //////////
        //  Transcoder
    public:
        virtual CCharacterSet*  GetCharacterSet() const override { return CUtf8CharacterSet::GetInstance(); }
        virtual void        Reset() override {}

        //////////
        //  Decoder
    public:
        virtual int         GetEncodingLength(const BYTE* pBytes, size_t numBytes) override;
        virtual size_t      Decode(const BYTE* pBytes, size_t numBytes, IDataDestination<TCHAR>& destination) override;
    };
};

END_TT3_NAMESPACE
//  End of Tt3.Util/CharacterSet.hpp
