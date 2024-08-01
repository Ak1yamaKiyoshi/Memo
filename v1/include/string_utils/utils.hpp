
#include <string>
#include <codecvt>
#include <algorithm>
#include <locale>
#include <vector>

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

std::vector<int> cast_to_int(std::string &str) {
    std::vector<int> resulting_vector;
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    std::u32string utf32 = converter.from_bytes(str);
    for (int i = 0; (unsigned long)i < str.length(); i++ )
        resulting_vector.push_back((int)utf32[i]);
    return resulting_vector;
}
