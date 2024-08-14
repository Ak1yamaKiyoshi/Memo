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


std::vector<std::string> vocablurary_read(const std::string &filename);
void vocablurary_write(const std::vector<std::string> vocab, const std::string &filename, const std::string delimiter = " ");
std::set<std::string> vector2set(const std::vector<std::string> &v);
std::vector<std::string> set2vector(const std::set<std::string> &s);
