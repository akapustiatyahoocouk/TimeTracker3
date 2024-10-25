//
//  Tt3.Util/PluginManager.hpp
//  
//  Tt3 plugin manager
//
//////////
BEGIN_TT3_NAMESPACE

//////////
//  An extension that is discovered, loaded and initialized at runtime.
//  Normally CPlugin - derived classes will be singletons exported
//  by the DLLs
class TT3_UTIL_PUBLIC CPlugin
{
    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CPlugin)

    //////////
    //  Types
public:
    struct Version
    {
        //////////
        //  Construction
        Version()
            :   major(0),
                minor(0),
                patch(0),
                build(0)
        {
        }

        Version(int major_, int minor_, int patch_, int build_)
            :   major(major_),
                minor(minor_),
                patch(patch_),
                build(build_)
        {
            ASSERT(major >= 0);
            ASSERT(minor >= 0);
            ASSERT(patch >= 0);
            ASSERT(build >= 0);
        }

        //////////
        //  Properties
        int     major;
        int     minor;
        int     patch;
        int     build;  //  build number
    };

    //////////
    //  Construction/destruction
public:
    CPlugin();
    virtual ~CPlugin();

    //////////
    //  Operations
public:
    //  Returns the user-readable display name of this plugin.
    //  The default implementation uses version resource of the
    //  module where thr plugin is defined.
    virtual CString     GetDisplayName() const;

    //  Returns the version of this plugin.
    //  The default implementation uses version resource of the
    //  module where thr plugin is defined.
    virtual Version     GetVersion() const;

    //  Returns the one-line copyright message for this plugin.
    //  The default implementation uses version resource of the
    //  module where thr plugin is defined.
    virtual CString     GetCopyright() const;

    //  Checks if this plugin has been initialized successfully.
    virtual bool        IsInitialized() const;

    //  Called by plugin manager to initialize this plugin.
    //  Upon success retrns; upon failure throws.
    virtual void        Initialize() throws(CException*) = 0;

    //  TODO document
protected:
    virtual HINSTANCE   GetHInstance() const = 0;
    
    //////////
    //  Implementation
private:
    bool                m_PropertiesParsed = false;
    CString             m_DisplayName;
    Version             m_Version;
    CString             m_Copyright;

    //  Helpers
    void                _LoadPropertiesFromResources();
};

using CPluginList = CVector<CPlugin *>;

//////////
//  An agent that can discover plugins at runtime, load and initialize them
class TT3_UTIL_PUBLIC CPluginManager
{
    UTILITY_CLASS(CPluginManager)

    friend class CPlugin;

    //////////
    //  Operations
public:
    //  Discovers plugins defined in the DLLs that reside in the same
    //  directory as the running .EXE, loads them and tries to initialize.
    //  If for some specific plugin initialization attempt fails, continues
    //  with trying to initialize other plugins (this may help if plugins
    //  have dependencies) until nothing further can be done.
    static void         LoadPlugins();

    //  TODO document
    static CPluginList  GetLoadedPlugins();
    static CPluginList  GetInitializedPlugins();

    //////////
    //  Implementation
private:
    static CMapStringToString   m_ProcessedDlls;    //  full paths always
    static CDictionary<CPlugin *, bool> m_Plugins; //  true == initialized, else false
};

END_TT3_NAMESPACE

//////////
//  Helper macros for plugin export
#define BEGIN_PLUGIN_EXPORT()                       \
    extern "C" __declspec(dllexport)                \
    void Tt3GetPlugins(CPluginList & pluginList)    \
    {

#define PLUGIN_EXPORT(Clazz)                        \
        pluginList.Add(Clazz::GetInstance());

#define END_PLUGIN_EXPORT()                         \
    }

//  End of Tt3.Util/PluginManager.hpp
