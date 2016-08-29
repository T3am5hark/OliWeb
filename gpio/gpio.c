// File:      gpio.c
// Author:    Jeff Moore
// Date:      March 29, 2014
//
// Setup for gpio controls

#include "gpio.h"
#include <stdio.h>
#include <string.h>

int  mem_fd;
char *gpio_mem, *gpio_map;
char *spi0_mem, *spi0_map;

// I/O access
volatile unsigned *gpio;
//
// Set up a memory regions to access GPIO
//
void setup_io()
{

   /* open /dev/mem */
   if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
      printf("can't open /dev/mem \n");
      exit (-1);
   }

   /* mmap GPIO */

   // Allocate MAP block
   if ((gpio_mem = malloc(BLOCK_SIZE + (PAGE_SIZE-1))) == NULL) {
      printf("allocation error \n");
      exit (-1);
   }

   // Make sure pointer is on 4K boundary
   if ((unsigned long)gpio_mem % PAGE_SIZE)
     gpio_mem += PAGE_SIZE - ((unsigned long)gpio_mem % PAGE_SIZE);

   // Now map it
   gpio_map = (unsigned char *)mmap(
      (caddr_t)gpio_mem,
      BLOCK_SIZE,
      PROT_READ|PROT_WRITE,
      MAP_SHARED|MAP_FIXED,
      mem_fd,
      GPIO_BASE
   );

   close(mem_fd);

   if ((long)gpio_map < 0) {
      printf("mmap error %d\n", (int)gpio_map);
      exit (-1);
   }

   // Always use volatile pointer!
   gpio = (volatile unsigned *)gpio_map;

} // setup_io


void init_output(int g)
{
    INP_GPIO(g);
    OUT_GPIO(g);
}

void init_input(int g)
{
    INP_GPIO(g);
}

void gpioExport(const int pin)
{
  char pinString[10];
  sprintf(pinString, "%d", pin);
  int handle = open("/sys/class/gpio/export", O_WRONLY );
  if (handle < 0)
  {
    printf("handle = %d", &handle);
    printf("Error exporting pin!\n\n");
    //exit(-1);
  }
  write(handle, pinString, strlen(pinString) );
  close(handle);
}

void gpioUnexport(const int pin)
{
  char pinString[10];
  sprintf(pinString, "%d", pin);

  int handle = open("/sys/class/gpio/unexport", O_WRONLY );
  if (handle < 0)
  {
    printf("Error unexporting pin!");
    //exit(-1);
  }
  write(handle, pinString, strlen(pinString) );
  close(handle);
}

int get_pin(const int g)
{
  char pinString[10];
  sprintf(pinString, "%d", g);

  char value[10];
  char filename[80];
  int handle = -1;
  gpioExport(g);
  strcpy(filename, "/sys/class/gpio/gpio");
  strcat(filename, pinString);
  strcat(filename, "/value");
  handle = open(filename, O_RDONLY);
  if ( handle < 1 )
  {
    printf("Error reading from pin");
    //exit(-1);
  }
  read(handle, value, 10);
  return atoi(value);
}
