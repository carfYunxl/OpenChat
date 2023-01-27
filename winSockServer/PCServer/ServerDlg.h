#pragma once
#include "afxwin.h"
#include "ClientItem.h"

CString GetTime();


class TcpServer;
class Cxads_PCServerDlg : public CDialogEx
{
public:
	Cxads_PCServerDlg(CWnd* pParent = NULL);
	~Cxads_PCServerDlg();
	enum { IDD = IDD_XADS_PCSERVER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

	HICON m_hIcon;

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedButtonend();
	afx_msg void OnBnClickedButtonquit();
	afx_msg void OnBnClickedButtonsend();
	afx_msg void OnEnChangeEditsendbox();
	afx_msg void OnBnClickedButtonhide();
	afx_msg void OnMenuShow();
	afx_msg void OnMenuQuit();
	afx_msg void OnMenuServer();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonstart();

	DECLARE_MESSAGE_MAP();

public:
	BOOL EnableWindow(DWORD DlgId, BOOL bUsed);
	void SetRevBoxText(CString strMsg);
	void RemoveClientFromArray(CClientItem in_item);
	void SendClientMsg(CString strMsg,CClientItem * WhoseItem);
	BOOL TrayMyIcon(BOOL isAdd);

	UINT	m_ServicePort;
	CEdit	m_EditRevBox;
private:
	BOOL	equal(const CClientItem *p1 , const CClientItem * p2);
	LRESULT OnTrayCallbackMsg(WPARAM wparam , LPARAM lparam);
private:
	TcpServer* mServer;
};
