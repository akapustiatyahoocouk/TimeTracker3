//
//  Tt3.UnitTests/API.hpp
//  
//  Tt3 UnitTests master header
//
//////////
#pragma once

//////////
//  Dependencies
#include "Tt3.Util/API.hpp"

#ifdef _UNICODE
    #if defined _M_IX86
        #pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
    #elif defined _M_X64
        #pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
    #else
        #pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
    #endif
#endif

//////////
//  Tt3.UnitTests components
#include "Tt3.UnitTests/Exceptions.hpp"
#include "Tt3.UnitTests/CharacterSet.hpp"
#include "Tt3.UnitTests/Io.hpp"

//  End of Tt3.UnitTests/API.hpp
