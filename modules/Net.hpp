#ifndef NET_H
#define NET_H
#include <vector>
using namespace std;
class Net {
public:
    Net(vector<vector<double> > learning_data, vector<vector<double> > targets, const char* path = "src/net_layers.txt");
    Net(vector<vector<double> > learning_data, vector<vector<double> > targets, const char* path_weights,  const char* path);
    void show();
    void start_learning(unsigned int epoch_number, double expected_error = 0.05);
    vector<vector<vector<double> > > weights;
    int epoch_counter;
    double one_epoch_error;
    vector<double> errors;
    vector<double> answer(vector<double> ask);
private:
    vector<vector<vector<double> > > inputs;
    vector<vector<double> > learning_data;
    vector<vector<double> > outputs;
    vector<vector<double> > bias_weights;
    vector<vector<double> > targets;
    vector<vector<double> > deltas;
    double learning_rate;
    vector<double> count_one_set();
    void set_input(unsigned int iteration_number);
    void back_propagation(unsigned int iteration_number);
    void repair_weights();
    void make_one_epoch();

    void take_weights_snapshot(const char* path = "src/weights_snapshot.txt");

};
#endif
