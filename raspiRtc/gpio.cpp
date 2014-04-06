// File:      gpio.c
// Author:    Jeff Moore
// Date:      March 29, 2014
//
// Setup for gpio controls

#include "gpio.h"
#include <iostream>

int  mem_fd;
char *gpio_mem, *gpio_map;
char *spi0_mem, *spi0_map;

// I/O access
volatile unsigned *gpio;


using namespace std;
//
// Set up a memory regions to access GPIO
//
extern "C" void setup_io()
{

   //cout << "Opening..." << endl;

   /* open /dev/mem */
   if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
      printf("can't open /dev/mem \n");
      exit (-1);
   }

   /* mmap GPIO */

   // Allocate MAP block
   if ((gpio_mem = (char *)malloc(BLOCK_SIZE + (PAGE_SIZE-1))) == NULL) {
      printf("allocation error \n");
      exit (-1);
   }

   // Make sure pointer is on 4K boundary
   if ((unsigned long)gpio_mem % PAGE_SIZE)
     gpio_mem += PAGE_SIZE - ((unsigned long)gpio_mem % PAGE_SIZE);

   // Now map it
   gpio_map = (char *)mmap(
      (caddr_t)gpio_mem,
      BLOCK_SIZE,
      PROT_READ|PROT_WRITE,
      MAP_SHARED|MAP_FIXED,
      mem_fd,
      GPIO_BASE
   );

   if ((long)gpio_map < 0) {
      printf("mmap error %d\n", (int)gpio_map);
      exit (-1);
   }

   // Always use volatile pointer!
   gpio = (volatile unsigned *)gpio_map;

} // setup_io


extern "C" void init_output(int g)
{
    INP_GPIO(g);
    OUT_GPIO(g);
}

extern "C" void init_input(int g)
{
    INP_GPIO(g);
}

extern "C" void set_pin(int g)
{
    SET_PIN(g);
}

extern "C" void clr_pin(int g)
{
    CLR_PIN(g);
}
