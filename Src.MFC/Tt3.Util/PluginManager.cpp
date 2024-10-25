//
//  Tt3.Util/PluginManager.cpp
//  
//  CPluginManager class implementation
//
//////////
#include "Tt3.Util/API.hpp"
USING_TT3_NAMESPACE

CMapStringToString CPluginManager::m_ProcessedDlls;
CDictionary<CPlugin *, bool> CPluginManager::m_Plugins;

//////////
//  Operations
void CPluginManager::LoadPlugins()
{
    //  Determine the startup directory to load DLLs from
    CString startupDirectory = _T(".");

    TCHAR exeFileName[_MAX_PATH];
    DWORD dw = ::GetModuleFileName(NULL, exeFileName, _MAX_PATH);
    if (dw > 0 && dw < _MAX_PATH)
    {
        for (int i = 0; exeFileName[i] != 0; i++)
        {
            if (exeFileName[i] == '/' || exeFileName[i] == '\\')
            {
                startupDirectory = CString(exeFileName, i);
            }
        }
    }

    //  Load the DLLs
    CFileFind finder;
    BOOL bWorking = finder.FindFile(startupDirectory + _T("\\*.dll"));
    while (bWorking)
    {
        bWorking = finder.FindNextFile();
        CString dllFileName = startupDirectory + _T("\\") + finder.GetFileName();
        //  Process every DLL only once
        CString dummy;
        if (m_ProcessedDlls.Lookup(dllFileName, dummy))
        {   //  Already processed earlier
            continue;
        }
        m_ProcessedDlls.SetAt(dllFileName, dllFileName);
        //  Load the DLL...
        HMODULE hDll = ::LoadLibrary(dllFileName);
        if (hDll == NULL)
        {   //  OOPS! TODO log ?
            continue;
        }
        //  Is there a plugin export proc ?
        typedef void (*Tt3GetPluginsProc)(CPluginList & pluginList);
        Tt3GetPluginsProc getPluginsProc = (Tt3GetPluginsProc)::GetProcAddress(hDll, "Tt3GetPlugins");
        if (getPluginsProc == NULL)
        {   //  No
            ::FreeLibrary(hDll);
            continue;
        }
        //  Record the plugins defined by this DLL
        CPluginList newPlugins;
        (*getPluginsProc)(newPlugins);
        for (size_t i = 0; i < newPlugins.GetSize(); i++)
        {
            m_Plugins[newPlugins[i]] = false;
        }
    }

    //  Now try to initialize the not-yet-initialized plugins
    for (bool keepGoing = true; keepGoing; )
    {
        keepGoing = false;  //  ...hoping this will be the last pass
        for (CPlugin * pPlugin : m_Plugins.Keys)
        {
            bool initialized = m_Plugins[pPlugin];
            if (!initialized)
            {
                try
                {
                    pPlugin->Initialize();  //  if this returns, plugin is now initialized
                    m_Plugins[pPlugin] = true;
                    keepGoing = true;
                }
                catch (...)
                {
                }
            }
        }
    }

    //  Go one last time over non-initialized plugins, logging error messages
    for (CPlugin * pPlugin : m_Plugins.Keys)
    {
        bool initialized = m_Plugins[pPlugin];
        if (!initialized)
        {
            try
            {
                pPlugin->Initialize();  //  if this returns, plugin is now initialized
                m_Plugins[pPlugin] = true;
            }
            catch (CException * pEx)
            {   //  TODO log
                pEx->Delete();
            }
            catch (...)
            {   //  TODO log
            }
        }
    }
}

CPluginList CPluginManager::GetLoadedPlugins()
{
    return CPluginList(m_Plugins.Keys);
}

CPluginList CPluginManager::GetInitializedPlugins()
{
    CPluginList result;
    for (CPlugin * pPlugin : m_Plugins.Keys)
    {
        if (m_Plugins[pPlugin])
        {
            result.Add(pPlugin);
        }
    }
    return result;
}

//  End of Tt3.Util/PluginManager.cpp
