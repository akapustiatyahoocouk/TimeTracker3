//
//  Tt3.XML/Linkage.hpp
//  
//  Tt3.XML linkage definitions
//
//////////

#ifdef BUILD_TT3_XML
    #define TT3_XML_PUBLIC  __declspec(dllexport)
#else
    #define TT3_XML_PUBLIC  __declspec(dllimport)
#endif

//  End of Tt3.XML/Linkage.hpp
