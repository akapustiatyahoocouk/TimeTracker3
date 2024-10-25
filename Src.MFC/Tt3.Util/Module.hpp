//
//  Tt3.Util/Module.hpp
//  
//  Tt3 module framework
//
//////////
BEGIN_TT3_NAMESPACE

//  A generic module descriptor
class TT3_UTIL_PUBLIC CModule
{
    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CModule)

    //////////
    //  Construction/destruction
public:
    CModule() = default;
    virtual ~CModule() = default;
};

//  An .EXE module descriptor
class TT3_UTIL_PUBLIC CExeModule : public CModule
{
    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CExeModule)

    //////////
    //  Construction/destruction
public:
    CExeModule() = default;
    virtual ~CExeModule() = default;
};

//  A .DLL module descriptor
class TT3_UTIL_PUBLIC CDllModule : public CModule, public AFX_EXTENSION_MODULE
{
    CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CDllModule)

        //////////
        //  Construction/destruction
public:
    CDllModule() = default;
    virtual ~CDllModule() = default;
};

END_TT3_NAMESPACE
//  End of Tt3.Util/Module.hpp
