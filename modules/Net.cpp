#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <omp.h>
#include <unordered_map>
#include <vector>
#include <string>
#include <set>
#include <sstream>
#include <algorithm>
#include <exception>
#include <math.h>
#include "Net.hpp"
using namespace std;

Net::Net(vector<vector<double> > learning_data, vector<vector<double> > targets, const char* path) {
    srand(time(NULL));
    learning_rate = 0.5;
    epoch_counter = 0;
    this->learning_data = learning_data;
    this->targets = targets;
    ifstream in(path);
    if(learning_data.size() == 0) {
        cout << "Error (line 20): learning_data.size() == 0" << endl;
        exit(0);
    }
    int input_size = learning_data[0].size();
    int output_size;
    vector<vector<double> > vvd_temp;
    vector<double> vd_temp;
    in >> output_size;

    weights.push_back(vvd_temp);
    inputs.push_back(vvd_temp);
    outputs.push_back(vd_temp);
    bias_weights.push_back(vd_temp);
    for(int i = 0; i < output_size; i++) {
        inputs[0].push_back(vd_temp);
        weights[0].push_back(vd_temp);
        outputs[0].push_back(0);
        bias_weights[0].push_back(((double)rand()/(double)RAND_MAX*0.4 + 0.2)*(-1 * (rand()%2 ? 1 : -1)));
        for(int j = 0; j < input_size; j++) {
            inputs[0][i].push_back(0);
            weights[0][i].push_back(((double)rand()/(double)RAND_MAX*0.4 + 0.2)*(-1 * (rand()%2 ? 1 : -1)));
        }
    }
    for(int i = 1; !in.eof(); i++) {
        in >> output_size;
        if(in.eof()) break;
        input_size = outputs[i - 1].size();
        inputs.push_back(vvd_temp);
        weights.push_back(vvd_temp);
        outputs.push_back(vd_temp);
        bias_weights.push_back(vd_temp);
        for(int j = 0; j < output_size; j++) {
            inputs[i].push_back(vd_temp);
            weights[i].push_back(vd_temp);
            outputs[i].push_back(0);
            bias_weights[i].push_back((((double)rand()/(double)RAND_MAX)*0.4 + 0.2)* (-1 * (rand()%2 ? 1 : -1)));
            for(int k = 0; k < input_size; k++) {
                inputs[i][j].push_back(0);
                weights[i][j].push_back(((double)rand()/(double)RAND_MAX*0.4 + 0.2)* (-1 * (rand()%2 ? 1 : -1)));
            }
        }
    }
    deltas = outputs;
    take_weights_snapshot("src/constructor_weights_snapshot.txt");
}

Net::Net(vector<vector<double> > learning_data, vector<vector<double> > targets, const char *path_weights,  const char* path) {
    learning_rate = 0.5;
    epoch_counter = 0;
    this->learning_data = learning_data;
    this->targets = targets;
    for(unsigned int i = 0; i < targets.size(); i++) {
        cout << targets[i][0] << endl;
    }
    ifstream in(path);
    ifstream weights_snap(path_weights);
    double weights_temp;
    if(learning_data.size() == 0) {
        cout << "Error (line 20): learning_data.size() == 0" << endl;
        exit(0);
    }
    int input_size = learning_data[0].size();
    int output_size;
    vector<vector<double> > vvd_temp;
    vector<double> vd_temp;
    in >> output_size;

    weights.push_back(vvd_temp);
    inputs.push_back(vvd_temp);
    outputs.push_back(vd_temp);
    bias_weights.push_back(vd_temp);
    for(int i = 0; i < output_size; i++) {
        inputs[0].push_back(vd_temp);
        weights[0].push_back(vd_temp);
        outputs[0].push_back(0);
        weights_snap >> weights_temp;
        bias_weights[0].push_back(weights_temp);
        for(int j = 0; j < input_size; j++) {
            inputs[0][i].push_back(0);
            weights_snap >> weights_temp;
            weights[0][i].push_back(weights_temp);
        }
    }
    for(int i = 1; !in.eof(); i++) {
        in >> output_size;
        if(in.eof()) break;
        input_size = outputs[i - 1].size();
        inputs.push_back(vvd_temp);
        weights.push_back(vvd_temp);
        outputs.push_back(vd_temp);
        bias_weights.push_back(vd_temp);
        for(int j = 0; j < output_size; j++) {
            inputs[i].push_back(vd_temp);
            weights[i].push_back(vd_temp);
            outputs[i].push_back(0);
            weights_snap >> weights_temp;
            bias_weights[i].push_back(weights_temp);
            for(int k = 0; k < input_size; k++) {
                inputs[i][j].push_back(0);
                weights_snap >> weights_temp;
                weights[i][j].push_back(weights_temp);
            }
        }
    }
    deltas = outputs;
}



void Net::show() {

    for(unsigned int i = 0; i < weights.size(); i++) {
        for(unsigned int j = 0; j < weights[i].size(); j++) {
            cout << weights[i][j].size() << " ";
        }
        cout << endl;
    }
}

void Net::set_input(unsigned int iteration_number) {
    for(unsigned int i = 0; i < inputs[0].size(); i++) {
        inputs[0][i] = learning_data[iteration_number];
    }
}

vector<double> Net::count_one_set() {
    for(unsigned int i = 0; i < inputs.size(); i++) {
        for(unsigned int j = 0; j < inputs[i].size(); j++) {
            outputs[i][j] = 0;
            for(unsigned int k = 0; k < inputs[i][j].size(); k++) {
                outputs[i][j] += inputs[i][j][k] * weights[i][j][k];
            }
            outputs[i][j] += bias_weights[i][j];
            outputs[i][j] = tanh(outputs[i][j]);
            if(i < inputs.size() - 1) {
                for(unsigned int h = 0; h < inputs[i+1].size(); h++) {
                    inputs[i+1][h] = outputs[i];
                }
            }
        }
    }
    return outputs[outputs.size() - 1];
}
void Net::back_propagation(unsigned int iteration_number) {
    for(unsigned int i = 0; i < deltas[deltas.size()-1].size(); i++) {
        deltas[deltas.size() - 1][i] = targets[iteration_number][i] - outputs[outputs.size() - 1][i];
    }
    for(int i = deltas.size() - 2; i >= 0; i--) {
        for(unsigned int j = 0; j < deltas[i].size(); j++) {
            deltas[i][j] = 0;
            for(unsigned int k = 0; k < deltas[i+1].size(); k++) {
                for(unsigned int g = 0; g < weights[i+1][k].size(); g++){
                    deltas[i][j] += deltas[i+1][k] * weights[i+1][k][g];
                }
            }
        }
    }
}

void Net::repair_weights() {
    double temp;
    for(unsigned int i = 0; i < weights.size(); i++) {
        for(unsigned int j = 0; j < weights[i].size(); j++) {

            for(unsigned int k = 0; k < weights[i][j].size(); k++) {
                temp = 1 - tanh(outputs[i][j]) * tanh(outputs[i][j]);
                weights[i][j][k] = weights[i][j][k] + learning_rate * deltas[i][j] * temp * inputs[i][j][k];
            }
            bias_weights[i][j] = bias_weights[i][j] + learning_rate * deltas[i][j] * temp;
        }
    }
}

void Net::make_one_epoch() {
    vector<double> answers;
    one_epoch_error = 0;
    for(unsigned int i = 0; i < learning_data.size(); i++) {
        set_input(i);
        answers = count_one_set();
        back_propagation(i);
        for(unsigned int j = 0; j < answers.size(); j++) {
            one_epoch_error += sqrt((targets[i][j] - answers[j])*(targets[i][j] - answers[j]));
        }
        repair_weights();
    }
    epoch_counter++;
    errors.push_back(one_epoch_error);
    cout << "Current error " << one_epoch_error << "(" << epoch_counter << ")" << endl;
}

void Net::take_weights_snapshot(const char* path) {
    ofstream out(path);
    for(unsigned int i = 0; i < weights.size(); i++) {
        for(unsigned int j = 0; j < weights[i].size(); j++) {
            out << bias_weights[i][j] << " ";
            for(unsigned int k = 0; k < weights[i][j].size(); k++) {
                out << weights[i][j][k] << " ";
            }
            //out << endl;
        }
    }
}

void Net::start_learning(unsigned int epoch_number, double expected_error) {
    for(unsigned int i = 0; i < epoch_number; i++) {
        make_one_epoch();
        if(expected_error >= one_epoch_error) break;
    }
    take_weights_snapshot();
    cout << "Koniec uczenia!" << endl;
}

vector<double> Net::answer(vector<double> ask) {
    for(double a : ask) cout << a << " ";
    for(unsigned int i = 0; i < inputs[0].size(); i++) {
        inputs[0][i] = ask;
    }
    return count_one_set();
}
