/**
* This class hold the information of the client.
* When the client connect to server,server will get all this client information.
*/
#ifndef _CLIENT_ITEM_H_
#define _CLIENT_ITEM_H_


#include "stdafx.h"
class Cxads_PCServerDlg;

class CClientItem
{
public:
	CClientItem():
		cSocket(INVALID_SOCKET),
		m_hThread(NULL),
		m_pMainWnd(NULL)
	{}
	CString cIp;				//客户端的IP
	SOCKET	cSocket;			//客户端的Socket
	//CString cName;				//客户端的名字
	HANDLE m_hThread;
	Cxads_PCServerDlg *m_pMainWnd;
};


#endif //_CLIENT_ITEM_H_