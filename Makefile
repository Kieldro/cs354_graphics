# CC = g++
CXXFLAGS += -Wall -ggdb
INCLUDE = -I/usr/include
LIBDIR = -L/usr/lib/x86_64-linux-gnu
# Libraries that use native graphics hardware --
LIBS = -lglut -lGLU -lGL -lpthread -lm
RM := rm -f

###########################################################
# Options if compiling on Mac
UNAME := $(shell uname)		# immediate evalutation																									
ifeq ($(UNAME), Darwin)
CC = g++
CFLAGS = -Wall -g -D__MAC__
INCLUDE = 
LIBDIR = -L/lusr/X11/lib
LIBS = -framework OpenGL -framework GLUT
endif

# Uncomment the following line if you are using Mesa
#LIBS = -lglut -lMesaGLU -lMesGaL -lm
###########################################################
all: canvas
	$(PROJECTDIR1)/canvas

check: canvas
	# run unit tests

hello:
	$(CXX) hello.c++ $(INCLUDE) $(LIBDIR) $(LIBS)
	./a.out 1 2

clean:
	@- $(RM) canvas *.o core		# run silently and ignore errors

###########################################################
# Project 1
PROJECTDIR1 := ./project1
p1_SRCS := $(shell find $(PROJECTDIR1) -regextype egrep -regex ".+\.c(\+\+)?")
p1_HEADERS := $(wildcard $(PROJECTDIR1)/*.h)
canvas: $(p1_SRCS) $(p1_HEADERS)
	echo $(p1_SRCS)
	${CXX} ${CXXFLAGS} ${INCLUDE} -o canvas ${LIBDIR} $(p1_SRCS) ${LIBS}

turnin:	;	turnin --submit muhibur cs354_project1_code README Makefile $(p1_SRCS) $(p1_HEADERS)

turnin_written:	;	turnin --submit muhibur cs354_project1_written project1.txt
	
# PROJECTDIR = project1

# include ./project1/Makefile



###########################################################
# Project 2
plant: drawplant.cpp drawplant.h plant.cpp readppm.cpp
	${CXX} ${CXXFLAGS} ${INCLUDE} -o plant ${LIBDIR} drawplant.cpp plant.cpp readppm.cpp ${LIBS}
