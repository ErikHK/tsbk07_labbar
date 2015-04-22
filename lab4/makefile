
all : lab4-1

lab4-1: lab4-1.c GL_utilities.c VectorUtils3.c loadobj.c LoadTGA.c MicroGlut.c
	gcc -DGL_GLEXT_PROTOTYPES lab4-1.c GL_utilities.c VectorUtils3.c loadobj.c LoadTGA.c MicroGlut.c -std=c99 -lGL -lglut -lX11 -lm -o lab4-1

clean :
	rm lab4-1
