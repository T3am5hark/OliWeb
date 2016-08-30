// File:      ranger.c
// Author:    Jeff Moore
// Date:      May 29, 2016
//

#include "gpio.h"
#include <stdio.h>
#include <errno.h>
#include <sys/time.h>

#define V_SOUND_M_PER_S 343.2
#define T_US_PER_M 2913.75
#define T_US_PER_CM 29.1375
#define T_US_PER_IN 74.00928

void showHelp();
void sendInitiatingPulse();
time_t detectReturn();
int time_diff(struct timeval *x , struct timeval *y);

int signalPin, echoPin;
time_t initPulseLengthUs = 10;     // initiation pulse uS
time_t waitTimeNoReturn = 50*1000; // 50ms max time
time_t waitTimeUs = 10;

int timeval_subtract(struct timeval *result, struct timeval *x, struct timeval *y)
{
    struct timeval xx = *x;
    struct timeval yy = *y;
    x = &xx; y = &yy;
    if (x->tv_usec > 999999)
    {
        x->tv_sec += x->tv_usec / 1000000;
        x->tv_usec %= 1000000;
    }
    if (y->tv_usec > 999999)
    {
        y->tv_sec += y->tv_usec / 1000000;
        y->tv_usec %= 1000000;
    }
    result->tv_sec = x->tv_sec - y->tv_sec;
    if ((result->tv_usec = x->tv_usec - y->tv_usec) < 0)
    {
        result->tv_usec += 1000000;
        result->tv_sec--; // borrow
    }
    return result->tv_sec < 0;
}

int time_diff(struct timeval *x, struct timeval *y)
{
    long int x_us , y_us , diff;
    x_us = x->tv_sec*1000000 + x->tv_usec;
    y_us = y->tv_sec*1000000 + y->tv_usec;
    diff = y_us - x_us;
    return (int)diff;
}


int main(int argc, char **argv)
{
    int repCount = 1;
    int rep = 0 ;
    time_t echoReturnTime = 0;

    if (argc < 3 ) showHelp();

    // Process input arguments
    if (argc > 1) signalPin = atoi( argv[1] );
    if (argc > 2) echoPin = atoi( argv[2] );
    if (argc > 3) repCount = atoi( argv[3] );

    printf("signalPin=%d\n", signalPin);
    printf("echoPin=%d\n", echoPin);
    printf("repCount=%d\n", repCount);

    // Set up gpi pointer for direct register access
    setup_io();
    init_output(signalPin);
    CLR_PIN( signalPin );
    // Wait a short time
    usleep(100*1000);

    for (rep=0; rep<repCount; rep++)
    {
      echoReturnTime = detectReturn();
      printf("%d\n", echoReturnTime);
      // 50ms delay between readings minimum
      usleep(100*1000);
    }
    CLR_PIN( signalPin );
    return 0;
} // main

// 10us pulse out
void sendInitiatingPulse()
{
    printf("Initiating pulse\n");
    CLR_PIN( signalPin );
    usleep(5);  // Short wait on low state
    SET_PIN( signalPin );
    usleep(initPulseLengthUs);
    CLR_PIN( signalPin );
    //usleep(100);
}

int pulseIn(const int pin)
{
    struct timeval start, now, diff;
    int durationUs = 0.0;
    int errCode = 0;
    int usleepReturn = 0;
    int i = 0;

    errCode=gettimeofday(&start, NULL);
    // Wait for rising edge
    while (durationUs < waitTimeNoReturn )
    {
        int pinValue = GPIO_GET(pin);

        // Detect rising edge
        if (pinValue > 0)
        {
            //printf("Up\n");
            i = 0;
            gettimeofday(&start, NULL);
            // Now wait for falling edge
            while (pinValue > 0 && durationUs < waitTimeNoReturn)
            {
                usleepReturn=usleep(waitTimeUs);
                if (i%10 == 0){
                    errCode=gettimeofday(&now, NULL);
                    //timeval_subtract(&diff, &now, &start);
                    //durationUs = (diff.tv_usec);
                    durationUs = time_diff(&start, &now);
                }
                i++;
                pinValue = GPIO_GET(echoPin);
            }
            if (pinValue == 0)
            {
                errCode=gettimeofday(&now, NULL);
                //timeval_subtract(&diff, &now, &start);
                //durationUs = (diff.tv_usec);
                durationUs = time_diff(&start, &now);

                //printf("Down!\n");
                //printf("i=%d\n", i);
                //printf("%d uS\n", i*waitTimeUs);
                //printf("measuredDuration = %d\n", durationUs);

                return durationUs;
            }
        }
        usleepReturn=usleep(waitTimeUs);
        errCode=gettimeofday(&now, NULL);
        //timeval_subtract(&diff, &now, &start);
        //durationUs = diff.tv_usec;
        durationUs = time_diff(&start, &now);
        i++;
    }
    return -durationUs;
}

time_t detectReturn()
{

  time_t durationUs = 0.0;
  double distanceCm = 0.0;
  double distanceIn = 0.0;

  sendInitiatingPulse();
  durationUs = pulseIn(echoPin);
  if ( durationUs > 0 )
  {
      distanceCm = (durationUs/2.0) / T_US_PER_CM;
      distanceIn = (durationUs/2.0) / T_US_PER_IN;
      printf("Distance = %4.2fcm (%4.2fin)\n", distanceCm, distanceIn);
      return( (time_t) durationUs);
  } else {
      printf("Timeout at duration = %d\n", durationUs);
      return ( (time_t) durationUs);
  }
}

void showHelp()
{
  printf("ranger utility for ultrasonic range finder \n");
  printf("(developed for HC-SR04 distance sensor)\n");
  printf("------------------------------------------- \n");
  printf("Usage: \n");
  printf("ranger sigPin echoPin [count=1] \n");
  printf("\n");
  printf("sigPin   = GPIO pin number for trigger \n");
  printf("echoPin  = GPIO pin number for echo \n");
  printf("count    = number of ranging iterations (default 1) \n");
  printf("\n\n");
  exit(0);
}


