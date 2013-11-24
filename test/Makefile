CXX = g++
CFLAGS = -g -DGL_GLEXT_PROTOTYPES -I./include/ -I/usr/X11/include -I../lib -DOSX
LDFLAGS = -framework GLUT -framework OpenGL \
		  -L"/System/Library/Frameworks/OpenGL.framework/Libraries" \
		  -lGL -lGLU -lm -lstdc++
	
# gtest stuff
GTEST_DIR=../lib/gtest
AS_DIR=../..

default: test

gtest:
	$(CXX) -isystem $(GTEST_DIR)/include -I$(GTEST_DIR) \
		-pthread -c $(GTEST_DIR)/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o

compile: gtest
	$(CXX) $(CFLAGS) -isystem $(GTEST_DIR)/include -pthread main.cpp libgtest.a \
		 -I$(AS_DIR) -o main $(LDFLAGS)

test: compile
	./main

clean:
	rm -rf libgtest.a gtest-all.o main *.o *.dSYM
