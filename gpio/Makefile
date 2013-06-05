#  File:    Makefile
#  Author:  Jeff Moore
#  Date:    June 15, 2013
#
#  Here's a Makefile


DEBUG	= 	-O2
CC	= 	gcc
INCLUDE	= 	-I/usr/local/include
FLAGS   = 	$(DEBUG)
SOURCE	= 	gpioTester.c
OBJ 	=	$(SOURCE:.c=.o)
LIBS	=	-lpthread

all:		gpioTester install

gpioTester:	$(OBJ)
		@echo [Link]
		@$(CC) -o $@ $(OBJ) $(LIBS)

.c.o:		
		@echo [Compile] $<
		@$(CC) -c $(FLAGS) $< -o $@

clean:
		@echo "[Clean]"
		@rm -f $(OBJ) gpioTester

install:
		@echo "[Install]"
		@cp gpioTester /usr/local/bin
		@chown root.root /usr/local/bin/gpioTester
		@chmod 4755 /usr/local/bin/gpioTester

uninstall:
		@echo "[UnInstall]"
		@rm -f /usr/local/bin/gpioTester
