#pragma once

#include <time.h>
#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>
#include <sstream>

const size_t MEMORY_MAXSIZE_TEXT = 512;
const char FILEPATH_STORAGE_NOTES[] = "memory.bin";

const char MEMORY_FORMAT_READ[] = "%d%d%d%2000[^\n]";
const char MEMORY_FORMAT_WRITE[] = "%d %d %d %s\n";
const char MEMORY_FORMAT_PRINT[] = "[%4d][%s] %s";
const char MEMORY_FORMAT_PRINT_CONFIDENCE[] = "[%.1f][%4d][%s] %s";

const char TIME_FORMAT_FULL[] = "%H:%M %d.%m.%y";
const char TIME_BUFFER_MAXSIZE = 90;

struct memory
{
    int id;
    char *text;
    time_t edit;
    time_t create;
};

void copy(const struct memory &src, struct memory *dst);

/* Time operations */

void to_string(const time_t timestamp, const char *format, char *output_buffer);

/* Single memory operations; */

struct memory *create(const char *text, const int id);
struct memory *edit(struct memory *mem, const char *new_text);
struct memory *deallocate(struct memory mem);

void to_string(struct memory *mem, char *output_buffer);
void to_string(struct memory *mem, float confidence, char *output_buffer);
std::string to_string(struct memory *r);
std::string to_string(struct memory *r, float confidence);

/* Array of memories operations (LEGACY) */

struct memory *add(struct memory *memories, int len, const struct memory *new_memory);
struct memory *remove(struct memory *memories, int len, int id);
struct memory *edit(struct memory *memories, int len, const char *text, const int id);

/* Vector of memories operations */

std::vector<memory *> add(
    std::vector<memory *> &memories,
    struct memory *new_memory);

std::vector<memory *> remove(
    std::vector<memory *> &memories, int id);

std::vector<memory *> edit(
    std::vector<memory *> memories,
    std::string &text, const int id);

/* Singular IO Operations */

int write(FILE *f, const memory mem);
int read(FILE *f, memory *mem);
int __get_file_line_amount(FILE *f);

/* IO Operations with array of memories */

struct memory *from_file(const char *filename, int *lenght);
void to_file(const char *filename, struct memory *memories, int len);

std::vector<memory*> mem_from_file(std::string filename);
void mem_to_file(std::string filename, std::vector<memory> memories);

struct memory *from_file(const char *filename, int lenght);
