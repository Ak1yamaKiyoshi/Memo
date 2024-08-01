#include "memory.hpp"



char *ltrim(char *s)
{
    while(isspace(*s)) s++;
    return s;
}

char *rtrim(char *s)
{
    char* back = s + strlen(s);
    while(isspace(*--back));
    *(back+1) = '\0';
    return s;
}

char *trim(char *s)
{
    return rtrim(ltrim(s)); 
}

void to_string(const time_t timestamp, const char *format, char *output_buffer)
{
    tm tstruct = *localtime(&timestamp);
    strftime(output_buffer, TIME_BUFFER_MAXSIZE, format, &tstruct);
}

/* Single memory operations; */

struct memory *create(const char *text, const int id)
{
    struct memory *mem = new memory;
    mem->id = id;
    mem->text = (char *)text;
    mem->create = time(0);
    mem->edit = time(0);
    return mem;
}

struct memory *edit(struct memory *mem, const char *new_text)
{
    free(mem->text);
    mem->text = new char[strlen(new_text)];
    return mem;
}

struct memory *deallocate(struct memory *mem)
{
    free(mem->text);
    free(mem);
    mem->text = nullptr;
    return nullptr;
}

// b - created, not allocated text field 
void copy(const struct memory &src, struct memory *dst) {
    dst->create = src.create;
    dst->edit = src.edit;
    dst->id = src.id;
    
    // std::cout << "checkpoint 1.7" << std::endl;
    // strlen(src->text);
    //std::cout << "checkpoint 1.75" << std::endl;
    // delete[] dst->text;

    dst->text = (char*)malloc(sizeof(char)*strlen(src.text));//new char[strlen(src->text)];
    strcpy(dst->text, src.text);
}

void to_string(struct memory *mem, char *output_buffer)
{
    char *time_buffer = new char[TIME_BUFFER_MAXSIZE];
    to_string(
        mem->edit,
        const_cast<char *>(TIME_FORMAT_FULL),
        time_buffer);

    sprintf(output_buffer,
            MEMORY_FORMAT_PRINT,
            mem->id,
            time_buffer,
            trim((char *)mem->text));
}

void to_string(struct memory *mem, float confidence, char *output_buffer)
{
    char *time_buffer = new char[TIME_BUFFER_MAXSIZE];
    to_string(
        mem->edit,
        const_cast<char *>(TIME_FORMAT_FULL),
        time_buffer);

    sprintf(output_buffer,
            MEMORY_FORMAT_PRINT_CONFIDENCE,
            confidence,
            mem->id,
            time_buffer,
            trim((char *)mem->text));
}

std::string to_string(struct memory *r)
{
    char memory_search_buffer[512];
    to_string(r, memory_search_buffer);
    return std::string(memory_search_buffer);
}

std::string to_string(struct memory *r, float confidence)
{
    char memory_search_buffer[512];
    to_string(r, confidence, memory_search_buffer);
    return std::string(memory_search_buffer);
}

/* Array of memories operations */

struct memory *add(struct memory *memories, int len, const struct memory *new_memory)
{
    memory *new_memories = new memory[(len + 1)];
    if (len > 0)
        memcpy(new_memories, memories, len * sizeof(memory));

    new_memories[len].edit = new_memory->edit;
    new_memories[len].create = new_memory->create;
    new_memories[len].id = new_memory->id;
    new_memories[len].text = new char[strlen(new_memory->text) + 1];
    strcpy(new_memories[len].text, new_memory->text);

    delete memories;
    return new_memories;
}

std::vector<memory *> add(
    std::vector<memory *> &memories,
    struct memory *new_memory)
{

    memories.push_back(new_memory);
    return memories;
}

std::vector<memory *> remove(
    std::vector<memory *> &memories, int id)
{
    // TODO: use usual for loop
    int idx = 0;
    for (auto &mem : memories)
    {
        idx++;
        if (mem->id == id)
            memories.erase(memories.begin() + idx, memories.begin() + idx + 1);
    }
    return memories;
}

// Todo: make method to check if memory exists at all
std::vector<memory *> edit(
    std::vector<memory *> &memories,
    std::string &text, const int id)
{

    for (int i = 0; i < memories.size(); i++)
        if (memories[i]->id == id)
        {
            memories[i]->edit = time(0);

            // TODO: review pls. memory leak???
            memories[i]->text = new char[text.length()];
            strcpy(memories[i]->text, text.c_str());
            
            //(char *)text.c_str();
            break;
        }
    return memories;
}


// TODO: review or delete
struct memory *remove(
    struct memory *memories,
    int len,
    int id)
{

    int flag = 0;
    memory *new_memories = new memory[(len)-1];
    for (int i = 0; i < (len); i++)
    {
        if (memories[i].id == id)
        {
            flag = 1;
            continue;
        }
        else
        {
            new_memories[len].edit = memories[i].edit;
            new_memories[len].create = memories[i].create;
            new_memories[len].id = memories[i].id;
            new_memories[len].text = new char[strlen(memories[i].text) + 1];
            strcpy(new_memories[len].text, memories[i].text);
        }
    }
    if (flag == 1)
    {
        // delete [] memories;
    }
    return new_memories;
}

struct memory *edit(struct memory *memories, int len, const char *text, const int id)
{
    for (int i = 0; i < len; i++)
        if (memories[i].id == id)
        {
            memories[i].edit = time(0);
            free(memories[i].text);
            memories[i].text = new char[strlen(text)];
            sprintf(memories[i].text, "%s", text);
            break;
        }
    return memories;
}

/* Singular IO Operations*/

int write(
    FILE *f,
    const memory mem)
{
    fprintf(f, MEMORY_FORMAT_WRITE, mem.id, (int)mem.create, (int)mem.edit, mem.text);
    return 0;
}

int read(FILE *f, memory *mem) 
{
    int creation_timestamp = 0;
    int edit_timestamp = 0;
    int id = 0;
    fscanf(f, MEMORY_FORMAT_READ, &id, &creation_timestamp, &edit_timestamp, mem->text);
    mem->id = id;
    mem->create = (time_t)creation_timestamp;
    mem->edit = (time_t)edit_timestamp;
    return 0;
}

/* IO Operations with array of memories */

int __get_file_line_amount(FILE *f)
{
    int ch, number_of_lines = 0;
    while (EOF != (ch = getc(f)))
        if ('\n' == ch)
            ++number_of_lines;
    return number_of_lines;
}

int get_amount_of_cached_memories(const char *filename)
{
    FILE *f = fopen(filename, "rb");
    int lines = __get_file_line_amount(f);
    fseek(f, 0, SEEK_SET);
    fclose(f);
    return lines;
}


std::vector<memory*> mem_from_file(std::string filename) {
    std::vector<memory*> memories;
    std::string line;
    std::fstream file(filename, std::ios::in);
    while (getline(file, line)) {
        memory* mem = new memory; 
        std::istringstream iss(line);
        if (!(iss >> mem->id >> mem->create >> mem->edit)) {
            delete mem;
            continue; 
        }
        std::getline(iss, line);

        mem->text = new char[line.size() + 1];
        std::strcpy(mem->text, (char*)line.c_str());
        memories.push_back(mem);
    }
    return memories;
}

void mem_to_file(std::string filename, std::vector<memory*> memories) {
    std::ofstream file(filename, std::ios::out);
    if (file.is_open()) {
        for (auto &mem: memories) {
            file << mem->id 
                << " " << (int)mem->create 
                << " " << (int)mem->edit 
                << " " << mem->text << "\n";
            //std::cout << to_string(mem) << "\n";
            //std::string str = std::string(mem->text) ;
            //mem->text = ltrim(str).c_str();
        }
    } else {
        std::runtime_error("Failed to open file");
    }

}


struct memory *from_file(const char *filename, int *lenght)
{
    FILE *f = fopen(filename, "rb");
    int lines = __get_file_line_amount(f);
    fseek(f, 0, SEEK_SET);
    std::cout << "LINES: " << lines << "\n";
    struct memory *output_buffer = (struct memory *)malloc(sizeof(memory) * lines);

    for (
        int i = 0;
        i < lines;
        output_buffer[i].text = (char *)malloc(sizeof(char) * MEMORY_MAXSIZE_TEXT),
            read(f, &output_buffer[i++]), *lenght++)
        ;
    fclose(f);
    return output_buffer;
}

void to_file(const char *filename, struct memory *memories, int len)
{
    FILE *f = fopen(filename, "wb");
    for (int i = 0; i < len; i++)
        write(f, memories[i]);
    fclose(f);
}
