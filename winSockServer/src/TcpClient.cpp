#include "stdafx.h"
#include "TcpClient.h"
#include "ClientDlg.h"

TcpClient::TcpClient(size_t port, const std::string& ip, void* parent)
    : m_port(port)
    , m_ip(ip)
    , m_socket(INVALID_SOCKET)
    , m_pMainWin(parent)
{}

/**
 * @brief   connect to server
 * 
 */
bool TcpClient::Connect()
{
    m_socket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    if (m_socket == INVALID_SOCKET)
    {
        return false;
    }

    sockaddr_in clientAddr;
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_port = htons(m_port);
    clientAddr.sin_addr.S_un.S_addr = inet_addr(m_ip.c_str());

    int ret = connect(m_socket,(sockaddr*)&clientAddr,sizeof(clientAddr));

    if (SOCKET_ERROR == ret)
    {
        return false;
    }

    //success connect to server,then create a thread to receive infomation from server.
    std::thread client_thread(std::bind(&TcpClient::ClientFunc,this, m_pMainWin));

    client_thread.detach();
    return true;
}

/**
 * @brief   disconnect from server
 * 
 */
void TcpClient::DisConnect()
{

}

/**
 * @brief   client thread function
 * 
 */
void TcpClient::ClientFunc(void* pMainWin)
{
    PCClientDlg* pDlg = (PCClientDlg*)pMainWin;
    while (pDlg)
    {
        if (Select(m_socket,100,MODE::READ))
        {
            char msg[BUF_SIZE] = { 0 };
            int read = recv(m_socket, msg, BUF_SIZE, 0);
            if (read > 0)
            {
                sockaddr_in connAddr;
                int len = sizeof(connAddr);
                getpeername(m_socket,(sockaddr*)&connAddr,&len);
                pDlg->AddInfo(std::to_string(ntohs(connAddr.sin_port)) + " Say:\n" + std::string(msg) + "\n");

                send(m_socket, msg, BUF_SIZE,0);
            }
            else
            {
                pDlg->AddInfo("已断线，请重新连接\n");
                pDlg->SetServerState(ServerStatus::OFF);
                shutdown(m_socket, SD_BOTH);
                break;
            }
        }
    }
}

/**
 * @brief   client select socket ; private member function
 * 
 * @param socket
 * @param timeOut
 * @param read
 * @return 
 */
bool TcpClient::Select(SOCKET socket, long timeOut, MODE mode)
{
    timeOut = timeOut > 1000 ? 1000 : timeOut;
    timeval tv{0,timeOut };

    FD_SET fdset;

    FD_ZERO(&fdset);
    FD_SET(socket, &fdset);

    int ret = 0;
    if (MODE::READ == mode)
    {
        //WSAAsyncSelect();
        ret = select(0, &fdset, NULL, NULL, &tv);
    }
    else if(MODE::WRITE == mode)
    {
        ret = select(0, NULL, &fdset, NULL, &tv);
    }

    if (ret <= 0)
    {
        return false;
    }

    if (FD_ISSET(socket, &fdset))
    {
        return true;
    }
    return false;
}

void TcpClient::SetPort(size_t port)
{
    m_port = port;
}

void TcpClient::SetIp(const std::string& ip)
{
    m_ip = ip;
}
