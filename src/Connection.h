#include "Common.h"
#include "Request.h"
#include "HttpProcess.h"
#include <netinet/in.h>

#pragma once

class Connection
{
public:
    int fd;
    pthread_t thread;

    Request GetRequest(int &rc);
    int SendResponse(Response &res);
    void Close();
private:
    int m_recv(string &str);
};

class Listener
{
public:
    Listener() { sockfd=-1; }
    int Init();
    int Listen(int port = 8000);
    int GetConnection(Connection&);
private:
    int sockfd;
    struct sockaddr_in sAddr;
};
