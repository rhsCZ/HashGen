
// hasher2Dlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "hasher2.h"
#include "hasher2Dlg.h"
#include "afxdialogex.h"
//#include "atltypes.h"
//#include "afxdd_.h"
//#include "string"
//#include "stdio.h"

char* bin2hex(const unsigned char* bin, size_t len);
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
using namespace std;
// Chasher2Dlg dialog

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
char* convert(const char* str) {
	// Decimal vers hexa sera au max de même taille.
	// +1 pour le ''\0'
	int n = strlen(str) + 1;
	char* res =(char*) malloc(n);
	if (!res) {
		return res;
	}

	int number = strtol(str, NULL, 0);
	snprintf(res, n, "%x", number);
	return res;
}

Chasher2Dlg::Chasher2Dlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_HASHER2_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_ICON);
}

void Chasher2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Chasher2Dlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &Chasher2Dlg::OnBnClickedButton1)
	ON_CBN_SELCHANGE(IDC_COMBO1, &Chasher2Dlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(ID_EXIT, &Chasher2Dlg::OnBnClickedExit)
END_MESSAGE_MAP()


// Chasher2Dlg message handlers

BOOL Chasher2Dlg::OnInitDialog()
{
	
	//AfxInitRichEdit2();
	CDialog::OnInitDialog();
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	SetDlgItemTextW(IDC_COMBO1, L"MD5");
	/*LPWSTR dir = L"";
	char buff[500], buff2[500];
	GetCurrentDirectory(MAX_PATH, dir);
	TRACE(dir);
	//swprintf(buff,"%ls\\libssl-3.dll\0",dir);
	//swprintf(buff2, "%ls\\lcrypto.dll\0",dir);
	sprintf(buff, "%ls\\libssl-3.dll\0", dir);
	sprintf(buff2, "%ls\\lcrypto-3.dll\0", dir);
	/*if (PathFileExists(buff) && PathFileExists(buff2))
	{
		LoadLibrary(buff);
		LoadLibrary(buff2);
	}
	else
	{
		exit(-5);
	}*/
	// TODO: Add extra initialization here
	/*Chasher2Dlg dlg;
	RECT cr = {
	0,0,0,0
	};
	class CRichEditCtrl Create;
	EDITSTREAM es;
	CWnd* pWnd = CWnd::FromHandle(dlg.m_hWnd);
	//es.dwCookie = (DWORD)"test";
	//es.dwError = 0;
	//Create.Create(ES_LEFT, cr,pWnd, IDC_RICHEDIT21);
	//Create.StreamIn(SF_TEXT, es);*/
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void Chasher2Dlg::OnPaint()
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
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR Chasher2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void Chasher2Dlg::OnBnClickedButton1()
{
	
	SetDlgItemTextW(IDC_RICHEDIT22, L"");
	GetDlgItemText(IDC_COMBO1,combo);
	GetDlgItemTextA(Chasher2Dlg::m_hWnd, IDC_RICHEDIT21, input, 1000);
	//GetDlgItemText(IDC_RICHEDIT21,input);
	
	memcpy(&input2, input, 1000);
	if (combo == L"")
	{
		hash = 1;
	}
	else if (combo == L"MD5")
	{
		hash = 1;
	}
	else if (combo == L"SHA256")
	{
		hash = 2;
	}
	else if (combo == L"SHA512")
	{
		hash = 3;
	} 
	else if (combo == L"WHIRLPOOL")
	{
		hash = 4;
	}
	switch (hash)
	{
		case 1:default:
		{
			MD5(input2, strlen(input),md5);
			hex = bin2hex(md5, MD5_DIGEST_LENGTH);
			memcpy(md5_2, hex, MD5_DIGEST_LENGTH*2);
			SetDlgItemTextA(Chasher2Dlg::m_hWnd, IDC_RICHEDIT22, md5_2);
			break;
		}
		case 2:
		{
			SHA256(input2,strlen(input),sha256);
			hex = bin2hex(sha256,SHA256_DIGEST_LENGTH);
			memcpy(sha256_2, hex, SHA256_DIGEST_LENGTH*2);
			SetDlgItemTextA(Chasher2Dlg::m_hWnd, IDC_RICHEDIT22, sha256_2);
			break;
		}
		case 3:
		{
			SHA512(input2, strlen(input), sha512);
			hex = bin2hex(sha512, SHA512_DIGEST_LENGTH);
			memcpy(sha512_2, hex, SHA512_DIGEST_LENGTH*2);
			SetDlgItemTextA(Chasher2Dlg::m_hWnd, IDC_RICHEDIT22, sha512_2);
			break;
		}
		case 4:
		{
			WHIRLPOOL(input2, strlen(input), whirlpool);
			hex = bin2hex(whirlpool, WHIRLPOOL_DIGEST_LENGTH);
			memcpy(whirlpool_2, hex, WHIRLPOOL_DIGEST_LENGTH*2);
			SetDlgItemTextA(Chasher2Dlg::m_hWnd, IDC_RICHEDIT22, whirlpool_2);
			break;
		}
	}
	
	/*for (int i = 0; i < 32; i++) {
		sprintf(hex,"%x", res[i]);
	}
	for (int i = 0; i < 32; i++) {
		printf("%d", !!((res << i) & 0x80));
	}*/
	/*for (i = 0; i < 32; i++) {
		sprintf(&converted[i*2], "%0X", res[i]);
	}*/
	
	
	//SetDlgItemTextW(IDC_RICHEDIT22, L"test");
	//unsigned char *x = 0;
	//MD5((unsigned char*)c, wcsnlen_s(stringd, 100000), x);*/
	

		// TODO: Add your control notification handler code here
}


void Chasher2Dlg::OnCbnSelchangeCombo1()
{
	// TODO: Add your control notification handler code here
}


void Chasher2Dlg::OnBnClickedExit()
{
	exit(2);
	// TODO: Add your control notification handler code here
}
