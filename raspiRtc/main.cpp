#include "FifoReader.h"
//#include "gpio.h"
#include "GpioPwm.h"
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <sstream>
#include "stringTo.h"
#include "Agent.h"

#define CMD_BUFFER_LEN 5
string cmds[5];

using namespace RaspiController;

//GpioPwm pwm;
void handler( char * buffer, int nRead, FifoReader *reader);

int main( int argc, char **argv)
{

    Agent agent("ChirpBox");
    agent.configureFromXmlFile("controller.xml");
    agent.run();

    cout << "Start." << endl << flush;

    int fd = open("/dev/mem", O_RDWR|O_SYNC);
    cout << "fd=" << fd << endl << flush;


    string fifoName = "/tmp/myFifo";
    FifoReader::createSystemFifo( fifoName,O_CREAT | 0666);
    FifoReader reader;
    reader.fifoName = fifoName;
    cout << "SetupIo" << endl << flush;
    GpioPwm::setupIo();
    cout << "done." << endl << flush;

    cout << "chirp!" << endl;
    //GpioPwm::pulse(17, 50000, 5, 0.5);

    reader.listenAndRunOnMessage( &handler );

    return (0);
}

void parse( char * buffer )
{
    string str = buffer;
    istringstream parser(str);
    for (int i = 0; i < CMD_BUFFER_LEN; i++) parser >> cmds[i];
}

void handler( char * buffer, int nRead, FifoReader *reader)
{
    std::cout << endl << "Received Message:" << endl;
    std::cout << buffer << endl;

    parse(buffer);

    for (int i = 0; i < CMD_BUFFER_LEN; i++)
        cout << "cmd" << i << ": [" << cmds[i] << "]" << endl;

    if ( cmds[0] == "chirp" ) GpioPwm::pulse(17,50000, 5, 0.5);
    if ( cmds[0] == "left" ) GpioPwm::pulse(22, 10000, 50, 0.06);
    if ( cmds[0] == "center" ) GpioPwm::pulse(22, 10000, 50, 0.15);
    if ( cmds[0] == "right" ) GpioPwm::pulse(22, 10000, 50, 0.24);


/*
    if ( strcmp(buffer, "chirp")==0)
    {
        GpioPwm::pulse(17, 50000, 5,  0.5);
    }


    cout << "strcmp(buffer,\"chirp\") = " << strcmp(buffer,"chirp") << endl;

    for ( int i = 0; i < READ_BUFFER_SIZE; i++)
    {
        cout << (short)buffer[i] << " ";
        if (buffer[i] == 0) break;
    }

    string myString = buffer;
    cout << myString << endl;

    if ( myString == "chirp" ) GpioPwm::pulse(17, 50000, 5, 0.5);
*/

}
