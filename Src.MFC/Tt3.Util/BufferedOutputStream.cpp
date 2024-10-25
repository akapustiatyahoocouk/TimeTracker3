//
//  Tt3.Util/BufferedOutputStream.cpp
//  
//  CBufferedOutputStream class implementation
//
//////////
#include "Tt3.Util/API.hpp"
USING_TT3_NAMESPACE

//////////
//  Construction/destruction
CBufferedOutputStream::CBufferedOutputStream(IBinaryOutput * pDestination, size_t bufferSize)
    :   m_pDestination(pDestination),
        m_ContextName((pDestination != NULL) ? pDestination->GetContextName() : CString(_T("NULL"))),
        m_BufferSize(bufferSize),
        m_pBuffer(new BYTE[bufferSize]),
        m_DataSize(0)
{
    ASSERT(m_pDestination != NULL);
    ASSERT(m_BufferSize != 0);
}

CBufferedOutputStream::~CBufferedOutputStream()
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
IIo::Handle CBufferedOutputStream::GetNativeHandle() const
{
    return (m_pDestination == NULL) ? InvalidHandle : m_pDestination->GetNativeHandle();
}

bool CBufferedOutputStream::IsOpen() const
{
    return m_pDestination != NULL;
}

void CBufferedOutputStream::Close() throws(CIoException *)
{
    if (m_pDestination != NULL)
    {
        Flush();
        m_pDestination = NULL;
    }
}

CString CBufferedOutputStream::GetContextName() const
{
    return m_ContextName;
}

//////////
//  IOutput
void CBufferedOutputStream::Flush() throws(CIoException *)
{
    if (m_pDestination == NULL)
    {
        throw new CIoException(m_ContextName + _T(": Buffered output stream closed"));
    }
    _FlushBuffer();
    m_pDestination->Flush();
}

//////////
//  IBinaryOutput
void CBufferedOutputStream::Write(BYTE b) throws(CIoException *)
{
    Write(&b, 1);
}

void CBufferedOutputStream::Write(const void * pData, size_t bytesToWrite) throws(CIoException *)
{
    ASSERT(pData != NULL);

    if (m_pDestination == NULL)
    {
        throw new CIoException(m_ContextName + _T(": Buffered output stream closed"));
    }
    while (bytesToWrite > 0)
    {   //  Is the buffer full ?
        ASSERT(m_DataSize <= m_BufferSize);
        if (m_DataSize == m_BufferSize)
        {   //  Yes - sent its contents to the destination
            _FlushBuffer();
            ASSERT(m_DataSize == 0);
        }
        //  How much data can we stick into the buffer now ?
        size_t chunkSize = min(bytesToWrite, m_BufferSize - m_DataSize);
        //  Move a chunk of data into the buffer...
        memcpy(m_pBuffer + m_DataSize, pData, chunkSize);
        //  ...and advance past the moved chunk in the input
        m_DataSize += chunkSize;
        pData = (char *)pData + chunkSize;
        bytesToWrite -= chunkSize;
    }
}

//////////
//  Implementation helpers
void CBufferedOutputStream::_FlushBuffer() throws(CIoException *)
{
    ASSERT(m_pDestination != NULL);

    if (m_DataSize > 0)
    {
        m_pDestination->Write(m_pBuffer, m_DataSize);
        m_DataSize = 0;
    }
}

//  End of Tt3.Util/BufferedOutputStream.cpp
