#  File:    Makefile
#  Author:  Jeff Moore
#  Date:    June 15, 2013
#
#  Makefile for OliWeb.


DEBUG	= 	-O2
CC	= 	g++
INCLUDE	= 	-I'../tinyxml'
FLAGS	= 	$(DEBUG) $(INCLUDE) -g -pthread -std=c++0x
SOURCE	= 	IvySox.cpp OliWeb.cpp main.cpp ../tinyxml/tinyxml2.cpp
OBJ 	=	$(SOURCE:.cpp=.o)
LIBS	=	-lpthread

all:		OliWeb GetProperty local_install

OliWeb:		$(OBJ)
		@echo [Link]
		@$(CC) -o $@ $(OBJ) $(LIBS)

GetProperty:
		@cd ../GetQueryArgs;make all

.cpp.o:
		@echo [Compile] $<
		@$(CC) -c $(FLAGS) $< -o $@

clean:
		@echo "[Clean]"
		@rm -f $(OBJ) OliWeb
		@cd ../GetQueryArgs;make clean

local_install:
		@echo "[Local Install]"
		@chmod 775 cgi-bin/*.cgi
		@chmod 775 util/*.*
		@chmod 775 util/*
		@echo "Use sudo make install to activate on boot"

install:
		@echo "[Install]"
		@./install.sh
