all:
	g++ -I src/include *.h -L src/lib -o main *.cpp -lmingw32 -lSDL2main -lSDL2