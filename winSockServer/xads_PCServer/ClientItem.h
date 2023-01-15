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
	CString cIp;						//�ͻ��˵�IP
	SOCKET	cSocket;					//�ͻ��˵�Socket
	//CString cName;					//�ͻ��˵�����
	HANDLE m_hThread;					//ÿ����һ���ͻ������ӳɹ����������Ǹ��߳̾��
	Cxads_PCServerDlg *m_pMainWnd;		//this server dlg 
};


#endif //_CLIENT_ITEM_H_