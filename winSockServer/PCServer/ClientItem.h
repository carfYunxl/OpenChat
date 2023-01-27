/**
* This class hold the information of the client.
* When the client connect to server,server will get all this client information.
*/
#ifndef _CLIENT_ITEM_H_
#define _CLIENT_ITEM_H_

#include "stdafx.h"
#include <string>

class Cxads_PCServerDlg;

class CClientItem
{
public:
	CClientItem() = default;
	CClientItem(const char* addr,size_t port,SOCKET socket)
		: cAddr(addr)
		, cPort(port)
		, cSocket(socket)
	{}

	void operator=(const CClientItem& rhs) 
	{
		this->cAddr = rhs.cAddr;
		this->cPort = rhs.cPort;
		this->cSocket = rhs.cSocket;
	}
	std::string	cAddr;						//�ͻ��˵�IP
	size_t		cPort;						//�ͻ��˵Ķ˿ں�
	SOCKET		cSocket;					//�ͻ��˵�Socket
};


#endif //_CLIENT_ITEM_H_