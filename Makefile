CC = g++
ifeq ($(shell sw_vers 2>/dev/null | grep Mac | awk '{ print $$2}'),Mac)
	CFLAGS = -Wall -g -DGL_GLEXT_PROTOTYPES -I./include/ -I/usr/X11/include -DOSX -Ieigen
	LDFLAGS = -framework GLUT -framework OpenGL \
    	-L"/System/Library/Frameworks/OpenGL.framework/Libraries" \
    	-lGL -lGLU -lm -lstdc++ -O3
else
	CFLAGS = -Wall -g -DGL_GLEXT_PROTOTYPES -Iglut-3.7.6-bin -Ieigen -O3
	LDFLAGS = -lglut -lGLU
endif

RM = /bin/rm -f
all: main
main: Curve.o Shape.o
	$(CC) $(CFLAGS) -o subdivision Curve.o Shape.o $(LDFLAGS)
Curve.o: Curve.cpp Shape.h
	$(CC) $(CFLAGS) -c Curve.cpp -o Curve.o
Shape.o: Shape.cpp Shape.h
	$(CC) $(CFLAGS) -c Shape.cpp -o Shape.o
clean:
	$(RM) *.o subdivision



