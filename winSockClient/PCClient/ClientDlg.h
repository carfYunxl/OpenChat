#pragma once
#include "afxwin.h"
#include "resource.h"
#include "common_header.h"

class TcpClient;
class PCClientDlg : public CDialogEx
{
public:
	PCClientDlg(CWnd* pParent = NULL);
	~PCClientDlg();
	enum { IDD = IDD_PCCLIENT_DIALOG };
protected:
	HICON m_hIcon;

	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedMfcbuttonConnect();
	afx_msg void OnBnClickedMfcbuttonDisconnect();
	DECLARE_MESSAGE_MAP()
public:
	BOOL EnableWindow(DWORD DlgId, BOOL bUsed);
	void SetServerState(ServerStatus state) { m_ServerStatus = state; }
	void AddInfo(const std::string& info);
private:
	ServerStatus	m_ServerStatus;
	TcpClient*		m_Client;
	CEdit			mEditSend;
	CListBox		mInfoBox;
};