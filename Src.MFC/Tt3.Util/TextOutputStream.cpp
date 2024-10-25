//
//  Tt3.Util/TextOutputStream.cpp
//  
//  CTextOutputStream class implementation
//
//////////
#include "Tt3.Util/API.hpp"
USING_TT3_NAMESPACE

const TCHAR * const CTextOutputStream::m_pEoln = _T("\r\n");

//////////
//  Construction/destruction
CTextOutputStream::CTextOutputStream(IBinaryOutput* pDestination, const char* pCharacterSetName) throws(CIoException*)
    :   m_pDestination(pDestination),
        m_ContextName((pDestination != NULL) ? pDestination->GetContextName() : CString(_T("NULL"))),
        m_pEncoder((CCharacterSet::Find(pCharacterSetName) != NULL) ? CCharacterSet::Find(pCharacterSetName)->CreateEncoder() : NULL),
        m_NumEncodedBytes(0),
        m_ByteDestination(this)
{
    ASSERT(m_pDestination != NULL);
    if (m_pEncoder == NULL)
    {
        throw new CNoSuchItemError("character set", pCharacterSetName);
    }
}

CTextOutputStream::CTextOutputStream(IBinaryOutput * pDestination, const CString & characterSetName) throws(CIoException *)
    :   m_pDestination(pDestination),
        m_ContextName((pDestination != NULL) ? pDestination->GetContextName() : CString(_T("NULL"))),
        m_pEncoder((CCharacterSet::Find(characterSetName) != NULL) ? CCharacterSet::Find(characterSetName)->CreateEncoder() : NULL),
        m_NumEncodedBytes(0),
        m_ByteDestination(this)
{
    ASSERT(m_pDestination != NULL);
    if (m_pEncoder == NULL)
    {
        throw new CNoSuchItemError("character set", characterSetName);
    }
}

CTextOutputStream::CTextOutputStream(IBinaryOutput * pDestination, CCharacterSet * pCharacterSet)
    :   m_pDestination(pDestination),
        m_ContextName((pDestination != NULL) ? pDestination->GetContextName() : CString(_T("NULL"))),
        m_pEncoder((pCharacterSet != NULL) ? pCharacterSet->CreateEncoder() : NULL),
        m_NumEncodedBytes(0),
        m_ByteDestination(this)
{
    ASSERT(m_pDestination != NULL);
    ASSERT(m_pEncoder != NULL);
}

CTextOutputStream::~CTextOutputStream()
{
    try
    {
        Close();
    }
    catch (...)
    {   //  destructors must be noexcept
    }
    delete m_pEncoder;  //  "delete NULL" is safe
}

//////////
//  IIo
IIo::Handle CTextOutputStream::GetNativeHandle() const
{
    return (m_pDestination == NULL) ? InvalidHandle : m_pDestination->GetNativeHandle();
}

bool CTextOutputStream::IsOpen() const
{
    return m_pDestination != NULL;
}

void CTextOutputStream::Close() throws(CIoException *)
{
    if (m_pDestination != NULL)
    {   //  not yet closed
        _FlushEncodedBytes();   //  may throw!
        m_pDestination = NULL;
    }
}

CString CTextOutputStream::GetContextName() const
{
    return m_ContextName;
}

//////////
//  IOutput
void CTextOutputStream::Flush() throws(CIoException *)
{
    if (m_pDestination == NULL)
    {
        throw new CIoException(m_ContextName + _T(": Text output stream closed"));
    }
    _FlushEncodedBytes();       //  may throw
    m_pDestination->Flush();    //  may throw
}

//////////
//  ITextOutput
void CTextOutputStream::Write(TCHAR c) throws(CIoException *)
{
    if (m_pDestination == NULL)
    {
        throw new CIoException(m_ContextName + _T(": Text output stream closed"));
    }
    if (c == '\n')
    {
        for (size_t i = 0; m_pEoln[i] != 0; i++)
        {
            _WriteRaw(m_pEoln[i]);
        }
        //  TODO uncomment ? _FlushEncodedBytes();
    }
    else
    {
        _WriteRaw(c);
    }
    //  TODO uncomment ? _FlushEncodedBytes();
}

void CTextOutputStream::Write(const TCHAR * pChars, size_t charsToWrite) throws(CIoException *)
{
    ASSERT(pChars != NULL);

    for (size_t i = 0; i < charsToWrite; i++)
    {
        Write(pChars[i]);
    }
    //  TODO uncomment ? _FlushEncodedBytes();
}

//////////
//  Implementation helpers
void CTextOutputStream::_WriteRaw(TCHAR c) throws(CIoException *)
{
    if (m_pEncoder->Encode(&c, 1, m_ByteDestination) == 0)
    {   //  OOPS! Character not encodable!
        TCHAR dc = m_pEncoder->GetCharacterSet()->GetDefaultCharacter();
        m_pEncoder->Encode(&dc, 1, m_ByteDestination);  //  default character is always encodable by definition
    }
}

void CTextOutputStream::_FlushEncodedBytes() throws(CIoException *)
{
    ASSERT(m_pDestination != NULL);

    if (m_NumEncodedBytes > 0)
    {
        try
        {
            m_pDestination->Write(m_EncodedBytes, m_NumEncodedBytes);   //  may throw
            m_NumEncodedBytes = 0;
        }
        catch (...)
        {   //  OOPS! Mark as "flushed" and re-throw
            m_NumEncodedBytes = 0;
            throw;
        }
    }
}

//  End of Tt3.Util/TextOutputStream.cpp
