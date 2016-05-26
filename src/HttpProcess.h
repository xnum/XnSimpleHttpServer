#include "Common.h"
#include "Request.h"

#pragma once

class Response
{
    public:
        bool error;
        int status_code;
        unique_ptr<char> content;
        int size;
        map<string,string> param;

        Response() { error = false; status_code = 0; size = 0; }
        Response(bool e) { error = e; size = 0; } // fast consturct error response

        string toString();
        string getStatusCodeStr();
};

class HttpProcess
{
    public:
        Response DealRequest(Request);

    private:
        Response GetRequest(Request);

        int getFileContent(Response &res, string filePath);
        string getDate();
        string getFileExt(string str);
        string getMIMEType(string fileExt);
        int isExists(string path);
        int isAccessible(string path);
        int isDir(string path);
};
