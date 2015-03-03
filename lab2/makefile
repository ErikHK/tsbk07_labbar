
all : lab2-1, lab2-2, lab2-3, lab2-4, lab2-5, lab2-7

lab2-1: lab2-1.c GL_utilities.c 
	gcc -Wall -DGL_GLEXT_PROTOTYPES lab2-1.c loadobj.c GL_utilities.c MicroGlut.c -std=c99 -lGL -lglut -lX11 -lm -o lab2-1

lab2-2: lab2-2.c GL_utilities.c 
	gcc  -DGL_GLEXT_PROTOTYPES lab2-2.c loadobj.c LoadTGA.c GL_utilities.c MicroGlut.c -std=c99 -lGL -lglut -lX11 -lm -o lab2-2

lab2-3: lab2-3.c GL_utilities.c 
	gcc  -DGL_GLEXT_PROTOTYPES lab2-3.c loadobj.c LoadTGA.c VectorUtils3.c GL_utilities.c MicroGlut.c -std=c99 -lGL -lglut -lX11 -lm -o lab2-3

lab2-4: lab2-4.c GL_utilities.c 
	gcc  -DGL_GLEXT_PROTOTYPES lab2-4.c loadobj.c LoadTGA.c VectorUtils3.c GL_utilities.c MicroGlut.c -std=c99 -lGL -lglut -lX11 -lm -o lab2-4

lab2-5: lab2-5.c GL_utilities.c 
	gcc  -DGL_GLEXT_PROTOTYPES lab2-5.c loadobj.c LoadTGA.c VectorUtils3.c GL_utilities.c MicroGlut.c -std=c99 -lGL -lglut -lX11 -lm -o lab2-5

lab2-7: lab2-7.c GL_utilities.c 
	gcc  -DGL_GLEXT_PROTOTYPES lab2-7.c loadobj.c LoadTGA.c VectorUtils3.c GL_utilities.c MicroGlut.c -std=c99 -lGL -lglut -lX11 -lm -o lab2-7



clean :
	rm -f lab2-1
	rm -f lab2-2
	rm -f lab2-3
	rm -f lab2-4
	rm -f lab2-5
	rm -f lab2-7
