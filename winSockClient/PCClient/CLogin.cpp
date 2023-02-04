#include "stdafx.h"
#include "Client.h"
#include "CLogin.h"

IMPLEMENT_DYNAMIC(CLogin, CDialogEx)

CLogin::CLogin(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_LOGIN, pParent)
{
}

CLogin::~CLogin()
{
}

void CLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLogin, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CLogin::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CLogin::OnBnClickedButton2)
END_MESSAGE_MAP()

void CLogin::OnBnClickedButton1()
{
	GetDlgItemText(IDC_EDIT1, m_acc);
	GetDlgItemText(IDC_EDIT2, m_pwd);
	EndDialog(10);
}


void CLogin::OnBnClickedButton2()
{
	GetDlgItemText(IDC_EDIT1, m_acc);
	GetDlgItemText(IDC_EDIT2, m_pwd);
	EndDialog(11);
}
