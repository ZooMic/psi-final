#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "Net.hpp"
using namespace std;

class sfLine : public sf::Drawable
{
    public:
        sfLine(const sf::Vector2f& point1, const sf::Vector2f& point2, float thick, sf::Color c)
        {
            thickness = thick;
            color = c;
            sf::Vector2f direction = point2 - point1;
            sf::Vector2f unitDirection = direction/std::sqrt(direction.x*direction.x+direction.y*direction.y);
            sf::Vector2f unitPerpendicular(-unitDirection.y,unitDirection.x);

            sf::Vector2f offset = (thickness/2.f)*unitPerpendicular;

            vertices[0].position = point1 + offset;
            vertices[1].position = point2 + offset;
            vertices[2].position = point2 - offset;
            vertices[3].position = point1 - offset;

            for (int i=0; i<4; ++i)
                vertices[i].color = color;
        }

        void draw(sf::RenderTarget &target, sf::RenderStates states) const
        {
            target.draw(vertices,4,sf::Quads);
        }

        void setThickness(float t) {
            thickness = t;
        }
        float thickness;
    private:
        sf::Vertex vertices[4];

        sf::Color color;
};

void test() {
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    sf::CircleShape shape(10);
    //shape.setPosition(150,50);
    shape.setFillColor(sf::Color::Green);
    int pos_w;
    int pos_h;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        pos_w = 0;
        pos_h = 0;
        for(int i = 0; i < 10; i++) {
            shape.setPosition(pos_w, pos_h);
            pos_w += 10;
            pos_h += 10;
            window.draw(shape);
        }
        window.display();
    }
}

vector<vector<vector<double> > > find_elements_positions(int width,int height, vector<vector<vector<double> > > weights) {
    vector<vector<vector<double> > > elements_pos;
    vector<vector<double> > vvi;
    vector<double> vi;
    //int w, h, t_w, t_h, sum;
    int sum;
    double w, h, t_w, t_h;
    w = (width / (double)(weights.size()+1));
    sum = 0;
    elements_pos.push_back(vvi);
    for(unsigned int j = 0; j < weights[0].size(); j++) {
        sum += weights[0][j].size();
    }
    sum = sum < 60 ? sum : 60;
    //cout << sum << endl;
    h = (height / (double)sum);
    t_h = h / 2.0;
    t_w = w / 2.0;
    for(int j = 0; j < sum; j++) {
        elements_pos[0].push_back(vi);
        elements_pos[0][j].push_back(t_w);
        elements_pos[0][j].push_back(t_h - 5);
        elements_pos[0][j].push_back(5);
        t_h += h;
    }
    t_w += w;
    for(unsigned int i = 0; i < weights.size(); i++) {
        elements_pos.push_back(vvi);
        sum = weights[i].size();
        // for(int j = 0; j < weights[i].size(); j++) {
        //     sum += weights[i].size();
        // }

        //sum = sum < 30 ? sum : 30;
        h = (height /(double) sum);
        t_h = h / 2;
        for(int j = 0; j < sum; j++) {
            elements_pos[i+1].push_back(vi);
            elements_pos[i+1][j].push_back(t_w);
            elements_pos[i+1][j].push_back(t_h - 30);
            elements_pos[i+1][j].push_back(30);
            t_h += h;
        }
        t_w += w;
    }
    return elements_pos;
}


void draw_net_state(Net *net) {
    //vector<vector<vector<double> > > weights = net->weights;
    int width = 1200;
    int height = 900;
    double max = 0;
    double max_ave = 0;
    vector<vector<vector<double> > > elements_pos = find_elements_positions(width, height, net->weights);
    sf::RenderWindow window(sf::VideoMode(width, height), "Neural Network State");
    sf::RenderStates states;
    sf::CircleShape shape(10);
    sfLine line(sf::Vector2f(0,0),sf::Vector2f(1,1),5.f,sf::Color::Yellow);
    shape.setFillColor(sf::Color::Green);
    double weight;
    sf::Color color;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        for(unsigned int i = 1; i < elements_pos.size(); i++) {
            for(unsigned int j = 0; j < elements_pos[i].size(); j++) {
                for(unsigned int k = 0; k < elements_pos[i-1].size(); k++) {
                    //cout << "weights " << weights[i-1][k][0] << endl;
                    if((net->weights[i-1][j][k] >= 0 ? net->weights[i-1][j][k] : -1 * net->weights[i-1][j][k]) > max) {
                        max = (net->weights[i-1][j][k] >= 0 ? net->weights[i-1][j][k] : -1 * net->weights[i-1][j][k]);
                    }
                    if(net->weights[i-1][j][k]*5 / max > 0.1) {
                        if(net->weights[i-1][j][k]*5 / max > 1) {
                            weight = net->weights[i-1][j][k]*5 / max;
                        } else {
                            weight = 1;
                        }
                        color = sf::Color::Blue;
                    } else if(net->weights[i-1][j][k]*5 / max < -0.1) {
                        if(net->weights[i-1][j][k]*5 / max < -1) {
                            weight = net->weights[i-1][j][k]*5 * -1 / max;
                        } else {
                            weight = 1;
                        }
                        color = sf::Color::Red;
                    } else {
                        weight = 1;
                        color = sf::Color(210,210,210); //greay
                    }
                    line = sfLine(sf::Vector2f(elements_pos[i-1][k][0]+(elements_pos[i-1][k][2]),elements_pos[i-1][k][1]+(elements_pos[i-1][k][2])),
                                  sf::Vector2f(elements_pos[i][j][0]+(elements_pos[i][j][2]),elements_pos[i][j][1]+(elements_pos[i][j][2])),
                                  weight, color);
                                  //cout << "Weights " <<  (net->weights[i-1][j][k] >= 0 ? net->weights[i-1][j][k] : 0) << endl;
                    line.draw(window, states);
                }
            }
        }
        for(unsigned int i = 0; i < elements_pos.size(); i++) {
            for(unsigned int j = 0; j < elements_pos[i].size(); j++) {
                shape.setPosition((float)elements_pos[i][j][0], (float)elements_pos[i][j][1]);
                shape.setRadius((float)elements_pos[i][j][2]);
                // line.draw(window, states);
                // line = sfLine(sf::Vector2f(300,300),sf::Vector2f(600,600));
                // line.draw(window, states);
                //shape.setPosition(600, 450);
                window.draw(shape);
            }
        }


        window.display();
    }
}

void draw_net_chart(Net *net) {
    float width = 800;
    float height = 600;
    double jh;
    double jw;
    double max = 0;
    sf::RenderWindow window(sf::VideoMode(width, height), "Neural Network Chart");
    sf::RenderStates states;
    sf::CircleShape shape(2);

    while (window.isOpen())
    {
        max = 0;
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        for(unsigned int i = 0; i < net->errors.size(); i++) {
            if(max < net->errors[i]) max = net->errors[i];
        }
        jh = height / max;
        jw = width / net->errors.size();
        for(unsigned int i = 0; i < net->errors.size(); i++) {
            shape.setPosition((float)i*jw,height - (float)net->errors[i]*jh);
            window.draw(shape);
        }
        window.display();
    }
}
