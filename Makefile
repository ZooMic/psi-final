
LIB = -lsfml-graphics -lsfml-window -lsfml-system
OPT = -std=c++14 -std=gnu++14 -Wall -fopenmp

run: main
	./main && rm -rf *.o
main: clear main_o formatter_o Net_o drawer_o LineShape_o
	g++  $(OPT) main.o formatter.o Net.o drawer.o LineShape.o -o main $(LIB)
main_o: main.cpp
	g++ -c $(OPT) main.cpp -o main.o $(LIB)
formatter_o:
	g++ -c $(OPT) modules/formatter.cpp -o formatter.o $(LIB)
Net_o:
	g++ -c $(OPT) modules/Net.cpp -o Net.o $(LIB)
drawer_o:
	g++ -c $(OPT) modules/drawer.cpp -o drawer.o $(LIB)
LineShape_o:
	g++ -c $(OPT) modules/LineShape.cpp -o LineShape.o $(LIB)
clear:
	rm -rf *.o
