//
//  Tt3.Util/Utf8CharacterSet.cpp
//  
//  CUtf8CharacterSet class implementation
//
//////////
#include "Tt3.Util/API.hpp"
USING_TT3_NAMESPACE

//////////
//  Singleton
IMPLEMENT_SINGLETON(CUtf8CharacterSet)

CUtf8CharacterSet::CUtf8CharacterSet()
    :   m_CanonicalName(_T("UTF-8")),
        m_PreferredName(_T("UTF-8"))
{
    m_AllNames.Add(_T("UTF-8"));
}

CUtf8CharacterSet::~CUtf8CharacterSet() {}

//////////
//  CUtf8CharacterSet::_Encoder
int CUtf8CharacterSet::_Encoder::GetEncodingLength(TCHAR c)
{
    if ((c & 0x007F) == c)
    {
        return 1;
    }
    else if ((c & 0x07FF) == c)
    {
        return 2;
    }
    else
    {
        return 3;
    }
}

size_t CUtf8CharacterSet::_Encoder::Encode(const TCHAR* pChars, size_t numChars, IDataDestination<BYTE>& destination)
{
    ASSERT(pChars != NULL);

    for (size_t i = 0; i < numChars; i++)
    {
        TCHAR c = pChars[i];
        if ((c & 0x007F) == c)
        {
            if (!destination.Send(BYTE(c)))
            {
                return i;
            }
        }
        else if ((c & 0x07FF) == c)
        {
            if (!destination.Send(BYTE(0xC0 | (c >> 6))) ||
                !destination.Send(BYTE(0x80 | (c & 0x3F))))
            {
                return -1;
            }
        }
        else
        {
            if (!destination.Send(BYTE(0xE0 | (c >> 12))) ||
                !destination.Send(BYTE(0x80 | ((c >> 6) & 0x3F))) ||
                !destination.Send(BYTE(0x80 | (c & 0x3F))))
            {
                return -1;
            }
        }
    }
    return numChars;
}

//////////
//  CUtf8CharacterSet::_Decoder
int CUtf8CharacterSet::_Decoder::GetEncodingLength(const BYTE* pBytes, size_t numBytes)
{
    ASSERT(pBytes != NULL);

    if (numBytes >= 1 && (pBytes[0] & 0x80) == 0)
    {
        return 1;
    }
    else if (numBytes >= 2 && (pBytes[0] & 0xE0) == 0xC0 && (pBytes[1] & 0xC0) == 0x80)
    {
        return 2;
    }
    else if (numBytes >= 3 && (pBytes[0] & 0xF0) == 0xE0 && (pBytes[1] & 0xC0) == 0x80 && (pBytes[2] & 0xC0) == 0x80)
    {
        return 3;
    }
    else
    {
        return -1;
    }
}

size_t CUtf8CharacterSet::_Decoder::Decode(const BYTE* pBytes, size_t numBytes, IDataDestination<TCHAR>& destination)
{
    ASSERT(pBytes != NULL);

    for (size_t scan = 0; scan < numBytes; )
    {
        if ((numBytes - scan) >= 1 && (pBytes[scan] & 0x80) == 0)
        {
            TCHAR c = pBytes[scan];
            if (!destination.Send(c))
            {
                return scan;
            }
            scan += 1;
        }
        else if ((numBytes - scan) >= 2 && (pBytes[scan] & 0xE0) == 0xC0 && (pBytes[scan + 1] & 0xC0) == 0x80)
        {
            TCHAR c = ((pBytes[scan] & 0x1F) << 6) |
                      (pBytes[scan + 1] & 0x3F);
            if (!destination.Send(c))
            {
                return scan;
            }
            scan += 2;
        }
        else if ((numBytes - scan) >= 3 && (pBytes[scan] & 0xF0) == 0xE0 && (pBytes[scan + 1] & 0xC0) == 0x80 && (pBytes[scan + 2] & 0xC0) == 0x80)
        {
            TCHAR c = ((pBytes[scan] & 0x0F) << 12) |
                      ((pBytes[scan + 1] & 0x3F) << 6) |
                      (pBytes[scan + 2] & 0x3F);
            if (!destination.Send(c))
            {
                return scan;
            }
            scan += 3;
        }
        else
        {
            return scan;
        }
    }
    return numBytes;
}

//  End of Tt3.Util/Utf8CharacterSet.cpp
