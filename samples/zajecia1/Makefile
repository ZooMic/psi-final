run: clear
clear: run-program
	rm -rf *.o main
run-program: main
	./main
main: main.o perceptron.o printchart.o
	g++ main.o Perceptron.o PrintChart.o -o main -lsfml-graphics -lsfml-window -lsfml-system
main.o: perceptron.o
	g++ -c main.cpp -o main.o
perceptron.o:
	g++ -c Perceptron.cpp -o Perceptron.o
printchart.o:
	g++ -c PrintChart.cpp -o PrintChart.o -lsfml-graphics -lsfml-window -lsfml-system
