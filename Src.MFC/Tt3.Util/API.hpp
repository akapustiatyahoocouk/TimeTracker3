//
//  Tt3.Util/API.hpp
//  
//  Tt3.Util master header
//
//////////
#pragma once

//////////
//  MFC + Win32 + CRT
#define _CRT_SECURE_NO_WARNINGS
#include <memory>

#include <SDKDDKVer.h>  //  defines the highest available Windows platform.
#include <Winsock2.h>
#include <Windows.h>

#ifndef VC_EXTRALEAN
    #define VC_EXTRALEAN    // Exclude rarely-used stuff from Windows headers
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

#include <afxwin.h>         //  MFC core and standard components
#include <afxext.h>         //  MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         //  MFC OLE classes
#include <afxodlgs.h>       //  MFC OLE dialog classes
#include <afxdisp.h>        //  MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT

//#ifndef _AFX_NO_DB_SUPPORT
//#include <afxdb.h>          //  MFC ODBC database classes
//#endif // _AFX_NO_DB_SUPPORT

//#ifndef _AFX_NO_DAO_SUPPORT
//#include <afxdao.h>         //  MFC DAO database classes
//#endif // _AFX_NO_DAO_SUPPORT

//#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>         //  MFC support for Windows Common Controls
//#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxsock.h>        //  MFC socket extensions
#include <afxcontrolbars.h> //  MFC support for ribbons and control bars

#include "afxdialogex.h"

//////////
//  Tt3.Util components
#include "Tt3.Util/Linkage.hpp"
#include "Tt3.Util/Macros.hpp"

#include "Tt3.Util/Exceptions.hpp"
#include "Tt3.Util/Algorithms.hpp"
#include "Tt3.Util/ToString.hpp"
#include "Tt3.Util/Iterator.hpp"
#include "Tt3.Util/Collection.hpp"
#include "Tt3.Util/Vector.hpp"
#include "Tt3.Util/Dictionary.hpp"
#include "Tt3.Util/Stack.hpp"
#include "Tt3.Util/Set.hpp"

#include "Tt3.Util/Streaming.hpp"

#include "Tt3.Util/Locale.hpp"
#include "Tt3.Util/CharacterSet.hpp"
#include "Tt3.Util/Cryptography.hpp"

#include "Tt3.Util/Io.hpp"

#include "Tt3.Util/Module.hpp"
#include "Tt3.Util/PluginManager.hpp"

//  End of Tt3.Util/API.hpp
