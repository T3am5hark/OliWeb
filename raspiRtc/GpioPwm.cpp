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

#include "GpioPwm.h"
#include <time.h>

void GpioPwm::setupIo()
{
    setup_io();
}

void GpioPwm::pulse(int pin, int pulseTimeMs,
                    int pulseCount, double dutyCycle)
{
    struct timespec waitStructHi;
    struct timespec waitStructLo;
    waitStructHi.tv_sec = 0;
    waitStructHi.tv_nsec = (time_t)(pulseTimeMs*dutyCycle*1000);
    waitStructLo.tv_sec = 0;
    waitStructLo.tv_nsec = (time_t)(pulseTimeMs*(1.0-dutyCycle)*1000);

    for (int i = 0; i < pulseCount; i++)
    {
        set_pin( pin );
        nanosleep(&waitStructHi, NULL);
        clr_pin( pin );
        nanosleep(&waitStructLo,NULL);
    }

    clr_pin( pin );

}


