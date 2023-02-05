//#include "pch.h"
#include "stdafx.h"
#include "Client.h"
#include "LoginDlg.h"
#include "afxdialogex.h"
#include <string>
#include "RMysql.h"
#include "ClientDlg.h"
#include "mysql.h"
using R::RMysql;
using R::RESULT;

IMPLEMENT_DYNAMIC(LoginDlg, CDialogEx)

LoginDlg::LoginDlg(CWnd* pParent)
	: CDialogEx(IDD_DIALOG_LOGIN_PAGE, pParent)
	, mysql(new RMysql("localhost", "root", "123456", "new"))
	, mpParent((PCClientDlg*)pParent)
{
}

LoginDlg::~LoginDlg()
{
}

void LoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(LoginDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_MFCBUTTON_LOGIN, &LoginDlg::OnBnClickedMfcbuttonLogin)
	ON_BN_CLICKED(IDC_MFCBUTTON_CANCEL, &LoginDlg::OnBnClickedMfcbuttonCancel)
	ON_BN_CLICKED(IDC_MFCBUTTON_REGISTER, &LoginDlg::OnBnClickedMfcbuttonRegister)
END_MESSAGE_MAP()


BOOL LoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	(CMFCButton*)GetDlgItem(IDC_MFCBUTTON_REGISTER)->EnableWindow(FALSE);

	return TRUE;
}


HBRUSH LoginDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	HBRUSH aBrush;
	switch (nCtlColor)
	{
	case CTLCOLOR_DLG:
	{
		aBrush = CreateSolidBrush(RGB(193, 210, 240));
		hbr = aBrush;
	}
		break;
	case CTLCOLOR_EDIT:
		aBrush = CreateSolidBrush(RGB(255, 0, 0));
		hbr = aBrush;
		break;
	}
	return hbr;
}

CString LoginDlg::GetAccount()
{
	CString str;
	GetDlgItemText(IDC_RICHEDIT22_ACCOUNT,str);
	return str;
}
CString LoginDlg::GetPassword()
{
	CString str;
	GetDlgItemText(IDC_RICHEDIT23_PASSWORD, str);
	return str;
}

void LoginDlg::SetInfo(const CString& info)
{
	SetDlgItemText(IDC_RICHEDIT21_INFO,info);
}

void LoginDlg::OnBnClickedMfcbuttonLogin()
{
	CString strAccount;
	CString strPassword;
	GetDlgItemText(IDC_RICHEDIT22_ACCOUNT, strAccount);
	GetDlgItemText(IDC_RICHEDIT23_PASSWORD, strPassword);

	RESULT result;
	std::string str = "select account from info where password = '" + std::string(CT2A(strPassword.GetString())) + "'";
	mysql->RQuery(str.c_str(), result, true);
	if (result.at(0).empty())
	{
		SetInfo(_T("没有账号？点击注册！"));
		(CMFCButton*)GetDlgItem(IDC_MFCBUTTON_REGISTER)->EnableWindow(TRUE);

		SetDlgItemText(IDC_RICHEDIT22_ACCOUNT, _T(""));
		SetDlgItemText(IDC_RICHEDIT23_PASSWORD, _T(""));
		return;
	}

	if (CString(result.at(0).at(0).c_str()) != strAccount)
	{
		SetInfo(_T("没有账号？点击注册！"));
		(CMFCButton*)GetDlgItem(IDC_MFCBUTTON_REGISTER)->EnableWindow(TRUE);
		SetDlgItemText(IDC_RICHEDIT22_ACCOUNT, _T(""));
		SetDlgItemText(IDC_RICHEDIT23_PASSWORD, _T(""));
		return;
	}
	mpParent->SetWindowText(strAccount);
	EndDialog(11);
}


void LoginDlg::OnBnClickedMfcbuttonCancel()
{
	EndDialog(10);
}

void LoginDlg::Init(PCClientDlg* parent)
{
	mpParent = parent;
}


void LoginDlg::OnBnClickedMfcbuttonRegister()
{
	CString strAccount;
	CString strPassword;
	GetDlgItemText(IDC_RICHEDIT22_ACCOUNT, strAccount);
	GetDlgItemText(IDC_RICHEDIT23_PASSWORD, strPassword);

	SetWindowText(_T("注册账号"));
	RESULT result;
	std::string strQuery = "insert into info(ip,port,socket,account,password) values";
	std::string ip = mpParent->GetQueryString("127.0.0.1");
	std::string port = mpParent->GetQueryString(std::to_string(mpParent->GetPort()));
	std::string socket = mpParent->GetQueryString(std::to_string(mpParent->GetClientSocket()));
	std::string account = mpParent->GetQueryString(std::string(CT2A(strAccount.GetString())));
	std::string password = mpParent->GetQueryString(std::string(CT2A(strPassword.GetString())));
	strQuery = strQuery + "(" + ip + "," + port + "," + socket + "," + account + "," + password + ")";
	mysql->RQuery(strQuery.c_str(), result, false);
	mpParent->SetWindowText(strAccount);
	EndDialog(11);
}
