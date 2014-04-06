#ifndef GPIO_PWM_DEFINED
#define GPIO_PWM_DEFINED

#include "gpio.h"

class GpioPwm
{
    public:
    static void setupIo();
    static void pulse( int pin, int pulseTimeMs,
                       int pulseCount, double dutyCycle);
};

#endif
