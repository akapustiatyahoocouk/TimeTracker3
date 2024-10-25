//
//  Tt3.Util/Locale.hpp
//  
//  Tt3 localization framework
//
//////////
BEGIN_TT3_NAMESPACE

//  An ISO-639 language
class TT3_UTIL_PUBLIC CLanguage
{
    //////////
    //  Constants
public:
    static const CLanguage  Invalid;    //  An "invalid" language

    //////////
    //  Construction/destruction/assignment
public:
    CLanguage();    //  constructs an "invalid" language
    explicit CLanguage(const CString & code2);
    explicit CLanguage(const char * pCode2);

    //////////
    //  Operators
public:
    bool            operator == (const CLanguage & op2) const { return m_pImpl == op2.m_pImpl; }
    bool            operator != (const CLanguage & op2) const { return m_pImpl != op2.m_pImpl; }

    //////////
    //  Operations
public:
    //  TODO document
    bool            IsValid() const { return m_pImpl != Invalid.m_pImpl; }
    CString         GetCode2() const { return m_pImpl->code2;}
    CString         GetCode3() const { return m_pImpl->code3; }
    CString         GetDisplayName() const { return m_pImpl->displayName; }

    //////////
    //  Implementation
private:
    struct _Impl
    {
        _Impl(const CString & code2Param, const CString & code3Param, const CString & displayNameParam)
            :   code2(code2Param), code3(code3Param), displayName(displayNameParam) {}

        CString     code2;          //  lowercase 2-letter
        CString     code3;          //  lowercase 3-letter
        CString     displayName;    //  for the invariant locale
    };
    _Impl *         m_pImpl;    //  never NULL

    static const TCHAR* const m_pInvalidLanguageCode2;
    static const TCHAR* const m_pInvalidLanguageCode3;

    //  Helpers
    static CDictionary<CString, _Impl*>& _Registry();
    static void     _InitRegistry();
};

//  An ISO-3166 country
class TT3_UTIL_PUBLIC CCountry
{
    //////////
    //  Constants
public:
    static const CCountry   Invalid;    //  An "invalid" country

    //////////
    //  Construction/destruction/assignment
public:
    CCountry();    //  constructs an "invalid" country
    explicit CCountry(const CString & code2);
    explicit CCountry(const char * pCode2);

    //////////
    //  Operators
public:
    bool            operator == (const CCountry & op2) const { return m_pImpl == op2.m_pImpl; }
    bool            operator != (const CCountry & op2) const { return m_pImpl != op2.m_pImpl; }

    //////////
    //  Operations
public:
    //  TODO document
    bool            IsValid() const { return m_pImpl != Invalid.m_pImpl; }
    CString         GetCode2() const { return m_pImpl->code2; }
    CString         GetCode3() const { return m_pImpl->code3; }
    int             GetCodeN() const { return m_pImpl->codeN; }
    CString         GetDisplayName() const { return m_pImpl->displayName; }

    //////////
    //  Implementation
private:
    struct _Impl
    {
        _Impl(const CString & code2Param, const CString & code3Param, int codeNParam, const CString & displayNameParam)
            :   code2(code2Param), code3(code3Param), codeN(codeNParam), displayName(displayNameParam) {}

        CString     code2;          //  uppercase 2-letter
        CString     code3;          //  uppercase 3-letter
        int         codeN;          //  numeric code
        CString     displayName;    //  for the invariant locale
    };
    _Impl *         m_pImpl;

    static const TCHAR * const  m_pInvalidCountryCode2;
    static const TCHAR * const  m_pInvalidCountryCode3;
    static const int        m_InvalidCountryCodeN;

    //  Helpers
    static CDictionary<CString, _Impl *> &  _Registry();
    static void     _InitRegistry();
};

//  A specification of the locale
class TT3_UTIL_PUBLIC CLocale final
{
    //////////
    //  Constants
public:
    static const CLocale Invariant; //  The root (invariant) locale

    //////////
    //  Construction/destruction/assignment
public:
    CLocale();  //  constructs an "invariant" locale
    explicit CLocale(const CLanguage & language);
    explicit CLocale(const CString & language);
    explicit CLocale(const char * pLanguage);
    CLocale(const CLanguage & language, const CCountry & country);
    CLocale(const CString & language, const CString & country);
    CLocale(const char * pLanguage, const char * pCountry);
    CLocale(const CLanguage & language, const CCountry & country, const CString & variant);
    CLocale(const CString & language, const CString & country, const CString & variant);
    CLocale(const char * pLanguage, const char * pCountry, const char * pVariant);
    ~CLocale();

    CLocale &       operator = (const CLocale & src);

    //////////
    //  Operators
public:
    bool            operator == (const CLocale & op2) const;
    bool            operator != (const CLocale & op2) const { return !(*this == op2);  }

    //////////
    //  Operations
public:
    //  TODO document
    CLanguage       GetLanguage() const { return m_Language; }
    CCountry        GetCountry() const { return m_Country; }
    CString         GetVariant() const { return (m_pVariant != NULL) ? *m_pVariant : m_EmptyString; }

    CString         ToString() const;
    static CLocale  FromString(const CString & s);

    //  Returns the system locale
    static CLocale  GetSystem();

    //  Gets/sets the current "default locale"
    static CLocale  GetDefault();
    static void     GetDefault(const CLocale & locale);

    //////////
    //  Implementation
private:
    CLanguage       m_Language;
    CCountry        m_Country;
    CString *       m_pVariant;     //  NULL == not specified

    static const CString    m_EmptyString;
    static CLocale  m_Default;

    //  Helpers
    void            _NormalizeVariant();
};

END_TT3_NAMESPACE
//  End of Tt3.Util/Locale.hpp
