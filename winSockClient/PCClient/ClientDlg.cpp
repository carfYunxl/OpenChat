#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"
#include "TcpClient.h"
#include "mysql.h"

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

	ConnectDataBase();

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
		MessageBox(_T("��������ȷIP��ַ��˿�"));
		return;
	}

	m_Client->SetPort(port);
	m_Client->SetIp("127.0.0.1");

	m_Client->Connect();

	sockaddr_in cliAddr;
	int len = sizeof(cliAddr);
	if (getsockname(m_Client->GetSocket(), (sockaddr*)&cliAddr, &len) != 0)
	{
		MessageBox(_T("��ȡ�ͻ��˶˿�ʧ�ܣ�"));
		return;
	}

	CString strCliTitle;
	strCliTitle.Format(_T("Client : %d"), ntohs(cliAddr.sin_port));
	SetWindowTextW(strCliTitle);

	mInfoBox.AddString(_T("���ӷ������ɹ�"));
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

void PCClientDlg::ConnectDataBase()
{
	MYSQL mSql;
	mysql_init(&mSql);

	if (!mysql_real_connect(&mSql, "localhost", "root", "123456", "new", 3306, NULL, 0))
	{
		AfxMessageBox(_T("connect failed!��"));
		return;
	}
	AfxMessageBox(_T("connect success!��"));

	//�������ݿ��ַ���ʽ��������������
	//mysql_query(&mSql,"set names gb1232");
	//mysql_query(&mSql, "set names gbk");

	if (mysql_query(&mSql, "select * from user"))
	{
		AfxMessageBox(_T("query failed!"));
		return;
	}

	MYSQL_FIELD* fd;    //�ֶ�������
	char field[32][32]; //���ֶ�����ά����
	MYSQL_RES* res;     //�е�һ����ѯ�����
	MYSQL_ROW column;   //�����е���

	res = mysql_store_result(&mSql);
	if (!res)
	{
		AfxMessageBox(_T("Couldn't get result from %s\n", mysql_error(&mSql)));
		return ;
	}

	uint64_t rows = mysql_affected_rows(&mSql);

	//��ȡ����
	int j = mysql_num_fields(res);

	//�洢�ֶ���Ϣ
	char* str_field[32];

	//��ȡ�ֶ���
	for (int i = 0; i < j; i++)
	{
		str_field[i] = mysql_fetch_field(res)->name;
	}

	//��ӡ�ֶ�
	CString str;
	for (int i = 0; i < j; i++)
	{
		str.AppendFormat(_T("%S\t"),str_field[i]);
	}
	AfxMessageBox(str);

	//��ӡ��ѯ���
	str.Format(_T(""));
	while (column = mysql_fetch_row(res))
	{
		for (int i = 0; i < j; i++)
		{
			str.AppendFormat(_T("%S\t"),column[i]);
		}
		str.AppendFormat(_T("\n"));
	}
	AfxMessageBox(str);
	mysql_free_result(res);
	mysql_close(&mSql);
}
