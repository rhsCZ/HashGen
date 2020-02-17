
// hasher2.cpp : Defines the class behaviors for the application.
//

#include "pch.h"
#include "framework.h"
#include "hasher2.h"
#include "hasher2Dlg.h"
#include <tchar.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Chasher2App

BEGIN_MESSAGE_MAP(Chasher2App, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// Chasher2App construction

Chasher2App::Chasher2App()
{
	
	
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only Chasher2App object

Chasher2App theApp;


// Chasher2App initialization

BOOL Chasher2App::InitInstance()
{
// TODO: call AfxInitRichEdit2() to initialize richedit2 library.\n"
	AfxInitRichEdit2();
	CWinApp::InitInstance();
	/*LPWSTR dir = L"";
	char buff[500], buff2[500];

	GetCurrentDirectory(MAX_PATH, dir);
	//swprintf(buff,"%ls\\libssl-3.dll\0",dir);
	//swprintf(buff2, "%ls\\lcrypto.dll\0",dir);
	sprintf(buff, "%ls\\libssl-3.dll\0", dir);
	sprintf(buff2, "%ls\\lcrypto-3.dll\0", dir);
	if (PathFileExists(buff) && PathFileExists(buff2))
	{
		LoadLibrary(buff);
		LoadLibrary(buff2);
	}
	else
	{
		exit(-5);
	}*/
	SetErrorMode(0);
	/*if (LoadLibraryA("libssl-3.dll") == NULL)
	{
		errormes = GetLastError();
		error++;
	}*/
	if (LoadLibraryA("libcrypto-3.dll") == NULL)
	{
		errormes = GetLastError();
		error++;
	}
	if (error > 0)
	{
		exit(-5);
	}
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	//SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	Chasher2Dlg dlg;
	m_pMainWnd = &dlg;
	//DWORD resultxy;
	INT_PTR nResponse = dlg.DoModal();
	//resultxy = GetLastError();
	/*INT_PTR response =*/ //if (dlg.Create(IDD_HASHER2_DIALOG, 0) == 0)
	//{
		//exit(-1);
	//}
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "Warning: dialog creation failed, so application is terminating unexpectedly.\n");
	}
	else if (nResponse == IDABORT)
	{
		TRACE(traceAppMsg, 0, "Warning: dialog creation failed, so application is terminating unexpectedly2.\n");
	}


#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

