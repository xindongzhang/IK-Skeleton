CXX = g++
CXXFLAGS = -O2 -g -Wall -fmessage-length=0
INCPATH =
LIBPATH =

TARGET = main

LIBS =

$(TARGET):main.c main.c bvh_data.h bvh_data.c writer.h writer.c utils.h utils.c parser.h parser.c core.h core.c
	       $(CXX) -o $(TARGET) main.c bvh_data.h bvh_data.c writer.h writer.c utils.h utils.c parser.h parser.c core.h core.c
clean:
	    rm -rf main *.gch main.dSYM
