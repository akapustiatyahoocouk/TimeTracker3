//
//  Tt3.Util/Language.cpp
//  
//  CLanguage class implementation
//
//////////
#include "Tt3.Util/API.hpp"
USING_TT3_NAMESPACE

const CLanguage CLanguage::Invalid;

const TCHAR* const CLanguage::m_pInvalidLanguageCode2 = _T("xx");
const TCHAR* const CLanguage::m_pInvalidLanguageCode3 = _T("xxx");

//////////
//  Construction/destruction
CLanguage::CLanguage()
{
    _InitRegistry();

    ASSERT(_Registry().ContainsKey(m_pInvalidLanguageCode2));
    m_pImpl = _Registry()[m_pInvalidLanguageCode2];
}

CLanguage::CLanguage(const CString & code2)
{
    _InitRegistry();

    if (code2.GetLength() == 2 && _istalpha(code2[0]) && _istalpha(code2[1]))
    {
        CString key = code2;
        key.MakeLower();
        m_pImpl = _Registry().ContainsKey(key) ? _Registry()[key] : _Registry()[m_pInvalidLanguageCode2];
    }
    else
    {
        m_pImpl = _Registry()[m_pInvalidLanguageCode2];
    }
}

CLanguage::CLanguage(const char * pCode2)
    :   CLanguage(CString(pCode2))
{
}

//////////
//  Implementation helpers
CDictionary<CString, CLanguage::_Impl*>& CLanguage::_Registry()
{
    static CDictionary<CString, CLanguage::_Impl*> theRegistry;
    return theRegistry;
}

void CLanguage::_InitRegistry()
{
    if (_Registry().GetSize() == 0)
    {   //  Initialize ONCE
        _Registry()[m_pInvalidLanguageCode2] = new _Impl(m_pInvalidLanguageCode2, m_pInvalidLanguageCode3, _T("Invalid"));
        _Registry()[_T("en")] = new _Impl(_T("en"), _T("eng"), _T("English"));
    }
}

//  End of Tt3.Util/Language.cpp
