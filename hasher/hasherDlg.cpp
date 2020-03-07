
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
	m_hIcon = AfxGetApp()->LoadIcon(IDR_ICON);
}

void ChasherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(ChasherDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &ChasherDlg::OnBnClickedButton1)
	//	ON_CBN_SELCHANGE(IDC_COMBO1, &ChasherDlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(ID_EXIT, &ChasherDlg::OnBnClickedExit)
	ON_BN_CLICKED(IDC_RADIO1, &ChasherDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &ChasherDlg::OnBnClickedRadio2)
END_MESSAGE_MAP()


BOOL ChasherDlg::OnInitDialog()
{
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
	
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	
	if (ChasherDlg::IsWindowVisible() != 0)
	{
		//CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_COMBO1);
		//pCB->SetCurSel(0);
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
		//radio1->SetFocus();
		ChasherDlg::CheckDlgButton(IDC_RADIO1, BST_CHECKED);
		//radio1->SetCheck(BST_CHECKED);
		//radio1->SetCheck(BST_UNCHECKED);
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
/*std::ifstream file(filename, std::ifstream::binary);
MD5_CTX md5Context;
MD5_Init(&md5Context);
char buf[1024 * 16];
while (file.good()) {
	file.read(buf, sizeof(buf));
	MD5_Update(&md5Context, buf, file.gcount());
}
unsigned char result[MD5_DIGEST_LENGTH];
MD5_Final(result, &md5Context);
*/
void ChasherDlg::OnBnClickedButton1()
{
	int hashlen;
	//MD5check = (CButton*)GetDlgItem(IDC_MD5CHECK);
	int ChkBox1 = MD5check->GetCheck();
	//SHA256check = (CButton*)GetDlgItem(IDC_SHA256CHECK);
	int ChkBox2 = SHA256check->GetCheck();
	//SHA512check = (CButton*)GetDlgItem(IDC_SHA512CHECK);
	int ChkBox3 = SHA512check->GetCheck();
	//WHIRLcheck = (CButton*)GetDlgItem(IDC_WHIRLCHECK);
	int ChkBox4 = WHIRLcheck->GetCheck();
	char buf[1024 * 500] = {};
	MD5_CTX md5Context;
	SHA256_CTX sha256Context;
	SHA512_CTX sha512Context;
	WHIRLPOOL_CTX whirlContext;
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
				for (int i = 0; i <= sizeof(buf); i++)
				{
					buf[i] = 0;
				}
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
				for (int i = 0; i <= sizeof(buf); i++)
				{
					buf[i] = 0;
				}
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
				for (int i = 0; i <= sizeof(buf); i++)
				{
					buf[i] = 0;
				}
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
				for (int i = 0; i <= sizeof(buf); i++)
				{
					buf[i] = 0;
				}
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

