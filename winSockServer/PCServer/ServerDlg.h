#pragma once
#include "afxwin.h"
#include "ClientItem.h"
#include "resource.h"

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

	HICON m_hIcon;

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedMfcbuttonSend();
	afx_msg void OnBnClickedMfcbuttonOpen();
	afx_msg void OnBnClickedMfcbuttonClose();
	afx_msg void OnEnChangeEditSend();
	DECLARE_MESSAGE_MAP();

public:
	BOOL EnableWindow(DWORD DlgId, BOOL bUsed);
	void AddInfo(const std::string& strMsg);
	void SendClientMsg(const std::string& strMsg,const CClientItem * client);
	BOOL TrayMyIcon(BOOL isAdd);
	void InsertClient(const sockaddr_in& clientAddr);
private:
	TcpServer*	m_Server;
	size_t		m_ServerPort;
	CListBox	mInfoBox;
	CListCtrl	mClientList;
	CEdit		mEditSend;
};
