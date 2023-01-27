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
#include <vector>

constexpr size_t MAX_BUFF = 256;

class TcpServer
{
    using size_t = unsigned int;
    using cItem = std::vector<CClientItem>;
public:
    TcpServer(size_t port);

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
    size_t  CliNum() { return mConVec.size(); }

    CClientItem GetCli(size_t cIndex) { return mConVec.at(cIndex); }

private:
    SOCKET  mLisSock;       //����Socket
    size_t  mPort;          //�������˿�
    BOOL    mRun;           //server�Ƿ��ڹ���
    cItem   mConVec;        //�������������ϵ�ClientItem

private:
    BOOL    Init();
    void    UnInit();
};

UINT SelectFunc(LPVOID Lparam);
UINT CliFunc(LPVOID Lparam);

BOOL Select(SOCKET hSocket, DWORD nTimeOut, BOOL bRead);

#endif //TCP_SERVER_H_


