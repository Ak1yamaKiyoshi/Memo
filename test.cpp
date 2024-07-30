#include <iostream>
#include <iomanip>  // for std::hex and std::showbase
#include <string>


// int to_lowercase( char c) {
//     if (isalpha(c)) {
//         return tolower(c);
//     }

// }


/*
    //"абвгґдеєжзиіїйклмнопрстуфхцчшщьюя"
    //
*/





int main() {
    const std::string uppercase = "АБВГҐДЕЄЖЗИІЇЙКЛМНОПРСТУФХЦЧШЩЬЮЯ"; //
    const std::string lowercase = "абвгґдеєжзиіїйклмнопрстуфхцчшщьюя";

    std::cout <<( (char32_t)uppercase[1]) << " " << (char32_t)'В';//== (char16_t)'А');
    //std::string((char32_t)'г');
    // std::cout << uppercase.c_str() << std::endl;
    // for (int i = 0; i < uppercase.length(); i++) {
    //     //printf("%c", 65436);
    //     std::cout << uppercase.c_str()[i] << " ";
    //}
    
    /*
    // Define a string containing the characters
    std::string text = "АБВГҐДЕЄЖЗИІЇЙКЛМНОПРСТУФХЦЧШЩЬЮЯ";
    setlocale(LC_ALL, "ukranian");
    
    // Iterate through each character in the string
    for (size_t i = 0; i < text.size(); ++i) {
        // Get the character at the current position
        char ch = static_cast<char>(text[i]);

        // Print the character and its integer (decimal and hexadecimal) representation
        std::cout << "Character: " << text[i]
                  << " | Integer (decimal): " << static_cast<int>(ch)
                  << " | Integer (hex): " << std::hex << std::showbase << static_cast<int>(ch)
                  << std::dec << std::endl;  // Reset to decimal
    }

    std::cout <<text;
    
    */
    // std::string txt = "d";
    // std::cout << isalpha(txt[0]);

    return 0;
}


