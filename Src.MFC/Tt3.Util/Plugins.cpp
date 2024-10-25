//
//  Tt3.Util/Plugins.cpp
//  
//  Tt3.Util plugins
//
//////////
#include "Tt3.Util/API.hpp"
USING_TT3_NAMESPACE

namespace
{
    class CUtilPlugin final : public CPlugin
    {
        DECLARE_SINGLETON(CUtilPlugin)

        //////////
        //  CPlugin
    public:
        virtual void        Initialize() throws(CException *)
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

    IMPLEMENT_SINGLETON(CUtilPlugin)
    CUtilPlugin::CUtilPlugin() {}
    CUtilPlugin::~CUtilPlugin() {}
}

//////////
//  Plugin export table
BEGIN_PLUGIN_EXPORT()
    PLUGIN_EXPORT(CUtilPlugin)
END_PLUGIN_EXPORT()

//  End of Tt3.Util/Plugins.cpp
