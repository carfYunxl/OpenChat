#include "TcpServer.h"
#include "ServerDlg.h"

TcpServer::TcpServer(size_t port, void* parent) :
    mLisSock(INVALID_SOCKET)
    , mPort(port)
    , mRun(FALSE)
    , mpMainWind(parent)
{
}

/**
 * @brief   server init,private member function.
 * 
 * @return fail or success.
 */
bool TcpServer::Init()
{
    //create a listen socket
    mLisSock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    if (mLisSock == INVALID_SOCKET)
    {
        return FALSE;
    }

    //bind
    sockaddr_in serAddr;
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(mPort);
    serAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

    int ret = bind(mLisSock,(sockaddr*)&serAddr,sizeof(sockaddr_in));

    if (ret == -1)
    {
        return FALSE;
    }

    /**
    * @brif 
    * @param    SOCKET      listen socket
    * @param    backlog     the length of the queue of connection;
    *                       wifi        SOMAXCONN
    *                       bluetooth   2-4
    *                       few client  2-4
    *           to save source;
    * @return   status      -1 error
    */
    ret = listen(mLisSock,SOMAXCONN);

    if (ret == -1)
    {
        return FALSE;
    }
    mRun = TRUE;
    return TRUE;
}

/**
 * @brief   uninit the server.private member function.
 * 
 */
void TcpServer::UnInit()
{
    for (size_t i = 0;i < mClientVec.size();++i)
    {
        closesocket(mClientVec.at(i).cSocket);
    }

    closesocket(mLisSock);

    mLisSock = INVALID_SOCKET;
    mPort = 0;
    mRun = FALSE;
    mClientVec.clear();
}

/**
 * @brief   start the server.after create a Tcp server,call this function,then the server will start.
 * 
 */
void TcpServer::Start()
{
    if (Init())
    {
        std::thread serThread(std::bind(&TcpServer::SelectFunc,this));
        serThread.detach();
    }
}

/**
 * @brief   stop the server.if you when to stop the server,call this function directly.
 * 
 */
void TcpServer::Stop()
{
    UnInit();
}

/**
 * @brief the typical socket which hSocket means
 * 
 * @param socket    is the handle of socket;
 * @param nTimeOut  is the time value of timeout;
 * @param bRead     indicates the flag of reading socket or writting socket;
 * @return fail or success.false fail.
 */
bool TcpServer::Select(SOCKET socket, long nTimeOut, MODE mode)
{
    nTimeOut = nTimeOut > 1000 ? 1000 : nTimeOut;
    timeval time{ 0,nTimeOut };

    FD_SET fdset;
    FD_ZERO(&fdset);
    FD_SET(socket, &fdset);

    int ret = 0;
    if (MODE::READ == mode)
    {
        ret = select(0, &fdset, NULL, NULL, &time);
    }
    else if(MODE::WRITE == mode)
    {
        ret = select(0, NULL, &fdset, NULL, &time);
    }

    if (ret <= 0)
    {
        return false;
    }

    if (!FD_ISSET(socket, &fdset))
    {
        return false;
    }
    return true;
}

/**
 * @brief server thread function for selectting the typical socket to connect to the client.
 */
void TcpServer::SelectFunc()
{
    while (IsRun())
    {
        if (Select(GetSocket(),100,MODE::READ))
        {
            sockaddr_in cliAddr;
            int addLen = sizeof(sockaddr_in);

            SOCKET connSock = accept(GetSocket(),(sockaddr*)&cliAddr,&addLen);
            if (connSock == INVALID_SOCKET)
            {
                continue;
            }

            AddClient(cliAddr,connSock);

            std::thread client_thread
            (
                std::bind
                (
                    &TcpServer::ClientFunc,
                    this,
                    GetClient(ClientNum()-1),
                    mpMainWind
                )
            );

            client_thread.detach();
        }
    }
}

/**
 * @brief client thread function for getting message from typical client socket.
 * 
 * @param client    is the client who connect to the server
 * @param pMainWin  is the WinUI pointer to show the status.
 */
void TcpServer::ClientFunc(const CClientItem& client, void* pMainWin)
{
    PCServerDlg* pMainDlg = (PCServerDlg*)pMainWin;
    while (pMainDlg)
    {
        if (Select(client.cSocket, 100, MODE::READ))
        {
            char szRev[MAX_BUFF] = { 0 };
            int iRet = recv(client.cSocket, szRev, sizeof(szRev), 0);
            if (iRet > 0)
            {
                pMainDlg->SetRevBoxText(client.cAddr + ">>" + std::string(szRev) + "\r\n");
                pMainDlg->SendClientMsg(std::string(szRev) + "\r\n",&client);
            }
            else {
                DeleteClient(client.cPort);
                pMainDlg->SetRevBoxText(client.cAddr + " ÒÑÀë¿ª\r\n");
                break;
            }
        }
    }
}

void TcpServer::DeleteClient(size_t port)
{
    for (auto it = mClientVec.begin();it != mClientVec.end();++it)
    {
        if ((*it).cPort == port)
        {
            mClientVec.erase(it);
            break;
        }
    }
}
