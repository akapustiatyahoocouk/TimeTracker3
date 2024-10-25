//
//  Tt3.Db.API/Linkage.hpp
//  
//  Tt3.Db.API linkage definitions
//
//////////

#ifdef BUILD_TT3_DB_API
    #define TT3_DB_API_PUBLIC   __declspec(dllexport)
#else
    #define TT3_DB_API_PUBLIC   __declspec(dllimport)
#endif

//  End of Tt3.Db.API/Linkage.hpp
