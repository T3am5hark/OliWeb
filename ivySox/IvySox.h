/*
   File:   IvySox.h
   Author: Jeffrey Moore

   IvySox is a lightweight C++ wrapper for socket-based network
   communications.

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

#ifndef INCLUDE_IVYSOX
#define INCLUDE_IVYSOX

#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#ifdef WINDOWS
#include "winsock.h"
// TODO: add the winsock start/stop code for win compat
#endif

#define INBOUND_CONNECTION_LIMIT 20
#define DEFAULT_BUFFER_SIZE 10000


using namespace std;

typedef enum IvySoxState
{
    socketClosed = 0,
    socketOpen = 1
} IvySoxState;

class InboundConnection
{
    public:

    int bufferSize;
    int socketNumber;
    struct sockaddr_storage connection;
    socklen_t connectionStructSize;

    InboundConnection();
    ~InboundConnection();
    int receive(void *message, ssize_t maxLength);
    int sendFile(string filename);
    int sendMessage(string message);
    int sendMessage(void *message, ssize_t length);
    int sendPartial(void *message, ssize_t length);
    unsigned short int getPortNumber();
    void closeConnection();
    string getIpAddress();

};

class IvySox
{
    public:

    IvySox();
    ~IvySox();
    void closeSocket();
    void closeInbound();
    void closeSocket(int socketToClose);
    IvySoxState getState();
    int setIpAddress(string ipAddress);
    int setIpAddress(const char *address);
    string getIpAddress();
    string getInboundIpAddress();
    unsigned short int getInboundPortNumber();
    int getAddressInfo(const char *node, const char *service,
                       const struct addrinfo *hints,
                       struct addrinfo **res);
    int connectTo(const char *url, const char *service);
    int connectTo(string url, string service);
    int getConnectionInfo(string url, string service);
    int getConnectionInfo(const char *url, const char *service);
    int openPort(int portNumber);
    int openServerOnPort(int portNumber);
    int listenToPort();
    int acceptInbound();
    //int acceptInbound(IvySox *newSocket);
    int acceptInbound(InboundConnection *inbound);

    int receiveInbound(void *message, ssize_t maxLength);
    int sendMessage(void *message, ssize_t length);
    int sendMessage(string message);
    int sendPartial(void *message, ssize_t length);
    string getErrorMessage(int errorCode);
    int getSocketNumber();
    int bindSocket();
    int sendFile(string filename);
    //void duplicate(int socketNumberIn, int inboundSocketNumberIn,
    //               struct sockaddr_storage *inbound, 
    //               socklen_t inboundConnectionStructSizeIn);

    string messageToString(char *message, int length);


    private:

    struct sockaddr_storage inboundConnection;
    socklen_t inboundConnectionStructSize;
    int inboundConnectionLimit;
    struct in_addr *getAddressStruct();
    int inboundSocketNumber;
    int socketNumber;
    int portNumber;
    int bufferSize;
    IvySoxState state;
    struct sockaddr socketAddress;
    struct sockaddr_in iSocketAddress;
    struct addrinfo addressInfo;
    struct addrinfo *addressInfoResults;
    struct addrinfo hints;
    bool useIpv6;
    //in_addr internetAddress;

};

int charArrayToInt(const char *array);
void sigPipeHandler(int signalId);


#endif


/*
   Some stuff to know:
   htons() // host to network short
   htonl() // host to network long
   ntohs() // net to host short
   ntohl() // net to host long
*/
