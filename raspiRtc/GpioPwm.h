#ifndef GPIO_PWM_DEFINED
#define GPIO_PWM_DEFINED

#include "gpio.h"
#include <set>

using namespace std;

class GpioPwm
{
    public:
    static void setupIo();
    static void pulse( int pin, int pulseTimeMs,
                       int pulseCount, double dutyCycle);

    static void pulse( const set<int> &pins, int pulseTimeMs,
                       int pulseCount, double dutyCycle);
};

#endif
