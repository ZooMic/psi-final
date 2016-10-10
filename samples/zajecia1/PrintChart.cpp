#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include "PrintChart.h"
void printChart(double x, double y, int color) {
    sf::RenderWindow window(sf::VideoMode(500, 500), "SFML works!");
    sf::CircleShape point(1);
    switch(color) {
        case 0:
            point.setFillColor(sf::Color(0,0,255));
            break;
        case 1:
            point.setFillColor(sf::Color(255, 0, 0));
            break;
    }
    point.setPosition((float)x,(float)y);
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(point);
        window.display();
    }
}
