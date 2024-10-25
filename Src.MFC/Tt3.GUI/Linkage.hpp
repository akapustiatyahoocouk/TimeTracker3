//
//  Tt3.GUI/Linkage.hpp
//  
//  Tt3.GUI linkage definitions
//
//////////

#ifdef BUILD_TT3_GUI
    #define TT3_GUI_PUBLIC  __declspec(dllexport)
#else
    #define TT3_GUI_PUBLIC  __declspec(dllimport)
#endif

//  End of Tt3.GUI/Linkage.hpp

