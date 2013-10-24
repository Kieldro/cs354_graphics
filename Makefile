CXXFLAGS += -Wall -ggdb -std=c++11# used for vector brace initialization
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
	./canvas

check: canvas plant
	./canvas
	./plant

hello:
	$(CXX) hello.c++ $(INCLUDE) $(LIBDIR) $(LIBS)
	./a.out 1 2

clean:
	@- $(RM) canvas plant *.o core		# run silently and ignore errors

###########################################################
# Project 1
PROJECTDIR1 := ./project1
p1_SRCS := $(shell find $(PROJECTDIR1) -regextype egrep -regex ".+\.c(\+\+)?")
p1_HEADERS := $(wildcard $(PROJECTDIR1)/*.h)

canvas: $(p1_HEADERS) $(p1_SRCS)
	${CXX} ${CXXFLAGS} ${INCLUDE} -o canvas ${LIBDIR} $(p1_SRCS) ${LIBS}

turnin:
	zip project1_code README Makefile $(p1_SRCS) $(p1_HEADERS)
	turnin --submit muhibur cs354_project1_code project1_code
	turnin --list muhibur cs354_project1_code
	# turnin --verify muhibur cs354_project1_code

turnin_written:	;	turnin --submit muhibur cs354_project1_written project1.txt
	
###########################################################
# Project 2
PROJECTDIR2 := ./project2
p2_HEADERS := $(wildcard $(PROJECTDIR2)/*.h)
p2_SRCS := $(wildcard $(PROJECTDIR2)/*.c++)

plant: $(p2_HEADERS) $(p2_SRCS)
	${CXX} ${CXXFLAGS} ${INCLUDE} -o plant ${LIBDIR} $(p2_SRCS) ${LIBS}
