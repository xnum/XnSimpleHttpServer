#include "ConnManager.h"

string webroot = "/home/num";

int main(int argc, char **argv)
{
    int port = 8787;

    for( int i = 1; i < argc ; ++i )
    {
        int res = strtol(argv[i],NULL,10);
        if( res != 0 )
        {
            port = res;
            continue;
        }

        struct stat sb;

        if (stat(argv[i], &sb) == 0 && S_ISDIR(sb.st_mode))
        {
            webroot = string(argv[i]);
        }
    }

    Listener serv;
    ConnManager connManager;

    if( Ok != serv.Init() ) 
    {
        return 1;
    }
    puts("Init Ok");

    if( Ok != serv.Listen(port) )
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
