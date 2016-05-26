#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <memory>

#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <pthread.h>
#include <sys/select.h>
#include <sys/stat.h>

using std::string;
using std::vector;
using std::stringstream;
using std::map;
using std::endl;
using std::unique_ptr;

#pragma once

enum Result {
    Ok = 0,
    Err
};


