
// hasher2Dlg.h : header file
//
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <openssl/whrlpool.h>
#include "afxdialogex.h"
#include "afxwin.h"
#include <thread>
#include <fstream>
using namespace std;
#pragma once
#define bufferSize 10
#define WM_SHOWPAGE WM_APP+2
// Chasher2Dlg dialog
class ChasherDlg : public CDialog
{
// Construction
public:
	ChasherDlg(CWnd* pParent = nullptr);	// standard constructor

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
	CFont Font;
	CButton* MD5check = {};
	CButton* SHA256check = {};
	CButton* SHA512check = {};
	CButton* WHIRLcheck = {};
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	CRichEditCtrl* input_1 = {};
	CMFCEditBrowseCtrl* input_2 = {};
	int file_descript = 0;
	fstream filex;
	unsigned long file_size = 0;
	char* file_buffer = {};
};
