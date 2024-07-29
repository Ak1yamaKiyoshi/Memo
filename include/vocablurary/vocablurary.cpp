#include "vocablurary.hpp"
#include <fstream>

/* Word Operations */

int word_id(std::string word, std::string vocablurary)
{
    std::stringstream ss(vocablurary);
    std::string buffer;
    int idx = 0, result = -1;
    while (ss >> buffer) {
        int value = buffer.compare(word); //strcmp(, );
        if (value == 0) {
            result = idx;
            break;
        }
        idx++;
    }
    buffer.erase();
    return result;
}


std::string remove_special_characters(std::string &text) {
    text.erase(std::remove_if(text.begin(), text.end(),
        [](char c) {
            return (std::isalpha(c) || std::isspace(c) || SPECIAL_CHARACTERS.find(c) != std::string::npos);
        }),
        text.end());
    return text;
}


std::string discover_words(std::string sentence, std::string vocablurary)
{
    std::stringstream new_vocablurary_ss;
    new_vocablurary_ss << vocablurary;
    std::stringstream ss(sentence);
    std::string buffer;   
    int found_word_id = -1;
    while (ss >> buffer) {
        found_word_id = word_id(buffer, vocablurary);
        if (found_word_id == -1) {
            // TODO: prerpocess, remove commas? 
            new_vocablurary_ss << " " << remove_special_characters(buffer);
            //std::cout << buffer << "\n";
        }
    }
    
    return new_vocablurary_ss.str();
}


std::vector<int> sentence_vector(std::string sentence, std::string vocablurary)
{
    std::stringstream ss(sentence);
    std::string word;
    std::vector<int> output_vec;


    int current_word_idx = -1;
    int words = 0;
    while (ss >> word) {
        current_word_idx = word_id(remove_special_characters(word), vocablurary);
        output_vec.push_back(current_word_idx);
    }

    return output_vec;
}

/* IO Operations */

std::string from_file(std::string filename)
{
    std::stringstream ss;
    std::fstream fs(filename, std::ios::in );
    if (!fs.bad()) {
        ss << fs.rdbuf();
        fs.close();
    }
    return ss.str();
}

void to_file(std::string filename, std::string vocablurary)
{
    std::ofstream fs;
    fs.open(filename);
    fs << vocablurary << std::endl;
    fs.close();
}
