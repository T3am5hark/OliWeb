#include "FifoReader.h"
#include <iostream>
#include <string.h>

using namespace std;

FifoReader::FifoReader()
{
    haltListen = false;
}

FifoReader::~FifoReader()
{
    haltListen = true;
}

void FifoReader::createSystemFifo( string fifoName, int permissions)
{
    mkfifo( fifoName.c_str(), permissions);
}

void FifoReader::listenAndRunOnMessage( void  (*callBack)(char *, int, FifoReader *) )
{
    if (callBack == NULL) callBack = &defaultHandler;

    haltListen = false;
    int fd = open(fifoName.c_str(), O_RDONLY);

    struct timespec waitStruct;
    waitStruct.tv_sec = 0;
    waitStruct.tv_nsec = (time_t)(50*1000000); // 50ms wait

    while (!haltListen)
    {
        int nRead = read(fd, readBuffer, READ_BUFFER_SIZE);

        if (nRead > 0)
        {
            // Null-terminate
            readBuffer[nRead] = 0;
            // ... and call callback
            callBack(readBuffer, nRead, this);
        }
        //sleep(1);
        nanosleep(&waitStruct, NULL);
    }
    close(fd);
}

string FifoReader::readFifo()
{
    string bufferContents="";
    if (fd >0)
    {
        int nRead = read(fd, readBuffer, READ_BUFFER_SIZE);
        bufferContents=readBuffer;
    }
    return bufferContents;
}

void FifoReader::openFifo(string inFifoName)
{
    fifoName=inFifoName;
    openFifo();
}

void FifoReader::openFifo()
{
    fd = open(fifoName.c_str(), O_RDONLY);
}

void FifoReader::closeFifo()
{
    close(fd);
}

void FifoReader::defaultHandler( char * buffer, int nRead, FifoReader *reader)
{
    std::cout << endl << "Received message: " << endl;
    std::cout << buffer << endl;
    if (strcmp( buffer, "HALT LISTEN")== 0) reader->haltListen = true;
}
