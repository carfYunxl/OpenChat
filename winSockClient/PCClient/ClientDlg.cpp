#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"
#include "TcpClient.h"

#include "LoginDlg.h"
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
	, m_port(0)
	, m_Client(new TcpClient(8888,"127.0.0.1",this))
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
	DDX_Control(pDX, IDC_RICHEDIT21_SEND, mEditSend);
	DDX_Control(pDX, IDC_RICHEDIT21_INFO, mInfoBox);
}

BEGIN_MESSAGE_MAP(PCClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_MFCBUTTON_CONNECT, &PCClientDlg::OnBnClickedMfcbuttonConnect)
END_MESSAGE_MAP()

BOOL PCClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

#if 0
	//自动连接服务器
	if (!ConnectToServer())
	{
		AfxMessageBox(_T("连接服务器失败！"));
		AfxGetMainWnd()->SendMessage(WM_CLOSE);
		return FALSE;
	}
	LoginDlg m_LoginDlg;
	m_LoginDlg.Init(this);
	int ret = m_LoginDlg.DoModal();
	if (ret == IDCANCEL)
	{
		AfxGetMainWnd()->SendMessage(WM_CLOSE);
	}
#endif
	mInfoBox.SetBackgroundColor(FALSE, RGB(192, 210, 240));
	mEditSend.SetBackgroundColor(FALSE, RGB(152, 210, 240));
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

	mInfoBox.SetSel(-1, -1);
	mInfoBox.ReplaceSel(_T("连接服务器成功\n"));
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
	mInfoBox.SetSel(-1, -1);
	mInfoBox.ReplaceSel(CString(info.c_str()));
}

int PCClientDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}

SOCKET PCClientDlg::GetClientSocket()
{
	return m_Client->GetSocket();
}


void PCClientDlg::OnBnClickedMfcbuttonConnect()
{
	ConnectToServer();
}
