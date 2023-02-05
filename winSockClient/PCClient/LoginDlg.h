#pragma once 

namespace R
{
	class RMysql;
}
class PCClientDlg;
class LoginDlg : public CDialogEx
{
	DECLARE_DYNAMIC(LoginDlg)

public:
	LoginDlg(CWnd* pParent = nullptr);
	virtual ~LoginDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_LOGIN_PAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedMfcbuttonLogin();
	afx_msg void OnBnClickedMfcbuttonCancel();

public:
	CString GetAccount();
	CString GetPassword();
	void SetInfo(const CString& info);
	void Init(PCClientDlg* parent);
private:
	R::RMysql * mysql;
	PCClientDlg* mpParent;
public:
	afx_msg void OnBnClickedMfcbuttonRegister();
};
