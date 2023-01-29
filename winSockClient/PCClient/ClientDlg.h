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
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	HICON m_hIcon;

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedButtonstop();
	afx_msg void OnBnClickedButtonquit();                       
	afx_msg void OnBnClickedButtonsend();
	afx_msg void OnEnChangeEditsendbox();
	afx_msg void OnBnClickedButtonconnect();
	DECLARE_MESSAGE_MAP()
public:
	BOOL EnableWindow(DWORD DlgId, BOOL bUsed);
	void SetRevBoxText(const std::string& msg);

	void SetServerState(ServerStatus state) { m_ServerStatus = state; }

private:
	ServerStatus	m_ServerStatus;
	CEdit			m_EditRevBox;
	TcpClient*		m_Client;
};