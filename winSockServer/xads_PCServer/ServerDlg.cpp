#include "stdafx.h"
#include "Server.h"
#include "ServerDlg.h"
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


Cxads_PCServerDlg::Cxads_PCServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Cxads_PCServerDlg::IDD, pParent)
	, m_isServerOpen(FALSE)
	, m_SockListen(NULL)
	, m_ServicePort(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

Cxads_PCServerDlg::~Cxads_PCServerDlg()
{
	
}

void Cxads_PCServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDITGETBOX, m_EditRevBox);
}

BEGIN_MESSAGE_MAP(Cxads_PCServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTONSTART, &Cxads_PCServerDlg::OnBnClickedButtonstart)
	ON_BN_CLICKED(IDC_BUTTONEND, &Cxads_PCServerDlg::OnBnClickedButtonend)
	ON_BN_CLICKED(IDC_BUTTONQUIT, &Cxads_PCServerDlg::OnBnClickedButtonquit)
	ON_BN_CLICKED(IDC_BUTTONSEND, &Cxads_PCServerDlg::OnBnClickedButtonsend)
	ON_EN_CHANGE(IDC_EDITSENDBOX, &Cxads_PCServerDlg::OnEnChangeEditsendbox)
	ON_MESSAGE(WM_TRAYICON_SERVER,OnTrayCallbackMsg)
	ON_BN_CLICKED(IDC_BUTTONHIDE, &Cxads_PCServerDlg::OnBnClickedButtonhide)
	ON_COMMAND(ID_MENU_SHOW, &Cxads_PCServerDlg::OnMenuShow)
	ON_COMMAND(ID_MENU_QUIT, &Cxads_PCServerDlg::OnMenuQuit)
	ON_COMMAND(ID_MENU_SERVER, &Cxads_PCServerDlg::OnMenuServer)
	ON_WM_SIZE()
END_MESSAGE_MAP()

BOOL Cxads_PCServerDlg::OnInitDialog()
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

void Cxads_PCServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void Cxads_PCServerDlg::OnPaint()
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

HCURSOR Cxads_PCServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL Cxads_PCServerDlg::EnableWindow(DWORD DlgId, BOOL bUsed)
{
	return GetDlgItem(DlgId)->EnableWindow(bUsed);
}

void Cxads_PCServerDlg::OnBnClickedButtonstart()
{
	m_ServicePort = GetDlgItemInt(IDC_EDITPORT);
	if (m_ServicePort <= 1024 || m_ServicePort > 65535)
	{
		AfxMessageBox(_T("��������ʶ˿�"));
		SetDlgItemText(IDC_EDITPORT,_T(""));
		GetDlgItem(IDC_EDITPORT)->SetFocus();
		return ;
	}
	StartServer();
	m_isServerOpen = TRUE;
	OnEnChangeEditsendbox();

	CString strServerTitle;
	strServerTitle.Format(_T("Server : %d"),m_ServicePort);
	SetWindowText(strServerTitle);
}


BOOL Cxads_PCServerDlg::StartServer(void)
{
	//�����߳�
	AfxBeginThread(ListenThreadFunc,this);
	return TRUE;
}

UINT ListenThreadFunc(LPVOID Lparam)
{
	Cxads_PCServerDlg * pServer = (Cxads_PCServerDlg *)Lparam;;
	if (INVALID_SOCKET == (pServer->m_SockListen = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)))
	{
		AfxMessageBox(_T("����socketʧ��"));
		return 0;
	}
	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = htonl(INADDR_ANY);
	service.sin_port = htons(pServer->m_ServicePort);
	if (0 != bind(pServer->m_SockListen,(sockaddr *)&service,sizeof(sockaddr_in)))
	{
		AfxMessageBox(_T("�󶨶˿�ʧ��"));
		return 0;
	}
	if (0 != listen(pServer->m_SockListen,5))
	{
		AfxMessageBox(_T("�����˿�ʧ��"));
		return 0;
	}

	//��ʾ����socket�ɹ�
	pServer->EnableWindow(IDC_BUTTONEND,TRUE);
	pServer->EnableWindow(IDC_BUTTONSTART,FALSE);
	pServer->SetRevBoxText(_T("���������ɹ�,��ʼ�����˿�"));
	//����ѭ���������˿�
	while (pServer->m_isServerOpen)
	{
		if (socket_Select(pServer->m_SockListen,100,TRUE))
		{
			sockaddr_in clientAddr;
			int iLen = sizeof(sockaddr_in);
			SOCKET accSock = accept(pServer->m_SockListen,(struct sockaddr *)&clientAddr,&iLen);
			if (accSock == INVALID_SOCKET)
			{
				continue;
			}
			//���ڵ��������
			CClientItem cItem;
			cItem.cSocket = accSock;						//�ͻ��˵�Socket��������ͻ���ͨ��
			cItem.cIp = inet_ntoa(clientAddr.sin_addr);		//IP��ַ
			cItem.cPort = ntohs(clientAddr.sin_port);		//�˿�
			cItem.m_pMainWnd = pServer;						//Serverָ��
			int idx = pServer->m_ClientArray.Add(cItem);	//idx�ǵ�x�����ӵĿͻ���
			AfxBeginThread(ClientThreadProc, &pServer->m_ClientArray.GetAt(idx));

			CString strCliPort;
			strCliPort.Format(_T(":%d"),cItem.cPort);
			pServer->SetRevBoxText(_T("[") + cItem.cIp + strCliPort + _T("] ����"));
			Sleep(100);
		}
	}
}

#define MAX_BUFF 256
UINT ClientThreadProc(LPVOID Lparam)
{ 
	//�����첽IOģ��ѭ����ȡsocket�ڵ���Ϣ�������͸������û�
	USES_CONVERSION;
	CString strMsg;
	CClientItem ClientItem = *(CClientItem *)Lparam;
	while (TRUE)
	{
		if (socket_Select(ClientItem.cSocket,100,TRUE))
		{
			char szRev[MAX_BUFF] = {0};
			int iRet = recv(ClientItem.cSocket,szRev,sizeof(szRev),0);
			if (iRet > 0)
			{
				strMsg = A2T(szRev); //���ĳ������룬Ӣ������
//				strMsg.Format(_T("%s"),szRev); //��ôд��Ӣ�Ķ�������
				ClientItem.m_pMainWnd->SetRevBoxText(ClientItem.cIp + _T(">>") + strMsg);
				ClientItem.m_pMainWnd->SendClientMsg(strMsg,&ClientItem);
			}else{
				strMsg = ClientItem.cIp + _T(" ���뿪");
				ClientItem.m_pMainWnd->RemoveClientFromArray(ClientItem);
				ClientItem.m_pMainWnd->SetRevBoxText(strMsg);
				break;
			}
		}
	}
	return 0;
}

BOOL socket_Select(SOCKET hSocket,DWORD nTimeOut,BOOL bRead){
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

void Cxads_PCServerDlg::OnBnClickedButtonend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int AllClient = m_ClientArray.GetCount();
	for (int idx = 0 ; idx < AllClient ; idx++)
	{
		closesocket(m_ClientArray.GetAt(idx).cSocket);
	}
	m_ClientArray.RemoveAll();
	closesocket(m_SockListen);
	EnableWindow(IDC_BUTTONSEND,FALSE);
	EnableWindow(IDC_BUTTONSTART,TRUE);
	EnableWindow(IDC_BUTTONEND,FALSE);
	SetRevBoxText(_T("ֹͣ�����˿�"));
	m_isServerOpen = FALSE;
}

//�����ı����ı�
void Cxads_PCServerDlg::SetRevBoxText(CString strMsg){
	m_EditRevBox.SetSel(-1,-1);
	m_EditRevBox.ReplaceSel(GetTime() + _T("\r\n  ") + strMsg + _T("\r\n"));
}

//�ͻ������ߣ��������Ƴ��ýڵ�
void Cxads_PCServerDlg::RemoveClientFromArray(CClientItem in_item)
{
	for (int idx = 0 ; idx < m_ClientArray.GetCount() ; idx++)
	{
		if (in_item.cSocket == m_ClientArray[idx].cSocket &&
			in_item.cIp == m_ClientArray[idx].cIp &&
			in_item.m_pMainWnd == m_ClientArray[idx].m_pMainWnd)
		{
			m_ClientArray.RemoveAt(idx);
		}
	}
	return;
}

CString GetTime(){
	SYSTEMTIME time;
	CString strTime;
	GetLocalTime(&time);
	strTime.Format(_T("%d%02d%02d %02d:%02d"),time.wYear,time.wMonth,time.wDay,
		time.wHour,time.wMinute);
	return strTime;
}

//�˳���ť
void Cxads_PCServerDlg::OnBnClickedButtonquit()
{
	if (m_isServerOpen)
	{
		OnBnClickedButtonend();
	}
	SendMessage(WM_CLOSE);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void Cxads_PCServerDlg::SendClientMsg(CString strMsg,CClientItem * pWhoseItem)
{
	USES_CONVERSION;
	char szBuf[256] = {0};
	strcpy_s(szBuf,256,T2A(strMsg));
	for (int i = 0 ; i < m_ClientArray.GetCount() ; i++)
	{
		if (!pWhoseItem || !equal(pWhoseItem,&(m_ClientArray.GetAt(i))))
		{
			send(m_ClientArray.GetAt(i).cSocket,szBuf,256,0);
		}
	}
}

BOOL Cxads_PCServerDlg::equal(const CClientItem * p1 , const CClientItem * p2)
{
	if (p1->cSocket == p2->cSocket && p1->cIp == p2->cIp)
	{
		return TRUE;
	} 
	else
	{
		return FALSE;
	}
}

void Cxads_PCServerDlg::OnBnClickedButtonsend()
{
	CString strMsg;
	GetDlgItemText(IDC_EDITSENDBOX,strMsg);
	SendClientMsg(strMsg,NULL);
	SetRevBoxText(_T("������>>") + strMsg);
	SetDlgItemText(IDC_EDITSENDBOX,_T(""));
}


/**
* �������������ݣ���enable���Ͱ�ť
*/
void Cxads_PCServerDlg::OnEnChangeEditsendbox()
{
	CString strMsg;
	GetDlgItemText(IDC_EDITSENDBOX,strMsg);
	if (strMsg == _T("") || !m_isServerOpen)
	{
		EnableWindow(IDC_BUTTONSEND,FALSE);
	}
	else{
		EnableWindow(IDC_BUTTONSEND,TRUE);
	}
}

BOOL Cxads_PCServerDlg::TrayMyIcon(BOOL isAdd)
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

LRESULT Cxads_PCServerDlg::OnTrayCallbackMsg(WPARAM wparam , LPARAM lparam)
{
	CBitmap IconBitmapOn;
	CBitmap IconBitmapOff;
	CMenu mMenu, * pMenu = NULL;
//	CBitmap IconBitmapOn ,IconBitmapOff;
	IconBitmapOn.LoadBitmap(IDB_BITMAPON);
	IconBitmapOff.LoadBitmap(IDB_BITMAPOFF);
	if (m_isServerOpen)
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
void Cxads_PCServerDlg::OnBnClickedButtonhide()
{
	ShowWindow(SW_MINIMIZE);
	Sleep(200);
	TrayMyIcon(TRUE);
}

//����˵��ϵ���ʾ
void Cxads_PCServerDlg::OnMenuShow()
{
	TrayMyIcon(FALSE);
	ShowWindow(SW_RESTORE);
	SetForegroundWindow();
}

//����˵����˳�
void Cxads_PCServerDlg::OnMenuQuit()
{
	TrayMyIcon(FALSE);
	OnBnClickedButtonquit();
}

//����˵��ϴ�/�رշ�������ѡ��
void Cxads_PCServerDlg::OnMenuServer()
{
	if (TRUE == m_isServerOpen)
	{
		OnBnClickedButtonend();
	} 
	else
	{
		OnBnClickedButtonstart();
	}

}

//�����ڴ�С�ı�ʱ���͸���Ϣ
void Cxads_PCServerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if (SIZE_MINIMIZED == nType) //������С���������ص�����
	{
		ShowWindow(SW_MINIMIZE);
		Sleep(200);
		TrayMyIcon(TRUE);
	}
}
