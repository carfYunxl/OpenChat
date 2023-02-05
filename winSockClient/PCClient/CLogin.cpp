#include "stdafx.h"
#include "Client.h"
#include "CLogin.h"

IMPLEMENT_DYNAMIC(CLogin, CDialogEx)

CLogin::CLogin(CWnd* pParent)
	: CDialogEx(IDD_DIALOG_MLOG, pParent)
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
END_MESSAGE_MAP()
