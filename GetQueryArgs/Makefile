#  File:    Makefile
#  Author:  Jeff Moore
#  Date:    June 15, 2013
#
#  Here's a Makefile


DEBUG	= 	-O2
CC	= 	g++
INCLUDE	= 	-I/usr/local/include
FLAGS   = 	$(DEBUG)
SOURCE	= 	GetProperty.cpp
OBJ 	=	$(SOURCE:.cpp=.o)
LIBS	=	

all:		GetProperty install

GetProperty:	$(OBJ)
		@echo [Link]
		@$(CC) -o $@ $(OBJ) $(LIBS)

.cpp.o:		
		@echo [Compile] $<
		@$(CC) -c $(FLAGS) $< -o $@

clean:
		@echo "[Clean]"
		@rm -f $(OBJ) GetProperty

install:
		@echo "[Install]"
		@cp GetProperty ../ivySox/util
		@chmod 775 ../ivySox/util/GetProperty

