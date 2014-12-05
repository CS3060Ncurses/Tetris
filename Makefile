default: clean build

build:
	g++ -o Tetris main.cpp -lncurses -lrt

clean:
	rm -f Tetris
