#pragma once

#include <ctime>
#include <time.h>

#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <set>

#include <regex>
#include <codecvt>
#include <iostream>
#include <iomanip>
#include <stdlib.h> 

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h> 
#endif


struct search_result {
    memo* reference;
    float confidence;
};


double jaccardSimilarity(const std::set<int>& setA, const std::set<int>& setB);
double wjaccardSimilarity(const std::vector<int> &a, const std::vector<int> b);
void discover(const std::string &ngram, std::vector<std::string> &terms);
std::vector<std::string> to_ngrams(const std::string &sentence, std::vector<std::string> &terms, int n);
std::vector<search_result> sort_results(std::vector<search_result> results);
int word_to_id(const std::string &word, std::vector<std::string> &terms);
std::vector<int> sentence_to_vector(std::string&sentence, std::vector<std::string> &terms);
std::vector<int> vectorize(std::string sentence, std::vector<std::string> &terms, int n =-1);
std::vector<int> filter(std::vector<int> a);

template <typename T>
float levenshtain_distance(std::vector<T> a, std::vector<T> b);

std::vector<search_result> search(const std::string &query, const std::vector<memo*> &memories, std::vector<std::string> &terms, int n= 3);
