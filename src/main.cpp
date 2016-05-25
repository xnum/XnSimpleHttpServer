#include "ConnManager.h"

int main()
{
    Listener serv;
    ConnManager connManager;

    if( Ok != serv.Init() ) 
    {
        return 1;
    }
    puts("Init Ok");

    if( Ok != serv.Listen(8787) )
    {
        return 2;
    }
    puts("Listen Ok");

    while(1)
    {
        Connection conn;
        if( Ok != serv.GetConnection(conn) )
            break;
        int index = connManager.AddConn(conn);
        connManager.Serve(index);
    }

    return 0;
}
