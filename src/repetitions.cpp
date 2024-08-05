#include "../include/repetitions.hpp"
#include "../include/memories.hpp"
#include "../include/search.hpp"
#include "../include/string_utils.hpp"
#include "../include/utils.hpp"
#include "../include/vocablurary.hpp"

struct repetition {
    int memory_id;
    std::vector<int> timestamps;
    std::vector<bool> statuses;
    float penalty;
};

std::vector<int> string_vec_to_int(const std::vector<std::string> &from) {
    std::vector<int> to;
    for (const auto& token : from) to.push_back(std::stoi(token)); 
    return to;
}

std::vector<bool> string_vec_to_bool(const std::vector<std::string> &from) {
    std::vector<bool> to;
    for (const auto& token : from) to.push_back(static_cast<bool>(std::stoi(token))); 
    return to;
}

void print_repetition(const repetition* rep) {
    std::cout << "Memory ID: " << rep->memory_id << std::endl;
    std::cout << "Timestamps: ";
    for (const auto& timestamp : rep->timestamps) {
        std::cout << timestamp << " ";
    }
    std::cout << std::endl;
    std::cout << "Statuses: ";
    for (const auto& status : rep->statuses) {
        std::cout << (status ? "true" : "false") << " ";
    }
    std::cout << std::endl;
    std::cout << "Penalty: " << rep->penalty << std::endl;
}

template <typename T>
repetition* repetitions_read_single(T &in, const std::string& delimiter) {
    repetition* rep = new repetition();
    std::string buffer, token;
    getline(in, buffer);
    std::stringstream iss(buffer);
    std::vector<std::string> tokens;

    size_t pos = 0;
    while ((pos = buffer.find(delimiter)) != std::string::npos) {
        token = buffer.substr(0, pos);
        tokens.push_back(token);
        buffer.erase(0, pos + delimiter.length());
    }

    if (tokens.size() > 1) {
        rep->memory_id = std::stoi(tokens[0]);
        rep->penalty = std::stod(tokens[1]);

        rep->statuses = string_vec_to_bool(split_by_space(tokens[2]));
        rep->timestamps = string_vec_to_int(split_by_space(tokens[3]));
        return rep;
    } else {
        return nullptr;
    }
}

template <typename T>
void repetitions_write_single(T &out, const repetition* rep, const std::string& delim) {
    out << rep->memory_id << delim << rep->penalty << delim;
    for (auto status: rep->statuses) out << status << " ";
    out << delim;
    for (auto timestamp: rep->timestamps) out << timestamp << " ";
    out << delim << "\n";
}

void repetitions_to_file(const std::vector<repetition*>& repetitions, const std::string& filename, const std::string& delim) {
    std::ofstream f(filename);
    if (f.is_open()) {
        for (const auto& rep: repetitions) 
            repetitions_write_single(f, rep, delim);
        f.close();
    }
}

std::vector<repetition*> repetitions_from_file(const std::string& filename, const std::string& delimiter) {
    std::vector<repetition*> output;
    std::ifstream f_read(filename);
    if (f_read.is_open()) {
        repetition* rep;
        while (1) {
            rep = repetitions_read_single(f_read, delimiter);
            if (rep != nullptr) output.push_back(rep);
            else break;
        }

        f_read.close();
    }
    return output;
}

float repetition_eval(const repetition*rep) {
    const float n = 0.2;
    int k = 0;
    for (const auto s: rep->statuses) if (s) k++;
    auto no_of_secs = long(std::difftime(time(0), rep->timestamps[rep->timestamps.size()-1]));
    auto no_of_days = no_of_secs / (60 * 60 * 24);

    auto agnostic_part = (rep->timestamps.size() * n + k - rep->penalty);
    auto result = no_of_days + agnostic_part;

    return no_of_days  - result;
}

std::vector<repetition*> get_todays_repetitions(std::vector<repetition*> repetitions) {
    std::vector<float> days;
    for (const auto rep: repetitions) days.push_back(repetition_eval(rep));
    std::vector<repetition*> todays;
    for (int i = 0; i < days.size(); i++)
        if (days[i] <= 0.5)
            todays.push_back(repetitions[i]);

    return todays;
}