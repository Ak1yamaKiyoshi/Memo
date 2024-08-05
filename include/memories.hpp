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

struct memo {
    std::string text;
    std::vector<std::string> tags;

    time_t created;
    time_t last_edited;

    int viewed_times;
    int memo_id;

    std::vector<int> linked_memories;
};

void debug_print_memo(const memo* mem);
memo* create_memo(const std::string &text, int id, std::vector<int> linked = {});



template<typename T>
void memo_write(T &out, const memo* mem, std::string delimiter = "|");

template<typename T>
memo* memo_read(T &in, std::string delimiter = "|");

template<typename T>
void memories_write_all(T &out, std::vector<memo*> memories);

template<typename T>
std::vector<memo*> memories_read_all(T &in);

int is_file_exist(std::string filename);
std::vector<memo*> memories_read_all_w(std::string filename);
bool memories_write_all_w(std::string filename, const std::vector<memo*> &memories);
memo* memories_get_by_id(std::vector<memo*> &memories, int id);
bool memories_add(memo*mem, std::vector<memo*> &memories);
bool memories_remove_by_id(std::vector<memo*> &memories, int id);
bool memories_update_by_id(std::vector<memo*> &memories, int id, std::string new_text);
std::vector<int> memories_get_all_ids(const std::vector<memo*> &memories);
int memories_get_new_id(const std::vector<memo*> &memories);
bool file_create(std::string filename);
void copy(memo*dst, memo*src);

std::string memo_to_string(const memo *mem);
std::string memo_to_stringc(const memo* mem);
