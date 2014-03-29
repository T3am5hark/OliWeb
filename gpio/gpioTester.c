// File:      gpioTester.c
// Author:    Jeff Moore
// Date:      June 05 2013
//
// This program pulses (currently hard-coded) gpio pins at a rate specified
// via command line args.
//
// gpioTester [pulseDuration] [repCount]
//
// pulseDuration is specified in milliseconds.
//

#include "gpio.h"

int pins[] = {17,22,23};
const int nPins = 3;

int main(int argc, char **argv)
{
  int g,rep;
  int i=0;
  int repCount = 10;
  double dutyCycle = 0.50;
  time_t waitTime = 1000000*30; // Default 30ms per blink

  // Wait time in ms
  if (argc > 1) waitTime=atoi( argv[1] )*1000;
  // number of reps
  if (argc > 2) repCount=atoi( argv[2] );
  // pulse width duty cycle
  if (argc > 3) dutyCycle=(double)(atoi(argv[3])/100.0);

  if (dutyCycle > 1.0) dutyCycle = 1.0;
  if (dutyCycle < 0.0) dutyCycle = 0.0;

  // Set up gpi pointer for direct register access
  setup_io();

  struct timespec waitStruct;
  waitStruct.tv_sec = 0;
  waitStruct.tv_nsec = (time_t)(waitTime*dutyCycle);

  struct timespec waitStructLo;
  waitStructLo.tv_sec = 0;
  waitStructLo.tv_nsec = (time_t)(waitTime*(1.0-dutyCycle));

  for (i=0;i<nPins;i++)
  {
    //INP_GPIO( pins[i] ); // must use INP_GPIO before we can use OUT_GPIO
    //OUT_GPIO( pins[i] );
    init_output( pins[i] );
  }

  for (rep=0; rep<repCount; rep++)
  {
    for (i = 0; i < nPins; i++)
    {
        SET_PIN( pins[i] );
    }
    nanosleep(&waitStruct, NULL);
    for (i = 0; i < nPins; i++)
    {
        CLR_PIN( pins[i] );
    }
    nanosleep(&waitStructLo,NULL);
  }

  GPIO_CLR = 1<<pins[0] | 1<<pins[1] | 1<<pins[2];

  return 0;

} // main


