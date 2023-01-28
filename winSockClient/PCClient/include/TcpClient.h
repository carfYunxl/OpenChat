/*****************************************************************//**
 * @file   TcpClient.h
 * @brief  this class is a easy use class to create a tcp cliernt.
 * 
 * @author river
 * @date   January 2023
 *********************************************************************/
#ifndef TCP_CLIENT_H_
#define TCP_CLIENT_H_

#include <winsock2.h>
#include <string>

class TcpClient
{
public:
    TcpClient() :m_port(8888), m_ip("127.0.0.1"),m_socket(INVALID_SOCKET) {};
    TcpClient(size_t port,const std::string& ip);

    void    Connect();
    void    DisConnect();
private:
    SOCKET m_socket;
    size_t m_port;
    std::string m_ip;
};


#endif //TCP_CLIENT_H_
