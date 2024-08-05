
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include <regex>




int main () {
    
    std::string filename = "test.txt";
    std::vector<std::string> vocab = {"test", "test1"};
    vocablurary_write(vocab,filename); 
    

    std::vector<std::string> new_vocab = vocablurary_read(filename);

    std::cout << std::endl;
    for (const auto &word: new_vocab ){ 
        std::cout << word << " ";
    }
    std::cout << std::endl;

    return 0;
}