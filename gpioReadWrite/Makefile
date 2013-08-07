#  File:    Makefile
#  Author:  Jeff Moore
#  Date:    June 15, 2013
#
#  Here's a Makefile


DEBUG	= 	-O2
CC	= 	gcc
INCLUDE	= 	-I/usr/local/include
FLAGS   = 	$(DEBUG)
SOURCE	= 	gpioReadWrite.c
OBJ 	=	$(SOURCE:.c=.o)
LIBS	=	-lpthread

all:		gpioReadWrite install

gpioReadWrite:	$(OBJ)
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
		@cp gpioReadWrite /usr/local/bin
		@chown root.root /usr/local/bin/gpioReadWrite
		@chmod 4755 /usr/local/bin/gpioReadWrite

uninstall:
		@echo "[UnInstall]"
		@rm -f /usr/local/bin/gpioTester
