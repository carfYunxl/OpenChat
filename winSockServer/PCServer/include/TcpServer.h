/*****************************************************************//**
 * \file   TcpServer.h
 * \brief  this class is a easy use class to create a Tcp server.
 * 
 * \author yxl
 * \date   January 2023
 *********************************************************************/
#ifndef TCP_SERVER_H_

#define TCP_SERVER_H_
#include "stdafx.h"
#include <winsock2.h>
#include "ClientItem.h"
#include <vector>

constexpr size_t MAX_BUFF = 256;

class TcpServer
{
    using size_t = unsigned int;
    using cItem = std::vector<CClientItem>;
public:
    TcpServer(size_t port, void* parent);

    void Start();
    void Stop();

    SOCKET  GetSocket() { return mLisSock; }

    void    PushConInfo
    (
        const sockaddr_in& cliAddr,
        const SOCKET& connSock
    )
    {
        mConVec.emplace_back(inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port), connSock);
    };

    BOOL    IsRun() { return mRun; }
    void    SetRun(BOOL run) { mRun = run; }
    void    SetPort(size_t port) { mPort = port; }
    size_t  ClientNum() { return mConVec.size(); }

    CClientItem GetClient(size_t cIndex) { return mConVec.at(cIndex); }

    void SelectFunc();

    void ClientFunc(const CClientItem& client,void* pMainWin);
private:
    SOCKET  mLisSock;       //监听Socket
    size_t  mPort;          //服务器端口
    BOOL    mRun;           //server是否在工作
    cItem   mConVec;        //保存所有连接上的ClientItem

    void*   mpMainWind;

private:
    bool    Init();
    void    UnInit();
};

bool Select(SOCKET hSocket, DWORD nTimeOut, BOOL bRead);

#endif //TCP_SERVER_H_


