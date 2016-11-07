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
using namespace std;

void line_cleaner(string &line) {
    unsigned int size = line.size();
    for(unsigned int i = 0; i < size; i++) {
        switch(line[i]) {
            case '.': case '?': case '!':
            case ',': case ':': case ';':
            case '-': case '(': case ')':
            case '@': case '#': case '$':
            case '%': case '^': case '&':
            case '*': case '_': case '+':
            case '=': case '[': case ']':
            case '{': case '}': case '"':
            case '\'': case '/': case '\\':
            case '<': case '>': case '|':
            case '~': case '1': case '2': case '3':
            case '4': case '5': case '6': case '7':
            case '8': case '9': case '0': {
                line[i] = ' ';
            }
        }
        if(line[i] == -62) {
            if(line.size() > i+1 && line[i+1] == -80) {
                line[i] = ' ';
                line[i+1] = ' ';
            }
        }

        if(line[i] == -30) {
            if(line.size() > i+1 && line[i+1] == -128) {
                if(line.size() > i+2 && (line[i+2] == -109 || line[i+2] == -98 || line[i+2] == -99 || line[i+2] == -100 || line[i+2] == -90)) {
                    line[i] = ' ';
                    line[i+1] = ' ';
                    line[i+2] = ' ';
                }
            }
        }

        if(line[i] == -59 && i+1 < line.size()) {
            if(line[i+1] == -69) line[i+1] = -68;
            if(line[i+1] == -71) line[i+1] = -70;
            if(line[i+1] == -102) line[i+1] = -101;
            if(line[i+1] == -125) line[i+1] = -124;
            if(line[i+1] == -127) line[i+1] = -126;
        }
        if(line[i] == -60 && i+1 < line.size()) {
            if(line[i+1] == -104) line[i+1] = -103;
            if(line[i+1] == -122) line[i+1] = -121;
            if(line[i+1] == -124) line[i+1] = -123;
        }
        if(line[i] == -61 && i+1 < line.size()) {
            if(line[i+1] == -109) line[i+1] = -77;
        }
    }
    transform(line.begin(), line.end(), line.begin(), ::tolower);
}

unordered_map<string, unsigned int> get_words_map_sync (const char* path) {
    ifstream in(path);
    unordered_map<string, unsigned int> words;
    string line;
    string word;
    int line_num = 0;
    stringstream line_stream;
    // Opening file
    if(!in.is_open()) {
        cout << "Błąd otwarcia pliku " << path << endl;
        exit(0);
    }
    while(!in.eof()) {
        getline(in, line);
        line_cleaner(line);
        line_stream.str( string() );
        line_stream.clear();
        line_stream << line;
        while(!line_stream.eof()) {
            line_stream >> word;
            words.emplace(word, line_num);
        }
        line_num++;
    }
    return words;
}

unordered_map<string, unsigned int> get_words_map (const char* path) {
    ifstream in(path);
    unordered_map<string, unsigned int> words;
    string line;
    string word;
    int line_num = 0;
    // Opening file
    if(!in.is_open()) {
        cout << "Błąd otwarcia pliku " << path << endl;
        exit(0);
    }
    // Reading line in parallel environment
    #pragma omp parallel default(none) shared(cout, in, words) private(line, word, line_num)
    {
        #pragma omp single
        {
            while(!in.eof()) {
                getline(in, line);
                #pragma omp task
                {
                    line_cleaner(line);
                    stringstream line_stream(line);
                    while(!line_stream.eof()) {
                        line_stream >> word;

                        #pragma omp critical
                        {
                            words.emplace(word, line_num);
                        }
                    }
                }
                line_num++;
            }
        }
    }
    return words;
}

void get_articles_vectors(vector<vector<string> > &articles, vector<string> &tags, const char* path) {
    ifstream in(path);
    string line;
    string word;
    int line_num = 0;
    int size;
    vector<string> vec;
    if(!in.is_open()) {
        cout << "Błąd otwarcia pliku " << path << endl;
        exit(0);
    }
    in >> size;
    for(int i = 0; i < size; i++) {
        articles.push_back(vec);
        tags.push_back("");
    }
    #pragma omp parallel default(none) shared(cout, in, articles, tags) private(line, word) firstprivate(line_num)
    {
        #pragma omp single
        {
            while(!in.eof()){
                getline(in, line);
                if(line != "") {
                    #pragma omp task
                    {
                        int i = 0;
                        line_cleaner(line);
                        stringstream line_stream(line);
                        while(!line_stream.eof()) {
                            line_stream >> word;
                            if(word.size() > 3) {
                                #pragma omp critical
                                {
                                    if(i == 0) {
                                        tags.at(line_num) = word;
                                    }
                                    else {
                                        articles.at(line_num).emplace_back(word);
                                    }
                                }
                                i++;
                            }
                        }
                    }
                    line_num++;
                }
            }
        }
    }
}

void get_ready_articles(vector<vector<string> > &articles_w,
                        vector<string> &tags,
                        unordered_map<string, unsigned int> &words_map,
                        vector<vector<bool> > &articles_b,
                        vector<bool> &tags_b,
                        unordered_map<unsigned int, unsigned int> &needed_classes,
                        string master_tag) {
    // Torzenie needed_words <- zamiana słów z articles_w w klasy i ile klas tyle needed_classes.size()
    // Iteruje po articles_w -> biore klase z words_map -> wsadzam z nowym id do needed_classes
    int needed_it = 0; // needed iterations
    for(auto vec : articles_w) {
        for(auto word : vec) {
            auto it_words = words_map.find(word);
            if(it_words != words_map.end()) {
                auto it_needed = needed_classes.emplace(it_words->second, needed_it);
                if(it_needed.second) {
                    needed_it++;
                }
            }
        }
    }
    // Tworze vector articles_b, rozmiar tyle ile articles_w + ilosc slow tyle co needed_classes, jesli klasa wystepuje to 1 jesli nie to 0
    unsigned int class_numb;
    unsigned int position;
    vector<bool> temp(needed_it, false);
    for(unsigned int i = 0; i < articles_w.size(); i++) {
         articles_b.push_back(temp);
         for(unsigned int j = 0; j < articles_w[i].size(); j++) {
             auto iterator = words_map.find(articles_w[i][j]);
             if(iterator != words_map.end()) {
                 class_numb = iterator->second;
                 position = needed_classes.at(class_numb);
                 articles_b[i][position] = true;
             }
         }
    }
    // Tworzenie vectora tags_b, rozmiar taki jak tags, wartości tags zamienione na 0/1 - 1 gdy tags == master_tag
    for(unsigned int i = 0; i < tags.size(); i++) {
        if(tags[i] == master_tag) {
            tags_b.push_back(true);
        } else {
            tags_b.push_back(false);
        }
    }

}

vector<vector<double> > convert_vvb_to_vvd(vector<vector<bool> > vvb) {
    vector<vector<double> > vvd;
    vector<double> vd;
    for(unsigned int i = 0; i < vvb.size(); i++) {
        vvd.push_back(vd);
        for(unsigned int j = 0; j < vvb[i].size(); j++) {
            vvd[i].push_back(vvb[i][j] ? 1.0 : 0.0);
        }
    }
    return vvd;
}

vector<vector<double> > convert_vb_to_vvd(vector<bool> vb) {
    vector<vector<double> > vvd;
    vector<double> vd;
    vd.push_back(0);
    for(unsigned int i = 0; i < vb.size(); i++) {
        vd[0] = vb[i] ? 1.0 : -1.0;
        vvd.push_back(vd);
    }
    return vvd;
}


vector<double> convert_one_article(string article, unordered_map<string, unsigned int> &words_map, unordered_map<unsigned int, unsigned int> &needed_classes) {
    vector<double> new_article(needed_classes.size(), 0);
    string word;
    line_cleaner(article);
    stringstream line_stream(article);
    while(!line_stream.eof()) {
        line_stream >> word;
        auto iterator = words_map.find(word);
        if(iterator != words_map.end()) {
            auto it = needed_classes.find(iterator->second);
            if(it != needed_classes.end()) {
                new_article[it->second] = 1;
            }
        }
    }
    return new_article;
}
