main: main.cpp primitives.o
	g++ -I src/include -L src/lib -o main main.cpp primitives.o -lmingw32 -lSDL2main -lSDL2
primitives.o: primitives.cpp
	g++ -I src/include -L src/lib -c -o primitives.o primitives.cpp -lmingw32 -lSDL2main -lSDL2