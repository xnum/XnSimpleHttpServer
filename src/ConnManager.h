
#include "Connection.h"
#include "HttpProcess.h"


#pragma once


class ConnManager
{
public:
    ConnManager() { top = 0; }
    int AddConn(Connection);
    int Serve(int index);

private:
    Connection conns[1024];
    int top;
};


void* serve(void* connPtr);
