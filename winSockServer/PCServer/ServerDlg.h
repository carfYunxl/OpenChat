#pragma once
#include "afxwin.h"
#include "ClientItem.h"
#include "resource.h"

constexpr size_t TOOLBAR_HEIGHT = 80;
constexpr size_t STATUSBAR_HEIGHT = 40;
constexpr size_t CLIENT_LIST_WIDTH = 300;

constexpr size_t TIMER_COUNT = 1;
constexpr size_t TIMER_CLIENT = 2;

class TcpServer;
class PCServerDlg : public CDialogEx
{
public:
	PCServerDlg(CWnd* pParent = NULL);
	~PCServerDlg();
	enum { IDD = IDD_PCSERVER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	HICON m_hIcon;

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBtn_1();
	afx_msg void OnBtn_2();
	afx_msg BOOL OnToolTipNotify(UINT id,NMHDR* pNMHDR,LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP();

public:
	BOOL EnableWindow(DWORD DlgId, BOOL bUsed);
	void AddInfo(const std::string& strMsg);
	void SendClientMsg(const std::string& strMsg,const CClientItem * client);
	void InsertClient(const sockaddr_in& clientAddr,SOCKET socket);
	void RemoveClient(const CClientItem& item);
	void NotifyUi(const std::string& msg);
private:
	TcpServer*	m_Server;
	size_t		m_ServerPort;
	CListBox	mInfoBox;
	CListCtrl	mClientList;
	CEdit		mEditSend;
	CToolBar	mToolBar;
	CImageList	mImageList;
	BOOL		mToolBarVisible;
	CStatusBar  mStatusBar;
	CString		info;
private:
	void InitToolBar();
	void InitStatusBar();
	void MoveControls();
	void InitClientList();
};
