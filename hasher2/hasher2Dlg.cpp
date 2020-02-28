
// hasher2Dlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "hasher2.h"
#include "hasher2Dlg.h"
#include "afxdialogex.h"
#include "afxwin.h"
#include <thread>
char* bin2hex(const unsigned char* bin, size_t len);
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
using namespace std;

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
	//	ON_CBN_SELCHANGE(IDC_COMBO1, &Chasher2Dlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(ID_EXIT, &Chasher2Dlg::OnBnClickedExit)
END_MESSAGE_MAP()


BOOL Chasher2Dlg::OnInitDialog()
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
	Chasher2Dlg::ShowWindow(SW_SHOW);
	Chasher2Dlg::RedrawWindow();
	Chasher2Dlg::CenterWindow();
	CDialog::OnInitDialog();
	
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	
	if (Chasher2Dlg::IsWindowVisible() != 0)
	{
		//CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_COMBO1);
		//pCB->SetCurSel(0);
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
	}
	
	return TRUE;
}
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
	Chasher2Dlg::ShowWindow(SW_SHOW);
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR Chasher2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void Chasher2Dlg::OnBnClickedButton1()
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
	GetDlgItemTextA(Chasher2Dlg::m_hWnd, IDC_INPUT, input, 1000);
	memcpy(&input2, input, 1000);
	if (ChkBox1 == BST_CHECKED)
	{
		MD5(input2, strlen(input), res);
		hex = bin2hex(res, MD5_DIGEST_LENGTH);
		hashlen = MD5_DIGEST_LENGTH * 2;
		memcpy(result, hex, hashlen);
		SetDlgItemTextA(Chasher2Dlg::m_hWnd, IDC_MD5OUT, result);
	}
	if (ChkBox2 == BST_CHECKED)
	{
		SHA256(input2, strlen(input), res);
		hex = bin2hex(res, SHA256_DIGEST_LENGTH);
		hashlen = SHA256_DIGEST_LENGTH * 2;
		memcpy(result, hex, hashlen);
		SetDlgItemTextA(Chasher2Dlg::m_hWnd, IDC_SHA256OUT, result);
	}
	if (ChkBox3 == BST_CHECKED)
	{
		SHA512(input2, strlen(input), res);
		hex = bin2hex(res, SHA512_DIGEST_LENGTH);
		hashlen = SHA512_DIGEST_LENGTH * 2;
		memcpy(result, hex, hashlen);
		SetDlgItemTextA(Chasher2Dlg::m_hWnd, IDC_SHA512OUT, result);
	}
	if (ChkBox4 == BST_CHECKED)
	{
		WHIRLPOOL(input2, strlen(input), res);
		hex = bin2hex(res, WHIRLPOOL_DIGEST_LENGTH);
		hashlen = WHIRLPOOL_DIGEST_LENGTH * 2;
		memcpy(result, hex, hashlen);
		SetDlgItemTextA(Chasher2Dlg::m_hWnd, IDC_WHIRLOUT, result);
	}
}

void Chasher2Dlg::OnBnClickedExit()
{
	exit(2);
	// TODO: Add your control notification handler code here
}
