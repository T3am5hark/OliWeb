#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main()
{
    int fd;
    char * myfifo = "/tmp/myfifo";
    char buffer[BUFFER_SIZE];

    int i = 0;
    int nRead = 0;

    /* open, read, and display the message from the FIFO */
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
}
