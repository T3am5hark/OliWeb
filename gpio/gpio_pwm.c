// File:      gpio_pwm.c
// Author:    Jeff Moore
// Date:      May 29, 2014
//
// This program pulses a gpio pin with a PWM signal
//
// gpioTester [pin] [pulseDuration] [repCount] [dutyCycle]
//
// pulseDuration is specified in microseconds (T_cycle)
// repCount is a positive integer
// dutyCycle is an integer percentage from 0 to 100 (on pulse width)
//

#include "gpio.h"
#include "stdio.h"

void showHelp();

int main(int argc, char **argv)
{
  int g;
  int repCount = 10;
  int rep = 0;
  double dutyCycle = 0.50;
  time_t waitTime = 1000000*30; // Default 30ms per blink

  if (argc == 1) showHelp();

  if (argc > 1) g = atoi( argv[1] );
  // Wait time in ms
  if (argc > 2) waitTime=atoi( argv[2] )*1000;
  // number of reps
  if (argc > 3) repCount=atoi( argv[3] );
  // pulse width duty cycle
  //if (argc > 4) dutyCycle=(double)(atoi(argv[4])/100.0);
  if (argc > 4) dutyCycle=(double)atof(argv[4])/100.0;

  if (dutyCycle > 1.0) dutyCycle = 1.0;
  if (dutyCycle < 0.0) dutyCycle = 0.0;

  // Set up gpi pointer for direct register access
  setup_io();

  struct timespec waitStructHi;
  waitStructHi.tv_sec = 0;
  waitStructHi.tv_nsec = (time_t)(waitTime*dutyCycle);

  struct timespec waitStructLo;
  waitStructLo.tv_sec = 0;
  waitStructLo.tv_nsec = (time_t)(waitTime*(1.0-dutyCycle));

  init_output(g);

  for (rep=0; rep<repCount; rep++)
  {
    SET_PIN( g );
    nanosleep(&waitStructHi, NULL);
    CLR_PIN( g );
    nanosleep(&waitStructLo,NULL);
  }

  CLR_PIN( g );
  return 0;

} // main

void showHelp()
{
  printf("gpio_pwm utility for soft-PWM generation \n");
  printf("---------------------------------------- \n");
  printf("Usage: \n");
  printf("gpio_pwm pin pw pulses duty \n");
  printf("\n");
  printf("pin    = GPIO pin number \n");
  printf("pw     = pulse width, in usec \n");
  printf("pulses = pulse count\n");
  printf("duty   = duty cycle, as whole number percent \n");
  printf("\n\n");
  printf("Example: \n");
  printf("gpio_pwm 20 10000 25 15\n");
  printf("Create 25 pulses of 15pct duty (1500usec from T=10000usec) on pin 20\n");
  exit(0);
}


