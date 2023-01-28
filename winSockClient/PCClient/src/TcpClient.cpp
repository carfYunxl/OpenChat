#include "stdafx.h"
#include "TcpClient.h"

TcpClient::TcpClient(size_t port, const std::string& ip)
    : m_port(port)
    , m_ip(ip)
    , m_socket(INVALID_SOCKET)
{}

/**
 * @brief   connect to server
 * 
 */
void TcpClient::Connect()
{
    m_socket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    if (m_socket == INVALID_SOCKET)
    {
        return;
    }

    sockaddr_in clientAddr;
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_port = htons(m_port);
    clientAddr.sin_addr.S_un.S_addr = inet_addr(m_ip.c_str());

    int ret = connect(m_socket,(sockaddr*)&clientAddr,sizeof(clientAddr));

    if (SOCKET_ERROR == ret)
    {
        return;
    }

    //success connect to server,then create a thread to receive infomation from server.
    std::thread client_thread();
}

/**
 * @brief   disconnect from server
 * 
 */
void TcpClient::DisConnect()
{

}
