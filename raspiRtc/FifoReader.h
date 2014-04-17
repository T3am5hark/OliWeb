#ifndef INCLUDE_FIFOREADER
#define INCLUDE_FIFOREADER

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#include <string>

#define READ_BUFFER_SIZE 4096

using namespace std;

class FifoReader
{
    public:

    string fifoName;
    bool haltListen;

    FifoReader();
    ~FifoReader();

    static void createSystemFifo( string fifoName, int permissions);
    void openFifo();
    void openFifo(string inFifoName);
    void closeFifo();
    string readFifo();

    void listenAndRunOnMessage( void (*callBack)(char *, int, FifoReader *) );
    static void defaultHandler( char *message, int nRead, FifoReader * reader);

    private:

    int fd;

    char readBuffer[READ_BUFFER_SIZE];

};


/*
int main()
{
    int fd;
    char * myfifo = "/tmp/myfifo";
    char buffer[BUFFER_SIZE];

    int i = 0;
    int nRead = 0;

    mkfifo(myfifo, 7777);

    //for (i = 0; i < MAX_BUF; i++) buffer[i] = 0;

    fd = open(myfifo, O_RDONLY);

    while ( 1 )
    {
      nRead = read(fd, buffer, BUFFER_SIZE);
      if (nRead > 0 )
      {
        // Null-terminate
        buffer[nRead] = 0;
        printf("Received: %s\n", buffer);
        fflush(stdout);
      }
      sleep(1);
    }
    close(fd);

    return 0;
}*/

#endif
