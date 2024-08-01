#pragma once
#include <sstream>
#include <cstring>
#include <string>
#include <algorithm>
#include <vector>

const std::string SPECIAL_CHARACTERS = "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";
const char WORD_DELIMITERS[] = ",.-=?!;:`'\"/\\{^}";
const char FILEPATH_STORAGE_VOCABLURARY[] = "vocab.txt";

int word_id(std::string word, std::string vocablurary);
std::string discover_words(std::string sentence, std::string vocablurary);
std::vector<int> sentence_vector(std::string sentence, std::string vocablurary);

std::string from_file(std::string filename);
void to_file(std::string filename, std::string vocablurary);

