#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <sstream>
#include <map>

#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <pthread.h>
#include <sys/select.h>

using std::string;
using std::vector;
using std::stringstream;
using std::map;

#pragma once

enum Result {
    Ok = 0,
    Err
};


