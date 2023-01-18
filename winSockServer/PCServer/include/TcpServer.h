#ifndef TCP_SERVER_H_
#define TCP_SERVER_H_
#include "../stdafx.h"
#include "winsock2.h"

class TcpServer
{
public:
    TcpServer();
private:
    SOCKET mTcpSocket;
};

#endif //TCP_SERVER_H_