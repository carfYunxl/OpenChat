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
	virtual void DoDataExchange(CDataExchange* pDX);

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
	DDX_Control(pDX, IDC_LIST_SHOW, mInfoBox);
	DDX_Control(pDX, IDC_LIST_CLIENT_SHOW, mClientList);
	DDX_Control(pDX, IDC_EDIT_SEND, mEditSend);
}

BEGIN_MESSAGE_MAP(PCServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_COMMAND(ID_BUTTON_1, &OnBtn_1)
	ON_COMMAND(ID_BUTTON_2, &OnBtn_2)
	ON_NOTIFY_EX(TTN_NEEDTEXT,0, OnToolTipNotify)
END_MESSAGE_MAP()

BOOL PCServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	SetDlgItemText(IDC_EDIT_PORT,_T("8888"));
	GetDlgItem(IDC_EDIT_PORT)->SetFocus();

	LONG lStyle;
	lStyle = GetWindowLong(mClientList.m_hWnd, GWL_STYLE);
	lStyle &= ~LVS_TYPEMASK;
	lStyle |= LVS_REPORT;
	SetWindowLong(mClientList.m_hWnd,GWL_STYLE,lStyle);

	DWORD dwStyle = mClientList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;
	dwStyle |= LVS_EX_CHECKBOXES;
	mClientList.SetExtendedStyle(dwStyle);

	mClientList.InsertColumn(0, _T("Port"), LVCFMT_LEFT, 80);
	mClientList.InsertColumn(1, _T("Ip"), LVCFMT_LEFT, 80);
	mClientList.InsertColumn(2, _T("Socket"), LVCFMT_LEFT, 80);

	mToolBar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_BOTTOM);
	mToolBar.LoadToolBar(IDR_TOOLBAR_SERVER);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,0);

	mImageList.Create(48, 48, ILC_COLOR32 | ILC_MASK, 2, 2);
	mImageList.SetBkColor(RGB(255, 255, 255));

	CPngImage* pPngImage = new CPngImage();
	pPngImage->Load(IDB_PNG_OPEN);
	mImageList.Add(pPngImage, RGB(0, 0, 0));
	pPngImage->DeleteObject();

	CPngImage* pPngImage2 = new CPngImage();
	pPngImage2->Load(IDB_PNG_CLOSE);
	mImageList.Add(pPngImage2, RGB(0, 0, 0));
	pPngImage2->DeleteObject();
	mToolBar.GetToolBarCtrl().SetImageList(&mImageList);
	mToolBar.GetToolBarCtrl().EnableButton(ID_BUTTON_2,FALSE);

	return TRUE; 
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

//设置文本框文本
void PCServerDlg::AddInfo(const std::string& strMsg)
{
	mInfoBox.AddString(CString(strMsg.c_str()));
}

void PCServerDlg::SendClientMsg(const std::string& strMsg,const CClientItem * client)
{
	int ret = 0;
	if (client == NULL)
	{
		for (size_t i = 0;i < m_Server->ClientNum();++i)
		{
			ret = send(m_Server->GetClient(i).cSocket, strMsg.c_str(), 256, 0);
		}
	}
	else
	{
		ret = send(client->cSocket, strMsg.c_str(), 256, 0);
	}

	if (SOCKET_ERROR == ret)
	{
		AddInfo("发送错误!\r\n");
	}
}

void PCServerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if (SIZE_MINIMIZED == nType)
	{
		ShowWindow(SW_MINIMIZE);
		Sleep(200);
	}
}

void PCServerDlg::InsertClient(const sockaddr_in& clientAddr, SOCKET socket)
{
	int nRow = mClientList.InsertItem(LVIF_TEXT | LVIF_PARAM, m_Server->ClientNum(),_T(""),0,0,0,0);

	CString strPort;
	strPort.Format(_T("%d"), ntohs(clientAddr.sin_port));

	char szBuf[32];
	CString strIp(inet_ntop(AF_INET,&clientAddr.sin_addr,szBuf,sizeof(szBuf)));

	CString strSocket;
	strSocket.Format(_T("%d"),socket);

	mClientList.SetItemText(nRow, 0, strPort);
	mClientList.SetItemText(nRow, 1, strIp);
	mClientList.SetItemText(nRow, 2, strSocket);
}

void PCServerDlg::RemoveClient(const CClientItem& item)
{
	for (size_t i = 0;i < mClientList.GetItemCount();++i)
	{
		
		if(_ttoi(mClientList.GetItemText(i, 0)) == item.cPort)
		{ 
			mClientList.DeleteItem(i);
			break;
		}
	}
}


BOOL PCServerDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_ESCAPE:
			return TRUE;
			break;
		case VK_RETURN:
		{
			CString strSend;
			mEditSend.GetWindowText(strSend);

			std::string str = CT2A(strSend.GetString());

			for (size_t i = 0; i < mClientList.GetItemCount(); ++i)
			{
				if (mClientList.GetCheck(i))
				{
					SOCKET socket = _tcstoui64(mClientList.GetItemText(i, 2), NULL, 10);
					int sendbyte = send(socket, str.c_str(), str.length(), 0);
				}
			}
			mEditSend.SetWindowTextW(_T(""));
			return TRUE;
		}
		break;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void PCServerDlg::OnBtn_1()
{
	m_ServerPort = GetDlgItemInt(IDC_EDIT_PORT);
	if (m_ServerPort <= 1024 || m_ServerPort > 65535)
	{
		AfxMessageBox(_T("请输入合适端口"));
		SetDlgItemText(IDC_EDIT_PORT, _T(""));
		GetDlgItem(IDC_EDIT_PORT)->SetFocus();
		return;
	}
	//OnEnChangeEditsendbox();

	CString strServerTitle;
	strServerTitle.Format(_T("Server : %d"), m_ServerPort);
	SetWindowText(strServerTitle);

	m_Server->SetPort(GetDlgItemInt(IDC_EDIT_PORT));
	m_Server->Start();

	AddInfo("[SYS] 服务器已开启！\r\n");

	mToolBar.GetToolBarCtrl().EnableButton(ID_BUTTON_1, FALSE);
	mToolBar.GetToolBarCtrl().EnableButton(ID_BUTTON_2, TRUE);
}

void PCServerDlg::OnBtn_2()
{
	m_Server->Stop();
	AddInfo("[SYS] 服务器已关闭！\r\n");

	mToolBar.GetToolBarCtrl().EnableButton(ID_BUTTON_1, TRUE);
	mToolBar.GetToolBarCtrl().EnableButton(ID_BUTTON_2, FALSE);
}

BOOL PCServerDlg::OnToolTipNotify(UINT id, NMHDR* pNMHDR, LRESULT* pResult)
{
	TOOLTIPTEXT* pTTT = (TOOLTIPTEXT*)pNMHDR;
	CString str;

	UINT nId = pNMHDR->idFrom;

	if (nId)
	{
		nId = mToolBar.CommandToIndex(nId);
		if (nId != -1)
		{
			switch(nId)
			{
				case 0:
					pTTT->lpszText = _T("Open");
					break;
				case 1:
					pTTT->lpszText = _T("Close");
					break;
				default:
					pTTT->lpszText = _T("");
					break;
			}

			pTTT->hinst = AfxGetResourceHandle();
			return TRUE;
		}
	}

	return FALSE;
}
