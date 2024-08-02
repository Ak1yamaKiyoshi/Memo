#include <string>
#include <vector>
#include <regex>
#include <codecvt>
#include <iostream>
#include <iomanip> 
#include <ctime>   


const std::string TAG_REGEX = "#{1}[A-zА-яі]+";
const std::wstring WORDS_REGEX = L"[A-zА-яіҐґЄєІЇї]+";
const std::string COMMAND_REGEX = "^[/]{1}[A-z]+";



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

    memory->text = text;
    memory->tags = parse_tags_from_string(text);

    memory->viewed_times = 0;
    memory->memo_id = id;

    // linked memories
    memory->linked_memories = linked;
    return memory; 
}

template<typename T>
void write_memo(T &out, const memo* mem, std::string delimiter = "|") {
    out << mem->viewed_times << delimiter
        << (int)mem->last_edited << delimiter 
        << (int)mem->created << delimiter 
        << mem->memo_id << delimiter
        << mem->text.length() << delimiter
        << mem->text << delimiter
        << mem->tags.size() << delimiter
        << join(mem->tags, " ") << delimiter
        << mem->linked_memories.size() << delimiter
        << join(mem->linked_memories, " ") << delimiter << std::endl;
}

template<typename T>
memo* read_memo(T &in, std::string delimiter = "|") {
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
std::cout << std::endl;
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
}

struct search_result {
    memo reference;
    float confidence;
};



int main() {
    std::stringstream iss;
    std::string delimiter = "|";
    std::vector<int > linked = {1, 3, 1, 2, 4};
    memo * mem = create_memo("text #tag b #vss", 1, linked);
    debug_print_memo(mem);

    std::cout << std::endl;
    
    write_memo(iss, mem);
    memo* mem1 = read_memo(iss);

    debug_print_memo(mem1);

    std::cout << std::endl;

    std::cout << "iss: " << iss.str() << std::endl;

    return 0;
}

