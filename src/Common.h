#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <pthread.h>
#include <sys/select.h>

using std::string;
using std::vector;

#pragma once

enum Result {
    Ok = 0,
    Err
};


