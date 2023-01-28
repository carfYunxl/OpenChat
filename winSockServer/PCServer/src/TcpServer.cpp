#include "TcpServer.h"
#include "ServerDlg.h"
#include <thread>
#include <functional>

TcpServer::TcpServer(size_t port, void* parent) :
    mLisSock(INVALID_SOCKET)
    , mPort(port)
    , mRun(FALSE)
    , mpMainWind(parent)
{
}

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

void TcpServer::UnInit()
{
    mLisSock = INVALID_SOCKET;
    mPort = 0;
    mRun = FALSE;
    mConVec.clear();

    for (size_t i = 0;i < mConVec.size();++i)
    {
        closesocket(mConVec.at(i).cSocket);
    }

    closesocket(mLisSock);
}

void TcpServer::Start()
{
    if (Init())
    {
        std::thread serThread(std::bind(&TcpServer::SelectFunc,this));
        serThread.detach();
    }
}


void TcpServer::Stop()
{
    UnInit();
}

/**
 * select the typical socket which hSocket means
 * 
 * \param hSocket   socket
 * \param nTimeOut  the time value of timeout
 * \param bRead     indicate read or write
 * \return 
 */
bool Select(SOCKET hSocket, DWORD nTimeOut, BOOL bRead)
{
    nTimeOut = nTimeOut > 1000 ? 1000 : nTimeOut;
    timeval time{ 0,nTimeOut };

    FD_SET fdset;
    FD_ZERO(&fdset);
    FD_SET(hSocket, &fdset);

    int ret = 0;
    if (bRead)
    {
        ret = select(0, &fdset, NULL, NULL, &time);
    }
    else
    {
        ret = select(0, NULL, &fdset, NULL, &time);
    }

    if (ret <= 0)
    {
        return FALSE;
    }

    if (!FD_ISSET(hSocket, &fdset))
    {
        return FALSE;
    }
    return TRUE;
}

/**
 * callback function to select the typical socket
 * 
 * \param Lparam
 * \return 
 */
void TcpServer::SelectFunc()
{
    while (IsRun())
    {
        if (Select(GetSocket(),100,TRUE))
        {
            sockaddr_in cliAddr;
            int addLen = sizeof(sockaddr_in);

            SOCKET connSock = accept(GetSocket(),(sockaddr*)&cliAddr,&addLen);
            if (connSock == INVALID_SOCKET)
            {
                continue;
            }

            PushConInfo(cliAddr,connSock);

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

void TcpServer::ClientFunc(const CClientItem& client, void* pMainWin)
{
    PCServerDlg* pMainDlg = (PCServerDlg*)pMainWin;
    while (pMainDlg)
    {
        if (Select(client.cSocket, 100, TRUE))
        {
            char szRev[MAX_BUFF] = { 0 };
            int iRet = recv(client.cSocket, szRev, sizeof(szRev), 0);
            CString strMsg;
            if (iRet > 0)
            {
                USES_CONVERSION;
                strMsg = A2T(szRev); //中文出现乱码，英文正常
                pMainDlg->SetRevBoxText(CString(client.cAddr.c_str()) + _T(">>") + strMsg);
                pMainDlg->SendClientMsg(strMsg,&client);
            }
            else {
                strMsg = CString(client.cAddr.c_str()) + _T(" 已离开");
                pMainDlg->RemoveClientFromArray(client);
                pMainDlg->SetRevBoxText(strMsg);
                break;
            }
        }
    }
}
