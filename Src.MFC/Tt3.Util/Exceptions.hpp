//
//  Tt3.Util/Exceptions.hpp
//  
//  Tt3 exception framework
//
//////////
BEGIN_TT3_NAMESPACE

//  An exception that should be caught and handled (and declared in function signature)
class TT3_UTIL_PUBLIC CCheckedException : public CException
{
    //////////
    //  Construction/destruction/assignment
public:
    explicit CCheckedException(const CString& errorMessage) : m_ErrorMessage(errorMessage) {}

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

//  An exception that should not generally be caught because it cannot be recovered from
class TT3_UTIL_PUBLIC CError : public CException
{
    //////////
    //  Construction/destruction/assignment
public:
    explicit CError(const CString& errorMessage) : m_ErrorMessage(errorMessage) {}

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

//  An error in program
class TT3_UTIL_PUBLIC CProgramError : public CError
{
    //////////
    //  Construction/destruction/assignment
public:
    explicit CProgramError(const CString& errorMessage) : CError(errorMessage) {}
};

//  An attempt to access/use a nonexistent item
class TT3_UTIL_PUBLIC CNoSuchItemError : public CProgramError
{
    //////////
    //  Construction/destruction/assignment
public:
    CNoSuchItemError(const char* pItemType, const char* pItemName)
        : CProgramError(CString(pItemType ) + _T(" ") + CString(pItemName ) + _T(" does not exist")) {}
    CNoSuchItemError(const char* pItemType, const CString& itemName)
        : CProgramError(CString(pItemType) + _T(" ") + itemName + _T(" does not exist")) {}
    CNoSuchItemError(const CString& itemType, const char* pItemName)
        : CProgramError(itemType + _T(" ") + CString(pItemName) + _T(" does not exist")) {}
    CNoSuchItemError(const CString & itemType, const CString & itemName)
        :   CProgramError(itemType + _T(" ") + itemName + _T(" does not exist")) {}
};

//  An error in system
class TT3_UTIL_PUBLIC CSystemError : public CError
{
    //////////
    //  Construction/destruction/assignment
public:
    explicit CSystemError(const CString& errorMessage) : CError(errorMessage) {}
};

END_TT3_NAMESPACE
//  End of Tt3.Util/Exceptions.hpp
