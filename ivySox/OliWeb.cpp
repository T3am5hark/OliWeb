/*

[OliWeb and IvySox are provided under the MIT software license]

Copyright (C) 2013 Jeffrey Moore

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


#include "OliWeb.h"
#include <ctime>
#include <sstream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

using namespace std;
using namespace tinyxml2;

InboundRequest::InboundRequest()
{
}

InboundRequest::InboundRequest(int inboundSocketNumber)
{
    socketNumber = inboundSocketNumber;
}

OliWeb::OliWeb()
{
    // Initialize with default settings
    logMutex = PTHREAD_MUTEX_INITIALIZER;
    ivySoxMutex = PTHREAD_MUTEX_INITIALIZER;
    portNumber = DEFAULT_PORT_NUMBER;
    rootFileDirectory = ".";
    defaultFileName = "index.html";
    fileNotFoundPage = "oops404.html";
    scriptDirectory = "cgi-bin";
    logFileName = "OliWeb.log";
    phpEngine = "/usr/bin/php";
    phpFlags = "-f";

    threadIndex = 0;
    configXml();

    openLogFile();
}

OliWeb::~OliWeb()
{
    log.close();
}

void OliWeb::openLogFile()
{
    // Open Log File
    log.open(logFileName.c_str(), ios::app );
    writeLog("******************",false);
    writeLog("** OliWeb 0.9.1 **",false);
    writeLog("******************",false);
    writeLog("Starting OliWeb");
}

bool OliWeb::logIsOpen()
{
    return log.is_open();
}

int OliWeb::run()
{
    pthread_t thread;
    // Open the designated port
    writeLog("Opening port " + toString(portNumber) );
    ivySox.openServerOnPort(portNumber);
    // Listen on port
    writeLog("Listening on port " + toString(portNumber) + "...");
    if (ivySox.listenToPort() < 0)
    {
        perror("Listen: ");
        writeLog("Error opening port!");
        return -1;
    }

    while (1)
    {
        // Listen for requests
        writeLog("Waiting for inbound request.");
        writeLog("",false);
        handleInboundRequest();
    }
}

int OliWeb::fetchFile(InboundRequest *request)
{
    string requestedFile = request->requestedFile;

    pthread_mutex_lock(&ivySoxMutex);

    writeLog("Fetching " + requestedFile);

    // Check to see if requested file exists
    ifstream reqFileStream(requestedFile.c_str());
    if (reqFileStream.good())
    {
        sendStatusOk(request);
        reqFileStream.close();
    } else {
        // Finally, drop a 404
        sendStatusNotFound(request);
        writeLog("File not found, sending 404");
        requestedFile = rootFileDirectory + "/" + fileNotFoundPage;
    }

    //  Send file if it exists
    int bytesSent = -1;
    if (requestedFile != "")
    {
        //writeLog("Sending " + requestedFile, false);
        bytesSent = (request->inbound).sendFile(requestedFile);
        writeLog("Sent " + toString(bytesSent) + " bytes.");
    } else {
        writeLog("Error parsing for file name.");
    }

    pthread_mutex_unlock(&ivySoxMutex);
    return bytesSent;
}

// ToDo: move this method into the InboundRequest class
void OliWeb::getScriptFilename(InboundRequest *request)
{
    request->scriptFilename = request->requestedFile;
    // Split by ?
    size_t q = request->requestedFile.find("?");
    if ( q != string::npos )
    {
        request->scriptFilename = request->requestedFile.substr(0,q);
    } else {

    }
}


// ToDo: move this to the InboundRequest class
void OliWeb::getArgumentList(InboundRequest *request)
{
    request->scriptArguments = "";
    size_t a = request->requestedFile.find("?");
    size_t b = a;
    size_t strLen = request->requestedFile.length();
    string anArg = "";
    if ( a == string::npos) return;
    else
    {
        while (a != string::npos && b != string::npos)
        {
            a = b+1;
            if (a > strLen) break;
            b = request->requestedFile.find("&",a);
            if (b == string::npos)
            {
                anArg = request->requestedFile.substr(a);
            } else {
                anArg = request->requestedFile.substr(a,b-a-1);
            }
            request->scriptArguments += " --" + anArg;
            //writeLog(anArg);
        }
    }
}

void OliWeb::invokeCgi(InboundRequest *request)
{
    getScriptFilename(request);
    //getArgumentList(request);
    //string cmd = scriptDirectory + request->scriptFilename +
    //             request->scriptArguments + " >> " + outputFilename;
    string target = scriptDirectory + request->scriptFilename;
    string cmd = "/bin/sh";
    string flags = "-c";
    invoke(request, cmd, flags, target);
}

void OliWeb::invokePhp(InboundRequest *request)
{
    getScriptFilename(request);
    string target = rootFileDirectory + request->scriptFilename;
    //string flags = "-f";
    //string cmd = "/usr/bin/php";
    string flags = phpFlags;
    string cmd = phpEngine;
    invoke(request, cmd, flags, target);
}

void OliWeb::invoke(InboundRequest *request, string cmd,
                    string flags, string target)
{
    string outputFilename = "scriptOutput." + toString(request->socketNumber);

    writeLog("Invoking: '" + cmd + " " + flags + "'.");
    writeLog("Target:   '" + target + "'");
    //int returnValue = system(cmd.c_str());
    pid_t processId = fork();
    int returnValue = 0;
    if (processId < 0)
    {
        perror("Fork");
        writeLog("Could not fork!!");
        return;
    } else if (processId > 0)
    {
        // Parent process - wait.
        int waitStatus;
        waitpid(processId, &waitStatus, 0);
    } else {
        // Child process - set environment vars and exec command.
        //setenv("QUERY_STRING", request->requestedFile.c_str(), 1);
        setenv("QUERY_STRING", request->queryString.c_str(), 1);
        setenv("REQUEST_METHOD", request->method.c_str(), 1);
        setenv("CGI_BIN", scriptDirectory.c_str(), 1);
        setenv("WEB_ROOT",rootFileDirectory.c_str(), 1);
        setenv("BODY", request->body.c_str(), 1);
        setenv("UTIL", utilDirectory.c_str(), 1);
        // Redirect stdout to go where we want to pick it up.
        mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
        int fileDescriptor = open(outputFilename.c_str(),
                                  O_RDWR | O_CREAT, mode );
        int savedStdOut = dup(STDOUT_FILENO);
        dup2( fileDescriptor, STDOUT_FILENO );
        close(fileDescriptor);
        //returnValue = execl(cmd.c_str(), cmd.c_str(), (char *) 0);
        //returnValue = execl("/bin/sh", "/bin/sh", "-c", cmd.c_str(),
        //                    (char *) 0);
        returnValue = execl(cmd.c_str(), cmd.c_str(), flags.c_str(),
                            target.c_str(),
                            (char *) 0);
        if (returnValue)
        {
            int err = errno;
            writeLog("Errno = " + toString(err) );
        }

        dup2( savedStdOut, STDOUT_FILENO );
        writeLog("CGI return value = " + toString(returnValue),false);
        _exit(0);
    }

    // Parent process picks up here post execution of CGI.
    // writeLog("CGI return value = " + toString(returnValue));

    if (returnValue != 0)
    {
        ivySox.sendMessage("Error invoking " + request->scriptFilename + ".");
        ivySox.closeInbound();
    }
    else
    {
        request->requestedFile=outputFilename;
        fetchFile(request);
        cmd = "rm " + outputFilename;
        returnValue = system(cmd.c_str());
        if (returnValue != 0) writeLog("Error removing temp file " + outputFilename);
    }
}

bool OliWeb::isCgi(string str)
{
    // Look for supported CGI script extensions
    // Consider adding an extensible list in XML...
    if (str.find(".CGI") != string::npos ||
        str.find(".cgi") != string::npos )
        return true;
    return false;

}

bool OliWeb::isPhp(string str)
{
    if (str.find(".PHP") != string::npos ||
        str.find(".php") != string::npos)
        return true;
    return false;
}

bool OliWeb::isHtml(string str)
{
    if (str.find(".html") != string::npos ||
        str.find(".htm") != string::npos ||
        str.find(".HTML") != string::npos ||
        str.find(".HTM") != string::npos ||
        str.find(".css") != string::npos ||
        str.find(".CSS") != string::npos )
        return true;
    return false;
}

void OliWeb::handleInboundRequest()
{
    // Accept inbound & Log request
    InboundRequest *request = new InboundRequest();
    request->oliWebPtr = (void *)this;

    request->socketNumber = ivySox.acceptInbound(&request->inbound);

    //request->socketNumber = ivySox.acceptInbound(&request->ivySox);
    //pthread_mutex_lock(&ivySoxMutex);
    //request->socketNumber = ivySox.acceptInbound(&(request->ivySox));
    //pthread_mutex_unlock(&ivySoxMutex);

    // Create a detached thread to handle inbound request
    pthread_t aThread;
    pthread_attr_t threadAttribute;
    pthread_attr_init(&threadAttribute);
    pthread_attr_setdetachstate(&threadAttribute, PTHREAD_CREATE_DETACHED);
    int result = pthread_create( &aThread, &threadAttribute, threadEntryPoint, (void *) request);

}

void *threadEntryPoint(void *requestVoid)
{
    InboundRequest *request = (InboundRequest *) requestVoid;
    OliWeb *oliWeb = (OliWeb *)request->oliWebPtr;
    oliWeb->threadRequestHandler(request);
    cout << "Deleteding request handler!!" << endl;
    delete request;
    cout << "EXITING THREAD!!" << endl;
    pthread_exit(NULL);
}

//  Except for logging, this can be moved to request class
void OliWeb::threadRequestHandler(InboundRequest *request)
{
    //IvySox *ivy = &ivySox;
    //pthread_mutex_lock(&ivySoxMutex);
    writeLog("Received inbound request from " + request->inbound.getIpAddress() );
    //writeLog("Socket numbr = " + toString(request->socketNumber));
    request->receivedBytes = request->inbound.receive(request->inboundBuffer, INBOUND_BUFFER_SIZE);
    request->requestString = ivySox.messageToString(request->inboundBuffer, request->receivedBytes);
    writeLog(request->requestString, false);
    writeLog("(" + toString(request->receivedBytes) + " bytes)",false);
    // Parse the request string to get the file or script being requested
    // request->requestedFile = parseRequest(request->requestString);
    // request->queryString = extractQueryArgs(request->requestedFile);
    request->parse(defaultFileName);

    writeLog("Requested File = [" + request->requestedFile + "]",false);
    if (isCgi(request->requestedFile))
    {
        //writeLog("Request type = CGI script");
        invokeCgi(request);
    }
    else if (isPhp(request->requestedFile))
    {
        //writeLog("Request type = PHP script");
        invokePhp(request);
    }
    else {
        request->requestedFile = rootFileDirectory + request->requestedFile;
        fetchFile(request);
    }
}

int OliWeb::sendStatusOk(InboundRequest *request)
{
    int bytesSent = -1;
    bytesSent = request->inbound.sendMessage("HTTP/1.1 200 OK\r\n");
    //bytesSent += request->inbound.sendMessage("Content-Type: text/html; charset=UTF-8\r\n");
    bytesSent += request->inbound.sendMessage("Connection: close\r\n");
    bytesSent += request->inbound.sendMessage("\r\n");
    return bytesSent;
}

int OliWeb::sendStatusNotFound(InboundRequest *request)
{
    int bytesSent = -1;
    bytesSent = request->inbound.sendMessage("HTTP/1.1 404 Not Found\r\n");
    bytesSent = request->inbound.sendMessage("Connection: close\r\n");
    bytesSent += request->inbound.sendMessage("\r\n");
    return bytesSent;
}

int OliWeb::sendContentType(InboundRequest *request, string contentType)
{
    int bytesSent = -1;
    string contentTypeString = "Content-Type: ";
    contentTypeString+=contentType + "\n\n";

    pthread_mutex_lock(&ivySoxMutex);
    //bytesSent = request->inbound.sendMessage("HTTP/1.x 200 OK\n");
    bytesSent = request->inbound.sendMessage("HTTP/1.1 200 OK\r\n");
    //bytesSent += request->inbound.sendMessage("Connection: close\r\n");
    //bytesSent += request->inbound.sendMessage("Content-Type: text/html; charset=UTF-8\r\n");
    //bytesSent += request->inbound.sendMessage("Content-Type: text/html; charset=UTF-8\n\n\n\n\n");
    //bytesSent += request->inbound.sendMessage(contentTypeString);
    bytesSent += request->inbound.sendMessage("\r\n");
    pthread_mutex_unlock(&ivySoxMutex);
    return(bytesSent);
}

/*
string OliWeb::extractQueryArgs(string url)
{
    string args = "";
    size_t a = url.find("?");
    if ( a != string::npos && a < url.length() )
    {
        args = url.substr(a+1, string::npos);
    }

    // DEBUG!!
    //std::cout << "a = " << toString(a) << std::endl;
    //std::cout << "url  = '" << url << "'" << std::endl;
    //std::cout << "args = '" << args << "'" << std::endl; 

    return args;
}
*/

// ToDo: Move this to request class
// string OliWeb::parseRequest(string request)
void InboundRequest::parse(string defaultFilename)
{
    // Do some string manipulation to identify the GET request
    // Look at the first line
    istringstream requestStream(requestString);
    char firstLine[1024];
    requestStream.getline(firstLine, 1024);
    string searchString = firstLine;
    istringstream searchStream(searchString);
    //string getRequest = "";

    // Pull first two tokens out of the first line
    string sub1, sub2, sub3;
    searchStream >> sub1 >> sub2 >> sub3;

    // DEBUG!!
    //writeLog(searchString);
    //writeLog(sub1);
    //writeLog(sub2);

    // If we see a GET and a SLASH then we're in the right place...
    if ( upperCase(sub1).compare("GET") == 0 && sub2.compare("/") != 0 )
    {
        //getRequest = rootFileDirectory + sub2;
        //getRequest = sub2;
        //return sub2;
        method = "GET";
        requestedFile = sub2;
        protocol = sub3;
    }

    if ( upperCase(sub1).compare("POST") == 0 && sub2.compare("/") != 0)
    {
        //return sub2;
        method = "POST";
        requestedFile = sub2;
        protocol = sub3;
    }

    if ( upperCase(sub1).compare("PUT") == 0 && sub2.compare("/") != 0)
    {
        method = "PUT";
        requestedFile = sub2;
        protocol = sub3;
    }

    size_t a = requestedFile.find("?");
    if ( a != string::npos && a < requestedFile.length() )
    {
        queryString = requestedFile.substr(a+1, string::npos);
    }

    // Find a blank line...
    size_t emptyLinePos = requestString.find("\r\n\r\n");
    if ( emptyLinePos != string::npos )
    {
        body = requestString.substr(emptyLinePos+4, string::npos);
    }
    // Otherwise we'll forward to the default (index.html)
    if (requestedFile.length() == 0) requestedFile = "/"+defaultFilename;
}

template <class myType> string toString(myType item)
{
    ostringstream oss;
    oss << item;
    return (oss.str());
}

string upperCase(string input)
{
    for (string::iterator it = input.begin(); it != input.end(); ++ it)
      *it = toupper(*it);
    return input;
}

string lowerCase(string input)
{
    for (string::iterator it = input.begin(); it != input.end(); ++ it)
      *it = toupper(*it);
    return input;
}

void OliWeb::writeLog(const string &logMessage, const bool timestamp)
{
    // Thread safety
    //pthread_mutex_lock(&logMutex);
    //  Time stamp, add string, write to log.
    ostringstream message("");

    if (timestamp)
    {
        time_t currentTime = time(0);
        struct tm *timeStruct = localtime(&currentTime);

        message << "[" << (timeStruct->tm_year + 1900) << '-'
                       << (timeStruct->tm_mon + 1) << '-'
                       << (timeStruct->tm_mday) << " "
                       << (timeStruct->tm_hour) << ":";
        if (timeStruct->tm_min < 10) message << "0";
        message        << (timeStruct->tm_min) << ":";
        if (timeStruct->tm_sec < 10) message << "0";
        message        << (timeStruct->tm_sec) << "]  ";
    }
    message        << logMessage << endl;

    if (logIsOpen())
    {
        log << message.str();
        log.flush();
    }
    cout << message.str();
    //pthread_mutex_unlock(&logMutex);
}

int OliWeb::configXml()
{
    string msg = "Opening ";
    msg += OLIWEB_CONFIG;
    writeLog(msg);
    //writeLog("Parsing XML (tinyxml2)");
    //config.Parse(OLIWEB_CONFIG);
    //config.Parse("utf8test.xml");
    config.LoadFile(OLIWEB_CONFIG);
    //writeLog("Errorcode = " + toString(config.ErrorID()));

    //  Pick apart the XML for config options...
    XMLElement *root = config.RootElement();
    if ( root == NULL )
    {
        writeLog("Error parsing file - no root element found.");
        return -1;
    }

    //  Configuration Options
    XMLElement *parserElement = root->FirstChildElement("ConfigurationSettings");
    if (parserElement != NULL)
    {
        writeLog("Reading configuration parameters",false);
        XMLElement *configElement = parserElement->FirstChildElement();

        while (configElement != NULL)
        {
            string settingName = configElement->Name();
            string value = configElement->Attribute("value");

            writeLog(settingName + " = " + value,false);

            if ( settingName == "PortNumber")
                    portNumber = configElement->IntAttribute("value");
            else if ( settingName == "RootFileDirectory")
                    rootFileDirectory = value;
            else if ( settingName == "ScriptDirectory")
                    scriptDirectory = value;
            else if ( settingName == "DefaultFileName")
                    defaultFileName = value;
            else if ( settingName == "FileNotFoundPage")
                    fileNotFoundPage = value;
            else if ( settingName == "LogFile")
                    logFileName = value;
            else if ( settingName == "PhpEngine")
                    phpEngine = value;
            else if ( settingName == "PhpFlags")
                    phpFlags = value;
            else if ( settingName == "UtilDirectory")
                    utilDirectory = value;
            else    writeLog("Setting not recognized!!");

            configElement = configElement->NextSiblingElement();
        }
    }
    return 0; // Success
}
