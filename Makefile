.PHONY: test

RM = /bin/rm -f 
CXX = g++

ifeq ($(shell sw_vers 2>/dev/null | grep Mac | awk '{ print $$2}'),Mac)
	CFLAGS = -g -DGL_GLEXT_PROTOTYPES -I./include/ -I/usr/X11/include -I./lib -DOSX -I/usr/include/soil -O3
	LDFLAGS = -framework GLUT -framework OpenGL -framework CoreFoundation\
    	-L"/System/Library/Frameworks/OpenGL.framework/Libraries" \
    	-lGL -lGLU -lm -lstdc++ -lSOIL
else
	CFLAGS = -g -DGL_GLEXT_PROTOTYPES -Iglut-3.7.6-bin -I./lib -O2
	LDFLAGS = -lglut -lGLU -lSOIL
endif

	
all: clean main 
headers:
	$(CXX) $(CFLAGS) -fsyntax-only src/*.h
main: headers main.o 
	$(CXX) $(CFLAGS) -o main main.o $(LDFLAGS) 
main.o: src/main.cpp 
	$(CXX) $(CFLAGS) -c src/main.cpp -o main.o
clean: 
	$(RM) *.o main
	cd test; make clean

test:
	cd test; make
