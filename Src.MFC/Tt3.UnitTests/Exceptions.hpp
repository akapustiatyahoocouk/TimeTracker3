//
//  Tt3.UnitTests/Exceptions.hpp
//  
//  Tt3 UnitTests: Exceptions
//
//////////
BEGIN_TT3_NAMESPACE

class CTestException final : public CCheckedException
{
    //////////
    //  Construction/destruction
public:
    explicit CTestException(const CString & errorMessage)
        :   CCheckedException(errorMessage) {}
    explicit CTestException(const char* pErrorMessage)
        :   CCheckedException(CString(pErrorMessage)) {}
};

END_TT3_NAMESPACE
//  End of Tt3.UnitTests/Exceptions.hpp
