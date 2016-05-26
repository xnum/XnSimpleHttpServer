#include "ConnManager.h"

int ConnManager::AddConn(Connection conn)
{
    conns[top++] = conn;
    return top-1;
}

int ConnManager::Serve(int index)
{
    int rc = pthread_create(&conns[index].thread, NULL, serve, 
            (void *) &conns[index] );
}

void* serve(void* connPtr)
{
    Connection &conn = *(Connection *)connPtr;
    HttpProcess httpProc;

    while(1)
    {
        int rc = Ok;
        Request req = conn.GetRequest(rc);
        printf("Method: %s\n",req.method.c_str());
        printf("Path: %s\n",req.path.c_str());
        printf("Version: %s\n",req.version.c_str());
        if( rc == Err )
        {
            puts("Connection Closed");
            break;
        }
        Response res = httpProc.DealRequest(req);

        rc = conn.SendResponse(res.toString());
    }

    pthread_exit(NULL);
}
