//
//  Tt3.Util/Iso88591CharacterSet.cpp
//  
//  CIso88591CharacterSet class implementation
//
//////////
#include "Tt3.Util/API.hpp"
USING_TT3_NAMESPACE

//////////
//  Singleton
IMPLEMENT_SINGLETON(CIso88591CharacterSet)

CIso88591CharacterSet::CIso88591CharacterSet() 
    :   m_CanonicalName(_T("ISO/IEC 8859-1:1998")),
        m_PreferredName(_T("ISO-8859-1"))
{
    m_AllNames.Add(_T("ISO/IEC 8859-1:1998"));
    m_AllNames.Add(_T("ISO-8859-1"));
    m_AllNames.Add(_T("iso-ir-100"));
    m_AllNames.Add(_T("csISOLatin1"));
    m_AllNames.Add(_T("latin1"));
    m_AllNames.Add(_T("l1"));
    m_AllNames.Add(_T("IBM819"));
    m_AllNames.Add(_T("CP819"));
}

CIso88591CharacterSet::~CIso88591CharacterSet() {}

//////////
//  CIso88591CharacterSet::_Encoder
int CIso88591CharacterSet::_Encoder::GetEncodingLength(TCHAR c)
{
    if ((c & 0x00FF) == c)
    {
        return 1;
    }
    return -1;
}

size_t CIso88591CharacterSet::_Encoder::Encode(const TCHAR* pChars, size_t numChars, IDataDestination<BYTE>& destination)
{
    ASSERT(pChars != NULL);

    for (size_t i = 0; i < numChars; i++)
    {
        TCHAR c = pChars[i];
        if ((c & 0x00FF) != c)
        {   //  OOPS! Can't encode
            return i;
        }
        if (!destination.Send(BYTE(c)))
        {   //  OOPS! Can't send!
            return i;
        }
    }
    return numChars;
}

//////////
//  CIso88591CharacterSet::_Decoder
int CIso88591CharacterSet::_Decoder::GetEncodingLength(const BYTE* pBytes, size_t numBytes)
{
    ASSERT(pBytes != NULL);

    return (numBytes > 0) ? 1 : -1; //  any BYTE can be treated as a TCHAR
}

size_t CIso88591CharacterSet::_Decoder::Decode(const BYTE* pBytes, size_t numBytes, IDataDestination<TCHAR>& destination)
{
    ASSERT(pBytes != NULL);

    for (size_t i = 0; i < numBytes; i++)
    {
        destination.Send(TCHAR(pBytes[i]));
    }
    return numBytes;
}

//  End of Tt3.Util/Iso88591CharacterSet.cpp
