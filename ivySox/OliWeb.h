
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

#ifndef OLIWEB_INCLUDE
#define OLIWEB_INCLUDE

#include <string>
#include <iostream>
#include <fstream>
#include "tinyxml2.h"
#include "IvySox.h"
#include <pthread.h>

#define OLIWEB_CONFIG "OliWebConfig.xml"
#define DEFAULT_PORT_NUMBER 8077
#define INBOUND_BUFFER_SIZE 10000
#define OUTBOUND_BUFFER_SIZE 10000


using namespace std;
using namespace tinyxml2;

class InboundRequest
{
    public:

    InboundRequest(int socketNumber);
    InboundRequest();

    int socketNumber;
    string requestString;
    char inboundBuffer[INBOUND_BUFFER_SIZE];
    string requestedFile;
    string scriptFilename;
    string scriptArguments;

    void *oliWebPtr;
    int bytesSent;
    int receivedBytes;
    InboundConnection inbound;

    private:


};

class OliWeb
{
    public:

    OliWeb();
    ~OliWeb();
    int run();
    void threadRequestHandler(InboundRequest *request);

    private:

    int configXml();
    void writeLog(string logMessage, bool timestamp = true);
    void handleInboundRequest();
    string parseRequest(string request);
    bool isCgi(string str);
    bool isHtml(string str);
    void invokeCgi(InboundRequest *request);
    int fetchFile(InboundRequest *request);
    int sendContentType(InboundRequest *request, string contentType);
    int sendStatusOk(InboundRequest *request);
    int sendStatusNotFound(InboundRequest *request);
    void getScriptFilename(InboundRequest *request);
    void getArgumentList(InboundRequest *request);
    void openLogFile();
    bool logIsOpen();

    int portNumber;
    string rootFileDirectory;
    string defaultFileName;
    string fileNotFoundPage;
    string scriptDirectory;
    string logFileName;

    IvySox ivySox;
    XMLDocument config;
    ofstream log;

    pthread_mutex_t ivySoxMutex;
    pthread_mutex_t logMutex;
    pthread_t threadPool[INBOUND_CONNECTION_LIMIT];
    unsigned int threadIndex;
    //char inboundBuffer[INBOUND_BUFFER_SIZE];
    //int inboundSocketNumber;
    //string scriptArguments;
    //string request;
    //string requestedFile;
    //string scriptFilename;

};


string upperCase(string input);
string lowerCase(string input);

void *threadEntryPoint(void *requestVoid);

template <class myType> string toString(myType item);




#endif
