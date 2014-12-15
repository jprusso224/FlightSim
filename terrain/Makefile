
# World
EXE=main
SRCS = main.cpp terrain.cpp imageloader.cpp vec3f.cpp 
# Main target
all: $(EXE)

#  MinGW
ifeq "$(OS)" "Windows_NT"
CFLG=-O3 -Wall
LIBS=-lglut32cu -lglu32 -lopengl32
CLEAN=del *.exe *.o *.a
else
#  OSX
ifeq "$(shell uname)" "Darwin"
CFLG=-O3 -Wall -Wno-deprecated-declarations
LIBS=-framework GLUT -framework OpenGL
#  Linux/Unix/Solaris
else
CFLG=-O3 -Wall
LIBS= -lSDL -lSDL_mixer -lglut -lGLU -lGL -lm 
endif
#  OSX/Linux/Unix/Solaris
CLEAN=rm -f $(EXE) *.o *.a
endif

# Dependencies
main.o: main.cpp CSCIx229.h terrain.h vec3f.h imageloader.h
fatal.o: fatal.c CSCIx229.h
loadtexbmp.o: loadtexbmp.c CSCIx229.h
print.o: print.c CSCIx229.h
project.o: project.c CSCIx229.h
errcheck.o: errcheck.c CSCIx229.h
terrain.o: terrain.cpp terrain.h
vec3f.o: vec3f.cpp vec3f.h
imageloader.o: imageloader.cpp imageloader.h


#  Create archive
CSCIx229.a:fatal.o loadtexbmp.o print.o project.o errcheck.o
	ar -rcs $@ $^

# Compile rules
.c.o:
	gcc -c $(CFLG) $<
.cpp.o:
	g++ -c $(CFLG) $<

#  Link
main:main.o terrain.o imageloader.o vec3f.o CSCIx229.a 
	gcc -O3 -o $@ $^   $(LIBS)

#  Clean
clean:
	$(CLEAN)
