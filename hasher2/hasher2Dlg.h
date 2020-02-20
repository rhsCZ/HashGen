
// hasher2Dlg.h : header file
//
#include "openssl/md5.h"
#include <openssl/sha.h>
#include <openssl/whrlpool.h>
#pragma once
#define bufferSize 10
#define WM_SHOWPAGE WM_APP+2
// Chasher2Dlg dialog
class Chasher2Dlg : public CDialog
{
// Construction
public:
	Chasher2Dlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HASHER2_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	
public:
	afx_msg void OnBnClickedButton1();
//	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedExit();
	unsigned char input2[1000] = {};
	int hash=0;
	LPSTR input = new char[1000];
	CString combo;
	CString buffer, stringout;
	char* hex = {};
	unsigned char res[WHIRLPOOL_DIGEST_LENGTH *4] = {};
	char result[WHIRLPOOL_DIGEST_LENGTH * 4] = {};
	BOOL retu = 0;
	HANDLE thread1 = NULL;
};
