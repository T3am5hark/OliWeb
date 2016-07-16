
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

#include "IvySox.h"
#include <sstream>
#include <iostream>
#include <fstream>
//#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <stdexcept>

using namespace std;

/*
   Some stuff to know:
   htons() // host to network short
   htonl() // host to network long
   ntohs() // net to host short
   ntohl() // net to host long
*/

InboundConnection::InboundConnection()
{
    bufferSize = DEFAULT_BUFFER_SIZE;
}

void sigPipeHandler(int signalId)
{
    cout << "Signal handler received " << signalId << "!!!!!" << endl;
}

IvySox::IvySox()
{
    portNumber = 80;
    bufferSize = DEFAULT_BUFFER_SIZE;
    state = socketClosed;
    useIpv6 = false;
    memset(&hints,0,sizeof(hints));
    addressInfoResults = NULL;
    socketNumber = -1;
    inboundConnectionLimit = INBOUND_CONNECTION_LIMIT;
    //socketNumber = socket(PF_INET, SOCK_STREAM, getprotobyname("tcp"));
    //socketAddress = (struct sockaddr *) malloc(sizeof(struct sockaddr));
    //iSocketAddress = (struct sockaddr_in *) malloc(sizeof(struct sockaddr_in));
    signal(SIGPIPE, sigPipeHandler );
}

IvySox::~IvySox()
{
    // Destructor!!!
    if (addressInfoResults != NULL) freeaddrinfo(addressInfoResults);
    closeSocket();
    closeInbound();
    //freeaddrinfo(addressInfoResults);
}

InboundConnection::~InboundConnection()
{
    //closeConnection();
}

IvySoxState IvySox::getState()
{
    return state;
}

void IvySox::closeSocket()
{
    if (socketNumber > 0) close(socketNumber);
}

void InboundConnection::closeConnection()
{
    if (socketNumber > 0) close(socketNumber);
}

void IvySox::closeInbound()
{
    if (inboundSocketNumber > 0) close(inboundSocketNumber);
}

void IvySox::closeSocket(int socketToClose)
{
    if (socketToClose > 0) close(socketToClose);
}

int IvySox::setIpAddress(string ipAddress)
{
    return setIpAddress(ipAddress.c_str());
}

int IvySox::setIpAddress(const char *address)
{
    return inet_pton(AF_INET, address, &(iSocketAddress.sin_addr));
    // inet_pton(AF_INET6, address, &(iSocketAddress6.sin6_addr));
}

int IvySox::getAddressInfo(const char *node,
                           const char *service,
                           const struct addrinfo *hints,
                           struct addrinfo **res)
{
    return ( getaddrinfo( node, service, hints, res) );
}

int charArrayToInt(const char *array)
{
    int result = 0;
    string str = array;
    istringstream stringStream(str);
    stringStream >> result;
    return result;
}

int IvySox::getConnectionInfo( const char *url, const char *service)
{
    int yes = 1;
    if (addressInfoResults != NULL) freeaddrinfo(addressInfoResults);
    int ret = getaddrinfo( url, service, &hints,
                           &addressInfoResults);
    std::cout << "ai_protocol:  " << addressInfoResults->ai_protocol << endl;
    std::cout << "ai_family:    " << addressInfoResults->ai_family << endl;
    std::cout << "ai_socktype:  " << addressInfoResults->ai_socktype << endl;
    if (ret == 0) socketNumber = socket( addressInfoResults->ai_family,
                                         addressInfoResults->ai_socktype,
                                         addressInfoResults->ai_protocol );
    else perror("socket issue...");

    if (socketNumber >= 0)
    {
        if (setsockopt(socketNumber, SOL_SOCKET, SO_REUSEADDR,
                       &yes, sizeof(int)) < 0)
        {
            perror("setsockopt issue...");
        }
    }
    portNumber = charArrayToInt(service);
    return ret;
}

int IvySox::getConnectionInfo( string url, string service)
{
    return getConnectionInfo( url.c_str(), service.c_str());

}

int IvySox::connectTo( const char *url, const char *service)
{
    getConnectionInfo(url, service);
    int ret = connect(socketNumber, addressInfoResults->ai_addr, 
                      addressInfoResults->ai_addrlen);
    return ret;
}

int IvySox::connectTo(string url, string service)
{
    return ( connectTo(url.c_str(), service.c_str()) );
}

struct in_addr *IvySox::getAddressStruct()
{
    struct sockaddr_in *sockAddress = NULL;
    struct in_addr *addr = NULL;
    if (addressInfoResults != NULL)
    {
        sockAddress = (sockaddr_in *)(addressInfoResults->ai_addr);
        if (sockAddress != NULL)
        {
            addr = &(sockAddress->sin_addr);
        }
    }
    return addr;
}

string IvySox::getIpAddress()
{
    char ip4[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, getAddressStruct(), ip4, INET_ADDRSTRLEN);
    string addr = ip4;
    return addr;
}

string InboundConnection::getIpAddress()
{
    string addr;
    if ( connection.ss_family == AF_INET)
    {
        sockaddr_in *inbound = (sockaddr_in *)&connection;
        char ip4[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(inbound->sin_addr), ip4, INET_ADDRSTRLEN);
        addr = ip4;
    } else if ( connection.ss_family == AF_INET6) {
        sockaddr_in6 *inbound = (sockaddr_in6 *)&connection;
        char ip6[INET6_ADDRSTRLEN];
        inet_ntop(AF_INET6, &(inbound->sin6_addr), ip6, INET6_ADDRSTRLEN);
        addr = ip6;
    }
    return addr;
}

string IvySox::getInboundIpAddress()
{
    string addr;
    if ( inboundConnection.ss_family == AF_INET)
    {
        sockaddr_in *inbound = (sockaddr_in *)&inboundConnection;
        char ip4[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(inbound->sin_addr), ip4, INET_ADDRSTRLEN);
        addr = ip4;
    } else if ( inboundConnection.ss_family == AF_INET6) {
        sockaddr_in6 *inbound = (sockaddr_in6 *)&inboundConnection;
        char ip6[INET6_ADDRSTRLEN];
        inet_ntop(AF_INET6, &(inbound->sin6_addr), ip6, INET6_ADDRSTRLEN);
        addr = ip6;
    }
    return addr;
}

unsigned short int InboundConnection::getPortNumber()
{
    unsigned short int portNumber = -1;
    if ( connection.ss_family == AF_INET)
    {
        sockaddr_in *inbound = (sockaddr_in *)&connection;
        portNumber = inbound->sin_port;
    } else if (connection.ss_family == AF_INET6) {
        sockaddr_in6 *inbound = (sockaddr_in6 *)&connection;
        portNumber = inbound->sin6_port;
    }
    return portNumber;
}

unsigned short int IvySox::getInboundPortNumber()
{
    unsigned short int portNumber = -1;
    if ( inboundConnection.ss_family == AF_INET)
    {
        sockaddr_in *inbound = (sockaddr_in *)&inboundConnection;
        portNumber = inbound->sin_port;
    } else if (inboundConnection.ss_family == AF_INET6) {
        sockaddr_in6 *inbound = (sockaddr_in6 *)&inboundConnection;
        portNumber = inbound->sin6_port;
    }
    return portNumber;
}


int IvySox::bindSocket()
{
    // Scope operator "::" is to prevent confusion in some
    // systems with std::bind(...)
    std::cout << "ai_addr:    " << addressInfoResults->ai_addr << endl;
    std::cout << "ai_addrlen: " << addressInfoResults->ai_addrlen << endl;
    std::cout << "socketNum:  " << socketNumber << endl;

    int rval = ::bind(socketNumber, addressInfoResults->ai_addr,
                      addressInfoResults->ai_addrlen );
    if (rval < 0) perror("bindSocket...");
    return rval;
}

int IvySox::listenToPort()
{
    return listen(socketNumber, inboundConnectionLimit);
}

int IvySox::acceptInbound()
{
    //int addrlen = sizeof(struct sockaddr_storage);
    inboundConnectionStructSize = sizeof inboundConnection;
    inboundSocketNumber = accept( socketNumber,
                                  (struct sockaddr *)&inboundConnection,
                                  &inboundConnectionStructSize);
    return inboundSocketNumber;
}

int IvySox::acceptInbound(InboundConnection *inbound)
{
    inbound->connectionStructSize = sizeof inbound->connection;
    inbound->socketNumber = accept( socketNumber,
                                  ( struct sockaddr *)&(inbound->connection),
                                    &(inbound->connectionStructSize) );
    return inbound->socketNumber;
}

//  ToDo: remove this in favor of connection-based
int IvySox::receiveInbound(void *message, ssize_t maxLength)
{
    return( recv( inboundSocketNumber, message, maxLength, 0) );
}

int InboundConnection::receive(void *message, ssize_t maxLength)
{
    return( recv( socketNumber, message, maxLength, 0) );
}

//  ToDo: This should really be static
string IvySox::messageToString(char *message, int length)
{
    string str="";
    ostringstream stringStream(str);

    for (int i = 0; i < length; i++)
    {
        // solve null termination problem...
        if (message[i] == 0) continue;
        stringStream << message[i];
    }
    //stringStream << (char)0;
    str = stringStream.str();
    return str;
}


int InboundConnection::sendMessage(void *message, ssize_t length)
{
    ssize_t totalBytes = 0;
    while (totalBytes < length)
    {
        ssize_t txBytes = sendPartial( (void *)((ssize_t)message + totalBytes), length-totalBytes );
        if (txBytes < 0) 
        {
            cout << "??TXN??" << endl;
            perror("sendMessage");
            //break;
            return (-1);
        }
        totalBytes+=txBytes;
        //cout << txBytes << " ";
    }
    return( totalBytes );
}

int IvySox::sendMessage(void *message, ssize_t length)
{
    ssize_t totalBytes = 0;
    while (totalBytes < length)
    {
        ssize_t txBytes = sendPartial( (void *)((ssize_t)message + totalBytes), length-totalBytes );
        if (txBytes < 0) 
        {
            perror("sendMessage");
            break;
        }
        totalBytes+=txBytes;
    }
    return( totalBytes );
}

int InboundConnection::sendPartial(void *message, ssize_t length)
{
    return ( send(socketNumber, message, length, 0) );
}

int IvySox::sendPartial(void *message, ssize_t length)
{
    return ( ::send(inboundSocketNumber, message, length, 0) );
}

int InboundConnection::sendMessage(string message)
{
    ssize_t length = strlen( message.c_str() );
    return (sendMessage( (void *)message.c_str(), length));
}

int IvySox::sendMessage(string message)
{
    ssize_t length = strlen( message.c_str() );
    return (sendMessage( (void *)message.c_str(), length));
}

int InboundConnection::sendFile(string filename)
{
    char *dataBuffer = (char *)malloc( bufferSize );

    streamsize blockSize = 0;
    size_t totalBytes = 0;

    ifstream file;

    file.open( filename.c_str(), ios::binary | ios::in );

    ifstream::pos_type fileSize = file.tellg();

    if (file.is_open())
    {
        while ( file.good() )
        {
            file.read(dataBuffer, bufferSize);
            blockSize = file.gcount();
            totalBytes += (size_t)blockSize;
            int txBytes = sendMessage(dataBuffer, blockSize);
            if ( txBytes < 0) 
            {
                cout << "Error while sending!!" << endl;
                break;
            }
        }
    } else { totalBytes = -1; }

    file.close();
    free(dataBuffer);
    closeConnection();
    return totalBytes;
}

int IvySox::sendFile(string filename)
{
    char *dataBuffer = (char *)malloc( bufferSize );

    streamsize blockSize = 0;
    size_t totalBytes = 0;

    ifstream file;

    file.open( filename.c_str(), ios::binary | ios::in );

    ifstream::pos_type fileSize = file.tellg();

    if (file.is_open())
    {
        while ( file.good() )
        {
            file.read(dataBuffer, bufferSize);
            blockSize = file.gcount();
            totalBytes += (size_t)blockSize;
            sendMessage(dataBuffer, blockSize);
        }
    } else { totalBytes = -1; }

    file.close();
    free(dataBuffer);
    closeInbound();
    return(totalBytes);
}

int IvySox::openServerOnPort(int portNumber)
{
    stringstream ostr;
    ostr << portNumber;
    struct sockaddr_in sockAddr;

    const char *portNumStr = ostr.str().c_str();
    const char *service = NULL;
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(portNumber);
    sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);


    socketNumber = socket( AF_INET,
                           SOCK_STREAM,
                           0 );

    // This should allow the socket to be immediately reused...
    int option = 1;
#ifdef SO_REUSEPORT
    setsockopt(socketNumber, SOL_SOCKET, (SO_REUSEPORT | SO_REUSEADDR),
               (char*)&option,sizeof(option));
#else
    setsockopt(socketNumber, SOL_SOCKET, SO_REUSEADDR, 
               (char*)&option,sizeof(option));
#endif

    int rval = ::bind(socketNumber,
                      (struct sockaddr *) &sockAddr,
                      sizeof(sockAddr) );
    if (rval < 0) perror("bindSocket...");

    return rval;

}

int IvySox::openPort(int portNumber)
{
    stringstream ostr;
    ostr << portNumber;
    const char *portNumStr = ostr.str().c_str();
    const char *service = NULL;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    //hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    int ret1 = IvySox::connectTo(service, portNumStr );
    if (ret1 > 0)
    {
         return ret1;
    }

    int ret2 = IvySox::bindSocket();
    if (ret2 < 0) perror("bind error");
    return ret2;
}

string IvySox::getErrorMessage(int errorCode)
{
    string msg = gai_strerror(errorCode);
    return msg;
}

int IvySox::getSocketNumber()
{
    return socketNumber;
}
