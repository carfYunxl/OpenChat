#pragma once
#include "afxwin.h"

class PCClientDlg : public CDialogEx
{
public:
	PCClientDlg(CWnd* pParent = NULL);
	enum { IDD = IDD_XADS_PCCLIENT_DIALOG };
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
	BOOL ConnectSocket(PCClientDlg *);
	BOOL EnableWindow(DWORD DlgId, BOOL bUsed);
	void SetRevBoxText(CString strMsg);

private:
	BOOL isServerOn;
	SOCKET m_ClientSock;
	CEdit m_EditRevBox;
};

DWORD WINAPI ConnectNetworkThread(  LPVOID lpParameter);
BOOL socket_Select(SOCKET hSocket,DWORD nTimeOut,BOOL bRead);