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


struct repetition;

std::vector<int> string_vec_to_int(const std::vector<std::string> &from);
std::vector<bool> string_vec_to_bool(const std::vector<std::string> &from);
void print_repetition(const repetition* rep);

template <typename T>
repetition* repetitions_read_single(T &in, const std::string& delimiter);

template <typename T>
void repetitions_write_single(T &out, const repetition* rep, const std::string& delim = "|");

void repetitions_to_file(const std::vector<repetition*>& repetitions, const std::string& filename, const std::string& delim = "|");
std::vector<repetition*> repetitions_from_file(const std::string& filename, const std::string& delimiter = "|");
float repetition_eval(const repetition*rep);
std::vector<repetition*> get_todays_repetitions(std::vector<repetition*> repetitions);
