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

void clear_screen();
void cout_c(int color);
void cout_c(const std::string& color);

enum codes {
    error_user_side = 'u',
    error_program_side = 'p',
    ok = 'o',
    display = 'd'
};

void message(const enum codes code, const std::string &message);
