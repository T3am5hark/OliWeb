#  File:    Makefile
#  Author:  Jeff Moore
#  Date:    June 15, 2013
#
#  Makefile for OliWeb.


DEBUG	= 	-O2
CC	= 	g++
INCLUDE	= 	-I'../tinyxml'
FLAGS   = 	$(DEBUG) $(INCLUDE) -g -pthread -std=c++0x
SOURCE	= 	IvySox.cpp OliWeb.cpp main.cpp ../tinyxml/tinyxml2.cpp
OBJ 	=	$(SOURCE:.cpp=.o)
LIBS	=	-lpthread

all:		OliWeb

OliWeb:		$(OBJ)
		@echo [Link]
		@$(CC) -o $@ $(OBJ) $(LIBS)

.cpp.o:		
		@echo [Compile] $<
		@$(CC) -c $(FLAGS) $< -o $@

clean:
		@echo "[Clean]"
		@rm -f $(OBJ) OliWeb
