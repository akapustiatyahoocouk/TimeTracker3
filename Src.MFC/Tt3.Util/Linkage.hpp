//
//  Tt3.Util/Linkage.hpp
//  
//  Tt3.Util linkage definitions
//
//////////

#ifdef BUILD_TT3_UTIL
    #define TT3_UTIL_PUBLIC __declspec(dllexport)
#else
    #define TT3_UTIL_PUBLIC __declspec(dllimport)
#endif

//  Validate build settings
#if !defined(_CPPRTTI)
    #error C++ RTTI must be enabled
#endif

#if !defined(_CPPUNWIND)
    #error C++ exceptions must be enabled
#endif

#if !defined(_DLL) || !defined(_MT)
    #error Multithreaded DLL CRT must be used
#endif

#if !defined(_UNICODE)
    #error Unisode must be used
#endif

//  End of Tt3.Util/Linkage.hpp
