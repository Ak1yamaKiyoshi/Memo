#include "../include/string_utils.hpp"
#include "../include/memories.hpp"
#include "../include/repetitions.hpp"
#include "../include/search.hpp"
#include "../include/utils.hpp"
#include "../include/vocablurary.hpp"

#include <string>
#include <vector>
#include <ctime>
#include <set>


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

std::string remove_special_characters_from_string(const std::string &text, const std::string delimiter) {
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

std::string center(std::string input, int width) { 
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


inline std::ostream& operator<<(std::ostream& iss, const memo *mem) {
    iss << memo_to_stringc(mem);
    return iss;
}


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

bool parse_date(std::string str, std::tm &t_buffer) {
    std::regex command_regex(FULL_DATE_REGEX);
    auto rbegin = std::sregex_iterator(str.begin(), str.end(), command_regex);
    auto rend = std::sregex_iterator();
    std::string res = "";
    if (rbegin != rend) {
        std::smatch match = *rbegin;
        res = match.str();
        //"dd.mm.yyyy"
        //std::get_time(t_buffer, );

        std::tm t{};
        std::istringstream ss(res);
        ss >> std::get_time(&t, "%d.%m.%Y");
        if (ss.fail()) {
            throw std::runtime_error{"failed to parse time string"};
        }   
        //std::time_t time_stamp = mktime(&t);
    } 
    return false;
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
    return trim(remove_redundant_spaces(str));
}
std::string vec2string(const std::vector<std::string> &vec, std::string delimiter) {
    std::stringstream iss;
    for (int i = 0; (unsigned long)i < vec.size(); i++) 
        iss << vec[i] << delimiter;
    return iss.str();
}

std::string remove_spaces(std::string str) {
    std::regex spaces_regex("[ ]+");
    return std::regex_replace(str, spaces_regex, "");
}

std::string remove_special_characters(std::string &text) {
    text.erase(std::remove_if(text.begin(), text.end(),
        [](char c) { return (SPECIAL_CHARACTERS.find(c) != std::string::npos);}),
        text.end());
    return text;
}

std::string tolower_curlocale(std::string &str) {
    std::locale loc("");

    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
    std::wstring wide = converter.from_bytes(str);

    std::transform(
        wide.begin(), wide.end(), wide.begin(), 
        [&loc](wchar_t c) {
            return std::tolower(c, loc);
    });
    return converter.to_bytes(wide);
}

std::string normalize_string(std::string &str) {
    str = tolower_curlocale(str);
    str = remove_special_characters(str);
    str = remove_redundant_spaces(str);
    return str;
}

template <typename T>
std::set<T> vec2set(const std::vector<T> &v) {
    std::set<T> set;
    for (auto e: v) set.insert(e);
    return set;
} 

template<typename T>
std::string join(const std::vector<T> &vec, const std::string &delimiter) {
    std::stringstream iss;
    for (const auto &e: vec) iss << e << delimiter;
    return iss.str();
}

template  std::set<int> vec2set(const std::vector<int> &v);
template  std::set<std::string> vec2set(const std::vector<std::string> &v);
template  std::set<float> vec2set(const std::vector<float> &v);

template std::string join(const std::vector<int> &vec, const std::string &delimiter);
template std::string join(const std::vector<float> &vec, const std::string &delimiter);
template std::string join(const std::vector<std::string> &vec, const std::string &delimiter);
