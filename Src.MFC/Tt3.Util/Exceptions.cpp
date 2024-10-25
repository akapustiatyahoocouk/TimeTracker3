//
//  Tt3.Util/Exceptions.cpp
//  
//  Tt3.Util exceptions
//
//////////
#include "Tt3.Util/API.hpp"
USING_TT3_NAMESPACE

//////////
//  CError
BOOL CCheckedException::GetErrorMessage(LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext) const
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

BOOL CCheckedException::GetErrorMessage(LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext)
{
    return const_cast<const CCheckedException*>(this)->GetErrorMessage(lpszError, nMaxError, pnHelpContext);
}

//////////
//  CError
BOOL CError::GetErrorMessage(LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext) const
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

BOOL CError::GetErrorMessage(LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext)
{
    return const_cast<const CError*>(this)->GetErrorMessage(lpszError, nMaxError, pnHelpContext);
}

//	End of Tt3.Util/Exceptions.cpp
