//
//  Tt3.Util/Plugin.cpp
//  
//  CPlugin class implementation
//
//////////
#include "Tt3.Util/API.hpp"
USING_TT3_NAMESPACE

//////////
//  Construction/destruction
CPlugin::CPlugin()
{
}

CPlugin::~CPlugin()
{
}

//////////
//  Operations
CString CPlugin::GetDisplayName() const
{
    const_cast<CPlugin*>(this)->_LoadPropertiesFromResources();
    return m_DisplayName;
}

CPlugin::Version CPlugin::GetVersion() const
{
    const_cast<CPlugin *>(this)->_LoadPropertiesFromResources();
    return m_Version;
}

CString CPlugin::GetCopyright() const
{
    const_cast<CPlugin *>(this)->_LoadPropertiesFromResources();
    return m_Copyright;
}

bool CPlugin::IsInitialized() const
{
    return CPluginManager::m_Plugins.GetAt(const_cast<CPlugin *>(this), false);
}

//////////
//  Implementation helpers
void CPlugin::_LoadPropertiesFromResources()
{
    if (m_PropertiesParsed)
    {   //  nothing to do
        return;
    }

    //  Resolve module HINSTANCE to module file name
    TCHAR szFileName[MAX_PATH + 1] = { 0 };
    if (::GetModuleFileName(NULL, szFileName, MAX_PATH) == 0)
    {   //  OOPS! TODO log ?
        return;
    }

    //  Allocate a block of memory for the version info
    DWORD dummy;
    DWORD dwSize = GetFileVersionInfoSize(szFileName, &dummy);
    if (dwSize == 0)
    {   //  OOPS! Not available! TODO log ?
        return;
    }
    BYTE * pData = new BYTE[dwSize];

    //  Load the version info
    if (!GetFileVersionInfo(szFileName, NULL, dwSize, pData))
    {   //  OOPS! TODO log ?
        delete[] pData;
        return;
    }
}

//  End of Tt3.Util/Plugin.cpp
