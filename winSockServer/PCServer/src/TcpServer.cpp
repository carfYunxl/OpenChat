#include "../include/TcpServer.h"

TcpServer::TcpServer(size_t port) :
    mListenSocket(INVALID_SOCKET)
    , mPort(port)
    , bServerStart(FALSE)
{
    //mTcpSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
}

void TcpServer::InitServer()
{
    //create a listen socket
    mListenSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    if (mListenSocket == INVALID_SOCKET)
    {
        return;
    }

    //bind
    sockaddr_in serAddr;
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(mPort);
    serAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

    bind(mListenSocket,(sockaddr*)&serAddr,sizeof(sockaddr_in));

}

void TcpServer::StartServer()
{

}

void TcpServer::StopServer()
{

}