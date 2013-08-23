
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <string>
#include <sstream>
#include <exception>
#include "IvySox.h"
#include <errno.h>
#include <signal.h>
#include "tinyxml2.h"


#define BUFFERSIZE 5000

using namespace std;

void tellArgs( int argc, char *argv[])
{
    string str("");
    cout << "Arguments: " << endl;
    for (int i = 0; i < argc; i++)
    {
        cout << "  " << (str=argv[i]) << endl;
    }
}

int main( int argc, char *argv[])
{

    cout << "IvySox tester" << endl;
    tellArgs(argc, argv);
    int portNumber=8077;
    if (argc > 1 )
    {
        cout << "Using custom port " << argv[1] << '.' << endl;
        sscanf(argv[1],"%d", &portNumber);
        //string s = argv[1];
        //portNumber = stoi(s);
    }

    IvySox ivySox;

    char inboundBuffer[BUFFERSIZE];
    //ostringstream response("");
    const string headerMessage = "Hello there!\nThis is IvySox on Raspberry Pi.\n";
    string rxMessage = "";
    string responseMessage = "";
    char request[512];
    
    size_t messageLength = 0;
    memset(inboundBuffer,0,BUFFERSIZE);


    try {
        //cout << "Opening port " << portNumber << endl;
        //ivySox.open(portNumber);
        //cout << "Closing port " << portNumber << endl;
        //ivySox.close();

        cout << "Looking up addr infor for google.com" << endl;
        int retVal = ivySox.connectTo("www.google.com","80");
        cout << "Checking www.google.com: " << retVal << endl;
        cout << "Google's ip address: " << ivySox.getIpAddress() << endl;
        cout << "Socket number = " << ivySox.getSocketNumber() << endl;
        cout << "Listening on port " << portNumber << endl;
        ivySox.openPort(portNumber);
        cout << "Listening..." << endl;
        if (ivySox.listenToPort() < 0)
        {
            perror("Listen: ");
            //exit(1);
        }

        while (1)
        {
            cout << "Waiting for inbound request..." << endl;
            //cout << "Woke up!" << endl;
            int inboundSocketNumber = ivySox.acceptInbound();
            cout << "Received inbound request from "
                 << ivySox.getInboundIpAddress()
                 << endl;

            cout << "Inbound port = " << ivySox.getInboundPortNumber() << endl;
            cout << "Socket = " << inboundSocketNumber << endl;

            int receivedBytes = ivySox.receiveInbound(inboundBuffer, BUFFERSIZE);
            ostringstream response("");
            //response.clear();

            response << headerMessage;
            response << "My IP Address is: " << ivySox.getIpAddress() << endl;
            response << "IvySox inbound request:" << endl;
            response << "(" << receivedBytes << " bytes)" << endl;
            if (receivedBytes >= 0)
            {
                rxMessage = ivySox.messageToString(inboundBuffer, receivedBytes);
                response << "[BEGIN TRANSMISSION]"
                         << rxMessage
                         << "[END TRANSMISSION]" << endl;
                istringstream stream(rxMessage);
                stream.getline(request, 512);
                cout << "Request = [" << request << "]" << endl;

            } else
            {
                cout << "errno = " << errno << endl;
                perror("Something is wrong...");
                //continue;
            }

            int bytesSent = 0;
            string requestString = request;
            size_t pos = requestString.find("identifypi");
            if (pos != string::npos)
            {
                messageLength = strlen(response.str().c_str());
                cout << "Sending response (" << messageLength << " bytes)" <<endl;
                cout << response.str();
                bytesSent = ivySox.sendMessage(response.str());
            } else {
                //int bytesSent = ivySox.sendMessage(response.str());
                //int bytesSent = ivySox.sendFile("index.html");
                //int bytesSent = ivySox.sendFile("test.jpg");
                cout << "Sending \"BigMap.png\"" << endl;
                bytesSent = ivySox.sendFile("BigMap.png");
            }
            cout << "Sent " << bytesSent << " bytes." << endl;
            if (bytesSent < 0) perror("sendMessage ");
            cout << "Closing inbound." << endl;
            ivySox.closeInbound();
        }

    }  catch (exception &ex) {
        cout << "Exception caught!!!!\n";
        cout << ex.what() << endl;
    }
    cout << "Done!!" << endl;
}
