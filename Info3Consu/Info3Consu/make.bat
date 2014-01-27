g++ -c -o main.o main.cpp -I"C:\MinGW\freeglut\include"
<<<<<<< HEAD
g++ -c -o vectormath.o vectormath.c -I"C:\MinGW\freeglut\include"
g++ -c -o texture.o texture.cpp -I"C:\MinGW\freeglut\include"
g++ -o test.exe main.o vectormath.o texture.o -L"C:\MinGW\freeglut\lib" -lfreeglut -lglu32 -lopengl32
=======
g++ -c -o vectormath.o vectormath.cpp -I"C:\MinGW\freeglut\include"
g++ -o test.exe main.o vectormath.o -L"C:\MinGW\freeglut\lib" -lfreeglut -lglu32 -lopengl32 
>>>>>>> origin/CM
