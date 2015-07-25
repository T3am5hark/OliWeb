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

#include "../ivySox/ivySox.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>

#define INBOUND_BUFFER_SIZE 20000

template <class myType> string toString(myType item)
{
    ostringstream oss;
    oss << item;
    return (oss.str());
}

int main (int argc, char *argv[])
{
    int portNumber = 8077;
    char inboundMessage[INBOUND_BUFFER_SIZE];
    if (argc > 1)
    {
        portNumber = atoi(argv[1]);
    }
    std::cout << "\r\nListening on port " << toString(portNumber) << "\r\n";

    std::cout << "To connect, set your browser's URL to \r\nhttp://[PiIPAddress]:"
              << toString(portNumber) << "\r\n";
    // Create a socket listener and open the port
    IvySox listener;

    // This object will be used to manage communications.  In a more
    // sophisticated use case, you'd create these dynamically and spawn
    // handler threads to enable multiple simultaneous connections, but
    // for the sake of this simple example we'll just do one at a time.
    InboundConnection inboundConnection;
    InboundConnection *inbound = &inboundConnection;
    std::cout << "Opening port...\r\n";
    int result = listener.openServerOnPort(portNumber);

    // Try to listen on port
    if (result < 0 || listener.listenToPort() < 0 )
    {
        std::cout << "Error opening port! (" <<  "\r\n";
        exit(-1);
    }


    // Listen and wait for inbound messages.
    while (1)
    {
        std::cout << "Waiting for inbound...\r\n";
        //  This will populate the inbound request object with
        //  inbound connection metadata.  We use the inbound
        //  connection object to send and receive data as well.
        int socketNumber = listener.acceptInbound(inbound);
        std::cout << "Received request from " <<
                     inbound->getIpAddress() << "\r\n";
        std::cout << "(socket#=" << toString(socketNumber) << ")\r\n";
        size_t receivedBytes=inbound->receive(inboundMessage, INBOUND_BUFFER_SIZE);
        std::cout << "Received " << toString(receivedBytes) << " bytes.\r\n";
        std::cout << "Sending response...\r\n";
        // Message isn't null-terminated necessarily so need to do this:
        string inboundString = listener.messageToString(inboundMessage, receivedBytes);
        // Construct a response message.
        string response;
        // Do this to let HTTP client know request worked... won't show up in
        // browser but will make some HTTP clients feel better.
        response = "HTTP/1.1 200 OK\r\n\r\n";
        response += "Hello Socket!\r\n\r\n";
        response += "Recieved request from " + inbound->getIpAddress() + "\r\n";
        response += "on socket #" + toString(socketNumber) + "\r\n";
        response += "(" + toString(receivedBytes) + " bytes)\r\n";
        response += "Inbound request contents: \r\n\r\n" + inboundString;
        // Send response message
        size_t sentBytes = inbound->sendMessage(response);
        // You can send as many as you please as long as the connection stays open.
        // This is the string variant of sendMessage, but there's another
        // variant for binary data as well which you can use to shove images or
        // serialized objects or what have you.
        // Send another message to say how big the previous message was.
        inbound->sendMessage( "\r\n\r\n(" + toString(sentBytes) + " bytes sent)\r\n");
        std::cout << "Sent " << toString(sentBytes) << " bytes.\r\n\r\n";
        // Close the connection
        inbound->closeConnection();
        std::cout.flush();
    }
    exit(0);
}
