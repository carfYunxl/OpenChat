#include "TcpServer.h"

TcpServer::TcpServer(size_t port) :
    mLisSock(INVALID_SOCKET)
    , mPort(port)
    , mRun(FALSE)
{
}

BOOL TcpServer::Init()
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
        AfxBeginThread(SelectFunc,this);
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
BOOL Select(SOCKET hSocket, DWORD nTimeOut, BOOL bRead)
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
UINT SelectFunc(LPVOID Lparam)
{
    TcpServer* server = (TcpServer*)Lparam;
    while (server && server->IsRun())
    {
        if (Select(server->GetSocket(),100,TRUE))
        {
            sockaddr_in cliAddr;
            int addLen = sizeof(sockaddr_in);

            SOCKET connSock = accept(server->GetSocket(),(sockaddr*)&cliAddr,&addLen);
            if (connSock == INVALID_SOCKET)
            {
                continue;
            }

            server->PushConInfo(cliAddr,connSock);

            //������һ���̣߳���������ǰ�ͻ��˵���Ϣ 
            AfxBeginThread(CliFunc,&server->GetCli(server->CliNum()-1));
        }
    }
    return 0;
}

UINT CliFunc(LPVOID Lparam)
{
    CClientItem* client = (CClientItem*)Lparam;
    while (client)
    {
        if (Select(client->cSocket, 100, TRUE))
        {
            char szRev[MAX_BUFF] = { 0 };
            int iRet = recv(client->cSocket, szRev, sizeof(szRev), 0);
            if (iRet > 0)
            {
                CString str(szRev);
                AfxMessageBox(str);
                //strMsg = A2T(szRev); //���ĳ������룬Ӣ������
                //ClientItem.m_pMainWnd->SetRevBoxText(ClientItem.cAddr + _T(">>") + strMsg);
                //ClientItem.m_pMainWnd->SendClientMsg(strMsg,&ClientItem);
            }
            else {
                //strMsg = ClientItem.cAddr + _T(" ���뿪");
                //ClientItem.m_pMainWnd->RemoveClientFromArray(ClientItem);
                //ClientItem.m_pMainWnd->SetRevBoxText(strMsg);
                break;
            }
        }
    }
    return 0;
}
