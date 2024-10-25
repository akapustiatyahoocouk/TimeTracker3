//
//  Tt3.Db.API/Exceptions.cpp
//  
//  Tt3.Db.API exceptions
//
//////////
#include "Tt3.Db.API/API.hpp"
USING_TT3_NAMESPACE

//////////
//  CException
BOOL CDatabaseException::GetErrorMessage(LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext) const
{
    ASSERT(lpszError != NULL);
    ASSERT(nMaxError > 0);

    UINT msglen = m_ErrorMessage.GetLength();
    _tcsncpy(lpszError, m_ErrorMessage, min(msglen, nMaxError));
    lpszError[nMaxError - 1] = 0;   //  ...just in case

    if (pnHelpContext != NULL)
    {
        pnHelpContext = 0;
    }
    return TRUE;
}

BOOL CDatabaseException::GetErrorMessage(LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext)
{
    return const_cast<const CDatabaseException*>(this)->GetErrorMessage(lpszError, nMaxError, pnHelpContext);
}

//	End of Tt3.Db.API/Exceptions.cpp
