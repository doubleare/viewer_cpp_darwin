FL = -std=c++17 -Wall -Wextra -Werror 
SRC = ./code/3DViewer.cc ./code/AdapterModel.cc ./code/Controller.cc ./code/Memento.cc

all: tests

install:
	@mkdir build && cd build && qmake ../3D_v2 && make && make clean && rm Makefile 
open:
	@./build/3D_v2.app/Contents/MacOS/3D_v2

tests: clean
	@g++ $(FL) $(SRC) -lgtest tests.cc -o tests
	@./tests

uninstall:
	@rm -rf build

dvi:
	@open manual.tex

clean:
	@rm -rf *gcda *gcno Clone.tgz tests

leaks: clean tests
	@CK_FORK=no leaks --atExit -- ./tests

dist: clean
	@mkdir Clone/
	@mkdir Clone/src
	@cp -R build/3D_v2.app Clone/src/3D_v2.app
	@tar cvzf Clone.tgz Clone
	@rm -rf Clone
