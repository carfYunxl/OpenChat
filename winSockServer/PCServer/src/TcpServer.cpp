#include "../include/TcpServer.h"


TcpServer::TcpServer()
{
    mTcpSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
}