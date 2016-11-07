#include <unordered_map>
#include <vector>
#include <string>
using namespace std;
unordered_map<string, unsigned int> get_words_map_sync (const char* path = "src/odmiany.txt");
unordered_map<string, unsigned int> get_words_map (const char* path = "src/odmiany.txt");
void get_articles_vectors(vector<vector<string> > &articles, vector<string> &tags, const char* path = "src/artykuly.txt");
void get_ready_articles(vector<vector<string> > &articles_w,
                        vector<string> &tags,
                        unordered_map<string, unsigned int> &words_map,
                        vector<vector<bool> > &articles_b,
                        vector<bool> &tags_b,
                        unordered_map<unsigned int, unsigned int> &needed_classes,
                        string master_tag = "informatyka");
vector<vector<double> > convert_vvb_to_vvd(vector<vector<bool> > vvb);
vector<vector<double> > convert_vb_to_vvd(vector<bool> vb);
vector<double> convert_one_article(string article, unordered_map<string, unsigned int> &words_map,
                         unordered_map<unsigned int,
                         unsigned int> &needed_classes);
