#include "../include/vocablurary.hpp"
#include "../include/memories.hpp"
#include "../include/repetitions.hpp"
#include "../include/search.hpp"
#include "../include/string_utils.hpp"
#include "../include/utils.hpp"


#include <string>
#include <vector>
#include <set>

std::vector<std::string> vocablurary_read(const std::string &filename) {
    std::ifstream file_r(filename);
    std::vector<std::string> new_vocab;
    std::string buffer;
    while (file_r >> buffer) {
        new_vocab.push_back(buffer);
    }
    return new_vocab;
}

void vocablurary_write(const std::vector<std::string> vocab, const std::string &filename, const std::string delimiter) {
    std::ofstream file(filename);
    if (file.is_open())
        for (const auto &word: vocab) {
            file << word << delimiter;
    }
    file.close();
} 

std::set<std::string> vector2set(const std::vector<std::string> &v) {
    std::set<std::string> set;
    for (auto e: v) set.insert(e);
    return set;
} 

std::vector<std::string> set2vector(const std::set<std::string> &s) {
    std::vector<std::string> vec(s.begin(), s.end());
    return vec;
}


template<typename T>
int get_amount_of_lines(T &in) { 
    in.seekg(0, std::ios::beg);
    std::string buffer;
    int lines = 0;
    while (getline(in, buffer)) lines++;
    in.seekg(0, std::ios::beg);
    return lines;
}

