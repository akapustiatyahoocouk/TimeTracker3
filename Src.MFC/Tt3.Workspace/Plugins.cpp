//
//  Tt3.Workspace/Plugins.cpp
//  
//  Tt3.Workspace plugins
//
//////////
#include "Tt3.Workspace/API.hpp"
USING_TT3_NAMESPACE

namespace
{
    class CWorkspacePlugin final : public CPlugin
    {
        DECLARE_SINGLETON(CWorkspacePlugin)

        //////////
        //  CPlugin
    public:
        virtual void        Initialize() throws(CException*)
        {   //  Nothing here TODO really ?
        }

    protected:
        virtual HINSTANCE   GetHInstance() const
        {
            DWORD flags = GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS;
            HMODULE hm = 0;
            ::GetModuleHandleEx(flags, reinterpret_cast<LPCTSTR>(GetInstance), &hm);
            return hm;
        }
    };

    IMPLEMENT_SINGLETON(CWorkspacePlugin)
        CWorkspacePlugin::CWorkspacePlugin() {}
    CWorkspacePlugin::~CWorkspacePlugin() {}
}

//////////
//  Plugin export table
BEGIN_PLUGIN_EXPORT()
    PLUGIN_EXPORT(CWorkspacePlugin)
END_PLUGIN_EXPORT()

//  End of Tt3.Workspace/Plugins.cpp
