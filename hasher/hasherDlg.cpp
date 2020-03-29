
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
template<class TYPE>
bool RegSetKey(HKEY key, LPSTR keyloc, unsigned long type, REGSAM access, LPSTR name, TYPE indatax);
int RegCrtKey(HKEY key, LPSTR keyloc, REGSAM access);
template<class TYPE>
int RegGetKey(HKEY key, LPSTR keyloc, unsigned long type, REGSAM access, LPSTR name, TYPE outdatax);
#pragma warning( disable : 4244 )
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
char* bin2hex(const unsigned char* bin, size_t len)
{
	char* out;
	size_t  i;

	if (bin == NULL || len == 0)
		return NULL;

	out = (char*)malloc(len * 2 + 1);
	for (i = 0; i < len; i++) {
		out[i * 2] = "0123456789abcdef"[bin[i] >> 4];
		out[i * 2 + 1] = "0123456789abcdef"[bin[i] & 0x0F];
	}
	out[len * 2] = '\0';

	return out;
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
END_MESSAGE_MAP()


BOOL ChasherDlg::OnInitDialog()
{
	int out;
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
		checkbox = (CButton*)GetDlgItem(IDC_MINTRAY);
		trayen = (CButton*)GetDlgItem(IDC_TRAYEN);
		input_1 = (CRichEditCtrl*)GetDlgItem(IDC_INPUT);
		input_2 = (CMFCEditBrowseCtrl*)GetDlgItem(IDC_INPUT2);
		MD5check = (CButton*)GetDlgItem(IDC_MD5CHECK);
		SHA256check = (CButton*)GetDlgItem(IDC_SHA256CHECK);
		SHA512check = (CButton*)GetDlgItem(IDC_SHA512CHECK);
		WHIRLcheck = (CButton*)GetDlgItem(IDC_WHIRLCHECK);
		MD5check->SetCheck(1);
		SHA256check->SetCheck(1);
		SHA512check->SetCheck(1);
		WHIRLcheck->SetCheck(1);
		CRichEditCtrl* rich1 = (CRichEditCtrl*)GetDlgItem(IDC_MD5OUT);
		rich1->SetFont(&Font);
		rich1 = (CRichEditCtrl*)GetDlgItem(IDC_SHA256OUT);
		rich1->SetFont(&Font);
		rich1 = (CRichEditCtrl*)GetDlgItem(IDC_SHA512OUT);
		rich1->SetFont(&Font);
		rich1 = (CRichEditCtrl*)GetDlgItem(IDC_WHIRLOUT);
		rich1->SetFont(&Font);
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
	hashlen = 0;
	SetDlgItemTextA(ChasherDlg::m_hWnd, IDC_MD5OUT, "");
	SetDlgItemTextA(ChasherDlg::m_hWnd, IDC_SHA256OUT, "");
	SetDlgItemTextA(ChasherDlg::m_hWnd, IDC_SHA512OUT, "");
	SetDlgItemTextA(ChasherDlg::m_hWnd, IDC_WHIRLOUT, "");
	memset(buf,0, sizeof(buf));
	memset(res, 0, sizeof(res));
	memset(result, 0, sizeof(result));
	//MD5check = (CButton*)GetDlgItem(IDC_MD5CHECK);
	ChkBox1 = MD5check->GetCheck();
	//SHA256check = (CButton*)GetDlgItem(IDC_SHA256CHECK);
	ChkBox2 = SHA256check->GetCheck();
	//SHA512check = (CButton*)GetDlgItem(IDC_SHA512CHECK);
	ChkBox3 = SHA512check->GetCheck();
	//WHIRLcheck = (CButton*)GetDlgItem(IDC_WHIRLCHECK);
	ChkBox4 = WHIRLcheck->GetCheck();
	
	if (IsDlgButtonChecked(IDC_RADIO2) && !IsDlgButtonChecked(IDC_RADIO1))
	{
		GetDlgItemTextA(ChasherDlg::m_hWnd, IDC_INPUT2, input, 10000);
		filex.open(input, ifstream::binary|fstream::in);
		if (filex.is_open() == true)
		{
			if (ChkBox1 == BST_CHECKED)
			{ 
				MD5_Init(&md5Context);
				while (filex.good()) 
				{
					filex.read(buf, sizeof(buf));
					MD5_Update(&md5Context, buf, filex.gcount());
				}
				MD5_Final(res, &md5Context);
				hex = bin2hex(res, MD5_DIGEST_LENGTH);
				hashlen = MD5_DIGEST_LENGTH * 2;
				memcpy(result, hex, hashlen);
				SetDlgItemTextA(ChasherDlg::m_hWnd, IDC_MD5OUT, result);
				memset(buf, 0, sizeof(buf));
				filex.close();
			}
			if (ChkBox2 == BST_CHECKED)
			{
				filex.open(input, ifstream::binary | fstream::in);
				SHA256_Init(&sha256Context);
				while (filex.good())
				{
					filex.read(buf, sizeof(buf));
					SHA256_Update(&sha256Context, buf, filex.gcount());
				}
				SHA256_Final(res, &sha256Context);
				hex = bin2hex(res, SHA256_DIGEST_LENGTH);
				hashlen = SHA256_DIGEST_LENGTH * 2;
				memcpy(result, hex, hashlen);
				SetDlgItemTextA(ChasherDlg::m_hWnd, IDC_SHA256OUT, result);
				memset(buf, 0, sizeof(buf));
				filex.close();
			}
			if (ChkBox3 == BST_CHECKED)
			{
				filex.open(input, ifstream::binary | fstream::in);
				SHA512_Init(&sha512Context);
				while (filex.good())
				{
					filex.read(buf, sizeof(buf));
					SHA512_Update(&sha512Context, buf, filex.gcount());
				}
				SHA512_Final(res, &sha512Context);
				hex = bin2hex(res, SHA512_DIGEST_LENGTH);
				hashlen = SHA512_DIGEST_LENGTH * 2;
				memcpy(result, hex, hashlen);
				SetDlgItemTextA(ChasherDlg::m_hWnd, IDC_SHA512OUT, result);
				memset(buf, 0, sizeof(buf));
				filex.close();
			}
			if (ChkBox4 == BST_CHECKED)
			{
				filex.open(input, ifstream::binary | fstream::in);
				WHIRLPOOL_Init(&whirlContext);
				while (filex.good())
				{
					filex.read(buf, sizeof(buf));
					WHIRLPOOL_Update(&whirlContext, buf, filex.gcount());
				}
				WHIRLPOOL_Final(res, &whirlContext);
				hex = bin2hex(res, WHIRLPOOL_DIGEST_LENGTH);
				hashlen = WHIRLPOOL_DIGEST_LENGTH * 2;
				memcpy(result, hex, hashlen);
				SetDlgItemTextA(ChasherDlg::m_hWnd, IDC_WHIRLOUT, result);
				memset(buf, 0, sizeof(buf));
				filex.close();
			}
			
		}
		else
		{
			MessageBoxA(NULL, "Error while opening file. Please try again.", "ERROR", MB_ICONERROR | MB_TOPMOST | MB_OK);
		}

	}
	else
	{
		GetDlgItemTextA(ChasherDlg::m_hWnd, IDC_INPUT, input, 10000);
		memcpy(input2, input, strlen(input));
		if (ChkBox1 == BST_CHECKED)
		{
			MD5(input2, strlen(input), res);
			hex = bin2hex(res, MD5_DIGEST_LENGTH);
			hashlen = MD5_DIGEST_LENGTH * 2;
			memcpy(result, hex, hashlen);
			SetDlgItemTextA(ChasherDlg::m_hWnd, IDC_MD5OUT, result);
		}
		if (ChkBox2 == BST_CHECKED)
		{
			SHA256(input2, strlen(input), res);
			hex = bin2hex(res, SHA256_DIGEST_LENGTH);
			hashlen = SHA256_DIGEST_LENGTH * 2;
			memcpy(result, hex, hashlen);
			SetDlgItemTextA(ChasherDlg::m_hWnd, IDC_SHA256OUT, result);
		}
		if (ChkBox3 == BST_CHECKED)
		{
			SHA512(input2, strlen(input), res);
			hex = bin2hex(res, SHA512_DIGEST_LENGTH);
			hashlen = SHA512_DIGEST_LENGTH * 2;
			memcpy(result, hex, hashlen);
			SetDlgItemTextA(ChasherDlg::m_hWnd, IDC_SHA512OUT, result);
		}
		if (ChkBox4 == BST_CHECKED)
		{
			WHIRLPOOL(input2, strlen(input), res);
			hex = bin2hex(res, WHIRLPOOL_DIGEST_LENGTH);
			hashlen = WHIRLPOOL_DIGEST_LENGTH * 2;
			memcpy(result, hex, hashlen);
			SetDlgItemTextA(ChasherDlg::m_hWnd, IDC_WHIRLOUT, result);
		}
	
	}
	

	
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

