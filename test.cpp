#include <iostream>
#include <numeric>
#include <vector>
#include <algorithm>
#include <functional>
#include <math.h>
#include <locale>

#include "include/memory/memory.cpp"
#include "include/vocablurary/vocablurary.cpp"
#include "include/levenshtain/levenshtain.cpp"
#include "src/search.cpp"
#include "include/coloring.hpp"
#include "include/vector_utils/vectors.hpp"

/*
int main() {
    std::string t1 = "Хіхіхаха";
    std::string t2 = "гімна поїж";

    std::string voc = from_file(FILEPATH_STORAGE_VOCABLURARY);
    voc = discover_words(t1, voc);
    voc = discover_words(t2, voc);

    std::vector<int> v1 = sentence_vector(t1, voc);
    std::vector<int> v2 = sentence_vector(t2, voc);

    std::cout << "Vec 1:" << std::endl;
    for (auto num: v1) std::cout << num << " ";
    std::cout << std::endl;

    std::cout << "Vec 2:" << std::endl;
    for (auto num: v2) std::cout << num << " ";
    std::cout << std::endl;

    std::cout << "Similarity: " << weuclidian_distance(v1, v2) << std::endl;

    return 0;
}

*/