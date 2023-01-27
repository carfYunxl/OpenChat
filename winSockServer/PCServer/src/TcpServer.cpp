#include "../include/TcpServer.h"

TcpServer::TcpServer(size_t port) :
    mLisSock(INVALID_SOCKET)
    , mPort(port)
    , mRun(FALSE)
{
}

BOOL TcpServer::Init()
{
    //create a listen socket
    mLisSock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    if (mLisSock == INVALID_SOCKET)
    {
        return;
    }

    //bind
    sockaddr_in serAddr;
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(mPort);
    serAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

    int ret = bind(mLisSock,(sockaddr*)&serAddr,sizeof(sockaddr_in));

    if (ret == -1)
    {
        return;
    }

    /**
    * @brif 
    * @param    SOCKET      listen socket
    * @param    backlog     the length of the queue of connection;
    *                       wifi        SOMAXCONN
    *                       bluetooth   2-4
    *                       few client  2-4
    *           to save source;
    * @return   status      -1 error
    */
    ret = listen(mLisSock,SOMAXCONN);

    if (ret == -1)
    {
        return;
    }
}

void TcpServer::Start()
{
    if (Init())
    {
        //进入循环监听模式
        AfxBeginThread(SelectFunc,this);
    }
}

void TcpServer::Stop()
{

}

/**
 * select the typical socket which hSocket means
 * 
 * \param hSocket   socket
 * \param nTimeOut  the time value of timeout
 * \param bRead     indicate read or write
 * \return 
 */
BOOL Select(SOCKET hSocket, DWORD nTimeOut, BOOL bRead)
{
    nTimeOut = nTimeOut > 1000 ? 1000 : nTimeOut;
    timeval time{ 0,nTimeOut };

    FD_SET fdset;
    FD_ZERO(&fdset);
    FD_SET(hSocket, &fdset);

    int ret = 0;
    if (bRead)
    {
        ret = select(0, &fdset, NULL, NULL, &time);
    }
    else
    {
        ret = select(0, NULL, &fdset, NULL, &time);
    }

    if (ret <= 0)
    {
        return FALSE;
    }

    if (!FD_ISSET(hSocket, &fdset))
    {
        return FALSE;
    }
    return TRUE;
}

/**
 * callback function to select the typical socket
 * 
 * \param Lparam
 * \return 
 */
UINT SelectFunc(LPVOID Lparam)
{
    TcpServer* server = (TcpServer*)Lparam;
    while (server->IsRun())
    {
        if (Select(server->GetSocket(),100,TRUE))
        {
            sockaddr_in cliAddr;
            int addLen = sizeof(sockaddr_in);

            SOCKET connSock = accept(server->GetSocket(),(sockaddr*)&cliAddr,&addLen);
            if (connSock == INVALID_SOCKET)
            {
                continue;
            }
        }
    }
}

/**
 * 获取已连接客户端的相关连接信息信息
 *
 * \param cliAddr   【input】 已连接客户端结构体信息
 * \param item      【output】导出客户端信息
 */
void TcpServer::GetClientInfo(const sockaddr_in& cliAddr, CClientItem& item)
{

}