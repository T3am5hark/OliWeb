
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
    int bytesSent;
    int receivedBytes;
    //IvySox ivySox;
    InboundConnection inbound;

    void *oliWebPtr;

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
    void invokeCgi(InboundRequest *request);
    int fetchFile(InboundRequest *request);
    //int fetchFile(string requestedFile);
    void getScriptFilename(InboundRequest *request);
    void getArgumentList(InboundRequest *request);

    int portNumber;
    string rootFileDirectory;
    string defaultFileName;
    string fileNotFoundPage;
    string scriptDirectory;
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
