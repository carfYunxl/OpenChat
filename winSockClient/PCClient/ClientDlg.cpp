#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"
#include "TcpClient.h"
#include "mysql.h"
#include "RMysql.h"
#include "CLogin.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace R;

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

PCClientDlg::PCClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(PCClientDlg::IDD, pParent)
	, m_ServerStatus(ServerStatus::OFF)
	, m_Client(new TcpClient(8888,"127.0.0.1",this))
	, m_sql(new RMysql("localhost","root","123456","new"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

PCClientDlg::~PCClientDlg()
{
	closesocket(m_Client->GetSocket());
	m_ServerStatus = ServerStatus::OFF;
	delete m_Client;
}

void PCClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_SEND_BOX, mEditSend);
	DDX_Control(pDX, IDC_LIST_INFO, mInfoBox);
}

BEGIN_MESSAGE_MAP(PCClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_CREATE()
END_MESSAGE_MAP()

BOOL PCClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	//自动连接服务器
	if (!ConnectToServer())
	{
		AfxMessageBox(_T("连接服务器失败！"));
		AfxGetMainWnd()->SendMessage(WM_CLOSE);
		return FALSE;
	}

	//write info to mysql
	OperaDataBase();
	return TRUE;
}

void PCClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

void PCClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

BOOL PCClientDlg::EnableWindow(DWORD DlgId, BOOL bUsed)
{
	return GetDlgItem(DlgId)->EnableWindow(bUsed);
}

bool PCClientDlg::ConnectToServer()
{
	m_Client->SetPort(8888);
	m_Client->SetIp("127.0.0.1");

	if (!m_Client->Connect())
	{
		return false;
	}

	sockaddr_in cliAddr;
	int len = sizeof(cliAddr);
	if (getsockname(m_Client->GetSocket(), (sockaddr*)&cliAddr, &len) != 0)
	{
		MessageBox(_T("获取客户端端口失败！"));
		return false;
	}

	m_port = ntohs(cliAddr.sin_port);

	mInfoBox.AddString(_T("连接服务器成功"));
	m_ServerStatus = ServerStatus::ON;
	return true;
}

BOOL PCClientDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
			case VK_ESCAPE:
				return TRUE;
			case VK_RETURN:
			{
				CString strSend;
				mEditSend.GetWindowTextW(strSend);
				std::string str = CT2A(strSend.GetString());
				send(m_Client->GetSocket(), str.c_str(), str.length(), 0);
				mEditSend.SetWindowTextW(_T(""));
				return TRUE;
			}
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void PCClientDlg::AddInfo(const std::string& info)
{
	mInfoBox.AddString(CString(info.c_str()));
}

void PCClientDlg::OperaDataBase()
{
	CString strAcc;
	CString strpwd;
	CLogin m_loginWnd;
	int res = m_loginWnd.DoModal();
	strAcc = m_loginWnd.GetAcc();
	strpwd = m_loginWnd.GetPwd();
	if (res == LOGIN)
	{
		RESULT result;
		std::string str = "select account from info where password = '" + std::string(CT2A(strpwd.GetString())) + "'";
		m_sql->RQuery(str.c_str(), result, true);
		if (result.at(0).empty())
		{
			m_loginWnd.SetInfo(_T("没有账号？点击注册！"));
			AfxGetMainWnd()->SendMessage(WM_CLOSE);
			return;
		}

		if (CString(result.at(0).at(0).c_str()) != strAcc)
		{
			AfxMessageBox(_T("不存在的账户"));
			AfxGetMainWnd()->SendMessage(WM_CLOSE);
			return;
		}
		SetWindowText(strAcc);
	}
	else if (res == REGIS)
	{
		m_loginWnd.SetWindowText(_T("注册账号"));
		RESULT result;
		std::string strQuery = "insert into info(ip,port,socket,account,password) values";
		std::string ip = GetQueryString("127.0.0.1");
		std::string port = GetQueryString(std::to_string(GetPort()));
		std::string socket = GetQueryString(std::to_string(m_Client->GetSocket()));
		std::string account = GetQueryString(std::string(CT2A(strAcc.GetString())));
		std::string password = GetQueryString(std::string(CT2A(strpwd.GetString())));
		strQuery = strQuery + "(" + ip + ","  + port + "," + socket + "," + account + "," + password + ")";
		m_sql->RQuery(strQuery.c_str(), result, false);
		SetWindowText(strAcc);
	}
}


int PCClientDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}
