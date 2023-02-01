/*****************************************************************//**
 * @file   TcpServer.h
 * @brief  this class is a easy use class to create a Tcp server on win32.
 * 
 * @author yxl
 * @date   January 2023
 *********************************************************************/
#ifndef TCP_SERVER_H_

#define TCP_SERVER_H_
#include "stdafx.h"
#include "ClientItem.h"
#include "common_header.h"

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
    void    AddClient(const sockaddr_in& cliAddr,const SOCKET& connSock)
    {
        mClientVec.emplace_back(inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port), connSock);
    };

    BOOL    IsRun() { return mRun; }
    void    SetRun(BOOL run) { mRun = run; }
    void    SetPort(size_t port) { mPort = port; }
    size_t  ClientNum() { return mClientVec.size(); }
    void    SelectFunc(void* pMainWin);
    void    ClientFunc(const CClientItem& client,void* pMainWin);

    CClientItem GetClient(size_t cIndex) { return mClientVec.at(cIndex); }
private:
    SOCKET  mLisSock;       //监听Socket
    size_t  mPort;          //服务器端口
    BOOL    mRun;           //server是否在工作
    cItem   mClientVec;     //保存所有连接上的ClientItem
    void*   mpMainWind;
private:
    bool    Init();
    void    UnInit();
    bool    Select(SOCKET socket, long nTimeOut, MODE mode);
    void    DeleteClient(size_t port);
};

#endif //TCP_SERVER_H_

