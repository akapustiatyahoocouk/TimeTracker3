//
//  Tt3.Db.API/Plugins.cpp
//  
//  Tt3.Db.API plugins
//
//////////
#include "Tt3.Db.API/API.hpp"
USING_TT3_NAMESPACE

namespace
{
    class CDbApiPlugin final : public CPlugin
    {
        DECLARE_SINGLETON(CDbApiPlugin)

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

    IMPLEMENT_SINGLETON(CDbApiPlugin)
    CDbApiPlugin::CDbApiPlugin() {}
    CDbApiPlugin::~CDbApiPlugin() {}
}

//////////
//  Plugin export table
BEGIN_PLUGIN_EXPORT()
    PLUGIN_EXPORT(CDbApiPlugin)
END_PLUGIN_EXPORT()

//  End of Tt3.Db.API/Plugins.cpp
