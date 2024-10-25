//
//  Tt3.Util/Country.cpp
//  
//  CCountry class implementation
//
//////////
#include "Tt3.Util/API.hpp"
USING_TT3_NAMESPACE

const CCountry CCountry::Invalid;

const TCHAR* const  CCountry::m_pInvalidCountryCode2 = _T("XX");
const TCHAR* const  CCountry::m_pInvalidCountryCode3 = _T("XXX");
const int           CCountry::m_InvalidCountryCodeN = 0;

//////////
//  Construction/destruction
CCountry::CCountry()
{
    _InitRegistry();

    ASSERT(_Registry().ContainsKey(m_pInvalidCountryCode2));
    m_pImpl = _Registry()[m_pInvalidCountryCode2];
}

CCountry::CCountry(const CString & code2)
{
    _InitRegistry();

    if (code2.GetLength() == 2 && _istalpha(code2[0]) && _istalpha(code2[1]))
    {
        CString key = code2;
        key.MakeUpper();
        m_pImpl = _Registry().ContainsKey(key) ? _Registry()[key] : _Registry()[m_pInvalidCountryCode2];
    }
    else
    {
        m_pImpl = _Registry()[m_pInvalidCountryCode2];
    }
}

CCountry::CCountry(const char * pCode2)
    :   CCountry(CString(pCode2))
{
}

//////////
//  Implementation helpers
CDictionary<CString, CCountry::_Impl*>& CCountry::_Registry()
{
    static CDictionary<CString, CCountry::_Impl*> theRegistry;
    return theRegistry;
}

void CCountry::_InitRegistry()
{
    if (_Registry().GetSize() == 0)
    {   //  Initialize ONCE
        _Registry()[m_pInvalidCountryCode2] = new _Impl(m_pInvalidCountryCode2, m_pInvalidCountryCode3, m_InvalidCountryCodeN, _T("Invalid"));
        _Registry()[_T("GB")] = new _Impl(_T("GB"), _T("GBR"), 826, _T("United Kingdom"));
    }
}

//  End of Tt3.Util/Country.cpp
