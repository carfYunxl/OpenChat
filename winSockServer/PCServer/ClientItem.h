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
		cSocket(INVALID_SOCKET)
		,cPort(0)
		,m_pMainWnd(NULL)
	{}
	CString cIp;						//客户端的IP
	size_t	cPort;						//客户端的端口号
	SOCKET	cSocket;					//客户端的Socket
	Cxads_PCServerDlg *m_pMainWnd;		//this server dlg 
};


#endif //_CLIENT_ITEM_H_