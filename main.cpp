#include <string>
#include <vector>
#include <regex>
#include <codecvt>
#include <iostream>


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


struct memo {
    std::string text;
    std::vector<std::string> tags;

    time_t created;
    time_t last_edited;

    int viewed_times;
    int memo_id;

    std::vector<int> linked_memories;
};

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
        << join(mem->linked_memories, " ") << std::endl;
}

template<typename T>
void read_memo(T &in, const memo* mem, std::string delimiter = "|") {
    std::string buffer;
    getline(in, buffer);
}

struct search_result {
    memo reference;
    float confidence;
};



int main() {
    std::stringstream iss;
    std::string delimiter = "|";
    memo * mem = create_memo("text", 1);

    write_memo(iss, mem);

    std::cout << iss.str() << std::endl;

    return 0;
}

