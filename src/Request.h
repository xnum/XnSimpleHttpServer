#include "Common.h"
#pragma once

class Request {
    public:
    string method;
    string path;
    string version;
    map<string,string> parameter;
};
