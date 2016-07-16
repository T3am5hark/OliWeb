
gpio_pwm 17 18000 4 75
gpio_pwm 17 35000 3 75
sleep 0.085
gpio_pwm 17 50000 2 50 
sleep 0.035
gpio_pwm 17 35000 13 50

$UTIL/redirect.sh gpio.html
