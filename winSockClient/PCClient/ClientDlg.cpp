#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"
#include "TcpClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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
	,m_ServerStatus(ServerStatus::OFF)
	,m_Client(new TcpClient(8888,"127.0.0.1",this))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

PCClientDlg::~PCClientDlg()
{
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
	ON_BN_CLICKED(IDC_MFCBUTTON_CONNECT, &PCClientDlg::OnBnClickedMfcbuttonConnect)
	ON_BN_CLICKED(IDC_MFCBUTTON_DISCONNECT, &PCClientDlg::OnBnClickedMfcbuttonDisconnect)
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

void PCClientDlg::OnBnClickedMfcbuttonConnect()
{
	UINT port = GetDlgItemInt(IDC_EDITPORT);
	if ((port >= 65535 && port < 1024) || port == 0)
	{
		MessageBox(_T("请输入正确IP地址或端口"));
		return;
	}

	m_Client->SetPort(port);
	m_Client->SetIp("127.0.0.1");

	m_Client->Connect();

	sockaddr_in cliAddr;
	int len = sizeof(cliAddr);
	if (getsockname(m_Client->GetSocket(), (sockaddr*)&cliAddr, &len) != 0)
	{
		MessageBox(_T("获取客户端端口失败！"));
		return;
	}

	CString strCliTitle;
	strCliTitle.Format(_T("Client : %d"), ntohs(cliAddr.sin_port));
	SetWindowTextW(strCliTitle);

	mInfoBox.AddString(_T("连接服务器成功"));
	EnableWindow(IDC_MFCBUTTON_DISCONNECT, TRUE);
	EnableWindow(IDC_MFCBUTTON_CONNECT, FALSE);

	m_ServerStatus = ServerStatus::ON;
}


void PCClientDlg::OnBnClickedMfcbuttonDisconnect()
{
	EnableWindow(IDC_MFCBUTTON_CONNECT, TRUE);
	EnableWindow(IDC_MFCBUTTON_DISCONNECT, FALSE);
	closesocket(m_Client->GetSocket());

	m_ServerStatus = ServerStatus::OFF;
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
