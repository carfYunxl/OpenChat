#pragma once
#include "afxdialogex.h"

class CLogin : public CDialogEx
{
	DECLARE_DYNAMIC(CLogin)

public:
	CLogin(CWnd* pParent = nullptr);
	virtual ~CLogin();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_LOGIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();

	CString GetAcc() { return m_acc; }
	CString GetPwd() { return m_pwd; }

private:
	CString m_acc;
	CString m_pwd;
};
