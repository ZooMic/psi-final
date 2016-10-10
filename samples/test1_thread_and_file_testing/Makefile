
LIB = -L/usr/lib/mysql -lmysqlclient -lpthread
OPT = -std=c++11

run: clear
	./main
clear: main
	rm -rf *.o
main: main.o
	g++ main.o -o main $(LIB)
main.o: main.cpp
	g++ -c main.cpp $(OPT) -o main.o
