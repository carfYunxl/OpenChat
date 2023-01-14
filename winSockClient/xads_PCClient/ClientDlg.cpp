#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"

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

Cxads_PCClientDlg::Cxads_PCClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Cxads_PCClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_ClientSock = NULL;
	isServerOn = FALSE;
}

void Cxads_PCClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDITREVBOX, m_EditRevBox);
}

BEGIN_MESSAGE_MAP(Cxads_PCClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTONCONNECT, &Cxads_PCClientDlg::OnBnClickedButtonconnect)
	ON_BN_CLICKED(IDC_BUTTONSTOP, &Cxads_PCClientDlg::OnBnClickedButtonstop)
	ON_BN_CLICKED(IDC_BUTTONQUIT, &Cxads_PCClientDlg::OnBnClickedButtonquit)
	ON_BN_CLICKED(IDC_BUTTONSEND, &Cxads_PCClientDlg::OnBnClickedButtonsend)
	ON_WM_SETFOCUS()
	ON_WM_SETFOCUS()
	ON_EN_CHANGE(IDC_EDITSENDBOX, &Cxads_PCClientDlg::OnEnChangeEditsendbox)
END_MESSAGE_MAP()

BOOL Cxads_PCClientDlg::OnInitDialog()
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

	// TODO: 在此添加额外的初始化代码
	SetDlgItemText(IDC_IPADDRESS,_T("127.0.0.1"));
	SetDlgItemText(IDC_EDITPORT,_T("8260"));
	::HideCaret(GetDlgItem(IDC_EDITREVBOX)->GetSafeHwnd());

	return TRUE;
}

void Cxads_PCClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void Cxads_PCClientDlg::OnPaint()
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

HCURSOR Cxads_PCClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void Cxads_PCClientDlg::OnBnClickedButtonconnect()
{
	CreateThread(0,0,ConnectNetworkThread,this,0,NULL);
}

DWORD WINAPI ConnectNetworkThread(  LPVOID lpParameter)
{
	Cxads_PCClientDlg * pClient = (Cxads_PCClientDlg *)lpParameter;
	if(pClient->ConnectSocket(pClient))
	{
		
	}
	return 0;
}

#define MAX_BUFF 256
BOOL Cxads_PCClientDlg::ConnectSocket(Cxads_PCClientDlg * pClient)
{
	m_ClientSock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (NULL == m_ClientSock)
	{
		MessageBox(_T("创建socket失败"));
		return FALSE;
	}
	sockaddr_in sa;
	sa.sin_family = AF_INET;
	CString strIp;
	DWORD dPort = GetDlgItemInt(IDC_EDITPORT);
	GetDlgItemText(IDC_IPADDRESS,strIp);
	if (strIp == _T("0.0.0.0") || (dPort >= 65535 && dPort < 1024) || dPort == 0)
	{
		MessageBox(_T("请输入正确IP地址或端口"));
		return FALSE;
	}
	sa.sin_port = htons(dPort);
	char szIpAdd[32];
	USES_CONVERSION; //定义后才能使用T2A
	sprintf_s(szIpAdd,32,"%s",T2A(strIp));
	sa.sin_addr.S_un.S_addr = inet_addr(szIpAdd);
	if (SOCKET_ERROR == connect(m_ClientSock,(sockaddr *)&sa,sizeof(sa)))
	{
		MessageBox(_T("连接客户端错误,请检查你填写的IP和端口是否错误"));
		return FALSE;
	}

	pClient->SetRevBoxText(_T("连接服务器成功"));
	pClient->EnableWindow(IDC_BUTTONSTOP,TRUE);
	pClient->EnableWindow(IDC_BUTTONCONNECT,FALSE);
	isServerOn = TRUE;
	OnEnChangeEditsendbox();

	CString strMsg;
	while (TRUE)
	{
		if (socket_Select(m_ClientSock,100,TRUE))
		{
			char szMsg[MAX_BUFF] = {0};
			int iRead = recv(m_ClientSock,szMsg,MAX_BUFF,0);
			if (iRead > 0)
			{
				strMsg = szMsg;
				pClient->SetRevBoxText(strIp + _T(">>") + strMsg);
			} 
			else
			{
				pClient->SetRevBoxText(_T("已断线，请重新连接"));
				isServerOn = FALSE;
				return TRUE;
			}
		}
	}
	return TRUE;
}

BOOL Cxads_PCClientDlg::EnableWindow(DWORD DlgId, BOOL bUsed)
{
	return GetDlgItem(DlgId)->EnableWindow(bUsed);
}

BOOL socket_Select(SOCKET hSocket,DWORD nTimeOut,BOOL bRead)
{
	FD_SET fdset;
	timeval tv;
	FD_ZERO(&fdset);
	FD_SET(hSocket,&fdset);
	nTimeOut = nTimeOut > 1000 ? 1000 : nTimeOut;
	tv.tv_sec = 0;
	tv.tv_usec = nTimeOut;
	int iRet = 0;
	if (bRead)
	{
		iRet = select(0,&fdset,NULL,NULL,&tv);
	} 
	else
	{
		iRet = select(0,NULL,&fdset,NULL,&tv);
	}
	if (iRet <= 0)
	{
		return FALSE;
	} 
	else if (FD_ISSET(hSocket,&fdset))
	{
		return TRUE;
	}
	return FALSE;
}

void Cxads_PCClientDlg::SetRevBoxText(CString strMsg)
{
	m_EditRevBox.SetSel(-1,-1);
	m_EditRevBox.ReplaceSel(GetTime() + _T("\r\n  ") + strMsg + _T("\r\n"));
}

CString GetTime()
{
	SYSTEMTIME time;
	CString strTime;
	GetLocalTime(&time);
	strTime.Format(_T("%d%02d%02d %02d:%02d"),time.wYear,time.wMonth,time.wDay,
		time.wHour,time.wMinute);
	return strTime;
}

void Cxads_PCClientDlg::OnBnClickedButtonstop()
{
	EnableWindow(IDC_BUTTONCONNECT,TRUE);
	EnableWindow(IDC_BUTTONSEND,FALSE);
	EnableWindow(IDC_BUTTONSTOP,FALSE);
	closesocket(m_ClientSock);
	isServerOn = FALSE;
}


void Cxads_PCClientDlg::OnBnClickedButtonquit()
{
	OnBnClickedButtonstop();
	SendMessage(WM_CLOSE);
}


void Cxads_PCClientDlg::OnBnClickedButtonsend()
{
	USES_CONVERSION;
	char szBuf[256] = {0};
	CString strGetMsg;
	int iWrite;
	GetDlgItemText(IDC_EDITSENDBOX,strGetMsg);
	strcpy_s(szBuf,T2A(strGetMsg));
	iWrite = send(m_ClientSock,szBuf,256,0);
	if(SOCKET_ERROR == iWrite){
		SetRevBoxText(_T("发送错误"));
	}
	SetRevBoxText(_T("我自己 >>") + strGetMsg);
	SetDlgItemText(IDC_EDITSENDBOX,_T(""));
	return; 
}

void Cxads_PCClientDlg::OnEnChangeEditsendbox()
{
	CString strMsg;
	GetDlgItemText(IDC_EDITSENDBOX,strMsg);
	if (_T("") == strMsg || !isServerOn)
	{
		EnableWindow(IDC_BUTTONSEND,FALSE);
	}
	else 
	{
		EnableWindow(IDC_BUTTONSEND,TRUE);
	}
}
