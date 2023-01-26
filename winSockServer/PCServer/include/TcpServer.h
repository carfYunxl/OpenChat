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
    SOCKET mListenSocket;       //����Socket
    size_t mPort;               //�������˿�
    BOOL bServerStart;          //server�Ƿ��ڹ���

    void InitServer();
    void StartServer();
    void StopServer();

};

#endif //TCP_SERVER_H_