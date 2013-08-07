//  How to access GPIO registers from C-code on the Raspberry-Pi
//  Example program
//  15-January-2012
//  Dom and Gert

// Access from ARM Running Linux

#define BCM2708_PERI_BASE        0x20000000
#define GPIO_BASE                (BCM2708_PERI_BASE + 0x200000) /* GPIO controller */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <unistd.h>
#include <time.h>

#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)

int  mem_fd;
char *gpio_mem, *gpio_map;
char *spi0_mem, *spi0_map;

int pins[] = {24};
const int nPins = 1;

// I/O access
volatile unsigned *gpio;


// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))

#define GPIO_SET *(gpio+7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR *(gpio+10) // clears bits which are 1 ignores bits which are 0

void setup_io();

int main(int argc, char **argv)
{
  int g,rep;
  int i=0;
  int repCount = 10;
  time_t waitTime = 1000000*30; // Default 30ms per blink

  // Wait time in ms
  if (argc > 1) waitTime=atoi( argv[1] )*1000000;
  // number of reps
  if (argc > 2) repCount=atoi( argv[2] );


  // Set up gpi pointer for direct register access
  setup_io();

  // Switch GPIO 7..11 to output mode
  struct timespec waitStruct;
  waitStruct.tv_sec = 0;
  waitStruct.tv_nsec = waitTime;

  /* You are about to change the GPIO settings of your computer.          *
   * Mess this up and it will stop working!                               *
   * It might be a good idea to 'sync' before running this program        *
   * so at least you still have your code changes written to the SD-card! */

  // Set GPIO pins 7-11 to output
  //for (g=7; g<=11; g++)
  //for (g=17; g<=23; g++)
  for (i=0;i<nPins;i++)
  {
    INP_GPIO( pins[i] ); // must use INP_GPIO before we can use OUT_GPIO
    //OUT_GPIO( pins[i] );
  }

  while (1)
  {
  }

  for (rep=0; rep<repCount; rep++)
  {
    //for (i = 0;i<nPins;i++)
    //{
       GPIO_SET = 1<<pins[0];
       //sleep(1);
       nanosleep(&waitStruct, NULL);
       GPIO_CLR = 1<<pins[0];
       if (rep%2 == 0) GPIO_SET = 1<<pins[2];
       if (rep%2 == 1) GPIO_CLR = 1<<pins[2];
       if (rep%3 == 0) GPIO_SET = 1<<pins[1];
       if (rep%3 == 1) GPIO_CLR = 1<<pins[1];
       nanosleep(&waitStruct,NULL);
    //}
    //for (i =0; i<nPins; i++)
    //{
    //  GPIO_CLR = 1<<pins[i];
      //sleep(1);
    //  nanosleep(&waitStruct, NULL);
    //}
  }

  GPIO_CLR = 1<<pins[0] | 1<<pins[1] | 1<<pins[2];

  return 0;

} // main


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

   if ((long)gpio_map < 0) {
      printf("mmap error %d\n", (int)gpio_map);
      exit (-1);
   }

   // Always use volatile pointer!
   gpio = (volatile unsigned *)gpio_map;


} // setup_io
