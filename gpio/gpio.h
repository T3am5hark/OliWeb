// File:      gpio.h
// Author:    Jeff Moore
// Date:      March 29, 2014
//
// Header file containing definitions and includes needed for GPIO control.
//

#ifndef GPIO_INCLUDE
#define GPIO_INCLUDE

#define BCM2708_PERI_BASE        0x20000000
#define GPIO_BASE                (BCM2708_PERI_BASE + 0x200000) /* GPIO controller */


#include <stdio.h>
//#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)

extern int  mem_fd;
extern char *gpio_mem, *gpio_map;
extern char *spi0_mem, *spi0_map;

// I/O access
extern volatile unsigned *gpio;


// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))

#define GPIO_SET *(gpio+7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR *(gpio+10) // clears bits which are 1 ignores bits which are 0

#define SET_PIN(g) GPIO_SET = 1 << g
#define CLR_PIN(g) GPIO_CLR = 1 << g

#define INIT_OUT(g) INP_GPIO(g); OUT_GPIO(g)

void setup_io();
void init_output(int g);
void init_input(int g);

#endif
