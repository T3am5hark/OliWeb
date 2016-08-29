// File:      gpioReadWrite.c
// Author:    Jeff Moore
// Date:      June 05 2013
//
// Usage:
//     gpioReadWrite
//
// Need PI2 defined for BCM2709 - won't work with old base address
#include "gpio.h"
#include <time.h>
#include <string.h>

char *pinString = NULL;

// I/O access
#define GPIO_READ 0
#define GPIO_WRITE 1
#define GPIO_MODE 2
#define GPIO_ERROR -1

void gpioRead(const int pin);
void gpioWrite(const int pin, const int value);
//void gpioUnexport();
//void gpioExport();
void showUsage();

int main(int argc, char **argv)
{
  int pin = 24;
  int mode = GPIO_READ;
  int value = 1;
  if (argc <= 1) mode = GPIO_ERROR;
  if (argc > 1)
  {
    pin = atoi( argv[1] );
    pinString = argv[1];
  }
  if (argc > 2)
  {
    if ( strcmp(argv[2], "read" ) == 0 ) mode = GPIO_READ;
    else if ( strcmp(argv[2], "write") == 0 )
    {
      mode = GPIO_WRITE;
      if ( argc > 3 ) value = ( atoi( argv[3] ) != 0 );
    }
    else mode = GPIO_ERROR;
  }

  if (mode == GPIO_READ) gpioRead(pin);
  else if (mode == GPIO_WRITE) gpioWrite(pin, value);
  else if (mode == GPIO_ERROR) showUsage();

  return 0;
}

/*
void gpioExport(const int pin)
{
  int handle = open("/sys/class/gpio/export", O_WRONLY );
  if (handle < 0)
  {
    printf("handle = %d", &handle);
    printf("Error exporting pin!\n\n");
    exit(-1);
  }
  write(handle, pinString, strlen(pinString) );
  close(handle);
}

void gpioUnexport(const int pin)
{
  int handle = open("/sys/class/gpio/unexport", O_WRONLY );
  if (handle < 0)
  {
    printf("Error unexporting pin!");
    exit(-1);
  }
  write(handle, pinString, strlen(pinString) );
  close(handle);
}
*/
void gpioRead(const int pin)
{
  char value[10];
  char filename[80];
  int handle = -1;
  gpioExport(pin);
  strcpy(filename, "/sys/class/gpio/gpio");
  strcat(filename, pinString);
  strcat(filename, "/value");
  handle = open(filename, O_RDONLY);
  if ( handle < 1 )
  {
    printf("Error reading from pin");
    exit(-1);
  }
  read(handle, value, 10);
  printf(value);
  exit(0);
}


void gpioWrite(const int pin, const int value)
{
  setup_io();
  INP_GPIO( pin ); // must use INP_GPIO before we can use OUT_GPIO
  OUT_GPIO( pin );
  if ( value ) GPIO_SET = 1 << pin;
  else GPIO_CLR = 1 << pin;
  exit(0);
}

void showUsage()
{
  printf("gpioReadWrite command line utility\n");
  printf("----------------------------------\n");
  printf("Usage: \n");
  printf("gpioReadWrite [pin] [read/write/mode] [1/0]\n\n");
}


