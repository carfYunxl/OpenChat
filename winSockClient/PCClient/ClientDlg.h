#pragma once
#include "afxwin.h"
#include "resource.h"
#include "common_header.h"
constexpr size_t LOGIN = 11;
constexpr size_t REGIS = 10;

class TcpClient;
namespace R
{
	class RMysql;
}

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
	DECLARE_MESSAGE_MAP()
public:
	BOOL	EnableWindow(DWORD DlgId, BOOL bUsed);
	void	SetServerState(ServerStatus state) { m_ServerStatus = state; }
	void	AddInfo(const std::string& info);
	size_t	GetPort() { return m_port; }
	std::string	GetQueryString(const std::string& input)
	{
		return "'" + input + "'";
	}

	SOCKET GetClientSocket();

private:
	ServerStatus		m_ServerStatus;
	TcpClient*			m_Client;
	CRichEditCtrl		mEditSend;
	CRichEditCtrl		mInfoBox;
	size_t				m_port;
private:
	bool ConnectToServer();
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedMfcbuttonConnect();
};