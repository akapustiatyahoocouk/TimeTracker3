//
//  Tt3.Util/FileInputStream.cpp
//  
//  CFileInputStream class implementation
//
//////////
#include "Tt3.Util/API.hpp"
USING_TT3_NAMESPACE

//////////
//  Construction/destruction
CFileInputStream::CFileInputStream(const CString & fileName) throws(CIoException *)
    :   m_FileName(fileName),
        m_Handle(INVALID_HANDLE_VALUE)
{
    m_Handle = ::CreateFile(fileName,
                            GENERIC_READ,
                            FILE_SHARE_READ,
                            NULL,
                            OPEN_EXISTING,
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

CFileInputStream::~CFileInputStream()
{
    if (m_Handle != INVALID_HANDLE_VALUE)
    {
        ::CloseHandle(m_Handle);    //  destructors must be noexcept
    }
}

//////////
//  IIo
IIo::Handle CFileInputStream::GetNativeHandle() const
{
    return reinterpret_cast<IIo::Handle>(m_Handle);
}

bool CFileInputStream::IsOpen() const
{
    return m_Handle != INVALID_HANDLE_VALUE;
}

void CFileInputStream::Close() throws(CIoException *)
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

CString CFileInputStream::GetContextName() const
{
    return m_FileName;
}

//////////
//  IInput
bool CFileInputStream::IsReady() throws(CIoException *)
{
    if (m_Handle == INVALID_HANDLE_VALUE)
    {
        throw new CIoException(m_FileName + _T(": File stream closed"));
    }
    //  Retrieve the current "file pointer"
    LARGE_INTEGER distance, currentFilePointer;
    distance.QuadPart = 0;
    if (!::SetFilePointerEx(m_Handle, distance, &currentFilePointer, FILE_CURRENT))
    {   //  OOPS! Something is wrong!
        throw new CIoException(m_FileName + _T(": ") + GetWin32ErrorMessage(::GetLastError()));
    }
    LARGE_INTEGER endFilePointer;
    //  Go to the end of the file
    if (!::SetFilePointerEx(m_Handle, distance, &endFilePointer, FILE_END))
    {   //  OOPS! Something is wrong!
        throw new CIoException(m_FileName + _T(": ") + GetWin32ErrorMessage(::GetLastError()));
    }
    //  Return to the original "current" position
    if (!::SetFilePointerEx(m_Handle, currentFilePointer, &currentFilePointer, FILE_BEGIN))
    {   //  OOPS! Something is wrong!
        throw new CIoException(m_FileName + _T(": ") + GetWin32ErrorMessage(::GetLastError()));
    }
    //  Now we can determine the result
    return endFilePointer.QuadPart > currentFilePointer.QuadPart;
}

//////////
//  IBinaryInput
int CFileInputStream::Read() throws(CIoException *)
{
    BYTE b;
    if (Read(&b, 1) == 1)
    {
        return b;
    }
    return -1;
}

size_t CFileInputStream::Read(void * pData, size_t maxBytesToRead) throws(CIoException *)
{
    ASSERT(pData != NULL);

    if (m_Handle == INVALID_HANDLE_VALUE)
    {
        throw new CIoException(m_FileName + _T(": File stream closed"));
    }
    if ((DWORD)maxBytesToRead == maxBytesToRead)
    {   //  Read in one go
        DWORD bytesRead;
        if (!::ReadFile(m_Handle, pData, (DWORD)maxBytesToRead, &bytesRead, NULL))
        {   //  OOPS!
            throw new CIoException(m_FileName + _T(": ") + GetWin32ErrorMessage(::GetLastError()));
        }
        return bytesRead;
    }
    else
    {   //  Split a long read into two
        size_t chunkSize1 = maxBytesToRead / 2, 
               chunkSize2 = maxBytesToRead - chunkSize1;
        size_t bytesRead1 = Read(pData, chunkSize1);
        if (bytesRead1 < chunkSize1)
        {   //  No point in reading chunk #2
            return bytesRead1;
        }
        size_t bytesRead2 = Read((BYTE*)pData + chunkSize1, chunkSize2);
        return bytesRead1 + bytesRead2;
    }
}

//  End of Tt3.Util/FileInputStream.cpp
