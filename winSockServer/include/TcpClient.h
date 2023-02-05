/*****************************************************************//**
 * @file   TcpClient.h
 * @brief  this class is a easy use class to create a tcp cliernt.
 * 
 * @author river
 * @date   January 2023
 *********************************************************************/
#ifndef TCP_CLIENT_H_
#define TCP_CLIENT_H_
#include "ClientDlg.h"
#include "common_header.h"

constexpr size_t BUF_SIZE = 256;

class TcpClient
{
public:
    TcpClient() :
        m_port(8888), m_ip("127.0.0.1"),m_socket(INVALID_SOCKET),m_pMainWin(nullptr) {};
    TcpClient(size_t port,const std::string& ip,void* parent);

    bool    Connect();
    void    DisConnect();

    void    SetPort(size_t port);
    void    SetIp(const std::string& ip);
    SOCKET  GetSocket() { return m_socket; }
private:
    void    ClientFunc(void* pMainWin);
    bool    Select(SOCKET socket, long timeOut, MODE mode);
private:
    SOCKET      m_socket;
    size_t      m_port;
    std::string m_ip;
    void*       m_pMainWin;
};


#endif //TCP_CLIENT_H_
