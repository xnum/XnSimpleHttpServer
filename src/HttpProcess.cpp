#include "HttpProcess.h"

string Response::toString()
{
    stringstream ss;
    ss << "HTTP/1.1 " << status_code << "OK" << endl;
    for( auto& ele : param )
    {
        ss << ele.first << ": " << ele.second << endl;
    }
    ss << "Content-Length: " << content.size() << endl;
    ss << endl;
    ss << content << endl;

    return ss.str();
}

Response HttpProcess::DealRequest(Request req)
{
   if( req.method == "GET" )
   {
       return GetRequest(req);
   }

   return Response(true);
}

Response HttpProcess::GetRequest(Request req)
{
    string webroot = "/home/num";
    string path = req.path;
    if(path[0] != '/')
    {
        path = path.insert(0,"/");
    }

    path = webroot + path;

    Response res;
    res.content = getFileContent(path);
    res.status_code = 200;
    res.param["Date"] = getDate();
    res.param["Server"] = "XnSHS";
    res.param["Content-Type"] = getMIMEType(getFileExt(path));

    return res;
}

string HttpProcess::getFileContent(string filePath)
{
    FILE* f = fopen(filePath.c_str(), "r");
    if( f == NULL )
    {
        //fprintf(stderr,"fopen error\n");
        //return Response(true);
        return "";
    }

    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);

    char* where = new char[size];

    rewind(f);
    fread(where, sizeof(char), size, f);

    string content(where);

    delete[] where;

    return content;
}

string HttpProcess::getDate()
{
    char buf[1000];
    time_t now = time(0);
    struct tm tm = *gmtime(&now);
    strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S %Z", &tm);
    //printf("Time is: [%s]\n", buf);
    return string(buf);
}

string HttpProcess::getFileExt(string str)
{
    const char *filename = str.c_str();
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}

string HttpProcess::getMIMEType(string fileExt)
{
    if(fileExt == "html" || fileExt == "htm")
        return "text/html";    

    if(fileExt == "txt")
        return "text/plain";    

    if(fileExt == "css")
        return "text/css";    

    if(fileExt == "gif")
        return "image/gif";    

    if(fileExt == "jpg")
        return "image/jpeg";    

    if(fileExt == "png")
        return "image/png";    

    if(fileExt == "bmp")
        return "image/x-ms-bmp";    

    if(fileExt == "doc")
        return "application/msword";    

    if(fileExt == "pdf")
        return "application/pdf";    

    if(fileExt == "mp4")
        return "video/mp4";    

    if(fileExt == "swf")
        return "application/x-shockwave-flash";    

    if(fileExt == "ogg")
        return "audio/ogg";    

    if(fileExt == "bz2")
        return "application/x-bzip2";    

    if(fileExt == "gz")
        return "application/x-gzip";    

    return "";
}























