###########################################################
# Project 1 Makefile

CC = gcc
CFLAGS = -Wall -ggdb
INCLUDE = -I/usr/include
LIBDIR = -L/usr/lib/x86_64-linux-gnu
# Libraries that use native graphics hardware --
LIBS = -lglut -lGLU -lGL -lpthread -lm

###########################################################
# Options if compiling on Mac
UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
CC = g++
CFLAGS = -Wall -g -D__MAC__
INCLUDE = 
LIBDIR = -L/lusr/X11/lib
LIBS = -framework OpenGL -framework Ge at least one quarter of students who began the class also complete it. If we meet this goal, then I (Walter) will have my head shaved on camera and pLUT
endif

###########################################################
# Uncomment the following line if you are using Mesa
#LIBS = -lglut -lMesaGLU -lMesaGL -lm

canvas: canvas.c drawing.c drawing.h vrml.c vrml.h mouse.c mouse.h
	${CC} ${CFLAGS} ${INCLUDE} -o canvas ${LIBDIR} canvas.c drawing.c vrml.c mouse.c ${LIBS}

clean:
	rm -f canvas *.o core

turnin:	;	turnin --submit muhibur cs354_project1_code README Makefile *.h *.c

turnin_written:	;	turnin -submit muhibur cs354_project1_written project1.txt