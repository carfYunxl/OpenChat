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
	,m_ClientSock(INVALID_SOCKET)
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
	DDX_Control(pDX, IDC_EDITREVBOX, m_EditRevBox);
}

BEGIN_MESSAGE_MAP(PCClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTONCONNECT, &PCClientDlg::OnBnClickedButtonconnect)
	ON_BN_CLICKED(IDC_BUTTONSTOP, &PCClientDlg::OnBnClickedButtonstop)
	ON_BN_CLICKED(IDC_BUTTONQUIT, &PCClientDlg::OnBnClickedButtonquit)
	ON_BN_CLICKED(IDC_BUTTONSEND, &PCClientDlg::OnBnClickedButtonsend)
	ON_WM_SETFOCUS()
	ON_WM_SETFOCUS()
	ON_EN_CHANGE(IDC_EDITSENDBOX, &PCClientDlg::OnEnChangeEditsendbox)
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

	SetDlgItemText(IDC_IPADDRESS,_T("127.0.0.1"));
	SetDlgItemText(IDC_EDITPORT,_T("8888"));
	::HideCaret(GetDlgItem(IDC_EDITREVBOX)->GetSafeHwnd());

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

HCURSOR PCClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void PCClientDlg::OnBnClickedButtonconnect()
{
	CString strIp;
	UINT port = GetDlgItemInt(IDC_EDITPORT);
	GetDlgItemText(IDC_IPADDRESS, strIp);
	if (strIp == _T("0.0.0.0") || (port >= 65535 && port < 1024) || port == 0)
	{
		MessageBox(_T("请输入正确IP地址或端口"));
	}

	m_Client->SetPort(port);
	m_Client->SetIp(std::string(CT2A(strIp.GetString())));

	m_Client->Connect();

	sockaddr_in cliAddr;
	int len = sizeof(cliAddr);
	if (getsockname(m_ClientSock, (sockaddr*)&cliAddr, &len) != 0)
	{
		MessageBox(_T("获取客户端端口失败！"));
		return;
	}

	CString strCliTitle;
	strCliTitle.Format(_T("Client : %d"), ntohs(cliAddr.sin_port));
	SetWindowTextW(strCliTitle);

	SetRevBoxText("连接服务器成功\r\n");
	EnableWindow(IDC_BUTTONSTOP, TRUE);
	EnableWindow(IDC_BUTTONCONNECT, FALSE);

	m_ServerStatus = ServerStatus::ON;

	OnEnChangeEditsendbox();
}

BOOL PCClientDlg::EnableWindow(DWORD DlgId, BOOL bUsed)
{
	return GetDlgItem(DlgId)->EnableWindow(bUsed);
}

void PCClientDlg::SetRevBoxText(const std::string& msg)
{
	m_EditRevBox.SetSel(-1,-1);
	m_EditRevBox.ReplaceSel(_T("\r\n  ") + CString(msg.c_str()) + _T("\r\n"));
}

void PCClientDlg::OnBnClickedButtonstop()
{
	EnableWindow(IDC_BUTTONCONNECT,TRUE);
	EnableWindow(IDC_BUTTONSEND,FALSE);
	EnableWindow(IDC_BUTTONSTOP,FALSE);
	closesocket(m_ClientSock);
	m_ServerStatus = ServerStatus::OFF;
}


void PCClientDlg::OnBnClickedButtonquit()
{
	OnBnClickedButtonstop();
	SendMessage(WM_CLOSE);
}


void PCClientDlg::OnBnClickedButtonsend()
{
	USES_CONVERSION;
	char szBuf[256] = {0};
	CString strGetMsg;
	int iWrite;
	GetDlgItemText(IDC_EDITSENDBOX,strGetMsg);
	strcpy_s(szBuf,T2A(strGetMsg));
	iWrite = send(m_ClientSock,szBuf,256,0);
	if(SOCKET_ERROR == iWrite){
		SetRevBoxText("发送错误\r\n");
	}
	SetRevBoxText("我自己 >>" + std::string(CT2A(strGetMsg.GetString())));
	SetDlgItemText(IDC_EDITSENDBOX,_T(""));
	return; 
}

void PCClientDlg::OnEnChangeEditsendbox()
{
	CString strMsg;
	GetDlgItemText(IDC_EDITSENDBOX,strMsg);
	if (_T("") == strMsg || m_ServerStatus == ServerStatus::OFF)
	{
		EnableWindow(IDC_BUTTONSEND,FALSE);
	}
	else 
	{
		EnableWindow(IDC_BUTTONSEND,TRUE);
	}
}
