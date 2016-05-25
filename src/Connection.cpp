#include "Connection.h"

string Connection::GetRequest(int &rc)
{
    string s;
    rc = m_recv(s);
    fprintf(stderr, "Request str: '%s'\n",s.c_str());
    if( s=="" )
        sleep(3);

    return s;
}

int Connection::m_recv(string &str)
{
    const int len = 1024;
    char buff[len] = {}; 
RETRY:
    int rc = recv(fd, buff, len, 0);
    //fprintf(stderr, "Get %d bytes data\n",rc);
    //if(rc == -1 || rc == 0)printf("err=%s\n",strerror(errno));

    int tmp = errno;
    if( rc == 0 )  // connection shutdown
    {
        return Err;
    }
    if( rc == len ) {
        str = string(buff);
        return Ok;
    }
    else if( rc == -1 && tmp == EAGAIN ) // no data
    {
        str = "";
        return Ok;
    }
    else if( rc == -1 && (tmp == EINTR || tmp == EWOULDBLOCK) )
    {
        sleep(1); 
        goto RETRY;
    }
    else if( rc == -1 && tmp != EINTR && tmp != EWOULDBLOCK && tmp != EAGAIN )
    {
        printf("err=%s\n",strerror(tmp));
        puts("Error on recv");
        return Err;
    }
    else
    {
        //puts("recv return value != len");
        //printf("rc=%d len=%d err=%s\n",rc,len,strerror(tmp));
        str = string(buff);
        return Ok;
    }

    printf("err=%s\n",strerror(tmp));
    return Err;
}


int Listener::Init()
{
    sockfd = socket(AF_INET ,SOCK_STREAM ,0 );
    if( sockfd < 0 )
        return Err;
    return Ok;
}

int Listener::Listen(int port)
{
    if( sockfd == -1 )
    {
        fprintf(stderr,"uninit socket fd \n");
        return Err;
    }

    int useless = 1;
    if ( 0 > setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &useless, sizeof(int)) )
    {
        fprintf(stderr,"setsocketopt REUSEADDR error %s\n",strerror(errno));
        return Err;
    }

    bzero((char*)&sAddr,sizeof(sAddr));

    sAddr.sin_family=AF_INET;
    sAddr.sin_addr.s_addr=INADDR_ANY;
    sAddr.sin_port = htons(port);

    if( 0 > bind(sockfd,(struct sockaddr*)&sAddr,sizeof(sAddr)) )
    {
        fprintf(stderr,"bind error %s\n",strerror(errno));
        return Err;
    }

    if( 0 > listen(sockfd,10) )
    {
        fprintf(stderr,"listen error %s\n",strerror(errno));
        return Err;
    }

    return Ok;
}

int Listener::GetConnection(Connection &conn)
{
    if( sockfd==-1 )
    {
        fprintf(stderr,"uninit socket but wanna accept\n");
        return Err;
    }

    struct sockaddr_in cAddr;
    socklen_t len = sizeof(cAddr);
    bzero((char*)&cAddr,sizeof(cAddr));
    int connfd = accept( sockfd,(struct sockaddr*)&cAddr,&len);

    if( connfd < 0 )
    {
        fprintf(stderr,"accept error %s\n",strerror(errno));
        return Err; 
    }

    conn.fd = connfd;

    return Ok;
}
