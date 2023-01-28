/**
* This class hold the information of the client.
* When the client connect to server,server will get all this client information.
*/
#ifndef _CLIENT_ITEM_H_
#define _CLIENT_ITEM_H_

#include "stdafx.h"
#include <string>

class PCServerDlg;

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

	bool operator==(const CClientItem& rhs)
	{
		return (this->cAddr == rhs.cAddr) && (cPort == rhs.cPort) && (cSocket == rhs.cSocket);
	}
	std::string	cAddr;						//客户端的IP
	size_t		cPort;						//客户端的端口号
	SOCKET		cSocket;					//客户端的Socket
};


#endif //_CLIENT_ITEM_H_