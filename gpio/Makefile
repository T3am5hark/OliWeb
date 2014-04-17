#  File:    Makefile
#  Author:  Jeff Moore
#  Date:    June 15, 2013
#
#  Here's a Makefile


DEBUG	= 	-O2
CC	= 	gcc
INCLUDE	= 	-I/usr/local/include
FLAGS   = 	$(DEBUG)
SOURCE	= 	gpio.c gpioTester.c
OBJ 	=	$(SOURCE:.c=.o)
SOURCE2 =	gpio.c gpio_pwm.c
OBJ2	=	$(SOURCE2:.c=.o)
LIBS	=	-lpthread

all:		gpioTester gpio_pwm install

gpioTester:	$(OBJ)
		@echo [Link gpioTester]
		@$(CC) -o $@ $(OBJ) $(LIBS)

gpio_pwm:	$(OBJ2)
		@echo [Link gpio_pwm]
		@$(CC) -o $@ $(OBJ2) $(LIBS)

.c.o:		
		@echo [Compile] $<
		@$(CC) -c $(FLAGS) $< -o $@

clean:
		@echo "[Clean]"
		@rm -f $(OBJ) gpioTester
		@rm -f $(OBJ2) gpio_pwm

install:
		@echo "[Install]"
		@cp gpioTester /usr/local/bin
		@cp gpio_pwm /usr/local/bin
		@chown root.root /usr/local/bin/gpioTester
		@chown root.root /usr/local/bin/gpio_pwm
		@chmod 4755 /usr/local/bin/gpioTester
		@chmod 4755 /usr/local/bin/gpio_pwm

uninstall:
		@echo "[UnInstall]"
		@rm -f /usr/local/bin/gpioTester
		@rm -f /usr/local/bin/gpio_pwm
