/*****************************************************************//**
 * \file   TcpServer.h
 * \brief  this class is a easy use class to create a Tcp server.
 * 
 * \author yxl
 * \date   January 2023
 *********************************************************************/
#ifndef TCP_SERVER_H_

#define TCP_SERVER_H_
#include "../stdafx.h"
#include <winsock2.h>
#include "../ClientItem.h"

class TcpServer
{
    using size_t = unsigned int;
public:
    TcpServer(size_t port);

    void Start();
    void Stop();

    BOOL    IsRun() { return mRun; }
    void    SetRun(BOOL run) { mRun = run; }
    SOCKET  GetSocket() { return mLisSock; }
    void    GetClientInfo(const sockaddr_in& cliAddr, CClientItem& item);

private:
    SOCKET  mLisSock;       //监听Socket
    size_t  mPort;          //服务器端口
    BOOL    mRun;           //server是否在工作

    BOOL Init();
};

BOOL Select(SOCKET hSocket, DWORD nTimeOut, BOOL bRead);
UINT SelectFunc(LPVOID Lparam);
#endif //TCP_SERVER_H_