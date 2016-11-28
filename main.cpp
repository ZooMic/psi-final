#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <omp.h>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>
#include <set>
#include <fstream>

#include "modules/formatter.hpp"
#include "modules/Net.hpp"
#include "modules/drawer.hpp"

using namespace std;

int main() {
    //test();
    // Wczytywanie potrzebnych danych
    //      vector booli artykulów i tagów (dane i wyniki) do uczenia
    //      needed_classes do stworzenia warstwy wejściowej danych z size() pozycja odpowiednich slow w celu wyświetlenia graficznego
    //      words_map do odniesienia wartości z needed_classes do konkretnej klasy słów
    unordered_map<string, unsigned int> words_map = get_words_map_sync();
    cout << "Pobranie mapy słów." << endl;
    vector<vector<string> > articles_w;
    vector<string> tags;
    vector<bool> tags_b;
    vector<vector<bool> > articles_b;
    unordered_map<unsigned int, unsigned int> needed_classes;
    get_articles_vectors(articles_w, tags);
    cout << "Utworzenie vektora artykułów." << endl;
    get_ready_articles(articles_w, tags, words_map, articles_b, tags_b, needed_classes);
    cout << "Konwersja artykułów na potrzeby sieci." << endl;
    // Tworzenie sieci neuronowej na podstawie zgromadzonych danych
    //      konwersja z boolean na double (uniwersalność)
    //      tworzenie obiektu Net
    vector<vector<double> > articles_d = convert_vvb_to_vvd(articles_b);
    vector<vector<double> > tags_d = convert_vb_to_vvd(tags_b);
    Net *net = new Net(articles_d, tags_d);
    ifstream ask_neuron;
    string temp;
    string art;
    int answer;
    vector<double> article;
    vector<double> ans;
    while(1) {
        cout << "#####################################################################" << endl;
        cout << "# Sieci Neuronowe i artykuły                                        #" << endl;
        cout << "#     Utworz sieć z pliku (src/constructor_weights_snapshot)    [1] #" << endl;
        cout << "#     Ucz sieć                                                  [2] #" << endl;
        cout << "#     Wczytaj nauczoną sieć (src/weights_snapshot)              [3] #" << endl;
        cout << "#     Pytaj sieć z pliku (src/ask_neuron_net)                   [4] #" << endl;
        cout << "#     Pokaż sieć                                                [6] #" << endl;
        cout << "#     Wyjscie                                                   [0] #" << endl;
        cout << "#####################################################################" << endl;
        cin >> answer;
        if(answer == 1) {
            net = new Net(articles_d, tags_d, "src/constructor_weights_snapshot.txt", "src/net_layers.txt");
        }
        else if(answer == 2) {
            #pragma omp parallel sections shared(net)
            {
                #pragma omp section
                {
                    net->start_learning(1000);
                }
                #pragma omp section
                {
                    draw_net_state(net);
                }
            }
        }
        else if(answer == 3) {
            net = new Net(articles_d, tags_d, "src/weights_snapshot.txt", "src/net_layers.txt");
        }
        else if(answer == 4) {
            ask_neuron.open("src/ask_neuron_net.txt");
            article.clear();
            ans.clear();
            art = "";
            while(!ask_neuron.eof()) {
                temp = "";
                ask_neuron >> temp;
                cout << temp << " ";
                art += temp + " ";
            }
            cout << endl;
            article = convert_one_article(art, words_map, needed_classes);
            ans = net->answer(article);
            cout << "Odpowiedź to: ";
            for(unsigned int i = 0; i < ans.size(); i++) {
                cout << ans[i] << endl;
            }
            ask_neuron.close();
        }
        else if(answer == 5) break;
        else if(answer == 6) {
            draw_net_state(net);
        }
        else if(answer == 0) break;
        else cout << "Nie ma takiej opcji, sprobuj jeszcze raz." << endl;
    }

    //draw_net_chart(net);
    return 0;
}
