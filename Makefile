default: build

build: Block.o
	g++ -o Tetris main.cpp Block.o -lncurses -lrt

Block.o: Block.cpp Block.h
	g++ -c Block.cpp

clean: robjs
	rm -f Tetris

robjs:
	rm *.o
