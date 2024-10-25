//
//  Tt3.Util/TextInputStream.cpp
//  
//  CTextInputStream class implementation
//
//////////
#include "Tt3.Util/API.hpp"
USING_TT3_NAMESPACE

//////////
//  Construction/destruction
CTextInputStream::CTextInputStream(IBinaryInput * pSource, const CString & characterSetName) throws(CIoException *)
    :   m_pSource(pSource),
        m_ContextName((pSource != NULL) ? pSource->GetContextName() : CString(_T("NULL"))),
        m_pDecoder((CCharacterSet::Find(characterSetName) != NULL) ? CCharacterSet::Find(characterSetName)->CreateDecoder() : NULL)
{
    ASSERT(m_pSource != NULL);
    if (m_pDecoder == NULL)
    {
        throw new CNoSuchItemError(_T("character set"), characterSetName);
    }
}

CTextInputStream::CTextInputStream(IBinaryInput * pSource, CCharacterSet * pCharacterSet)
    :   m_pSource(pSource),
        m_ContextName((pSource != NULL) ? pSource->GetContextName() : CString(_T("NULL"))),
        m_pDecoder((pCharacterSet != NULL) ? pCharacterSet->CreateDecoder() : NULL)
{
    ASSERT(m_pSource != NULL);
    ASSERT(m_pDecoder != NULL);
}

CTextInputStream::~CTextInputStream()
{
    try
    {
        Close();
    }
    catch (...)
    {   //  destructors must be noexcept
    }
    delete m_pDecoder;  //  "delete NULL" is safe
}

//////////
//  IIo
IIo::Handle CTextInputStream::GetNativeHandle() const
{
    return (m_pSource == NULL) ? InvalidHandle : m_pSource->GetNativeHandle();
}

bool CTextInputStream::IsOpen() const
{
    return m_pSource != NULL;
}

void CTextInputStream::Close() throws(CIoException *)
{
    m_pSource = NULL;
}

CString CTextInputStream::GetContextName() const
{
    return m_ContextName;
}

//////////
//  IInput
bool CTextInputStream::IsReady() throws(CIoException *)
{
    if (m_pSource == NULL)
    {
        throw new CIoException(m_ContextName + _T(": Buffered input stream closed"));
    }
    return m_NumPrereadChars > 0 || m_pSource->IsReady();
}

//////////
//  ITextInput
int CTextInputStream::Read() throws(CIoException *)
{
    static size_t EolnLength = _tcslen(CTextOutputStream::m_pEoln);

    int c;
    for (; ; )
    {   //  If there are no pre-read characters, pre-read ONE
        if (m_NumPrereadChars == 0)
        {
            c = _ReadRaw();
            if (c == -1)
            {   //  EOF
                return -1;
            }
            m_PrereadChars[m_NumPrereadChars++] = (TCHAR)c;
            continue;
        }
        //  There IS a pre-read character. If it does NOT start an 
        //  EOLN sequence, consume it NOW
        if (m_PrereadChars[0] != CTextOutputStream::m_pEoln[0])
        {
            TCHAR result = m_PrereadChars[0];
            memmove(m_PrereadChars, m_PrereadChars + 1, (m_NumPrereadChars - 1) * sizeof(TCHAR));
            m_NumPrereadChars--;
            return result;
        }
        //  The 1st pre-read char starts tyhe EOLN sequence
        if (m_NumPrereadChars < EolnLength)
        {   //  Need to read enough to check for that EOLN sequencw
            c = _ReadRaw();
            if (c == -1)
            {   //  OOPS! EOF! we'll never read enough characters for an EOLN
                TCHAR result = m_PrereadChars[0];
                memmove(m_PrereadChars, m_PrereadChars + 1, (m_NumPrereadChars - 1) * sizeof(TCHAR));
                m_NumPrereadChars--;
                return result;
            }
            m_PrereadChars[m_NumPrereadChars++] = (TCHAR)c;
            continue;
        }
        else
        {   //  The 1st pre-read character starts an EOLN sequence, and
            //  we have enough characters pre-read to check for EOLN
            if (memcmp(m_PrereadChars, CTextOutputStream::m_pEoln, sizeof(TCHAR) * EolnLength) == 0)
            {   //  EOLN!
                memmove(m_PrereadChars, m_PrereadChars + EolnLength, (m_NumPrereadChars - EolnLength) * sizeof(TCHAR));
                m_NumPrereadChars -= EolnLength;
                return '\n';
            }
            else
            {   //  Consume the 1st cpre-read character only
                TCHAR result = m_PrereadChars[0];
                memmove(m_PrereadChars, m_PrereadChars + 1, (m_NumPrereadChars - 1) * sizeof(TCHAR));
                m_NumPrereadChars--;
                return result;
            }
        }
    }
}

size_t CTextInputStream::Read(TCHAR * pChars, size_t maxCharsToRead) throws(CIoException *)
{
    ASSERT(pChars != NULL);

    for (size_t i = 0; i < maxCharsToRead; i++)
    {
        int c = Read();
        if (c == -1)
        {
            return i;
        }
        pChars[i] = (TCHAR)c;
    }
    return maxCharsToRead;
}

//////////
//  Implementation helpers
int CTextInputStream::_ReadRaw() throws(CIoException *)
{
    for (; ; )
    {
        if (m_NumPrereadBytes >= m_pDecoder->GetCharacterSet()->GetMaximumEncodingLength())
        {   //  We MUST consume at least 1 byte!
            class Destination : public IDataDestination<TCHAR>
            {
            public:
                explicit Destination(CTextInputStream* pTextInputStream) : m_pTextInputStream(pTextInputStream) {}
                virtual bool Send(const TCHAR& item) override 
                { 
                    ASSERT(m_pTextInputStream->m_NumPrereadChars < sizeof(m_pTextInputStream->m_PrereadChars) / sizeof(m_pTextInputStream->m_PrereadChars[0]));
                    m_pTextInputStream->m_PrereadChars[m_pTextInputStream->m_NumPrereadChars++] = item;
                    return true;
                }
            private:
                CTextInputStream* m_pTextInputStream;
            };

            Destination destination(this);
            size_t bytesConsumed = m_pDecoder->Decode(m_PrereadBytes, m_NumPrereadBytes, destination);
            if (bytesConsumed > 0)
            {   //  Some character(s!) have been decoded - consume the bytes it was made of...
                ASSERT(bytesConsumed <= m_NumPrereadBytes);
                memmove(m_PrereadBytes, m_PrereadBytes + bytesConsumed, m_NumPrereadBytes - bytesConsumed);
                m_NumPrereadBytes -= bytesConsumed;
                //  ...and return that character
                TCHAR c = m_PrereadChars[m_NumPrereadChars];
                memmove(m_PrereadChars, m_PrereadChars + 1, sizeof(TCHAR) * (m_NumPrereadChars - 1));
                return c;
            }
            else
            {   //  OOPS! No character has been decoded - consume 1 byte...
                memmove(m_PrereadBytes, m_PrereadBytes + 1, m_NumPrereadBytes - 1);
                m_NumPrereadBytes--;
                //  ...and pretend we've recognized a "default" character
                return m_pDecoder->GetCharacterSet()->GetDefaultCharacter();
            }
        }
        //  Maybe we don't have enough bytes to represent a character?
        int b = m_pSource->Read();
        if (b == -1)
        {   //  ...and we'll never have any more of those bytes...
            if (m_NumPrereadBytes > 0)
            {   //  ...so consume the 1st pre-read byte...
                memmove(m_PrereadBytes, m_PrereadBytes + 1, m_NumPrereadBytes - 1);
                m_NumPrereadBytes--;
                //  ...and pretend we've recognized a "default" character
                return m_pDecoder->GetCharacterSet()->GetDefaultCharacter();
            }
            else
            {   //  no pre-read bytes, no new byte - we've hit an EOF
                return -1;
            }
        }
        else
        {   //  ...and we have one more byte
            m_PrereadBytes[m_NumPrereadBytes++] = (BYTE)b;
        }
    }
}

//  End of Tt3.Util/TextInputStream.cpp
