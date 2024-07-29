#pragma once
#include "../include/levenshtain/levenshtain.hpp"
#include "../include/memory/memory.hpp"
#include "../include/vocablurary/vocablurary.hpp"


struct search_result {
    struct memory *mem;
    float confidence;
    // std::vector<std::string> tags;
};

std::vector<search_result> search(
    struct memory *memories, int len,
    std::string text_query, std::string vocablurary
);

std::vector<search_result> search(
    std::vector<memory*> memories,
    std::string text_query, std::string vocablurary
);

std::vector<search_result> sort_results(std::vector<search_result> results);
std::vector<search_result> filter_results(std::vector<search_result> results);
