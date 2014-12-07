default: clean build

build: Brick.o Block.o
	g++ -o Tetris main.cpp Brick.o Block.o -lncurses -lrt

Brick.o: Brick.cpp Brick.h
	g++ -c Brick.cpp

Block.o: Block.cpp Block.h
	g++ -c Block.cpp

clean: robjs
	rm -f Tetris

robjs:
	rm *.o
