#include <iostream>
#include <fstream>
#include <string>
#include <thread>
using namespace std;

struct lineRange {
    int from;
    int to;
};

lineRange* countNeededThreads(int &n) {
    ifstream in("../../materialy/dane/odmiany.txt");
    int x = 0;
    int l = 1000;
    int t;
    lineRange *lr = NULL;
    string line;
    while(getline (in,line)) {
        x++;
    }
    cout << "Line number: " << x << endl;
    t = x / l;
    cout << "Thread number: " << t+1 << endl;
    lr = new lineRange[t+1];
    for(int i = 0; i < t; i++) {
        lr[i].from = i*l + 1;
        lr[i].to = (i+1)*l;
    }
    lr[t].from = t*l + 1;
    lr[t].to = x;
    n = t+1;
    return lr;
}
void createMultiWordQuerry(string word, int klasa, char *&querry, int &size) {
    string k;
    int nsize;
    int osize = size;
    string q;
    char *old;
    sprintf((char *)k.c_str(), "%d", klasa);
    if(size == 0) {
        q = "INSERT INTO words (word, klasa) VALUES ('"+word+"',"+k.c_str()+")";
        nsize = q.size()+1;
    } else {
        q = ", ('"+word+"',"+k.c_str()+")";
        nsize = q.size()+1;
    }
    old = querry;
    size += nsize;
    int l = 0;
    querry = new char[size];
    for(int i = 0; i < size; i++) {
        if(i < osize) {
            querry[i] = old[i];
        } else {
            querry[i] = q[l++];
        }
    }
}
void importWordToDb(lineRange lr) {
    int x = 1;
    string line;
    ifstream in("../../materialy/dane/odmiany.txt");
    if(in.is_open()) {
        while(x <= lr.to) {
            getline (in,line);
            if(x >= lr.from && x <= lr.to) {
                cout << line << endl;
            }
            x++;
        }
        in.close();
    } else {
        cout << "Something went wrong while opening file" << x << endl;
    }
}
void fireThreads() {
    int n;
    lineRange* lr = countNeededThreads(n);
    thread ** t = new thread*[n];
    for(int i = 0; i < n; i++) {
        t[i] = new thread(importWordToDb, lr[i]);
    }
    for(int i = 0; i < n; i++) {
        t[i]->join();
    }
}
int main() {
    string a = "Stół z powyłamywanymi";
    string b = " nogami!";
    cout << a << a.size() << endl;
    cout << b << b.size() << endl;

    return 0;
}
