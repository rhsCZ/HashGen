
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
#define WM_TRAY_ICON_NOTIFY_MESSAGE (WM_USER + 1)
// Chasher2Dlg dialog
class ChasherDlg : public CDialog
{
private:
	BOOL m_bMinimizeToTray;
	BOOL			m_bTrayIconVisible;
	CMenu			m_mnuTrayMenu;
	UINT			m_nDefaultMenuItem;
	afx_msg LRESULT OnTrayNotify(WPARAM wParam, LPARAM lParam);
// Construction
public:
	NOTIFYICONDATA	m_nidIconData;
	ChasherDlg(CWnd* pParent = nullptr);
	void TraySetMinimizeToTray(BOOL bMinimizeToTray = TRUE);
	BOOL TraySetMenu(UINT nResourceID, UINT nDefaultPos = 0);
	BOOL TraySetMenu(HMENU hMenu, UINT nDefaultPos = 0);
	BOOL TraySetMenu(LPCTSTR lpszMenuName, UINT nDefaultPos = 0);
	BOOL TrayUpdate();
	BOOL TrayShow();
	BOOL TrayHide();
	void ChasherDlg::hash(char* input, char* output, int hashtype, bool typein);
	void TraySetToolTip(LPCTSTR lpszToolTip);
	void TraySetIcon(HICON hIcon);
	void TraySetIcon(UINT nResourceID);
	void TraySetIcon(LPCTSTR lpszResourceName);
	BOOL TrayIsVisible();
	virtual void OnTrayLButtonDown(CPoint pt);
	virtual void OnTrayLButtonDblClk(CPoint pt);
	virtual void OnTrayRButtonDown(CPoint pt);
	virtual void OnTrayRButtonDblClk(CPoint pt);
	virtual void OnTrayMouseMove(CPoint pt);	// standard constructor

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
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
	
public:
	afx_msg void OnBnClickedButton1();
//	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedExit();
	//int hash=0;
	CString combo;
	CString buffer, stringout;
	BOOL retu = 0;
	HANDLE thread1 = NULL;
	CFont Font;
	bool trayenable;
	bool lowerenable;
	bool minimizeen;
	CButton* trayen = {};
	CButton* checkbox = {};
	CButton* MD5check = {};
	CButton* SHA256check = {};
	CButton* SHA512check = {};
	CButton* WHIRLcheck = {};
	CButton* LowerCase = {};
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	CRichEditCtrl *out1;
	CRichEditCtrl* out2;
	CRichEditCtrl* out3;
	CRichEditCtrl* out4;
	CRichEditCtrl* in;
	CRichEditCtrl* input_1 = {};
	CMFCEditBrowseCtrl* input_2 = {};
	int file_descript = 0;
	fstream filex;
	unsigned long file_size = 0;
	char* file_buffer = {};
	int boxcheck;
	MD5_CTX md5Context;
	SHA256_CTX sha256Context;
	SHA512_CTX sha512Context;
	WHIRLPOOL_CTX whirlContext;
	HKEY traykey;
	DWORD traykeyvalue;
	unsigned long size;
	LSTATUS error;
	unsigned long type;
	DWORD* test;
	DWORD keycreate;
	afx_msg void OnOpen();
	afx_msg void OnMinimize();
	afx_msg void OnBnClickedMinEn();
	afx_msg void OnBnClickedTrayEn();
	afx_msg LRESULT OnCopy(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnCopy1(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnCopy2(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnCopy3(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnClear(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnPaste(WPARAM wparam, LPARAM lparam);
	afx_msg void OnBnClickedButton2();
	PCHAR* CommandLineToArgvA(PCHAR CmdLine, int* _argc);
	afx_msg void OnBnClickedInstext();
	afx_msg void OnBnClickedUninext();
	bool ExtractResource(uint16_t ResourceID, char* OutputFileName, char* path, const char* ResType);
	afx_msg void OnBnClickedLowercase();
};
