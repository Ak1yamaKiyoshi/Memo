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

const std::string TAG_REGEX = "#{1}[A-zА-яі]+";
const std::wstring WORDS_REGEX = L"[A-zА-яіҐґЄєІЇї]+";
const std::string COMMAND_REGEX = "^[/]{1}[A-z]+";
const std::string FULL_DATE_REGEX = "\d{2}.\d{2}.\d{4}";


const std::string ANSI_RESET = "\033[0m";
const std::string ANSI_BLACK = "\033[30m";
const std::string ANSI_RED = "\033[31m";
const std::string ANSI_GREEN = "\033[32m";
const std::string ANSI_YELLOW = "\033[33m";
const std::string ANSI_BLUE = "\033[34m";
const std::string ANSI_MAGENTA = "\033[35m";
const std::string ANSI_CYAN = "\033[36m";
const std::string ANSI_WHITE = "\033[37m";

const std::string ANSI_BRIGHT_BLACK = "\033[90m";
const std::string ANSI_BRIGHT_RED = "\033[91m";
const std::string ANSI_BRIGHT_GREEN = "\033[92m";
const std::string ANSI_BRIGHT_YELLOW = "\033[93m";
const std::string ANSI_BRIGHT_BLUE = "\033[94m";
const std::string ANSI_BRIGHT_MAGENTA = "\033[95m";
const std::string ANSI_BRIGHT_CYAN = "\033[96m";
const std::string ANSI_BRIGHT_WHITE = "\033[97m";

const std::string SPECIAL_CHARACTERS = "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";

std::vector<std::string> parse_tags_from_string(const std::string &text);
std::string remove_tags_from_string(const std::string &text);
std::string remove_special_characters_from_string(const std::string &text, const std::string delimiter = " ");
std::vector<std::string> split_by_space(const std::string &str);
std::string weekday_to_string(int i);
std::string month_to_string(int i);
std::string time_to_string(const time_t timestamp, const char *format);
std::string center(std::string input, int width = 113);
std::string make_fixed_width_string(const std::string& input, size_t width);
std::string time_to_string_verbose(const time_t& current, const time_t& past);

template<typename T>
extern  std::string join(const std::vector<T> &vec, const std::string &delimiter);

std::string parse_command(std::string str);
bool parse_date(std::string str, std::tm &t_buffer);
std::string remove_commands(std::string str);
std::string remove_redundant_spaces(std::string str);
std::string trim(std::string str);
std::string to_view_string(std::string str);
std::string vec2string(const std::vector<std::string> &vec, std::string delimiter=" ");
std::string remove_spaces(std::string str);
std::string remove_special_characters(std::string &text);
std::string tolower_curlocale(std::string &str);
std::string normalize_string(std::string &str);

template <typename T>
extern std::set<T> vec2set(const std::vector<T> &v);
