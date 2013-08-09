#  File:    Makefile
#  Author:  Jeff Moore
#  Date:    June 15, 2013
#
#  Here's a Makefile


DEBUG	= 	-O2
CC	= 	g++
INCLUDE	= 	-I'../ivySox'
FLAGS   = 	$(DEBUG) $(INCLUDE) -g 
SOURCE	=	main.cpp ../ivySox/IvySox.cpp
OBJ 	=	$(SOURCE:.cpp=.o)
LIBS	=	-lpthread

all:		HelloSocket

HelloSocket:	$(OBJ)
		@echo [Link]
		@$(CC) -o $@ $(OBJ)

.cpp.o:		
		@echo [Compile] $<
		@$(CC) -c $(FLAGS) $< -o $@

clean:
		@echo "[Clean]"
		@rm -f $(OBJ) HelloSocket

install:
		@echo "[Install]"

uninstall:
		@echo "[UnInstall]"
