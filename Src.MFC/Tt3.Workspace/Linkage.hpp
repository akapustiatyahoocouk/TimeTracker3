//
//  Tt3.Workspace/Linkage.hpp
//  
//  Tt3.Workspace linkage definitions
//
//////////

#ifdef BUILD_TT3_WORKSPACE
    #define TT3_WORKSPACE_PUBLIC   __declspec(dllexport)
#else
    #define TT3_WORKSPACE_PUBLIC   __declspec(dllimport)
#endif

//  End of Tt3.Workspace/Linkage.hpp
