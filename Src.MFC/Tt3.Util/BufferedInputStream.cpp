//
//  Tt3.Util/BufferedInputStream.cpp
//  
//  CBufferedInputStream class implementation
//
//////////
#include "Tt3.Util/API.hpp"
USING_TT3_NAMESPACE

//////////
//  Construction/destruction
CBufferedInputStream::CBufferedInputStream(IBinaryInput * pSource, size_t bufferSize)
    :   m_pSource(pSource),
        m_ContextName((pSource != NULL) ? pSource->GetContextName() : CString(_T("NULL"))),
        m_BufferSize(bufferSize),
        m_pBuffer(new BYTE[bufferSize]),
        m_DataSize(0),
        m_Scan(0)
{
    ASSERT(m_pSource != NULL);
    ASSERT(m_BufferSize != 0);
}

CBufferedInputStream::~CBufferedInputStream()
{
    try
    {
        Close();
    }
    catch (...)
    {   //  destructors must be noexcept
    }
    delete[] m_pBuffer;
}

//////////
//  IIo
IIo::Handle CBufferedInputStream::GetNativeHandle() const
{
    return (m_pSource == NULL) ? InvalidHandle : m_pSource->GetNativeHandle();
}

bool CBufferedInputStream::IsOpen() const
{
    return m_pSource != NULL;
}

void CBufferedInputStream::Close() throws(CIoException *)
{
    if (m_pSource != NULL)
    {
        m_pSource = NULL;
    }
}

CString CBufferedInputStream::GetContextName() const
{
    return m_ContextName;
}

//////////
//  IInput
bool CBufferedInputStream::IsReady() throws(CIoException *)
{
    if (m_pSource == NULL)
    {
        throw new CIoException(m_ContextName + _T(": Buffered input stream closed"));
    }
    return m_Scan < m_DataSize || m_pSource->IsReady();
}

//////////
//  IBinaryInput
int CBufferedInputStream::Read() throws(CIoException *)
{
    BYTE b;
    return (Read(&b, 1) == 1) ? b : -1;
}

size_t CBufferedInputStream::Read(void * pData, size_t maxBytesToRead) throws(CIoException *)
{
    ASSERT(pData != 0);

    if (m_pSource == NULL)
    {
        throw new CIoException(m_ContextName + _T(": Buffered input stream closed"));
    }
    BYTE* pBytes = (BYTE*)pData;
    size_t result = 0;
    while (maxBytesToRead > 0)
    {   //  How much bytes that are already in the buffer can we read ?
        ASSERT(m_DataSize <= m_BufferSize);
        ASSERT(m_Scan <= m_DataSize);
        size_t chunkSize = min(maxBytesToRead, m_DataSize - m_Scan);
        if (chunkSize > 0)
        {   //  Some!
            memcpy(pData, m_pBuffer + m_Scan, chunkSize);
            result += chunkSize;
            pData = (char*)pData + chunkSize;
            maxBytesToRead -= chunkSize;
            m_Scan += chunkSize;
            continue;
        }
        //  None - must refill the buffer
        _RefillBuffer();
        ASSERT(m_DataSize <= m_BufferSize);
        ASSERT(m_Scan <= m_DataSize);
        if (m_Scan == m_DataSize)
        {   //  OOPS! No more data!
            break;
        }
    }
    return result;
}

//////////
//  Implementation helpers
void CBufferedInputStream::_RefillBuffer() throws(CIoException *)
{
    m_Scan = m_DataSize = 0;    //  ...in case the Read() below throws
    m_DataSize = m_pSource->Read(m_pBuffer, m_BufferSize);
}

//  End of Tt3.Util/BufferedInputStream.cpp
