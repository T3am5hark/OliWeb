MOTOR_PIN=21
position="$($UTIL/GetProperty position $QUERY_STRING)"
pulses="$($UTIL/GetProperty pulses $QUERY_STRING)"

# Use full install path - install version doesn't always have this in path
/usr/local/bin/gpio_pwm $MOTOR_PIN 10000 $pulses $position
