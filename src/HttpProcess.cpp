#include "HttpProcess.h"

string Response::toString()
{
    stringstream ss;
    ss << "HTTP/1.1 " << getStatusCodeStr() << endl;
    for( auto& ele : param )
    {
        std::cout << ele.first << endl;
        std::cout << ele.second << endl;
        ss << ele.first << ": " << ele.second << endl;
    }
    ss << "Content-Length: " << size << endl;
    ss << endl;

    return ss.str();
}

string Response::getStatusCodeStr()
{
    if( status_code == 200 )
        return "200 OK";

    if( status_code == 403 )
        return "403 Forbidden";

    if( status_code == 404 )
        return "404 Not Found";

    if( status_code == 301 )
        return "301 Moved Permanently";

    if( status_code == 302 )
        return "302 Found";

    return "500 Internal Server Error";
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
    res.param["Date"] = getDate();
    res.param["Server"] = "XnSHS";
    res.status_code = 500;

    if( Ok != isExists(path) )
    {
        res.param["Content-Type"] = "text/html";
        res.status_code = 403;
        res.content = unique_ptr<char>(new char[30]);
        sprintf(res.content.get(), "403 Forbidden (Missing file)\n%n",&res.size);
        return res;
    }

    if( Ok != isAccessible(path) )
    {
        res.param["Content-Type"] = "text/html";
        res.status_code = 404;
        res.content = unique_ptr<char>(new char[50]);
        sprintf(res.content.get(), "404 Not Found (Permission denied)\n%n",&res.size);
        return res;
    }

    if( *path.rbegin() != '/' )
    {
        if( Ok != isDir(path) )
        {
            getFileContent(res, path);
            res.status_code = 200;
            res.param["Content-Type"] = getMIMEType(getFileExt(path));
            return res;
        }
        else // 301
        {
            res.param["Content-Type"] = "text/html";
            res.status_code = 301;
            res.content = unique_ptr<char>(new char[30]);
            sprintf(res.content.get(), "301 (Moved)\n%n",&res.size);
            res.param["Location"] = "http://" +req.parameter["Host:"] + req.path + '/';
            return res;
        }
    }
    else // list file
    {
        if( Ok == isAccessible(path + "index.html") )
        {
            path = path + "index.html";

            getFileContent(res, path);
            res.status_code = 200;
            res.param["Content-Type"] = getMIMEType(getFileExt(path));
            return res;
        }
        else if( Ok == isAccessible(path + "index.htm") )
        {
            path = path + "index.htm";

            getFileContent(res, path);
            res.status_code = 200;
            res.param["Content-Type"] = getMIMEType(getFileExt(path));
            return res;
        }
        else
        {
            getDirContent(res, path);
            res.status_code = 200;
            res.param["Content-Type"] = "text/html";
            return res;
        }

        return Response(true);
    }

    return res;
}

int HttpProcess::getFileContent(Response &res, string filePath)
{
    FILE* f = fopen(filePath.c_str(), "r");
    if( f == NULL )
    {
        //fprintf(stderr,"fopen error\n");
        //return Response(true);
        return Err;
    }

    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);

    res.content = unique_ptr<char>(new char[size]);
    res.size = size;

    rewind(f);
    fread(res.content.get(), sizeof(char), size, f);

    return Ok;
}

int HttpProcess::getDirContent(Response &res, string filePath)
{
    struct dirent *readDir;
    DIR *dir;
    dir = opendir(filePath.c_str());

    stringstream ss;

    if(dir == NULL) {
        ss << "Not Found" << endl;
    } else {
        ss << "Dir: " << filePath << endl;
        ss << "-------------------" << endl;

        while((readDir = readdir(dir)) != NULL) {
            if(strcmp(readDir->d_name, "..") && strcmp(readDir->d_name, ".")) {
                ss << readDir->d_name << endl;
            }
        }
        closedir(dir);
        ss << "-------------------" << endl;
    }

    string s = ss.str();
    res.content = unique_ptr<char>(new char[s.size()+5]);
    res.size = s.size();
    strcpy(res.content.get(), s.c_str());

    return Ok;
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


int HttpProcess::isExists(string path)
{
    if( 0 == access(path.c_str(), F_OK) )
        return Ok;
    return Err;
}

int HttpProcess::isAccessible(string path)
{
    if( 0 == access(path.c_str(), R_OK) )
        return Ok;
    return Err;
}

int HttpProcess::isDir(string path)
{
    struct stat sb;

    if (stat(path.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))
    {
        return Ok;
    }
    else
    {
        return Err;
    }
}



















