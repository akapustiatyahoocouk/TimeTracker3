
// Tt3.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

BEGIN_TT3_NAMESPACE

// CTt3App:
// See Tt3.cpp for the implementation of this class
//

class CTt3App : public CWinApp
{
public:
	CTt3App();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CTt3App theApp; 

END_TT3_NAMESPACE

