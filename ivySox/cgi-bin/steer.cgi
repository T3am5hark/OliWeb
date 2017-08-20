MOTOR_PIN=21
position="$($UTIL/GetProperty position $QUERY_STRING)"
pulses="$($UTIL/GetProperty pulses $QUERY_STRING)"

gpio_pwm $MOTOR_PIN 10000 $pulses $position
