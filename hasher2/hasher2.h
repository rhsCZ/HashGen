
// hasher2.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols



// Chasher2App:
// See hasher2.cpp for the implementation of this class
//

class Chasher2App : public CWinApp
{
public:
	Chasher2App();

// Overrides
public:
	virtual BOOL InitInstance();
	int error = 0;
	DWORD errormes;
// Implementation

	DECLARE_MESSAGE_MAP()
};

extern Chasher2App theApp;