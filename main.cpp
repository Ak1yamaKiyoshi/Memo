
#include <ctime>
#include <time.h>

#include <fstream>
#include <vector>
#include <string>
#include <sstream>

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



std::vector<std::string> parse_tags_from_string(const std::string &text) {
    std::vector<std::string> tags;
    std::regex tag_regex(TAG_REGEX);
    auto tags_begin = 
        std::sregex_iterator(text.begin(), text.end(), tag_regex);
    auto tags_end = std::sregex_iterator();

    for (std::sregex_iterator i = tags_begin; i != tags_end; ++i) {
        std::smatch match = *i;
        tags.push_back(match.str());
    }
    return tags;
}

std::string remove_tags_from_string(const std::string &text) {
    std::regex tag_regex(TAG_REGEX);
    std::string new_string = 
        std::regex_replace(text, tag_regex, "");
    return new_string;
}

std::string remove_special_characters_from_string(const std::string &text, const std::string delimiter = " ") {
    std::wregex word_regex(WORDS_REGEX);
    std::stringstream iss;

    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
    std::wstring wide = converter.from_bytes(text);

    auto words_begin = 
        std::wsregex_iterator(wide.begin(), wide.end(), word_regex);
    auto words_end = std::wsregex_iterator();
    
    for (std::wsregex_iterator i = words_begin; i != words_end; ++i) {
        std::wsmatch match = *i;
        if (i != words_begin) iss << delimiter;
        iss << converter.to_bytes(match.str());
    }
    return iss.str();
}

std::vector<std::string> split_by_space(const std::string &str) {
    std::vector<std::string> terms;
    std::stringstream iss(str);
    while (1) {
        std::string buffer;
        if (!(iss >> buffer)) break;
        else terms.push_back(buffer);
    }
    return terms;
}

struct memo {
    std::string text;
    std::vector<std::string> tags;

    time_t created;
    time_t last_edited;

    int viewed_times;
    int memo_id;

    std::vector<int> linked_memories;
};


void debug_print_memo(const memo* mem) {
    if (mem == nullptr) {
        std::cerr << "Error: memo is null." << std::endl;
        return;
    }

    std::string output;
    output += "Text: " + mem->text + "\n";

    output += "Tags: ";
    for (const auto& tag : mem->tags) {
        output += tag + " ";
    }
    output += "\n";

    auto print_time = [](time_t time_val) {
        struct tm* time_info;
        char buffer[80];
        time_info = localtime(&time_val);
        strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", time_info);
        return std::string(buffer);
    };

    output += "Created: " + print_time(mem->created) + "\n";
    output += "Last Edited: " + print_time(mem->last_edited) + "\n";

    output += "Viewed Times: " + std::to_string(mem->viewed_times) + "\n";
    output += "Memo ID: " + std::to_string(mem->memo_id) + "\n";
    output += "Linked Memories: ";
    for (const auto& id : mem->linked_memories) {
        output += std::to_string(id) + " ";
    }
    output += "\n";
    std::cout << output;
}





std::string weekday_to_string(int i) {
    std::vector<std::string> weekdays = {"Monday", "Tueday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};


    std::string buffer;
    if (i < 7)
        buffer += weekdays[i];
    return buffer;
}

std::string month_to_string(int i) {
    std::vector<std::string> months = {
        "January", 
        "February", 
        "March", 
        "April", 
        "May", 
        "June", 
        "July", 
        "August", 
        "September", 
        "October", 
        "November", 
        "December"
    };

    std::string buffer;
    if (i < 12)
        buffer += months[i];
    return buffer;
}

std::string time_to_string(const time_t timestamp, const char *format) {
    char *output_buffer = new char[90];
    tm tstruct = *localtime(&timestamp);
    strftime(output_buffer, 90, format, &tstruct);
    return std::string(output_buffer);
}

std::string center(std::string input, int width = 113) { 
    return std::string((width - input.length()) / 2, ' ') + input;
}



std::string make_fixed_width_string(const std::string& input, size_t width) {
    std::ostringstream oss;
    oss << std::left << std::setw(width) << std::setfill(' ') << input;
    return oss.str().substr(0, width);
}


std::string time_to_string_verbose(const time_t& current, const time_t& past) {
    std::tm curs{};
    std::tm pasts{};
    
    #if defined(_WIN32) || defined(_WIN64)
        localtime_s(&curs, &current);
        localtime_s(&pasts, &past);
    #else
        localtime_r(&current, &curs);
        localtime_r(&past, &pasts);
    #endif

    std::stringstream iss;

    int days_ago_same_year = curs.tm_yday - pasts.tm_yday;
    
    if (curs.tm_yday == pasts.tm_yday && curs.tm_year == pasts.tm_year) {
        iss << "Today at " << std::setw(2) << std::setfill('0')
            << pasts.tm_hour << ":" << std::setw(2) << std::setfill('0')
            << pasts.tm_min;
    } else if (days_ago_same_year == 1 && curs.tm_year == pasts.tm_year) {
        iss << "Yesterday at "
            << std::setw(2) << std::setfill('0') << pasts.tm_hour << ":"
            << std::setw(2) << std::setfill('0') << pasts.tm_min;
    } else if (curs.tm_yday / 7 == pasts.tm_yday / 7 && curs.tm_year == pasts.tm_year) {
        iss << weekday_to_string(pasts.tm_wday) << " "
            << std::setw(2) << std::setfill('0') << pasts.tm_hour << ":"
            << std::setw(2) << std::setfill('0') << pasts.tm_min;
    } else if (curs.tm_mon == pasts.tm_mon) {
        iss << days_ago_same_year << " days ago at "
            << pasts.tm_mday << (pasts.tm_mday == 1 ? "st" : 
                                 pasts.tm_mday == 2 ? "nd" : 
                                 pasts.tm_mday == 3 ? "rd" : "th") << " "
            << std::setw(2) << std::setfill('0') << pasts.tm_hour << ":"
            << std::setw(2) << std::setfill('0') << pasts.tm_min;

    } else if (curs.tm_year == pasts.tm_year) {
        iss << pasts.tm_mday << (pasts.tm_mday == 1 ? "st" : 
                                 pasts.tm_mday == 2 ? "nd" : 
                                 pasts.tm_mday == 3 ? "rd" : "th") << " of "
            << month_to_string(pasts.tm_mon) + " "
            << std::setw(2) << std::setfill('0') << pasts.tm_hour << ":"
            << std::setw(2) << std::setfill('0') << pasts.tm_min;
    } else {
        iss << time_to_string(past, "%Y-%m-%d %H:%M:%S");
    }

    return make_fixed_width_string(center(iss.str(), 25), 25);
}

std::string memo_to_string(memo *mem) {
    std::stringstream iss;
    iss << std::setw(4) << std::setfill('0') << std::to_string(mem->memo_id)
    << " [" << time_to_string_verbose(time(0), mem->last_edited) + "] "
    << mem->text + " | ";
    for (const auto &tag: mem->tags) iss << tag + " ";
    return iss.str();
}

std::string memo_to_stringc(memo* mem) {
    std::stringstream iss;
    iss << ANSI_BLUE;
    iss << std::setw(4) << std::setfill('0');
    iss << std::to_string(mem->memo_id);
    iss<< ANSI_RESET<<"";
    iss << ANSI_YELLOW << time_to_string_verbose(time(0), mem->last_edited) << ANSI_RESET << " "
    << mem->text + " ";
    iss << ANSI_BRIGHT_MAGENTA;
    for (const auto &tag: mem->tags) iss << tag + " ";
    return iss.str();
}
std::string memo_to_stringc(memo* mem, float confidence) {
    std::string buffer;
}


/*
struct Address
{
   string street;
   string cross;
   int suite;
};

inline ostream& operator<<(ostream& oss, const Address& other)
{
   oss << "street: " << other.street << "cross: " << other.cross << "suite: " << other.suite;
   return oss;
}

*/

template<typename T>
std::string join(const std::vector<T> &vec, const std::string &delimiter) {
    std::stringstream iss;
    for (const auto &e: vec) iss << e << delimiter;
    return iss.str();
}

memo* create_memo(const std::string &text, int id, std::vector<int> linked = {}) {
    memo *memory = new memo();
    memory->created = time(0);
    memory->last_edited = time(0);

    memory->tags = parse_tags_from_string(text);
    memory->text = remove_tags_from_string(text);

    memory->viewed_times = 0;
    memory->memo_id = id;

    // linked memories
    memory->linked_memories = linked;
    return memory; 
}

template<typename T>
void memo_write(T &out, const memo* mem, std::string delimiter = "|") {
    out << mem->viewed_times << delimiter
        << (int)mem->last_edited << delimiter 
        << (int)mem->created << delimiter 
        << mem->memo_id << delimiter
        << mem->text.length() << delimiter
        << mem->text << delimiter
        << mem->tags.size() << delimiter
        << join(mem->tags, " ") << delimiter
        << mem->linked_memories.size() << delimiter
        << join(mem->linked_memories, " ") << delimiter << "\n";
}

template<typename T>
memo* memo_read(T &in, std::string delimiter = "|") {
    std::string buffer;
    getline(in, buffer);
    std::vector<std::string> tokens;
    size_t pos = 0;
    std::string token;
   
    while ((pos = buffer.find(delimiter)) != std::string::npos) {
        token = buffer.substr(0, pos);
        tokens.push_back(token);
        buffer.erase(0, pos + delimiter.length());
    }

    if (tokens.size() > 1) {
        memo* mem = new memo();

        mem->viewed_times = std::stoi(tokens[0]);
        mem->last_edited = (time_t)std::stoi(tokens[1]);
        mem->created = (time_t)std::stoi(tokens[2]);
        mem->memo_id = std::stoi(tokens[3]);
        mem->text = tokens[4+1];
        mem->tags = split_by_space(tokens[6+1]);
        std::vector<int> linked_memories;
        std::vector<std::string> linked_memories_str = split_by_space(tokens[9]);

        for (auto &str_val: linked_memories_str)  {
            linked_memories.push_back(std::stoi(str_val)); 
        }
        mem->linked_memories = linked_memories;
        return mem;
    } else {
        return nullptr;
    }
}


template<typename T>
int get_amount_of_lines(T &in) { 
    in.seekg(0, std::ios::beg);
    std::string buffer;
    int lines = 0;
    while (getline(in, buffer)) lines++;
    in.seekg(0, std::ios::beg);
    return lines;
}

template<typename T>
void memories_write_all(T &out, std::vector<memo*> memories) {
    for (const auto &mem: memories) memo_write(out, mem);
}


template<typename T>
std::vector<memo*> memories_read_all(T &in) {
    std::vector<memo*> memories;

    in.seekg(0, std::ios::beg);
    std::string buffer;

    while (1) {
        struct memo* mem = memo_read(in);
        if (mem == nullptr) break;
        else memories.push_back(mem);
    }

    return memories;
}

memo* memories_get_by_id(std::vector<memo*> &memories, int id) {
    std::vector<memo*>::iterator it = std::find_if(memories.begin(), memories.end(), 
        [id](const  memo *mem) { return mem->memo_id == id; });
    
    return  (it != memories.end()) ? (*it) : nullptr;
}

bool memories_add(memo*mem, std::vector<memo*> &memories) {
    if (mem != nullptr) {
        memories.push_back(mem);
        return true; }
    return false;

}

bool memories_remove_by_id(std::vector<memo*> &memories, int id) {
    auto it = std::find_if(memories.begin(), memories.end(), 
        [id](const  memo *mem) { return mem->memo_id == id; });

    if (it != memories.end()) {
        memories.erase(it);
        return true; }
    return false;
}


bool memories_update_by_id(std::vector<memo*> &memories, int id, std::string new_text) {
    std::vector<std::string> tags = parse_tags_from_string(new_text);
    std::string actual_text = remove_tags_from_string(new_text);

    memo* mem = memories_get_by_id(memories, id);
    if (mem != nullptr) {
        mem->text = actual_text;
        mem->tags = tags;
        mem->last_edited = time(0);
        mem->viewed_times += 1;
        return true;
    }
    return false;
}

std::vector<int> memories_get_all_ids(const std::vector<memo*> &memories) {
    std::vector<int> ids;
    for (const auto &mem: memories) ids.push_back(mem->memo_id);
    return ids;
}

int memories_get_new_id(const std::vector<memo*> &memories) {
    std::vector<int> ids = memories_get_all_ids(memories);
    return *std::max_element(ids.begin(), ids.end()); 
}


/*
void ngram_tokenizer();
void levenshtain_distance(); // use templates 

void cosine_similarity();
void euclidian_distance();
void tfidf(); 
// check https://medium.com/@thisisharrsh/hands-on-implementation-of-custom-tf-idf-vectorizer-cd4860c31ad1
// https://medium.com/@nikhilkarve007/implementing-tfidf-from-scratch-f2be7f9b6557
void bm25(); // improvement over tfidf? ?????
void suffix_arrays(); //?? 
// paper: https://www.sciencedirect.com/science/article/abs/pii/S0167819114000805

*/
void boolean_retrieval_model(); /*
Boolean Retrieval Model Using Inverted Index and Positional Index
https://medium.com/analytics-vidhya/boolean-retrieval-model-using-inverted-index-and-positional-index-2a9782bcec99
*/

/*
void glove();
//https://medium.com/@neri.vvo/word-embedding-a-powerful-tool-word2vec-glove-fasttext-dd6e2171d5

void search(); // multiprocessing if multiple search models used;
void metric(); // function pointers?

void memo_to_string();
void memo_to_string_colored();
void memo_to_string_colored(); // confidence


void create_file();

void get_command_prompt();

void parse_command();
void program_setup();
void command_help();
void command_add();
void command_remove();
void command_update();
void command_clear();
void command_ascii_art();
*/
// Features: search as type goes


struct search_result {
    memo reference;
    float confidence;
};


int main() {
    std::vector<int > linked = {1, 3, 1, 2, 4};
    memo* mem0 = create_memo("text0 #tag b #vss", 1, linked);
    memo* mem1 = create_memo("text1 #tag b #vss", 2, linked);
    memo* mem2 = create_memo("text2 #tag b #vss", 3, linked);
    std::vector<memo*> memories;
    std::stringstream test;

    memories_add(mem0, memories);
    memories_add(mem1, memories);
    memories_add(mem2, memories);
    //memories_remove_by_id(memories, 1);
    //memories_remove_by_id(memories, 2);
    //memories_remove_by_id(memories, 3);
    //memories_add(mem2, memories);
    memories_update_by_id(memories, 3, "New text and no tags!");

    for (const auto &mem: memories) std::cout << memo_to_stringc(mem) <<std::endl;


    /*
    memories_write_all(test, memories);
    std::vector<memo*> rmemories;

    rmemories = memories_read_all(test);    
    for (const auto &mem: rmemories)
        debug_print_memo(mem);
    */

    /*
    std::ofstream file_write("test.txt", std::ios::out);
    if (file_write.is_open()) {
        memories_write_all(file_write, memories);
        file_write.close();
    }

    std::vector<memo*> rmemories;
    std::fstream file_read("test.txt", std::ios::in);
    if (file_read.is_open()) {
        rmemories = memories_read_all(file_read);    
        file_read.close();
    }
    
     for (const auto &mem: rmemories)
         debug_print_memo(mem);

    */

    return 0;
}

