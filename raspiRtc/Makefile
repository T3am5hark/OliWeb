#  File:    Makefile
#  Author:  Jeff Moore
#  Date:    April 5, 2014
#
#  Makefile for raspiRtc (Real-time controller for pi)


DEBUG	= 	-O2
CC	= 	g++
INCLUDE	= 	-I'../tinyxml'
FLAGS	= 	$(DEBUG) $(INCLUDE) -g -pthread -std=c++0x -fpermissive
SOURCE2	= 	IvySox.cpp OliWeb.cpp main.cpp ../tinyxml/tinyxml2.cpp ../gpio/gpio.c
SOURCE  =       FifoReader.cpp gpio.cpp GpioPwm.cpp main.cpp ../tinyxml/tinyxml2.cpp
OBJ 	=	$(SOURCE:.cpp=.o)
OBJOBJ	=	IvySox.o OliWeb.o main.o tinyXml.o gpio.o 
LIBS	=	-lpthread

all:		raspiRtc install

raspiRtc:	$(OBJ)
		@echo [Link raspiRtc]
		@$(CC) -o $@ $(OBJ) $(LIBS)

.cpp.o:		
		@echo [Compile] $<
		@$(CC) -c $(FLAGS) $< -o $@

clean:
		@echo "[Clean]"
		@rm -f $(OBJ) raspiRtc

install:
		@echo "[Install]"
