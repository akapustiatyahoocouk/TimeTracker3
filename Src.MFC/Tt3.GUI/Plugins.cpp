//
//  Tt3.GUI/Plugins.cpp
//  
//  Tt3.GUI plugins
//
//////////
#include "Tt3.GUI/API.hpp"
USING_TT3_NAMESPACE

namespace
{
    class CGuiPlugin final : public CPlugin
    {
        DECLARE_SINGLETON(CGuiPlugin)

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

    IMPLEMENT_SINGLETON(CGuiPlugin)
    CGuiPlugin::CGuiPlugin() {}
    CGuiPlugin::~CGuiPlugin() {}
}

//////////
//  Plugin export table
BEGIN_PLUGIN_EXPORT()
    PLUGIN_EXPORT(CGuiPlugin)
END_PLUGIN_EXPORT()

//  End of Tt3.Workspace/GUI.cpp
