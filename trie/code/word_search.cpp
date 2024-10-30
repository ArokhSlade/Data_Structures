#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cctype>
#include <vector>
#include "profileapi.h"

using namespace std;

struct Trie{

    char prefix;
    bool is_word = false;
    vector<Trie*> children;

    Trie * add_word(string& prefix, string& word);
    void list_words_with_prefix(string& prefix);
    Trie * get_child(char next_char);

    string& get_first_word(string& dynamic_result) {
        Trie * first_child = children.front();
        if (first_child) {
            dynamic_result += prefix;
            first_child->get_first_word(dynamic_result);
        }
        return dynamic_result;
    }

    void print_all(int *word_count = nullptr, bool from_root = false, const string& start_value = "");

    Trie * get_prefix_node(string& prefix);

};


Trie * Trie::get_child(char next_char) {

    for (auto child : children) {
        if (child->prefix == next_char) {
            return child;
        }
    }
    return nullptr;
}


Trie * Trie::add_word(string& prefix, string& word) {

    Trie *added = this;

    //NOTE empty word usually means end of recursion
    //TODO special case when actually adding empty word to root -> not a word
    if (word.size() == 0) {
        is_word = true;
        return added;
    }


    char next_char = word[0];
    Trie *child = get_child(next_char);

    if (!child) {
        child = new Trie{};
        child->prefix = next_char;
        children.push_back(child);
    }

    //TODO: use string view instead of copy for substring
    string suffix = word.substr(1, word.size()-1);
    prefix += next_char;
    added = child->add_word(prefix, suffix);

    return added;
}


void pop_front(string& word) {
    if (word.size() == 0) {
        return;
    } else if (word.size() == 1) {
        word = "";
    } else {
        word = word.substr(1,word.size()-1);
    }

    return;
}


void pop_back(string& word) {
    word = word.substr(0, word.size()-1);
    return;
}


Trie * Trie::get_prefix_node(string& prefix_) {
    Trie * result = nullptr;

    if (prefix_.size() == 0) {
        return this;
    }

    for (auto& child : children) {
        if (child->prefix == prefix_[0]) {
            pop_front(prefix_);
            result = child -> get_prefix_node(prefix_);
            break;
        }
    }

    return result;
}


void append(string& word, char character) {
    if (character) word += character;
    return;
}


void Trie::print_all(int *word_count, bool from_root, const string& start_value) {
    // pre order traversal with marker_checks
    static string path_word = ""; //the path from root to this node.

    if (from_root) {
        path_word = start_value;  //reset static variable when user calls it on whatever they consider root of trie
        if (word_count) *word_count = 0;
    }

    append(path_word, prefix);
    if (is_word) {
        cout << path_word << "\n";
        if (word_count) ++*word_count;
    }
    for (auto child : children) {
        child->print_all(word_count); //nested calls work with the static variable in its current state
    }
    path_word = path_word.substr(0,path_word.size()-1);
    return;
}


int print_words_for_prefix(Trie& trie, string& prefix) {
    int word_count = 0;
    string backup = prefix;
    pop_back(backup); //the prefix node will append its own prefix-char, which is the last char of our search-prefix
    Trie *prefix_node = trie.get_prefix_node(prefix);
    if (prefix_node) prefix_node->print_all(&word_count, true, backup);
    return word_count;
}


Trie make_trie(istream& input) {
    string word{};
    Trie trie{};
    string temp_prefix = "";
    while (input >> word) {
        temp_prefix = "";
        trie.add_word(temp_prefix,word);
    }
    return trie;
}


//truncates the word before the first non-letter and lower-cases it
void clean_up(string& word) {
    int len = word.size();
    for (int i = 0 ; i < len ; ++i) {
        if (!isalpha(word[i])) {
            word = word.substr(0,i);
            break;
        }
        word[i] = tolower(word[i]);
    }
    return;
}


stringstream get_clean_text(string& filename) {
    ifstream ifs{filename};
    stringstream result{};
    string temp;
    while (ifs) {
        ifs >> temp;
        clean_up(temp);
        result << temp << '\n';
    }
    return result;
}


int main (int args_count, char *args[]) {
    cout << "arguments provided: " << args_count - 1 << "\n";
    string filename = "../../data/war_and_peace_ansi.txt";
    string search_prefix = "";
    if (args_count > 3) {
        cout << "usage  : Word_Search.exe <file name> <search prefix>\n";
        cout << "example: Word_Search.exe my_text.xt \"\"\n";
        cout << "only ascii files supported\n";
        return 1;
    }
    if (args_count >= 2) {
        filename = args[1];
    }
    if (args_count >= 3) {
        search_prefix = args[2];
    }
    cout << "filename      : " << filename << "\n";
    cout << "search_prefix : " << search_prefix << "\n";
    cout << "\n";

    stringstream cleaned_input = get_clean_text(filename);

    unsigned long long t0,t1,t2,delta1,delta2,freq;
    LARGE_INTEGER li_t0,li_t1,li_t2,li_delta1,li_delta2,li_freq;
    QueryPerformanceFrequency(&li_freq);
    freq = li_freq.QuadPart;


    QueryPerformanceCounter(&li_t0);
    t0 = li_t0.QuadPart;

    Trie trie = make_trie(cleaned_input);

    QueryPerformanceCounter(&li_t1);
    t1 = li_t1.QuadPart;

    int word_count = print_words_for_prefix(trie, search_prefix);

    QueryPerformanceCounter(&li_t2);
    t2 = li_t2.QuadPart;

    delta1 = t1-t0;
    delta2 = t2-t1;

    double time1,time2;
    time1 = delta1 / double(freq);
    time2 = delta2 / double(freq);

    cout << "\n";
    cout << "time for make_trie(): " << time1 * 1000.f << " ms\n";
    cout << "time for print_words_for_prefix(): " << time2 * 1000.f<< " ms\n";
    cout << "total time: " << (time1+time2) * 1000.f << " ms\n";
    cout << "\n";

    cout << "words counted with prefix: " << word_count << "\n";
    cout << "\n";

    return 0;
}
