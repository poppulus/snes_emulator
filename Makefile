#Check for OS, might be scuffed
ifeq ($(OS), Windows_NT)
	uname_S := Windows
else 
	uname_S := $(shell uname -s)
endif

ifeq ($(uname_S), Windows)
	target = snes.exe
endif
ifeq ($(uname_S), Linux)
	target = snes
endif

#OBJS specifies which files to compile as part of the project
OBJS = main.c snes_emu.c

#CC specifies which compiler we're using
CC = gcc

#COMPILER_FLAGS specifies the additional compilation options we're using
# `pkg-config --cflags gtk4`
COMPILER_FLAGS = -Wall -g 

#LINKER_FLAGS specifies the libraries we're linking against
# `pkg-config --libs gtk4` -lSDL2 -lSDL2_mixer gtk+-3.0 -ljack -lasound -pthread -lrt -lm -lportaudio 
LINKER_FLAGS =  #-lGL -lGLEW -lglut 

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = snes

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(COMPILER_FLAGS) -o $(OBJ_NAME) $(OBJS) $(LINKER_FLAGS) 
