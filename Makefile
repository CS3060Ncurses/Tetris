default: clean build

build:
	g++ -o Tetris main.cpp -lncurses

clean:
	rm -f Tetris
