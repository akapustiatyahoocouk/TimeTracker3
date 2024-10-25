//
//  Tt3.Util/FileOutputStream.cpp
//  
//  CFileOutputStream class implementation
//
//////////
#include "Tt3.Util/API.hpp"
USING_TT3_NAMESPACE

//////////
//  Construction/destruction
CFileOutputStream::CFileOutputStream(const CString & fileName, OpenMode openMode) throws(CIoException *)
    :   m_FileName(fileName),
        m_Handle(INVALID_HANDLE_VALUE)
{
    m_Handle = ::CreateFile(fileName,
                            GENERIC_WRITE,
                            0,
                            NULL,
                            (DWORD)openMode,
                            FILE_ATTRIBUTE_NORMAL,
                            NULL);
    if (m_Handle == INVALID_HANDLE_VALUE)
    {   //  OOPS! Why?
        if (::GetLastError() == ERROR_FILE_NOT_FOUND)
        {   //  A special case - need a special I/O exception
            throw new CFileNotFoundException(fileName);
        }
        throw new CIoException(fileName + _T(": ") + GetWin32ErrorMessage(::GetLastError()));
    }
}

CFileOutputStream::~CFileOutputStream()
{
    if (m_Handle != INVALID_HANDLE_VALUE)
    {
        ::CloseHandle(m_Handle);    //  destructors must be noexcept
    }
}

//////////
//  IIo
IIo::Handle CFileOutputStream::GetNativeHandle() const
{
    return reinterpret_cast<IIo::Handle>(m_Handle);
}

bool CFileOutputStream::IsOpen() const
{
    return m_Handle != INVALID_HANDLE_VALUE;
}

void CFileOutputStream::Close() throws(CIoException *)
{
    if (m_Handle != INVALID_HANDLE_VALUE)
    {
        if (::CloseHandle(m_Handle))
        {   //  Handle closed successfully
            m_Handle = INVALID_HANDLE_VALUE;
        }
        else
        {   //  OOPS! Close error!
            m_Handle = INVALID_HANDLE_VALUE;
            throw new CIoException(m_FileName + _T(": ") + GetWin32ErrorMessage(::GetLastError()));
        }
    }
}

CString CFileOutputStream::GetContextName() const
{
    return m_FileName;
}

//////////
//  IOutput
void CFileOutputStream::Flush() throws(CIoException *)
{
    if (m_Handle == INVALID_HANDLE_VALUE)
    {
        throw new CIoException(m_FileName + _T(": File stream closed"));
    }
    if (!::FlushFileBuffers(m_Handle))
    {   //  OOPS!
        throw new CIoException(m_FileName + _T(": ") + GetWin32ErrorMessage(::GetLastError()));
    }
}

//////////
//  IBinaryOutput
void CFileOutputStream::Write(BYTE b) throws(CIoException *)
{
    Write(&b, 1);
}

void CFileOutputStream::Write(const void * pData, size_t bytesToWrite) throws(CIoException *)
{
    ASSERT(pData != NULL);

    if (m_Handle == INVALID_HANDLE_VALUE)
    {
        throw new CIoException(m_FileName + _T(": File stream closed"));
    }
    if ((DWORD)bytesToWrite == bytesToWrite)
    {   //  Write in one go
        DWORD bytesWritten;
        if (!::WriteFile(m_Handle, pData, (DWORD)bytesToWrite, &bytesWritten, NULL))
        {   //  OOPS!
            throw new CIoException(m_FileName + _T(": ") + GetWin32ErrorMessage(::GetLastError()));
        }
        ASSERT(bytesWritten <= (DWORD)bytesToWrite);
        if (bytesWritten < (DWORD)bytesToWrite)
        {   //  OOPS!
            throw new CIoException(_T("Disk full"));
        }
    }
    else
    {   //  Split a long write into two
        size_t chunkSize1 = bytesToWrite / 2,
               chunkSize2 = bytesToWrite - chunkSize1;
        Write(pData, chunkSize1);
        Write((BYTE *)pData + chunkSize1, chunkSize2);
    }
}

//  End of Tt3.Util/FileOutputStream.cpp
