
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

std::string memo_to_string(const memo *mem) {
    std::stringstream iss;
    iss << std::setw(4) << std::setfill('0') << std::to_string(mem->memo_id)
    << " [" << time_to_string_verbose(time(0), mem->last_edited) + "] "
    << mem->text + " | ";
    for (const auto &tag: mem->tags) iss << tag + " ";
    return iss.str();
}

std::string memo_to_stringc(const memo* mem) {
    std::stringstream iss;
    iss << ANSI_BLUE;
    iss << std::setw(4) << std::setfill('0');
    iss << std::to_string(mem->memo_id);
    iss<< ANSI_RESET<<"";
    iss << ANSI_YELLOW << time_to_string_verbose(time(0), mem->last_edited) << ANSI_RESET << " "
    << remove_tags_from_string(mem->text) + " ";
    iss << ANSI_BRIGHT_MAGENTA;
    for (const auto &tag: mem->tags) iss << tag + " ";
    iss << ANSI_RESET;
    return iss.str();
}


inline std::ostream& operator<<(std::ostream& iss, const memo *mem) {
    iss << memo_to_stringc(mem);
    return iss;
}


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
    std::cout << memory->tags.size();

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
int is_file_exist(std::string filename) {
    std::ifstream file(filename.c_str());
    if (file.good()) return true;
    else return false;
}

std::vector<memo*> memories_read_all_w(std::string filename) {
    std::vector<memo*> memories;
    
    std::ifstream file(filename.c_str(), std::ios::in);
    
    if(!file.is_open()) return memories;
    memories = memories_read_all(file);
    file.close();
    return memories;
}

bool memories_write_all_w(std::string filename, const std::vector<memo*> &memories) {
    if (!is_file_exist(filename)) return false;
    std::ofstream file(filename.c_str(), std::ios::out | std::ios::trunc);

    if(!file.is_open()) return false;
    memories_write_all(file, memories);

    file.close();
    return true;
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
    if (memories.size() > 0 ) {
        std::vector<int> ids = memories_get_all_ids(memories);
        return *std::max_element(ids.begin(), ids.end())+1; 
    } else {
        return 1;
    }
}

bool file_create(std::string filename) {
    std::ofstream outfile(filename);
    if (outfile.is_open()){
        outfile << " " << std::endl;
        outfile.close();
        return true;
    } 
    return false;
}

struct search_result {
    memo reference;
    float confidence;
};


std::string parse_command(std::string str) {
    std::regex command_regex("[/]{1}[A-z]+");
    auto rbegin = std::sregex_iterator(str.begin(), str.end(), command_regex);
    auto rend = std::sregex_iterator();
    if (rbegin != rend) {
        std::smatch match = *rbegin;
        return match.str();
    }
    
    return "";
}

std::string remove_commands(std::string str) {
    std::regex command_regex("[/]{1}[A-z]+");
    return std::regex_replace(str, command_regex, "");
}

std::string remove_redundant_spaces(std::string str) {
    std::regex spaces_regex("[ ]+");
    return std::regex_replace(str, spaces_regex, " ");
}

std::string trim(std::string str) {
    std::regex start_regex("^[ ]+");
    std::regex end_regex("[ ]+$");
    str = std::regex_replace(str, start_regex, "");
    str = std::regex_replace(str, end_regex, "");
    return str;
}

std::string to_view_string(std::string str) {
    return trim(remove_redundant_spaces(remove_tags_from_string(str)));
}

enum codes {
    error_user_side = 'u',
    error_program_side = 'p',
    ok = 'o',
    display = 'd'
};

void message(const enum codes code, const std::string &message) {
    std::stringstream iss;
    switch (code) {
        case error_program_side:
            iss << ANSI_RESET << "[ " << ANSI_RED << "fail" << ANSI_RESET << " ]";
            break;
        case error_user_side:
            iss << ANSI_RESET << "[" << ANSI_YELLOW << "error " << ANSI_RESET << "]";
            break;
        case ok:
            iss << ANSI_RESET <<   "[  " << ANSI_GREEN << "ok"  << ANSI_RESET << "  ]";
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

void copy(memo*dst, memo*src) {
    dst->created = src->created;
    dst->last_edited = src->last_edited;

    for (const auto m: src->linked_memories)
        dst->linked_memories.push_back(m);

    for (auto t: src->tags)
        dst->tags.push_back(t);

    dst->memo_id = src->memo_id;
    dst->text = src->text;
    dst->viewed_times = src->viewed_times;
}

void program_setup();
void command_help();
void command_add();
void command_remove();
void command_update();
void command_clear();
void command_ascii_art();


int main() {

    std::string filename_memories = "memories.txt";
    if (!is_file_exist(filename_memories)) {
        file_create(filename_memories);
        message(ok, "Memories files not found. Created new one: " + filename_memories);
    }

    std::string user_input, command, arguments;
    std::vector<memo*> memories = memories_read_all_w(filename_memories);

    while (true) {
        
        std::cout << "|" << memories.size() << "> ";
        getline(std::cin, user_input);

        command = parse_command(user_input);
        arguments = remove_commands(user_input);
        if (command.length() == 0) {
            message(error_user_side, "Entered command is too short.");
            continue;
        } else if (trim(arguments).length() == 0) {
            message(error_user_side, "You provided no arguments.");
            continue;
        }
        if (!command.compare("/add") || !command.compare("/a")) {
            int id = memories_get_new_id(memories);
            memo* mem = create_memo(arguments, id);
            if (!memories_add(mem, memories)) {
                message(error_program_side, "Something went wrong.");
            } else {
                message(ok, "Memory successfuly created");
                message(display, "Created memory: " + to_view_string(memo_to_stringc(mem)));
            }
        } else if (!command.compare("/repetition") || !command.compare("/i")) {
        } else if (!command.compare("/search") || !command.compare("/s"))     {
        } else if (!command.compare("/remove") || !command.compare("/r"))     {
            std::stringstream iss(arguments);
            std::string id_str;
            int id = 0; iss >> id_str;
            id = std::stoi(id_str);

            memo* mem = memories_get_by_id(memories, id); 
            if (mem == nullptr) {
                message(error_user_side, "There is no memory with such id");
                continue;
            }

            if (!memories_remove_by_id(memories, id)) {
                message(error_program_side, "Something went wrong");
                continue;
            }

            std::string buffer = "Memory " + to_view_string(memo_to_stringc(mem)) + " has been removed";
            message(ok, buffer);

        } else if (!command.compare("/update") || !command.compare("/u")) {
            std::stringstream iss(arguments);
            std::string id_str, text;
            int id = 0; iss >> id_str;
            id = std::stoi(id_str);
            getline(iss, text);

            memo* mem = memories_get_by_id(memories, id); 
            
            mem->text = mem->text;
            if (mem == nullptr) {
                message(error_user_side, "There is no memory with such id");
                continue;
            }

            memo* old_mem = new memo(); 
            copy(old_mem, mem);

            if (!memories_update_by_id(memories, id, text)) {
                message(error_program_side, "Something went wrong");
                continue;
            }
            memo* mem_new = memories_get_by_id(memories, id); 
            std::string message_str = "Memory    : "+ to_view_string(memo_to_stringc(old_mem)) + " successfuly";
            message(ok, message_str);
            message(display,          "updated to: " + to_view_string(memo_to_stringc(mem_new)));
            delete mem;
        } else if (!command.compare("/all")) {
            message(ok, "Displaying all memories.");
            for (const auto &mem: memories) {
                std::cout << to_view_string(memo_to_stringc(mem)) << std::endl;
            }

        } else {
            message(error_user_side, "There is no such command.");
        }

        memories_write_all_w(filename_memories, memories);
    };

    return 0;
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


void get_command_prompt();

*/
// Features: search as type goes