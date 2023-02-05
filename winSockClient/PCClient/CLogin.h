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
	CString GetAcc() { return m_acc; }
	CString GetPwd() { return m_pwd; }
	void SetInfo(const CString& info) {
		m_login_info.SetWindowTextW(info);
	}

private:
	CString m_acc;
	CString m_pwd;
	CRichEditCtrl m_login_info;
};
