#include <iostream>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include "Perceptron.h"
#include "PrintChart.h"
using namespace std;

double ** funXOR() {
    double ** d = new double*[4];
    for(int i = 0; i < 4; i++) {
        d[i] = new double[3];
    }
    d[0][0] = 0.0;d[0][1] = 0.0;d[0][2] = 0;
    d[1][0] = 0.0;d[1][1] = 1.0;d[1][2] = 1;
    d[2][0] = 1.0;d[2][1] = 0.0;d[2][2] = 1;
    d[3][0] = 1.0;d[3][1] = 1.0;d[3][2] = 0;
    return d;
}
double ** fun(int n) {
    double **d = new double*[n];
    for(int i = 0; i < n ; i++) {
        d[i] = new double[2];
        d[i][0] = ((double)rand() / (RAND_MAX)) * n;
        d[i][1] = (d[i][0] >= n/2) ? 1 : 0;
    }
    return d;
}
double** fun2(int n) {
    double **d = new double*[n];
    for(int i = 0; i < n; i++) {
        d[i] = new double[3];
        d[i][0] = ((double)rand()/(RAND_MAX)) * 10;
        d[i][1] = ((double)rand()/(RAND_MAX)) * 10;
        d[i][2] = (d[i][1] - (1.5 * d[i][0]) >= 0) ? 1 : 0;
    }
    return d;
}

void fireProgram() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Neuron chart");
    sf::CircleShape shape(5);
    shape.setFillColor(sf::Color::Green);
    window.clear();

    //LOGIKA
    srand (time(NULL));
    int size = 100;
    float wX = 800 / 10.0;
    float wY = 600 / 10.0;
    double** dane = fun2(size);
    Perceptron *p = new Perceptron(2);
    p->trenujNeutron(dane,size);
    cout << "Możesz mnie pytać!" << endl;
    double p1;
    double p2;

    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }
        for(int i = 0; i < size ; i++){
            shape.setPosition((float)dane[i][0]*wX,-(float)dane[i][1]*wY + 600);
            if(dane[i][2] == 0) {
                shape.setFillColor(sf::Color::Red);
            } else {
                shape.setFillColor(sf::Color::Blue);
            }
            window.draw(shape);
            window.display();
        }
        for(double i = 0; i < 10; i+=0.1) {
            shape.setPosition((float)i*wX,-(float)(1.5*i)*wY + 600);
            shape.setFillColor(sf::Color::White);
            window.draw(shape);
            window.display();
        }
        for(int i = 0; i < 100; i++) {
            cout << "X: "; cin >> p1;
            cout << "Y: "; cin >> p2;
            shape.setPosition((float)p1*wX,-(float)p2*wY + 600);
            if(p->odpowiedz2(p1,p2) == 0) {
                shape.setFillColor(sf::Color::Yellow);
            } else {
                shape.setFillColor(sf::Color::Green);
            }
            window.draw(shape);
            window.display();
        }
        //
        //window.draw(shape);
        window.display();
    }
}

int main()
{
    fireProgram();

    // srand (time(NULL));
    // double** dane = fun2(10);
    // Perceptron *p = new Perceptron(2);
    // p->trenujNeutron(dane,10);
    // cout << "[Perceptron]: Teraz możesz mnie przepytać!" << endl;
    //  double p1;
    //  double p2;
    //  for(int i = 0; i < 10; i++) {
    //      cin >> p1;
    //      cin >> p2;
    //      p->odpowiedz2(p1,p2);
    //  }

    return 0;
}
