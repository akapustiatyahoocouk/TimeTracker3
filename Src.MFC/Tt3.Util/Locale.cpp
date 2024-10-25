//
//  Tt3.Util/Locale.cpp
//  
//  CLocale class implementation
//
//////////
#include "Tt3.Util/API.hpp"
USING_TT3_NAMESPACE

const CLocale   CLocale::Invariant;
const CString   CLocale::m_EmptyString;
CLocale         CLocale::m_Default = CLocale::GetSystem();

//////////
//  Construction/destruction/assignment
CLocale::CLocale()
    :   m_Language(),
        m_Country(),
        m_pVariant(NULL)
{
}

CLocale::CLocale(const CLanguage & language)
    :   m_Language(language),
        m_Country(),
        m_pVariant(NULL)
{
}

CLocale::CLocale(const char * pLanguage)
    :   CLocale(CString(pLanguage))
{
}

CLocale::CLocale(const CString & language)
    :   m_Language(language),
        m_Country(),
        m_pVariant(NULL)
{
}

CLocale::CLocale(const CLanguage & language, const CCountry & country)
    :   m_Language(language),
        m_Country(country),
        m_pVariant(NULL)
{   //  Adjust for sanity
    if (!m_Language.IsValid())
    {
        m_Country  = CCountry::Invalid;
    }
}

CLocale::CLocale(const CString & language, const CString & country)
    :   m_Language(language),
        m_Country(country),
        m_pVariant(NULL)
{   //  Adjust for sanity
    if (!m_Language.IsValid())
    {
        m_Country = CCountry::Invalid;
    }
}

CLocale::CLocale(const char * pLanguage, const char * pCountry)
    :   CLocale(CString(pLanguage), CString(pCountry))
{
}

CLocale::CLocale(const CLanguage & language, const CCountry & country, const CString & variant)
    :   m_Language(language),
        m_Country(country),
        m_pVariant(new CString(variant))
{   
    //  Adjust for sanity
    _NormalizeVariant();
    if (!m_Language.IsValid())
    {
        m_Country = CCountry::Invalid;
    }
    if (!m_Country.IsValid() || m_pVariant->GetLength() == 0)
    {
        delete m_pVariant;
        m_pVariant = NULL;
    }
}

CLocale::CLocale(const CString & language, const CString & country, const CString & variant)
    :   m_Language(language),
        m_Country(country),
        m_pVariant(new CString(variant))
{
    //  Adjust for sanity
    _NormalizeVariant();
    if (!m_Language.IsValid())
    {
        m_Country = CCountry::Invalid;
    }
    if (!m_Country.IsValid() || m_pVariant->GetLength() == 0)
    {
        delete m_pVariant;
        m_pVariant = NULL;
    }
}

CLocale::CLocale(const char * pLanguage, const char * pCountry, const char * pVariant)
    :   CLocale(CString(pLanguage), CString(pCountry), CString(pVariant))
{
}

CLocale::~CLocale()
{
    delete m_pVariant;  //  "delete NULL" is safe
}

CLocale & CLocale::operator = (const CLocale & src)
{
    if (this != &src)
    {
        CString * pNewVariant = (src.m_pVariant == NULL) ? NULL : new CString(*src.m_pVariant); //  may throw
        m_Language = src.m_Language;
        m_Country = src.m_Country;
        delete m_pVariant;  //  "delete NULL" is safe
        m_pVariant = pNewVariant;
    }
    return *this;
}

//////////
//  Operators
bool CLocale::operator == (const CLocale & op2) const
{
    return m_Language == op2.m_Language &&
           m_Country == op2.m_Country &&
           ((m_pVariant == NULL && op2.m_pVariant == NULL) ||
            (m_pVariant != NULL && op2.m_pVariant != NULL && *m_pVariant == *op2.m_pVariant));
}

//////////
//  Operations
CString CLocale::ToString() const
{
    if (!m_Language.IsValid())
    {
        return m_EmptyString;
    }
    else if (!m_Country.IsValid())
    {
        return m_Language.GetCode2();
    }
    else if (m_pVariant == NULL)
    {
        return m_Language.GetCode2() + _T("_") + m_Country.GetCode2();
    }
    else
    {
        return m_Language.GetCode2() + _T("_") + m_Country.GetCode2() + _T("_") + *m_pVariant;
    }
}

CLocale CLocale::FromString(const CString & s)
{
    if (s.GetLength() > 6 &&
        _istalpha(s[0]) && _istalpha(s[1]) &&
        s[2] == '_' &&
        _istalpha(s[3]) && _istalpha(s[4]) &&
        s[5] == '_')
    {   //  ll_CC_vvvvvv
        CString language(s.Mid(0, 2));
        CString country(s.Mid(3, 2));
        CString variant(s.Mid(6));
        return CLocale(language, country, variant);
    }
    else if (s.GetLength() == 5 &&
             _istalpha(s[0]) && _istalpha(s[1]) &&
             s[2] == '_' &&
             _istalpha(s[3]) && _istalpha(s[4]))
    {   //  ll_CC
        CString language(s.Mid(0, 2));
        CString country(s.Mid(3, 2));
        return CLocale(language, country);
    }
    else if (s.GetLength() == 2 &&
             _istalpha(s[0]) && _istalpha(s[1]))
    {   //  ll
        CString language(s.Mid(0, 2));
        return CLocale(language);
    }
    else
    {
        return Invariant;
    }
}

CLocale CLocale::GetSystem()
{
    static CLocale systemLocale;

    if (systemLocale == Invariant)
    {   //  Determine ONCE TODO properly
        systemLocale = CLocale("en", "GB");
    }
    return systemLocale;
}

CLocale CLocale::GetDefault()
{
    return m_Default;
}

void CLocale::GetDefault(const CLocale & locale)
{
    m_Default = locale;
}

//////////
//  Implementation helpers
void CLocale::_NormalizeVariant()
{
    if (m_pVariant != NULL)
    {
        m_pVariant->Trim();
        for (int i = 0; i < m_pVariant->GetLength(); i++)
        {
            if ((*m_pVariant)[i] <= 32 || (*m_pVariant)[i] >= 127)
            {
                m_pVariant->SetAt(i, '_');
            }
        }
    }
}

//  End of Tt3.Util/Locale.cpp
