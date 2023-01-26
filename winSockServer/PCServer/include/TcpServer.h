/**
* this class used to create a tcp server automatically.
*/
#ifndef TCP_SERVER_H_

#define TCP_SERVER_H_
#include "../stdafx.h"
#include <winsock2.h>

class TcpServer
{
    using size_t = unsigned int;
public:
    TcpServer(size_t port);
private:
    SOCKET mListenSocket;       //监听Socket
    size_t mPort;               //服务器端口
    BOOL bServerStart;          //server是否在工作

    void InitServer();
    void StartServer();
    void StopServer();

};

#endif //TCP_SERVER_H_