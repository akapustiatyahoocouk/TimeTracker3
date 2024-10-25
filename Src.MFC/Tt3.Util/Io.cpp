//
//  Tt3.Util/Io.cpp
//  
//  IIo interface implementation
//
//////////
#include "Tt3.Util/API.hpp"
USING_TT3_NAMESPACE

//////////
//  Helpers for derived classes
CString IIo::GetWin32ErrorMessage(DWORD errorCode)
{
    //  Ask Win32 to give us the string version of that message ID...
    LPTSTR messageBuffer = nullptr;
    DWORD size = ::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                 NULL, 
                                 errorCode, 
                                 MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
                                 (LPTSTR)&messageBuffer, 
                                 0, 
                                 NULL);
    //  ...copy the error message...
    CString message(messageBuffer, size);
    //  ...free the Win32's string's buffer...
    LocalFree(messageBuffer);
    //  ...and we're done
    return message;
}

//  End of Tt3.Util/Io.cpp
