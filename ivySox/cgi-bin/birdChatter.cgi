
gpio_pwm 17 19000 7 75
sleep 0.05 
gpio_pwm 17 19000 5 65
sleep 0.07
gpio_pwm 17 33000 3 75
sleep 0.25
gpio_pwm 17 75000 3 
gpio_pwm 17 47000 2 
sleep 0.05
gpio_pwm 17 18000 5 75
gpio_pwm 17 27000 2 75
sleep 0.05
gpio_pwm 17 22000 5 75
sleep 0.13
gpio_pwm 17 17000 4 75
sleep 0.1
gpio_pwm 17 44000 1 
sleep 0.06
gpio_pwm 17 25000 4 65
sleep 0.04
gpio_pwm 17 19000 12 75
sleep 0.125
gpio_pwm 17 14000 4 75
gpio_pwm 17 100000 1 75
gpio_pwm 17 45000 3 75
sleep 0.25
gpio_pwm 17 150000 1 5
gpio_pwm 17 14000 20 75

$UTIL/redirect.sh gpio.html
