#include "../include/utils.hpp"
#include "../include/memories.hpp"
#include "../include/repetitions.hpp"
#include "../include/search.hpp"
#include "../include/string_utils.hpp"
#include "../include/vocablurary.hpp"

void clear_screen()
{
#ifdef WINDOWS
    std::system("cls");
#else
    // Assume POSIX
    std::system("clear");
#endif
}


#ifdef _WIN32
void cout_c(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}
#else
void cout_c(const std::string& color) {
    std::cout << color;
}
#endif

void message(const enum codes code, const std::string &message) {
    std::stringstream iss;
    switch (code) {
        case error_program_side:
            iss << ANSI_RESET << "[ " << ANSI_BRIGHT_RED << "fail" << ANSI_RESET << " ]";
            break;
        case error_user_side:
            iss << ANSI_RESET << "[" << ANSI_BRIGHT_YELLOW << "error " << ANSI_RESET << "]";
            break;
        case ok:
            iss << ANSI_RESET <<   "[  " << ANSI_BRIGHT_GREEN << "ok"  << ANSI_RESET << "  ]";
            break;
        case display:
            iss << ANSI_RESET <<   "[  " << ANSI_WHITE << "--"  << ANSI_RESET << "  ]";
            break;
        default:
            break;
    }
    iss << " " << message << std::endl;
    std::cout << iss.str();
}