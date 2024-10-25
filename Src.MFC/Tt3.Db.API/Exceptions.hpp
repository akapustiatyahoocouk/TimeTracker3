//
//  Tt3.Db.API/Exceptions.hpp
//  
//  Tt3.Db.API exceptions
//
//////////
BEGIN_TT3_NAMESPACE

//  The common base class for all DB API - related exceptions.
class TT3_DB_API_PUBLIC CDatabaseException : public CException
{
    //////////
    //  Construction/destruction/assignment
public:
    explicit CDatabaseException(const CString & errorMessage) : m_ErrorMessage(errorMessage) {}

    //////////
    //  CException
public:
    virtual BOOL GetErrorMessage(LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext = NULL) const override;
    virtual BOOL GetErrorMessage(LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext = NULL) override;

    //////////
    //  Operations (general)
public:
    //  Returns this exception's error message
    CString         GetErrorMessage() const { return m_ErrorMessage; }

    //////////
    //  Implementation
private:
    CString         m_ErrorMessage;
};

END_TT3_NAMESPACE
//  End of Tt3.Db.API/Exceptions.hpp
