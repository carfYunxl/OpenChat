#ifndef _CLIENT_ITEM_H_
#define _CLIENT_ITEM_H_


#include "stdafx.h"
class Cxads_PCServerDlg;

class CClientItem
{
public:
	CClientItem():m_ClientSocket(INVALID_SOCKET), m_hThread(NULL), m_pMainWnd(NULL)
	{}
	CString m_strIp;
	SOCKET m_ClientSocket;
	HANDLE m_hThread;
	Cxads_PCServerDlg *m_pMainWnd;
};


#endif //_CLIENT_ITEM_H_