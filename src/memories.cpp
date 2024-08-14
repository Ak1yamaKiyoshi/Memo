#include "../include/memories.hpp"
#include "../include/string_utils.hpp"



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

memo* create_memo(const std::string &text, int id, std::vector<int> linked) {
    memo *memory = new memo();
    memory->created = time(0);
    memory->last_edited = time(0);

    memory->tags = parse_tags_from_string(text);

    memory->text = remove_tags_from_string(text);

    memory->viewed_times = 0;
    memory->memo_id = id;

    memory->linked_memories = linked;
    return memory; 
}

template<typename T>
void memo_write(T &out, const memo* mem, std::string delimiter) {
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
memo* memo_read(T &in, std::string delimiter) {
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
