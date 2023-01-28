#include "stdafx.h"
#include "Server.h"
#include "ServerDlg.h"
#include "afxdialogex.h"
#include "TcpServer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


PCServerDlg::PCServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(PCServerDlg::IDD, pParent)
	, m_ServerPort(0)
	, m_Server(new TcpServer(8888,this))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

PCServerDlg::~PCServerDlg()
{
	delete m_Server;
}

void PCServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDITGETBOX, m_EditRevBox);
}

BEGIN_MESSAGE_MAP(PCServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTONSTART, &PCServerDlg::OnBnClickedButtonstart)
	ON_BN_CLICKED(IDC_BUTTONEND, &PCServerDlg::OnBnClickedButtonend)
	ON_BN_CLICKED(IDC_BUTTONQUIT, &PCServerDlg::OnBnClickedButtonquit)
	ON_BN_CLICKED(IDC_BUTTONSEND, &PCServerDlg::OnBnClickedButtonsend)
	ON_EN_CHANGE(IDC_EDITSENDBOX, &PCServerDlg::OnEnChangeEditsendbox)
	ON_MESSAGE(WM_TRAYICON_SERVER,OnTrayCallbackMsg)
	ON_BN_CLICKED(IDC_BUTTONHIDE, &PCServerDlg::OnBnClickedButtonhide)
	ON_COMMAND(ID_MENU_SHOW, &PCServerDlg::OnMenuShow)
	ON_COMMAND(ID_MENU_QUIT, &PCServerDlg::OnMenuQuit)
	ON_COMMAND(ID_MENU_SERVER, &PCServerDlg::OnMenuServer)
	ON_WM_SIZE()
END_MESSAGE_MAP()

BOOL PCServerDlg::OnInitDialog()
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

	EnableWindow(IDC_BUTTONSEND,FALSE);
	EnableWindow(IDC_BUTTONEND,FALSE);
	SetDlgItemText(IDC_EDITPORT,_T("8888"));
	GetDlgItem(IDC_EDITPORT)->SetFocus();

	return TRUE; 
}

void PCServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void PCServerDlg::OnPaint()
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

HCURSOR PCServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL PCServerDlg::EnableWindow(DWORD DlgId, BOOL bUsed)
{
	return GetDlgItem(DlgId)->EnableWindow(bUsed);
}

void PCServerDlg::OnBnClickedButtonstart()
{
	m_ServerPort = GetDlgItemInt(IDC_EDITPORT);
	if (m_ServerPort <= 1024 || m_ServerPort > 65535)
	{
		AfxMessageBox(_T("��������ʶ˿�"));
		SetDlgItemText(IDC_EDITPORT,_T(""));
		GetDlgItem(IDC_EDITPORT)->SetFocus();
		return ;
	}
	OnEnChangeEditsendbox();

	CString strServerTitle;
	strServerTitle.Format(_T("Server : %d"), m_ServerPort);
	SetWindowText(strServerTitle);

	m_Server->SetPort(GetDlgItemInt(IDC_EDITPORT));
	m_Server->Start();

	EnableWindow(IDC_BUTTONSEND, TRUE);
	EnableWindow(IDC_BUTTONSTART, FALSE);
	EnableWindow(IDC_BUTTONEND, TRUE);

	SetRevBoxText(_T("�������ѿ�����"));
}

void PCServerDlg::OnBnClickedButtonend()
{
	m_Server->Stop();

	EnableWindow(IDC_BUTTONSEND,FALSE);
	EnableWindow(IDC_BUTTONSTART,TRUE);
	EnableWindow(IDC_BUTTONEND,FALSE);
	SetRevBoxText(_T("ֹͣ�����˿�"));
}

//�����ı����ı�
void PCServerDlg::SetRevBoxText(CString strMsg){
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

void PCServerDlg::OnBnClickedButtonquit()
{
	SendMessage(WM_CLOSE);
}

void PCServerDlg::SendClientMsg(const CString& strMsg,const CClientItem * client)
{
	USES_CONVERSION;
	char szBuf[256] = {0};
	strcpy_s(szBuf,256,T2A(strMsg));

	int ret = 0;
	if (client == NULL)
	{
		for (size_t i = 0;i < m_Server->ClientNum();++i)
		{
			ret &= send(m_Server->GetClient(i).cSocket, szBuf, 256, 0);
		}
	}
	else
	{
		ret = send(client->cSocket, szBuf, 256, 0);
	}

	if (SOCKET_ERROR == ret)
	{
		SetRevBoxText(_T("���ʹ���"));
	}
}

void PCServerDlg::OnBnClickedButtonsend()
{
	CString strSerPort;
	strSerPort.Format(_T("[Server : %d]:\n"), m_ServerPort);

	CString strMsg;
	GetDlgItemText(IDC_EDITSENDBOX,strMsg);
	SendClientMsg(strSerPort + strMsg,NULL);

	SetRevBoxText(strSerPort + strMsg);
	SetDlgItemText(IDC_EDITSENDBOX,_T(""));
}

void PCServerDlg::OnEnChangeEditsendbox()
{
	CString strMsg;
	GetDlgItemText(IDC_EDITSENDBOX,strMsg);
	if (strMsg == _T(""))
	{
		EnableWindow(IDC_BUTTONSEND,FALSE);
	}
	else{
		EnableWindow(IDC_BUTTONSEND,TRUE);
	}
}

BOOL PCServerDlg::TrayMyIcon(BOOL isAdd)
{
	BOOL bReturn = FALSE;
	NOTIFYICONDATA tnd;
	tnd.cbSize = sizeof(NOTIFYICONDATA);
	tnd.hWnd = GetSafeHwnd();
	tnd.uID = IDR_MAINFRAME;
	if (TRUE == isAdd)
	{
		tnd.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
		tnd.uCallbackMessage = WM_TRAYICON_SERVER;
		tnd.hIcon = LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
		wcscpy_s(tnd.szTip,_countof(tnd.szTip),L"WIFIͨ�� - �����");
		ShowWindow(SW_HIDE);
		bReturn = Shell_NotifyIcon(NIM_ADD,&tnd);
	} 
	else
	{
		ShowWindow(SW_SHOWNA);
		SetForegroundWindow();
		bReturn = Shell_NotifyIcon(NIM_DELETE,&tnd);
	}
	return bReturn;
}

LRESULT PCServerDlg::OnTrayCallbackMsg(WPARAM wparam , LPARAM lparam)
{
	CBitmap IconBitmapOn;
	CBitmap IconBitmapOff;
	CMenu mMenu, * pMenu = NULL;
	IconBitmapOn.LoadBitmap(IDB_BITMAPON);
	IconBitmapOff.LoadBitmap(IDB_BITMAPOFF);
	if (1)
	{
		mMenu.LoadMenu(IDR_MENU1);
		mMenu.SetMenuItemBitmaps(ID_MENU_SERVER,MF_BYCOMMAND | MF_STRING | MF_ENABLED,&IconBitmapOff,&IconBitmapOn);
		mMenu.ModifyMenu(ID_MENU_SERVER,MF_BYCOMMAND,ID_MENU_SERVER,_T("�رշ�����"));
	} 
	else
	{
		mMenu.LoadMenu(IDR_MENU1);
		mMenu.SetMenuItemBitmaps(ID_MENU_SERVER,MF_BYCOMMAND | MF_STRING | MF_ENABLED,&IconBitmapOn,&IconBitmapOff);
		mMenu.ModifyMenu(ID_MENU_SERVER,MF_BYCOMMAND,ID_MENU_SERVER,_T("�򿪷�����"));
	}
	switch(lparam){
	case WM_RBUTTONUP:
		{
			CPoint pt;
			GetCursorPos(&pt); //����������λ��
			pMenu = mMenu.GetSubMenu(0);
			SetForegroundWindow(); //���ӱ�����˵���������ʧ
			pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,pt.x,pt.y,this);
			break;
		}
	case WM_LBUTTONDBLCLK:
		{
			ShowWindow(SW_RESTORE);
			SetForegroundWindow();
			TrayMyIcon(FALSE);
			break;
		}
	default:
		break;
	}
	return NULL;
}

//������ذ�ť
void PCServerDlg::OnBnClickedButtonhide()
{
	ShowWindow(SW_MINIMIZE);
	Sleep(200);
	TrayMyIcon(TRUE);
}

//����˵��ϵ���ʾ
void PCServerDlg::OnMenuShow()
{
	TrayMyIcon(FALSE);
	ShowWindow(SW_RESTORE);
	SetForegroundWindow();
}

//����˵����˳�
void PCServerDlg::OnMenuQuit()
{
	TrayMyIcon(FALSE);
	OnBnClickedButtonquit();
}

//����˵��ϴ�/�رշ�������ѡ��
void PCServerDlg::OnMenuServer()
{
	if (1)
	{
		OnBnClickedButtonend();
	} 
	else
	{
		OnBnClickedButtonstart();
	}

}

//�����ڴ�С�ı�ʱ���͸���Ϣ
void PCServerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if (SIZE_MINIMIZED == nType) //������С���������ص�����
	{
		ShowWindow(SW_MINIMIZE);
		Sleep(200);
		TrayMyIcon(TRUE);
	}
}
