
// hasher2Dlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "hasher.h"
#include "hasherDlg.h"
#include "afxdialogex.h"
#include "afxwin.h"
#include <thread>
#include <fstream>
#include <Windows.h>
HWND mhwnd;
template<class TYPE>
bool RegSetKey(HKEY key, LPSTR keyloc, unsigned long type, REGSAM access, LPSTR name, TYPE indatax);
int RegCrtKey(HKEY key, LPSTR keyloc, REGSAM access);
template<class TYPE>
int RegGetKey(HKEY key, LPSTR keyloc, unsigned long type, REGSAM access, LPSTR name, TYPE outdatax);
#pragma warning( disable:4244)
#pragma warning(disable:6387)
#pragma warning(disable:6011)
unsigned long get_size_by_fd(int fd);
char* bin2hex(const unsigned char* bin, size_t len);
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
using namespace std;
unsigned long get_size_by_fd(int fd) {
	struct stat statbuf;
	if (fstat(fd, &statbuf) < 0) exit(-1);
	return statbuf.st_size;
}
char* bin2hex(const unsigned char* bin, size_t len) //binary(11101100) to hex(EC) convert function
{
	char* out;
	size_t  i;

	if (bin == NULL || len == 0) //if empty bin or len is zero return NULL
		return NULL;

	out = (char*)malloc(len * 2 + 1);//allocate char array in heap for char pointer out
	for (i = 0; i < len; i++) {//cycle 
		out[i * 2] = "0123456789abcdef"[bin[i] >> 4];
		out[i * 2 + 1] = "0123456789abcdef"[bin[i] & 0x0F];
	}
	out[len * 2] = '\0';

	return out;
}
bool ChasherDlg::ExtractResource(uint16_t ResourceID, char *OutputFileName,char *path, const char* ResType)
{
	try
	{
		HRSRC hResource = FindResourceA(nullptr, MAKEINTRESOURCEA(ResourceID), ResType);
		if (hResource == nullptr)
		{
			return false;
		}

		HGLOBAL hFileResource = LoadResource(nullptr, hResource);
		if (hFileResource == nullptr)
		{
			return false;
		}

		void* lpFile = LockResource(hFileResource);
		if (lpFile == nullptr)
		{
			return false;
		}

		std::uint32_t dwSize = SizeofResource(nullptr, hResource);
		if (dwSize == 0)
		{
			return false;
		}
		char paths[300];
		sprintf_s(paths,"%s\\%s",path,OutputFileName);
		HANDLE hFile = CreateFileA(paths, GENERIC_READ | GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
		HANDLE hFilemap = CreateFileMappingA(hFile, nullptr, PAGE_READWRITE, 0, dwSize, nullptr);
		if (hFilemap == nullptr)
		{
			return false;
		}

		void* lpBaseAddress = MapViewOfFile(hFilemap, FILE_MAP_WRITE, 0, 0, 0);
		CopyMemory(lpBaseAddress, lpFile, dwSize);
		UnmapViewOfFile(lpBaseAddress);
		CloseHandle(hFilemap);
		CloseHandle(hFile);

		return true;
	}
	catch (...) {}
	return false;
}
void ChasherDlg::hash(char* input, char* output, int hashtype, bool typein) //hash function
{
	unsigned char input2[1000] = {};
	char *buf = (char*)malloc(1024 * 500);
	char *hex = (char*)malloc(WHIRLPOOL_DIGEST_LENGTH*4);
	unsigned char res[WHIRLPOOL_DIGEST_LENGTH * 4] = {};
	MD5_CTX md5Context;
	SHA256_CTX sha256Context;
	SHA512_CTX sha512Context;
	WHIRLPOOL_CTX whirlContext;
	//char* hex = new char[1000];
	fstream filex;
	sprintf_s(output, sizeof(output), "\0");
	sprintf_s(buf, sizeof(buf), "\0");
	sprintf_s(hex, sizeof(hex), "\0");
	if (typein)
	{ 
		switch (hashtype)
		{
			case 1:
			{
				filex.open(input, ifstream::binary | ifstream::in);
				MD5_Init(&md5Context);
				while (filex.good())
				{
					filex.read(buf, sizeof(buf));
					MD5_Update(&md5Context, buf, filex.gcount());
				}
				MD5_Final(res, &md5Context);
				hex = bin2hex(res, MD5_DIGEST_LENGTH);
				memcpy(output, hex, strlen(hex));
				output[strlen(hex)] = '\0';
				filex.close();
				break;
			}
			case 2:
			{
				filex.open(input, ifstream::binary | ifstream::in);
				SHA256_Init(&sha256Context);
				while (filex.good())
				{
					filex.read(buf, sizeof(buf));
					SHA256_Update(&sha256Context, buf, filex.gcount());
				}
				SHA256_Final(res, &sha256Context);
				hex = bin2hex(res, SHA256_DIGEST_LENGTH);
				memcpy(output, hex, strlen(hex));
				output[strlen(hex)] = '\0';
				filex.close();
				break;
			}
			case 3:
			{
				filex.open(input, ifstream::binary | ifstream::in);
				SHA512_Init(&sha512Context);
				while (filex.good())
				{
				filex.read(buf, sizeof(buf));
				SHA512_Update(&sha512Context, buf, filex.gcount());
				}
				SHA512_Final(res, &sha512Context);
				hex = bin2hex(res, SHA512_DIGEST_LENGTH);
				memcpy(output, hex, strlen(hex));
				output[strlen(hex)] = '\0';
				filex.close();
				break;
			}
			case 4:
			{
				filex.open(input, ifstream::binary | ifstream::in);
				WHIRLPOOL_Init(&whirlContext);
				while (filex.good())
				{
					filex.read(buf, sizeof(buf));
					WHIRLPOOL_Update(&whirlContext, buf, filex.gcount());
				}
				WHIRLPOOL_Final(res, &whirlContext);
				hex = bin2hex(res, WHIRLPOOL_DIGEST_LENGTH);
				memcpy(output, hex, strlen(hex));
				output[strlen(hex)] = '\0';
				filex.close();
				break;
			}
		}
		for (unsigned int i = 0; i <= strlen(output); i++)
		{
			if (output[i] >= 97 && output[i] <= 122)
			{
				output[i] = output[i] - 32;
			}
		}
	}
	else
	{
		memcpy(input2, input, strlen(input));
		switch (hashtype)
		{
			case 1:
			{
				MD5(input2, strlen(input), res);
				hex = bin2hex(res, MD5_DIGEST_LENGTH);
				memcpy(output, hex, strlen(hex));
				output[strlen(hex)] = '\0';
				break;
			}
			case 2:
			{
				SHA256(input2, strlen(input), res);
				hex = bin2hex(res, SHA256_DIGEST_LENGTH);
				memcpy(output, hex, strlen(hex));
				output[strlen(hex)] = '\0';
				break;
			}
			case 3:
			{
				SHA512(input2, strlen(input), res);
				hex = bin2hex(res, SHA512_DIGEST_LENGTH);
				memcpy(output, hex, strlen(hex));
				output[strlen(hex)] = '\0';
				break;
			}
			case 4:
			{
				WHIRLPOOL(input2, strlen(input), res);
				hex = bin2hex(res, WHIRLPOOL_DIGEST_LENGTH);
				memcpy(output, hex, strlen(hex));
				output[strlen(hex)] = '\0';
				break;
			}
		}
		for (unsigned int i = 0; i <= strlen(output); i++)
		{
			if (output[i] >= 97 && output[i] <= 122)
			{
				output[i] = output[i] - 32;
			}
		}
	}
			
}

ChasherDlg::ChasherDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_HASHER2_DIALOG, pParent)
{
	m_nidIconData.cbSize = sizeof(NOTIFYICONDATA);
	m_nidIconData.hWnd = 0;
	m_nidIconData.uID = 1;
	m_nidIconData.uCallbackMessage = WM_TRAY_ICON_NOTIFY_MESSAGE;
	m_nidIconData.hIcon = 0;
	m_nidIconData.szTip[0] = 0;
	m_nidIconData.uFlags = NIF_MESSAGE;
	m_bTrayIconVisible = FALSE;
	m_nDefaultMenuItem = 0;
	m_bMinimizeToTray = TRUE;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_ICON);
}

void ChasherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
template<class TYPE>
bool RegSetKey(HKEY key,LPSTR keyloc,unsigned long type, REGSAM access,LPSTR name,TYPE indatax)
{
	unsigned long size = sizeof(type);
	char errorbuf[200];
	HKEY keyval;
	bool onerr = 1;
	int err;
	err = RegOpenKeyExA(key, keyloc, NULL, access, &keyval);
	if (err != ERROR_SUCCESS)
	{
		sprintf_s(errorbuf, "%i\n", err);
		onerr = 0;
		ASSERT(errorbuf);
	} else if(err == ERROR_SUCCESS)
	{ 
		err = RegSetValueExA(keyval, name, NULL, type, (BYTE*)indatax, size);
		if (err != ERROR_SUCCESS)
		{
			sprintf_s(errorbuf, "%i\n", err);
			onerr = 0;
			ASSERT(errorbuf);
		}
	}
	
		CloseHandle(keyval);
	return onerr;
}
int RegCrtKey(HKEY key, LPSTR keyloc, REGSAM access)
{
	HKEY keyval;
	int err;
	char errorbuf[200];
	DWORD dispvalue;
	err = RegCreateKeyExA(key, keyloc, NULL, NULL, REG_OPTION_NON_VOLATILE, access,NULL, &keyval, &dispvalue);
	CloseHandle(keyval);
	if (err == ERROR_SUCCESS)
	{
		if (dispvalue == REG_CREATED_NEW_KEY)
		{
			return 1;
		}
		else
		{
			return 2;
		}
	}
	else
	{
		sprintf_s(errorbuf, "%i\n", err);
		ASSERT(errorbuf);
		return 0;
	}
	//return onerr;
}
template<class TYPE>
int RegGetKey(HKEY key, LPSTR keyloc, unsigned long type, REGSAM access, LPSTR name, TYPE outdatax)
{
	unsigned long size = sizeof(type);
	char errorbuf[200];
	HKEY keyval;
	int onerr = 0;
	int err;
	err = RegOpenKeyExA(key, keyloc, NULL, access, &keyval);
	if (err != ERROR_SUCCESS)
	{
		onerr = false;
	}
	err = RegQueryValueExA(keyval, name, NULL, &type, (BYTE*)outdatax, &size);
	switch (err)
	{
	case ERROR_FILE_NOT_FOUND:
	{
		onerr = 2;
		break;
	} 
	case ERROR_MORE_DATA:
	{
		onerr = 3;
		break;
	}
	case ERROR_SUCCESS:
	{
		onerr = 1;
		break;
	}
	default:
	{
		sprintf_s(errorbuf, "%i\n", err);
		ASSERT(errorbuf);
		onerr = 0;
		break;
	}
	}
		CloseHandle(keyval);
	return onerr;
}

BEGIN_MESSAGE_MAP(ChasherDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SYSCOMMAND()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &ChasherDlg::OnBnClickedButton1)
	ON_BN_CLICKED(ID_EXIT, &ChasherDlg::OnBnClickedExit)
	ON_BN_CLICKED(IDC_RADIO1, &ChasherDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &ChasherDlg::OnBnClickedRadio2)
	ON_MESSAGE(WM_TRAY_ICON_NOTIFY_MESSAGE, OnTrayNotify)
	ON_BN_CLICKED(IDC_TRAYEN, &ChasherDlg::OnBnClickedTrayEn)
	ON_BN_CLICKED(IDC_MINTRAY, &ChasherDlg::OnBnClickedMinEn)
	ON_COMMAND(ID_MENU_CLOSE, &ChasherDlg::OnBnClickedExit)
	ON_COMMAND(ID_MENU_OPEN, &ChasherDlg::OnOpen)
	ON_COMMAND(ID_MENU_MINIMIZE, &ChasherDlg::OnMinimize)
	ON_MESSAGE(ID_HASH_COPY, &ChasherDlg::OnCopy)
	ON_MESSAGE(ID_HASH_COPY1, &ChasherDlg::OnCopy1)
	ON_MESSAGE(ID_HASH_COPY2, &ChasherDlg::OnCopy2)
	ON_MESSAGE(ID_HASH_COPY3, &ChasherDlg::OnCopy3)
	ON_MESSAGE(ID_HASH_PASTE, &ChasherDlg::OnPaste)
	ON_MESSAGE(ID_HASH_CLEAR, &ChasherDlg::OnClear)
	ON_BN_CLICKED(IDC_TEST, &ChasherDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_INSTEXT, &ChasherDlg::OnBnClickedInstext)
	ON_BN_CLICKED(IDC_UNINEXT, &ChasherDlg::OnBnClickedUninext)
END_MESSAGE_MAP()
BOOL ChasherDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: Add your specialized code here and/or call the base class
	MSGFILTER* lpMsgFilter = (MSGFILTER*)lParam;

	if ((wParam == IDC_MD5OUT) && (lpMsgFilter->nmhdr.code == EN_MSGFILTER)
		&& (lpMsgFilter->msg == WM_RBUTTONDOWN))

	{
		//if we get through here, we have trapped the right click event of the richeditctrl! 
		CPoint point;
		::GetCursorPos(&point); //where is the mouse?
		CMenu menu; //lets display out context menu :) 
		UINT dwSelectionMade;
		VERIFY(menu.LoadMenu(IDR_HASH));
		CMenu* pmenuPopup = menu.GetSubMenu(0);
		ASSERT(pmenuPopup != NULL);
		dwSelectionMade = pmenuPopup->TrackPopupMenu((TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD),
			point.x, point.y, this
		);

		pmenuPopup->DestroyMenu();
		PostMessage(dwSelectionMade, 0, 0);
	}
	else if ((wParam == IDC_SHA256OUT) && (lpMsgFilter->nmhdr.code == EN_MSGFILTER)
		&& (lpMsgFilter->msg == WM_RBUTTONDOWN))

	{
		//if we get through here, we have trapped the right click event of the richeditctrl! 
		CPoint point;
		::GetCursorPos(&point); //where is the mouse?
		CMenu menu; //lets display out context menu :) 
		UINT dwSelectionMade;
		VERIFY(menu.LoadMenu(IDR_HASH1));
		CMenu* pmenuPopup = menu.GetSubMenu(0);
		ASSERT(pmenuPopup != NULL);
		dwSelectionMade = pmenuPopup->TrackPopupMenu((TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD),
			point.x, point.y, this
		);

		pmenuPopup->DestroyMenu();
		PostMessage(dwSelectionMade, 0, 0);
	}
	else if ((wParam == IDC_SHA512OUT) && (lpMsgFilter->nmhdr.code == EN_MSGFILTER)
		&& (lpMsgFilter->msg == WM_RBUTTONDOWN))

	{
		//if we get through here, we have trapped the right click event of the richeditctrl! 
		CPoint point;
		::GetCursorPos(&point); //where is the mouse?
		CMenu menu; //lets display out context menu :) 
		UINT dwSelectionMade;
		VERIFY(menu.LoadMenu(IDR_HASH2));
		CMenu* pmenuPopup = menu.GetSubMenu(0);
		ASSERT(pmenuPopup != NULL);
		dwSelectionMade = pmenuPopup->TrackPopupMenu((TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD),
			point.x, point.y, this
		);

		pmenuPopup->DestroyMenu();
		PostMessage(dwSelectionMade, 0, 0);
	}
	else if ((wParam == IDC_WHIRLOUT) && (lpMsgFilter->nmhdr.code == EN_MSGFILTER)
		&& (lpMsgFilter->msg == WM_RBUTTONDOWN))

	{
		//if we get through here, we have trapped the right click event of the richeditctrl! 
		CPoint point;
		::GetCursorPos(&point); //where is the mouse?
		CMenu menu; //lets display out context menu :) 
		UINT dwSelectionMade;
		VERIFY(menu.LoadMenu(IDR_HASH3));
		CMenu* pmenuPopup = menu.GetSubMenu(0);
		ASSERT(pmenuPopup != NULL);
		dwSelectionMade = pmenuPopup->TrackPopupMenu((TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD),
			point.x, point.y, this
		);

		pmenuPopup->DestroyMenu();
		PostMessage(dwSelectionMade, 0, 0);
	}
	else if ((wParam == IDC_INPUT) && (lpMsgFilter->nmhdr.code == EN_MSGFILTER)
		&& (lpMsgFilter->msg == WM_RBUTTONDOWN))

	{
		//if we get through here, we have trapped the right click event of the richeditctrl! 
		CPoint point;
		::GetCursorPos(&point); //where is the mouse?
		CMenu menu; //lets display out context menu :) 
		UINT dwSelectionMade;
		VERIFY(menu.LoadMenu(IDR_MENU1));
		CMenu* pmenuPopup = menu.GetSubMenu(0);
		ASSERT(pmenuPopup != NULL);
		dwSelectionMade = pmenuPopup->TrackPopupMenu((TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD),
			point.x, point.y, this
		);

		pmenuPopup->DestroyMenu();
		PostMessage(dwSelectionMade, 0, 0);
	}

	return CDialog::OnNotify(wParam, lParam, pResult);
}

BOOL ChasherDlg::OnInitDialog()
{
	int out,argc;
	PCHAR *cmdarg;
	size = sizeof(DWORD);
	DWORD indata =  1;
	DWORD outdata = 0;
	BYTE cmp = 1;
	type = REG_DWORD;
	Font.CreateFont(12,                            // Height
		0,                             // Width	
		0,                             // Escapement
		0,                             // Orientation
		FW_NORMAL,                       // Weight
		FALSE,                         // Italic
		FALSE,                          // Underline
		0,                             // StrikeOut
		ANSI_CHARSET,                  // CharSet
		OUT_DEFAULT_PRECIS,            // OutPrecision
		CLIP_DEFAULT_PRECIS,           // ClipPrecision
		DEFAULT_QUALITY,               // Quality
		DEFAULT_PITCH | FF_SWISS,      // PitchAndFamily
		L"Arial");
	//AfxInitRichEdit2();
	ChasherDlg::ShowWindow(SW_SHOW);
	ChasherDlg::RedrawWindow();
	ChasherDlg::CenterWindow();
	CDialog::OnInitDialog();
	out = RegCrtKey(HKEY_CURRENT_USER, "Software\\HashGen", KEY_ALL_ACCESS | KEY_WOW64_64KEY);
	if (out == 1)
	{
		RegSetKey(HKEY_CURRENT_USER, "Software\\HashGen", REG_DWORD, KEY_ALL_ACCESS | KEY_WOW64_64KEY, "TrayEnable", &indata);
		RegSetKey(HKEY_CURRENT_USER, "Software\\HashGen", REG_DWORD, KEY_ALL_ACCESS | KEY_WOW64_64KEY, "TrayMinimize", &indata);
	}
	else if(out == 0)
	{
		trayenable = 1;
		minimizeen = 1;
		m_bMinimizeToTray = TRUE;
	}
	else if (out == 2)
	{
		out = RegGetKey(HKEY_CURRENT_USER, "Software\\HashGen", NULL, KEY_ALL_ACCESS | KEY_WOW64_64KEY, "TrayMinimize", &outdata);
		if (out == 2)
		{
			
			RegSetKey(HKEY_CURRENT_USER, "Software\\HashGen", REG_DWORD, KEY_ALL_ACCESS | KEY_WOW64_64KEY, "TrayMinimize", &indata);
			
			minimizeen = 1;
			m_bMinimizeToTray = TRUE;
		} else if (out == 1)
		{ 
			
			if (outdata == 1)
			{
				minimizeen = 1;
				m_bMinimizeToTray = TRUE;
			}
			else 
			{
				minimizeen = 0;
				m_bMinimizeToTray = FALSE;
			}
		}
		out = RegGetKey(HKEY_CURRENT_USER, "Software\\HashGen", REG_DWORD, KEY_ALL_ACCESS | KEY_WOW64_64KEY, "TrayEnable", &outdata);
		if (out == 2)
		{

			RegSetKey(HKEY_CURRENT_USER, "Software\\HashGen", REG_DWORD, KEY_ALL_ACCESS | KEY_WOW64_64KEY, "TrayMinimize", &indata);
			trayenable = 1;
		}
		else if (out == 1)
		{ 
			if (outdata == 1)
			{
				trayenable = 1;
			}
			else
			{
				trayenable = 0;
			}
		}
	}
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	TraySetIcon(m_hIcon);
	TraySetToolTip(L"HashGen Tray icon");
	TraySetMenu(ID_MENU_MINIMIZE);
	
	if (ChasherDlg::IsWindowVisible() != 0)
	{
		mhwnd = ChasherDlg::m_hWnd;
		checkbox = (CButton*)GetDlgItem(IDC_MINTRAY);
		trayen = (CButton*)GetDlgItem(IDC_TRAYEN);
		input_1 = (CRichEditCtrl*)GetDlgItem(IDC_INPUT);
		input_2 = (CMFCEditBrowseCtrl*)GetDlgItem(IDC_INPUT2);
		MD5check = (CButton*)GetDlgItem(IDC_MD5CHECK);
		SHA256check = (CButton*)GetDlgItem(IDC_SHA256CHECK);
		SHA512check = (CButton*)GetDlgItem(IDC_SHA512CHECK);
		WHIRLcheck = (CButton*)GetDlgItem(IDC_WHIRLCHECK);
		out1 = (CRichEditCtrl*)GetDlgItem(IDC_MD5OUT);
		out2 = (CRichEditCtrl*)GetDlgItem(IDC_SHA256OUT);
		out3 = (CRichEditCtrl*)GetDlgItem(IDC_SHA512OUT);
		out4 = (CRichEditCtrl*)GetDlgItem(IDC_WHIRLOUT);
		in = (CRichEditCtrl*)GetDlgItem(IDC_INPUT);
		out1->SetEventMask(ENM_MOUSEEVENTS);
		out2->SetEventMask(ENM_MOUSEEVENTS);
		out3->SetEventMask(ENM_MOUSEEVENTS);
		out4->SetEventMask(ENM_MOUSEEVENTS);
		in->SetEventMask(ENM_MOUSEEVENTS);
		out1->SetFont(&Font);
		out2->SetFont(&Font);
		out3->SetFont(&Font);
		out4->SetFont(&Font);
		MD5check->SetCheck(1);
		SHA256check->SetCheck(1);
		SHA512check->SetCheck(1);
		WHIRLcheck->SetCheck(1);
		ChasherDlg::CheckDlgButton(IDC_RADIO1, BST_CHECKED);
		if (minimizeen)
		{
			CheckDlgButton(IDC_MINTRAY, BST_CHECKED);
		}
		else
		{
			CheckDlgButton(IDC_MINTRAY, BST_UNCHECKED);
		}
		if (trayenable)
		{
			CheckDlgButton(IDC_TRAYEN, BST_CHECKED);
			TrayShow();
		}
		else
		{
			CheckDlgButton(IDC_TRAYEN, BST_UNCHECKED);
			TrayHide();
		}
		
	}
	cmdarg = CommandLineToArgvA(GetCommandLineA(), &argc);
	if (argc = 2)
	{
		ChasherDlg::OnBnClickedRadio2();
		SetDlgItemTextA(ChasherDlg::m_hWnd, IDC_INPUT2, cmdarg[1]);
		ChasherDlg::OnBnClickedButton1();
	}
	
	return TRUE;
}
void ChasherDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
	ChasherDlg::ShowWindow(SW_SHOW);
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR ChasherDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void ChasherDlg::OnBnClickedButton1()
{
	char* output = new char[1000];
	PSTR input = (PSTR)malloc(1000);
	SetDlgItemTextA(ChasherDlg::m_hWnd, IDC_MD5OUT, "");
	SetDlgItemTextA(ChasherDlg::m_hWnd, IDC_SHA256OUT, "");
	SetDlgItemTextA(ChasherDlg::m_hWnd, IDC_SHA512OUT, "");
	SetDlgItemTextA(ChasherDlg::m_hWnd, IDC_WHIRLOUT, "");
	int ChkBox1 = MD5check->GetCheck();
	int ChkBox2 = SHA256check->GetCheck();
	int ChkBox3 = SHA512check->GetCheck();
	int ChkBox4 = WHIRLcheck->GetCheck();
	memset(output, 0, sizeof(output));
	
	if (IsDlgButtonChecked(IDC_RADIO2) && !IsDlgButtonChecked(IDC_RADIO1))
	{
		GetDlgItemTextA(mhwnd, IDC_INPUT2, input, MAX_PATH);
		if (!strcmp(input, ""))
		{
			MessageBox(L"You didn't specified any file!!!\n Please select file to compute hash.", L"ERROR!!", MB_OK | MB_ICONERROR | MB_APPLMODAL | MB_TOPMOST);
			goto end1;
		}
		filex.open(input, ifstream::binary|ifstream::in);
		if (filex.is_open() == true)
		{
			filex.close();
			if (ChkBox1 == BST_CHECKED)
			{ 
				hash(input,output,1,1);
				SetDlgItemTextA(ChasherDlg::m_hWnd, IDC_MD5OUT, output);
				memset(output, 0, sizeof(output));
			}
			if (ChkBox2 == BST_CHECKED)
			{
				hash(input, output, 2, 1);
				SetDlgItemTextA(ChasherDlg::m_hWnd, IDC_SHA256OUT, output);
			}
			if (ChkBox3 == BST_CHECKED)
			{
				hash(input, output, 3, 1);
				SetDlgItemTextA(ChasherDlg::m_hWnd, IDC_SHA512OUT, output);
			}
			if (ChkBox4 == BST_CHECKED)
			{
				hash(input, output, 4, 1);
				SetDlgItemTextA(ChasherDlg::m_hWnd, IDC_WHIRLOUT, output);
			}
			
		}
		else
		{
			MessageBox(L"Error while opening file. Please try again.", L"ERROR", MB_OK | MB_ICONERROR | MB_APPLMODAL | MB_TOPMOST);
		}

	}
	else
	{
		GetDlgItemTextA(mhwnd, IDC_INPUT, input, 1000);
		if (!strcmp(input, ""))
		{
			MessageBox( L"You didn't specified any text!!!\n Please select text to compute hash.", L"ERROR!!", MB_OK | MB_ICONERROR | MB_APPLMODAL | MB_TOPMOST);
			goto end1;
		}
		if (ChkBox1 == BST_CHECKED)
		{
			hash(input, output, 1, 0);
			SetDlgItemTextA(ChasherDlg::m_hWnd, IDC_MD5OUT, output);
			memset(output, 0, sizeof(output));
		}
		if (ChkBox2 == BST_CHECKED)
		{
			hash(input, output, 2, 0);
			SetDlgItemTextA(ChasherDlg::m_hWnd, IDC_SHA256OUT, output);
		}
		if (ChkBox3 == BST_CHECKED)
		{
			hash(input, output, 3, 0);
			SetDlgItemTextA(ChasherDlg::m_hWnd, IDC_SHA512OUT, output);
		}
		if (ChkBox4 == BST_CHECKED)
		{
			hash(input, output, 4, 0);
			SetDlgItemTextA(ChasherDlg::m_hWnd, IDC_WHIRLOUT, output);
		}
	
	}
	
end1:
	Sleep(100);
	
}

void ChasherDlg::OnBnClickedExit()
{
	ChasherDlg::OnDestroy();
	exit(2);
}


void ChasherDlg::OnBnClickedRadio1()
{
	CheckDlgButton(IDC_RADIO1, BST_CHECKED);
	CheckDlgButton(IDC_RADIO2, BST_UNCHECKED);
	input_1->ShowWindow(SW_SHOW);
	input_2->ShowWindow(SW_HIDE);
}


void ChasherDlg::OnBnClickedRadio2()
{
	CheckDlgButton(IDC_RADIO1, BST_UNCHECKED);
	CheckDlgButton(IDC_RADIO2, BST_CHECKED);
	input_1->ShowWindow(SW_HIDE);
	input_2->ShowWindow(SW_SHOW);
}
void ChasherDlg::OnBnClickedMinEn()
{
	DWORD indata = 0;
	boxcheck = checkbox->GetCheck();
	if (boxcheck == BST_CHECKED)
	{
		indata = 1;
		RegSetKey(HKEY_CURRENT_USER, "Software\\HashGen", REG_DWORD, KEY_ALL_ACCESS | KEY_WOW64_64KEY, "TrayMinimize", &indata);
		m_bMinimizeToTray = TRUE;
		minimizeen = true;
	}
	else
	{
		indata = 0;
		RegSetKey(HKEY_CURRENT_USER, "Software\\HashGen", REG_DWORD, KEY_ALL_ACCESS | KEY_WOW64_64KEY, "TrayMinimize", &indata);
		m_bMinimizeToTray = FALSE;
		minimizeen = false;
	}
}
void ChasherDlg::OnBnClickedTrayEn()
{
	int tren;
	DWORD indata = 0;
	tren = trayen->GetCheck();
	if (tren == BST_CHECKED)
	{
		indata = 1;
		RegSetKey(HKEY_CURRENT_USER, "Software\\HashGen", REG_DWORD, KEY_ALL_ACCESS | KEY_WOW64_64KEY, "TrayEnable", &indata);
		trayenable = true;
		checkbox->EnableWindow();
		TrayShow();
	}
	else
	{
		indata = 0;
		RegSetKey(HKEY_CURRENT_USER, "Software\\HashGen", REG_DWORD, KEY_ALL_ACCESS | KEY_WOW64_64KEY, "TrayEnable", &indata);
		trayenable = false;
		m_bMinimizeToTray = FALSE;
		checkbox->EnableWindow(0);
		TrayHide();
	}
}
void ChasherDlg::OnOpen()
{
	if (this->IsWindowVisible())
	{ 
		this->ShowWindow(SW_NORMAL);
		this->SetFocus();
	}
	else
	{	
		this->ShowWindow(SW_SHOW);
		this->SetFocus();
	}
	
	
}

int ChasherDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_nidIconData.hWnd = this->m_hWnd;
	m_nidIconData.uID = 1;

	return 0;
}

void ChasherDlg::OnDestroy()
{
	CDialog::OnDestroy();
	if (m_nidIconData.hWnd && m_nidIconData.uID > 0 && TrayIsVisible())
	{
		Shell_NotifyIcon(NIM_DELETE, &m_nidIconData);
	}
}

BOOL ChasherDlg::TrayIsVisible()
{
	return m_bTrayIconVisible;
}

void ChasherDlg::TraySetIcon(HICON hIcon)
{
	ASSERT(hIcon);

	m_nidIconData.hIcon = hIcon;
	m_nidIconData.uFlags |= NIF_ICON;
}

void ChasherDlg::TraySetIcon(UINT nResourceID)
{
	ASSERT(nResourceID > 0);
	HICON hIcon = 0;
	hIcon = AfxGetApp()->LoadIcon(nResourceID);
	if (hIcon)
	{
		m_nidIconData.hIcon = hIcon;
		m_nidIconData.uFlags |= NIF_ICON;
	}
	else
	{
		TRACE0("FAILED TO LOAD ICON\n");
	}
}

void ChasherDlg::TraySetIcon(LPCTSTR lpszResourceName)
{
	HICON hIcon = 0;
	hIcon = AfxGetApp()->LoadIcon(lpszResourceName);
	if (hIcon)
	{
		m_nidIconData.hIcon = hIcon;
		m_nidIconData.uFlags |= NIF_ICON;
	}
	else
	{
		TRACE0("FAILED TO LOAD ICON\n");
	}
}

void ChasherDlg::TraySetToolTip(LPCTSTR lpszToolTip)
{
	ASSERT(strlen((char*)lpszToolTip) > 0 && strlen((char*)lpszToolTip) < 64);

	strcpy((char*)m_nidIconData.szTip, (char*)lpszToolTip);
	m_nidIconData.uFlags |= NIF_TIP;
}

BOOL ChasherDlg::TrayShow()
{
	BOOL bSuccess = FALSE;
	if (!m_bTrayIconVisible && trayenable)
	{
		bSuccess = Shell_NotifyIcon(NIM_ADD, &m_nidIconData);
		if (bSuccess)
			m_bTrayIconVisible = TRUE;
	}
	else
	{
		TRACE0("ICON ALREADY VISIBLE");
	}
	return bSuccess;
}

BOOL ChasherDlg::TrayHide()
{
	BOOL bSuccess = FALSE;
	if (m_bTrayIconVisible)
	{
		bSuccess = Shell_NotifyIcon(NIM_DELETE, &m_nidIconData);
		if (bSuccess)
			m_bTrayIconVisible = FALSE;
	}
	else
	{
		TRACE0("ICON ALREADY HIDDEN");
	}
	bSuccess = TRUE;
	return bSuccess;
}

BOOL ChasherDlg::TrayUpdate()
{
	BOOL bSuccess = FALSE;
	if (m_bTrayIconVisible)
	{
		bSuccess = Shell_NotifyIcon(NIM_MODIFY, &m_nidIconData);
	}
	else
	{
		TRACE0("ICON NOT VISIBLE");
	}
	return bSuccess;
}


BOOL ChasherDlg::TraySetMenu(UINT nResourceID, UINT nDefaultPos)
{
	BOOL bSuccess;
	bSuccess = m_mnuTrayMenu.LoadMenu(nResourceID);
	return bSuccess;
}


BOOL ChasherDlg::TraySetMenu(LPCTSTR lpszMenuName, UINT nDefaultPos)
{
	BOOL bSuccess;
	bSuccess = m_mnuTrayMenu.LoadMenu(lpszMenuName);
	return bSuccess;
}

BOOL ChasherDlg::TraySetMenu(HMENU hMenu, UINT nDefaultPos)
{
	m_mnuTrayMenu.Attach(hMenu);
	return TRUE;
}

LRESULT ChasherDlg::OnTrayNotify(WPARAM wParam, LPARAM lParam)
{
	UINT uID;
	UINT uMsg;

	uID = (UINT)wParam;
	uMsg = (UINT)lParam;

	if (uID != 1)
		return false;

	CPoint pt;

	switch (uMsg)
	{
	case WM_MOUSEMOVE:
		GetCursorPos(&pt);
		ClientToScreen(&pt);
		OnTrayMouseMove(pt);
		break;
	case WM_LBUTTONDOWN:
		GetCursorPos(&pt);
		ClientToScreen(&pt);
		OnTrayLButtonDown(pt);
		break;
	case WM_LBUTTONDBLCLK:
		GetCursorPos(&pt);
		ClientToScreen(&pt);
		OnTrayLButtonDblClk(pt);
		break;

	case WM_RBUTTONDOWN:
	case WM_CONTEXTMENU:
		GetCursorPos(&pt);
		//ClientToScreen(&pt);
		OnTrayRButtonDown(pt);
		break;
	case WM_RBUTTONDBLCLK:
		GetCursorPos(&pt);
		ClientToScreen(&pt);
		OnTrayRButtonDblClk(pt);
		break;
	}
	return true;
}
void ChasherDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if (m_bMinimizeToTray)
	{
		if ((nID & 0xFFF0) == SC_MINIMIZE)
		{
		
			if (TrayShow() || m_bTrayIconVisible)
			{
				this->ShowWindow(SW_HIDE);
			}
		}
		else
		{
			CDialog::OnSysCommand(nID, lParam);
		}
	}
	else
	{ 
		CDialog::OnSysCommand(nID, lParam);
	}
}
void ChasherDlg::TraySetMinimizeToTray(BOOL bMinimizeToTray)
{
	m_bMinimizeToTray = bMinimizeToTray;
}


void ChasherDlg::OnTrayRButtonDown(CPoint pt)
{
	m_mnuTrayMenu.GetSubMenu(0)->TrackPopupMenu(TPM_BOTTOMALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, pt.x, pt.y, this);
	m_mnuTrayMenu.GetSubMenu(0)->SetDefaultItem(m_nDefaultMenuItem, TRUE);
}

void ChasherDlg::OnTrayLButtonDown(CPoint pt)
{

}

void ChasherDlg::OnTrayLButtonDblClk(CPoint pt)
{
	if (m_bMinimizeToTray)
		if (TrayHide())
		{ 
			this->ShowWindow(SW_SHOW);
			this->SetFocus();
		}
}

void ChasherDlg::OnTrayRButtonDblClk(CPoint pt)
{
}

void ChasherDlg::OnTrayMouseMove(CPoint pt)
{
}
void ChasherDlg::OnMinimize()
{
	if(this->IsWindowVisible())
	{ 
		if (m_bMinimizeToTray && trayenable)
		{
			if (TrayHide())
			{
				ShowWindow(SW_HIDE);
			}
			} else
			{ 
				ShowWindow(SW_MINIMIZE);
			}
		}
}
LRESULT ChasherDlg::OnCopy(WPARAM wparam, LPARAM lparam)
{
	out1->SetSel(0, -1);
	out1->Copy();
	return 1;
}
LRESULT ChasherDlg::OnCopy1(WPARAM wparam, LPARAM lparam)
{
	out2->SetSel(0, -1);
	out2->Copy();
	return 1;
}
LRESULT ChasherDlg::OnCopy2(WPARAM wparam, LPARAM lparam)
{
	out3->SetSel(0, -1);
	out3->Copy();
	return 1;
}
LRESULT ChasherDlg::OnCopy3(WPARAM wparam, LPARAM lparam)
{
	out4->SetSel(0, -1);
	out4->Copy();
	return 1;
}
LRESULT ChasherDlg::OnClear(WPARAM wparam, LPARAM lparam)
{
	in->SetSel(0, -1);
	in->Clear();
	return 1;
}
LRESULT ChasherDlg::OnPaste(WPARAM wparam, LPARAM lparam)
{
	in->Paste();
	return 1;
}

void ChasherDlg::OnBnClickedButton2()
{
	out1->SetSel(1, -1);
}
PCHAR* ChasherDlg::CommandLineToArgvA(PCHAR CmdLine,int* _argc)
{
	PCHAR* argv;
	PCHAR  _argv;
	ULONG   len;
	ULONG   argc;
	CHAR   a;
	ULONG   i, j;

	BOOLEAN  in_QM;
	BOOLEAN  in_TEXT;
	BOOLEAN  in_SPACE;

	len = strlen(CmdLine);
	i = ((len + 2) / 2) * sizeof(PVOID) + sizeof(PVOID);

	argv = (PCHAR*)GlobalAlloc(GMEM_FIXED,
		i + (len + 2) * sizeof(CHAR));

	_argv = (PCHAR)(((PUCHAR)argv) + i);

	argc = 0;
	argv[argc] = _argv;
	in_QM = FALSE;
	in_TEXT = FALSE;
	in_SPACE = TRUE;
	i = 0;
	j = 0;

	while (a = CmdLine[i]) {
		if (in_QM) {
			if (a == '\"') {
				in_QM = FALSE;
			}
			else {
				_argv[j] = a;
				j++;
			}
		}
		else {
			switch (a) {
			case '\"':
				in_QM = TRUE;
				in_TEXT = TRUE;
				if (in_SPACE) {
					argv[argc] = _argv + j;
					argc++;
				}
				in_SPACE = FALSE;
				break;
			case ' ':
			case '\t':
			case '\n':
			case '\r':
				if (in_TEXT) {
					_argv[j] = '\0';
					j++;
				}
				in_TEXT = FALSE;
				in_SPACE = TRUE;
				break;
			default:
				in_TEXT = TRUE;
				if (in_SPACE) {
					argv[argc] = _argv + j;
					argc++;
				}
				_argv[j] = a;
				j++;
				in_SPACE = FALSE;
				break;
			}
		}
		i++;
	}
	_argv[j] = '\0';
	argv[argc] = NULL;

	(*_argc) = argc;
	return argv;
}

void ChasherDlg::OnBnClickedInstext()
{
	char temppath[250] = { '\0' };
	wchar_t params[300] = { '\0' }, path[350] = {'\0'};
	//wchar_t temppath2[500] = { '\0' };
	GetEnvironmentVariableA("TMP", temppath, sizeof(temppath));
	//err = GetEnvironmentVariableW(L"TEMP", temppath2, NULL);
	//err = GetLastError();
	ExtractResource(IDR_DATA, "shellentry.exe", temppath, MAKEINTRESOURCEA(10));
	LPWSTR *arg;
	int argc=0;
	arg = CommandLineToArgvW(GetCommandLineW(), &argc);
	swprintf_s(params, L"/install /path=%s", arg[0]);
	swprintf_s(path, L"%hs\\shellentry.exe",temppath);
	SHELLEXECUTEINFO shExInfo = { 0 };
	shExInfo.cbSize = sizeof(shExInfo);
	shExInfo.fMask = SEE_MASK_DEFAULT;
	shExInfo.hwnd = 0;
	shExInfo.lpVerb = _T("runas");
	shExInfo.lpFile = path;
	shExInfo.lpParameters = params;
	shExInfo.lpDirectory = 0;
	shExInfo.nShow = SW_SHOW;
	shExInfo.hInstApp = 0;
	if (ShellExecuteEx(&shExInfo) == TRUE)
	{
		TRACE(L"TRUE");
	}
	else
	{
		TRACE(L"FALSE");
	}
	DeleteFileW(path);
}


void ChasherDlg::OnBnClickedUninext()
{
	char temppath[250] = { '\0' };
	wchar_t params[300] = { '\0' }, path[350] = { '\0' };
	//wchar_t temppath2[500] = { '\0' };
	GetEnvironmentVariableA("TMP", temppath, sizeof(temppath));
	//err = GetEnvironmentVariableW(L"TEMP", temppath2, NULL);
	//err = GetLastError();
	ExtractResource(IDR_DATA, "shellentry.exe", temppath, MAKEINTRESOURCEA(10));
	swprintf_s(path, L"%hs\\shellentry.exe", temppath);
	SHELLEXECUTEINFO shExInfo = { 0 };
	shExInfo.cbSize = sizeof(shExInfo);
	shExInfo.fMask = SEE_MASK_DEFAULT;
	shExInfo.hwnd = 0;
	shExInfo.lpVerb = _T("runas");
	shExInfo.lpFile = path;
	shExInfo.lpParameters = _T("/uninstall");
	shExInfo.lpDirectory = 0;
	shExInfo.nShow = SW_SHOW;
	shExInfo.hInstApp = 0;
	if (ShellExecuteEx(&shExInfo) == TRUE)
	{
		TRACE(L"TRUE");
	}
	else
	{
		TRACE(L"FALSE");
	}
	DeleteFileW(path);
}
