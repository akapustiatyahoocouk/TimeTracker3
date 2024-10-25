//
//  Tt3.Util/Macros.hpp
//  
//  Tt3 helper macros
//
//////////

#define CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Clazz)      \
    private:                                        \
        Clazz(const Clazz &) = delete;              \
        Clazz & operator = (const Clazz &) = delete;

#define UTILITY_CLASS(Clazz)                        \
    private:                                        \
        Clazz() = delete;                           \
        ~Clazz() = delete;                          \
    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Clazz)

#define DECLARE_SINGLETON(Clazz)                    \
    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Clazz)          \
    public:                                         \
        Clazz();                                    \
        ~Clazz();                                   \
    public:                                         \
        static Clazz * GetInstance();

#define IMPLEMENT_SINGLETON(Clazz)                  \
        Clazz * Clazz::GetInstance()                \
        {                                           \
            static Clazz theInstance;               \
            return &theInstance;                    \
        }

#ifdef _DEBUG
    #define new DEBUG_NEW
#endif

//  TODO order/document
#define BEGIN_TT3_NAMESPACE     namespace tt3 {
#define END_TT3_NAMESPACE       }
#define USING_TT3_NAMESPACE     using namespace tt3;

#define throws(E,...)   noexcept(false) //  used for documenting function signatures

#define IMPLEMENT_DLL(TheDllVar)                                        \
    CDllModule TheDllVar;	                                            \
                                                                        \
    extern "C" int APIENTRY                                             \
    DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpReserved*/) \
    {                                                                   \
        if (dwReason == DLL_PROCESS_ATTACH)                             \
        {	/*	Extension DLL one-time initialization	*/              \
            if (!AfxInitExtensionModule(TheDllVar, hInstance))          \
            {                                                           \
                return 0;                                               \
            }                                                           \
            /*	Insert this DLL into the resource chain	*/              \
            new CDynLinkLibrary(TheDllVar);                             \
            /*	Sockets initialization */                               \
            if (!AfxSocketInit())	                                    \
            {                                                           \
                return FALSE;                                           \
            }                                                           \
        }                                                               \
        else if (dwReason == DLL_PROCESS_DETACH)                        \
        {	/*	Terminate the library before destructors are called	*/  \
            AfxTermExtensionModule(TheDllVar);                          \
        }                                                               \
        return 1;	/*	ok	*/                                          \
    }

//  End of Tt3.Util/Macros.hpp
