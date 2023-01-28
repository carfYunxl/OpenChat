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
    std::thread client_thread(std::bind(&TcpClient::ClientFunc,this, m_pMainWin));

    client_thread.detach();
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
    while (1)
    {
        if (Select(m_socket,100,MODE::READ))
        {
            char msg[BUF_SIZE] = { 0 };
            size_t read = recv(m_socket, msg, BUF_SIZE, 0);
            if (read > 0)
            {
                pDlg->SetRevBoxText(m_ip + ">>" + std::string(msg));
            }
            else
            {
                pDlg->SetRevBoxText("�Ѷ��ߣ�����������\r\n");
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

    int iRet = 0;
    if (MODE::READ == mode)
    {
        //WSAAsyncSelect();
        iRet = select(0, &fdset, NULL, NULL, &tv);
    }
    else if(MODE::WRITE == mode)
    {
        iRet = select(0, NULL, &fdset, NULL, &tv);
    }

    if (iRet <= 0)
    {
        return false;
    }

    if (!FD_ISSET(socket, &fdset))
    {
        return false;
    }
    return true;
}

void TcpClient::SetPort(size_t port)
{
    m_port = port;
}

void TcpClient::SetIp(const std::string& ip)
{
    m_ip = ip;
}
