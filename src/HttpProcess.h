#include "Common.h"
#include "Request.h"

#pragma once

class Response
{
    public:
        bool error;
        int status_code;
        string content;
        map<string,string> param;

        Response() { error = false; status_code = 0; }
        Response(bool e) { error = e; } // fast consturct error response

        string toString();
};

class HttpProcess
{
    public:
        Response DealRequest(Request);

    private:
        Response GetRequest(Request);

        string getFileContent(string filePath);
        string getDate();
        string getFileExt(string str);
        string getMIMEType(string fileExt);
};
