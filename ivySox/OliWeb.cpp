#include "OliWeb.h"
#include <ctime>
#include <sstream>
#include <stdlib.h>

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
    threadIndex = 0;

    // Open Log File
    string logFileName = "OliWeb.log";
    log.open(logFileName.c_str(), ios::app );
    writeLog("******************");
    writeLog("** OliWeb 0.1.7 **");
    writeLog("******************");
    configXml();
}

OliWeb::~OliWeb()
{
    log.close();
}

int OliWeb::run()
{
    pthread_t thread;
    // Open the designated port
    writeLog("Opening port " + toString(portNumber) );
    ivySox.openPort(portNumber);
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
        handleInboundRequest();
    }
}

int OliWeb::fetchFile(InboundRequest *request)
{
    string requestedFile = request->requestedFile;

    pthread_mutex_lock(&ivySoxMutex);

    writeLog("Checking for " + requestedFile);

    // Check to see if requested file exists
    ifstream reqFileStream(requestedFile.c_str());
    if (reqFileStream.good())
    {
        reqFileStream.close();
    } else {
        // Finally, drop a 404
        requestedFile = rootFileDirectory + "/" + fileNotFoundPage;
    }

    //  Send file if it exists
    int bytesSent = -1;
    if (requestedFile != "")
    {
        writeLog("Sending " + requestedFile);
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
            writeLog(anArg);
        }
    }
}

void OliWeb::invokeCgi(InboundRequest *request)
{
    string outputFilename = "scriptOutput." + toString(request->socketNumber);
    // TODO: parse the arguments by splitting on ?, &
    getScriptFilename(request);
    getArgumentList(request);
    string cmd = scriptDirectory + request->scriptFilename +
                 request->scriptArguments + " >> " + outputFilename;

    writeLog("Invoking: '" + cmd + "'.");
    int returnValue = system(cmd.c_str());
    writeLog("Return value = " + toString(returnValue));

    if (returnValue != 0)
    {
        //ivySox.sendMessage("Error invoking " + request->scriptFilename + ".");
        //ivySox.closeInbound();
        //ivySox.closeSocket(request->socketNumber);
    }
    else
    {
        //requestedFile = outputFilename;
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
        str.find(".cgi") != string::npos ||
        str.find(".py")  != string::npos ||
        str.find(".PY")  != string::npos )
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
    writeLog("Thread launch result = " + toString(result));

    //threadRequestHandler(request);
    //delete request;
    //ivySox.closeInbound();
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
    writeLog("Socket numbr = " + toString(request->socketNumber));
    request->receivedBytes = request->inbound.receive(request->inboundBuffer, INBOUND_BUFFER_SIZE);
    request->requestString = ivySox.messageToString(request->inboundBuffer, request->receivedBytes);
    //request->receivedBytes = ivy->receiveInbound(request->inboundBuffer, INBOUND_BUFFER_SIZE);
    //request->requestString = ivy->messageToString(request->inboundBuffer, request->receivedBytes);
    writeLog(request->requestString, false);
    writeLog("(" + toString(request->receivedBytes) + " bytes)");
    // Parse the request string to get the file or script being requested
    request->requestedFile = parseRequest(request->requestString);

    if (isCgi(request->requestedFile))
    {
        invokeCgi(request);
    } else {
        request->requestedFile = rootFileDirectory + request->requestedFile;
        fetchFile(request);
    }
    //pthread_mutex_unlock(&ivySoxMutex);
}

// ToDo: Move this to request class
string OliWeb::parseRequest(string request)
{
    // Do some string manipulation to identify the GET request
    // Look at the first line
    istringstream requestStream(request);
    char firstLine[1024];
    requestStream.getline(firstLine, 1024);
    string searchString = firstLine;
    istringstream searchStream(searchString);
    string getRequest = "";

    // Pull first two tokens out of the first line
    string sub1, sub2;
    searchStream >> sub1 >> sub2;

    // DEBUG!!
    //writeLog(searchString);
    //writeLog(sub1);
    //writeLog(sub2);

    // If we see a GET and a SLASH then we're in the right place...
    if ( upperCase(sub1).compare("GET") == 0 && sub2.compare("/") != 0 )
    {
        //getRequest = rootFileDirectory + sub2;
        getRequest = sub2;
        return getRequest;
    }

    // Otherwise we'll forward to the default (index.html)
    return ("/"+defaultFileName);
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

void OliWeb::writeLog(string logMessage, bool timestamp)
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

    log << message.str();
    cout << message.str();
    log.flush();
    //pthread_mutex_unlock(&logMutex);
}

int OliWeb::configXml()
{
    string msg = "Opening ";
    msg += OLIWEB_CONFIG;
    writeLog(msg);
    writeLog("Parsing XML (tinyxml2)");
    //config.Parse(OLIWEB_CONFIG);
    //config.Parse("utf8test.xml");
    config.LoadFile(OLIWEB_CONFIG);
    writeLog("Errorcode = " + toString(config.ErrorID()));

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
        writeLog("Reading configuration parameters");
        XMLElement *configElement = parserElement->FirstChildElement();

        while (configElement != NULL)
        {
            string settingName = configElement->Name();
            string value = configElement->Attribute("value");

            writeLog(settingName + " = " + value);

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
            else    writeLog("Setting not recognized!!");

            configElement = configElement->NextSiblingElement();
        }
    }
}
